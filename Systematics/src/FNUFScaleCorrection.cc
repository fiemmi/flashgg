#include <iostream>

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TMath.h"

#include "flashgg/Systematics/interface/FNUFScaleCorrection.h"

FNUFScaleCorrection::FNUFScaleCorrection() {}

FNUFScaleCorrection::FNUFScaleCorrection(std::string RR0maps_name, std::string models_name) {
  //load graphs for R/R0 vs run number; compute average R/R0
  RR0maps_file_ = TFile::Open(RR0maps_name.c_str());
  RR0maps_file_->cd();
  for (int ir = 0; ir < nRings_; ir++) {
    RR0_vs_run_[ir] = (TGraph*)RR0maps_file_->Get(Form("gr_RoverR0_vs_Run_ring_%d",ir));
    avgRR0[ir] = RR0_vs_run_[ir]->GetMean(2);
  }

  /////////////////////////////////////////////////////////////////////////////////
  //////////// LOAD LSIM VERSUS R/R0 MAPS AND /////////////////////////////////////
  //////////// DOUBLE RATIOS OF ECAL RESPONSES (photon/electron) //////////////////
  //////////// BASED ON THE ECAL LONGEVITY MODEL //////////////////////////////////
  //////////// BY SASHA LEDOVSKOY /////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////

  models_file_ = TFile::Open(models_name.c_str());
  models_file_->cd();
  
  //LSim vs R/R0 maps
  for (int ir = 0; ir < nRings_; ir++) {
    grLvsR_[ir] = (TGraph*)models_file_->Get(Form("Lsim_vs_RR0_fineSteps/gr_Lsim_vs_RR0_ring_%d",ir));
    grLvsR_[ir]->Sort();
  }
  //Double ratios
  for (int ir = 0; ir < nRings_; ir++) {
    F_[ir] = (TGraph2D*)models_file_->Get(Form("F_vs_E_vs_Lumi_ring_%d",ir));
  }
}//end constructor


FNUFScaleCorrection::~FNUFScaleCorrection(){}

double FNUFScaleCorrection::scaleCorr(bool isData, double runNo, double eta, double R9, double energy, double R9thresh, double munat_corr, bool debug) {
  if (debug) {
    std::string what;
    if (isData) what = "data";
    else what = "MC";
    std::cout << Form("Running FNUFScaleCorrection::scaleCorr for %s", what.c_str()) << std::endl;
    std::cout << "++++ Printing parameters of scaleCorr method ++++" << std::endl;
    std::cout << Form("Inputs: runNo = %f; eta = %f; R9 = %f; energy = %f", runNo, eta, R9, energy) << std::endl;
  }
  double F = 1.; //default value for double ratio of energy responses
  if (R9 > R9thresh) { //only correct highR9 photons; lowR9 photons are converted and their shower max is the same as electrons 
    int iring = getRing(eta);
    if (debug) std::cout << Form("iring = %d", iring) << std::endl;
    double runmin = TMath::MinElement(RR0_vs_run_[iring]->GetN(), RR0_vs_run_[iring]->GetX()); 
    double runmax = TMath::MaxElement(RR0_vs_run_[iring]->GetN(), RR0_vs_run_[iring]->GetX());
    if (debug) std::cout << Form("runmin = %f; runmax = %f", runmin, runmax) << std::endl;
    double runNo_; // used to check for run number protection. Actual run number if it's data, toy value for MC
    if (isData) runNo_ = runNo;
    else runNo_ = (runmin + runmax)/2.; //for MC, just take the middle point of runmin and runmax as a toy value to have the event pass the run number protection 
    if (runNo_ >= runmin && runNo_ <= runmax) {
      if (debug) std::cout << "Passed the run-number protection" << std::endl;
      double RR0;
      if (isData) RR0 = RR0_vs_run_[iring]->Eval(runNo); //data: use actual runNo
      else RR0 = avgRR0[iring]; //MC: use average R/R0
      if (debug) std::cout << Form("R/R0 = %f", RR0) << std::endl;
      double RR0min = TMath::MinElement(grLvsR_[iring]->GetN(), grLvsR_[iring]->GetX());
      double RR0max = TMath::MaxElement(grLvsR_[iring]->GetN(), grLvsR_[iring]->GetX());
      if (debug) std::cout << Form("RR0min = %f; RR0max = %f", RR0min, RR0max) << std::endl;
      if (RR0 >= RR0min && RR0 <= RR0max) {
	if (debug) std::cout << "Passed the R/R0 protection" << std::endl;
	double Lsim = getLsim(iring, RR0);
	if (debug) std::cout << Form("Lsim = %f", Lsim) << std::endl;
	double emin = *std::min_element(std::begin(energies_), std::end(energies_));
	double emax = *std::max_element(std::begin(energies_), std::end(energies_));
	double lsimmin = *std::min_element(std::begin(lumi_), std::end(lumi_));
	double lsimmax = *std::max_element(std::begin(lumi_), std::end(lumi_));
	if (debug) std::cout << Form("emin = %f; emax = %f; lsimmin = %f, lsimmax = %f", emin, emax, lsimmin, lsimmax) << std::endl;
	if (energy >= emin && energy <= emax && Lsim >= lsimmin && Lsim <= lsimmax) {
	  if (debug) std::cout << "Passed the energy and Lsim protections" << std::endl;
	  F = F_[iring]->Interpolate(energy, Lsim); //fetch FNUF correction
	} // end energy and Lsim protection
	else { // if failing energy and Lsim protection, use closest value in energy and Lsim
	  F = F_[iring]->Interpolate(getClosest(energy,energies_v_), getClosest(Lsim, lumi_v_));
	}
      }// end RR0 protection 
    } // end run number protection
    if (debug) std::cout << Form("1./F = %f", 1./F) << std::endl;
  }// end highR9
  else {
    if (debug) {
      std::cout << "LowR9" << std::endl;
      std::cout << Form("1./F = %f", 1./F) << std::endl;
    }
  }
  if (F==0.) {// protect against any residual misbehaviours
    std::cout << "+++ WARNING +++" << std::endl << "Double ratio is 0. Returning 1." << std::endl;
    return 1.;
  }
  else return 1./F;
}

double FNUFScaleCorrection::scaleCorrUncert(double corr, double percent, bool debug) {

  //return corr * percent;
  if (debug) std::cout << Form("Returned by scaleCorrUncert: %f", (percent*(1. - corr))) << std::endl;
  return percent*(1. - corr);
  
}

double FNUFScaleCorrection::getLsim(int iring, double rr0) {
  if(iring > 29) iring = 29;
  double y = grLvsR_[iring]->Eval(rr0);
  if(y<0) y=0;
  return y;
}

int FNUFScaleCorrection::getRing (double eta) {
    eta = abs(eta);
    int iRing = int(eta*10);
    if(eta<1.479 && iRing>14) iRing = 14;
    if(eta>1.479 && iRing<15) iRing = 15;
    if(iRing>29) iRing = 29;
    return iRing;      
}

double FNUFScaleCorrection::getClosest(double x, std::vector<double> v) {
  
  auto low = std::lower_bound(v.begin(), v.end() - 1, x); //get pointer to first element greater or equal to x
  if ((low - v.begin()) == 0) return v.at(0); // if x is below the lowest element in v
  else if ( abs(x - v.at(low - v.begin()) ) > abs(x - v.at(low - v.begin() - 1)) ) return v.at(low - v.begin() - 1);
  else return v.at(low - v.begin());
}
