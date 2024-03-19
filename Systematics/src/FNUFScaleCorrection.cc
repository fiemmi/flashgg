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
  //load graphs for R/R0 vs run number
  RR0maps_file_ = TFile::Open(RR0maps_name.c_str());
  RR0maps_file_->cd();
  for (int ir = 0; ir < nRings_; ir++) {
    RR0_vs_run_[ir] = (TGraph*)RR0maps_file_->Get(Form("gr_RoverR0_vs_Run_ring_%d",ir));
  }

  ///////////////////////////////////////////////////
  //////////// LOAD A.LEDOVSKOY'S MODEL /////////////
  ///////////////////////////////////////////////////
  
  models_file_ = TFile::Open(models_name.c_str());
  models_file_->cd();
  
  //LSimv vs R/R0 maps
  for (int ir = 0; ir < nRings_; ir++) {
    grLvsR_[ir] = (TGraph*)models_file_->Get(Form("Lsim_vs_RR0_fineSteps/gr_Lsim_vs_RR0_ring_%d",ir));
    grLvsR_[ir]->Sort();
  }

  TString SP_path = "Shower_Profiles_0X0/";
  TString SP_preshower_path = "Shower_Profiles_3X0/"; //having 3 radiation lenghts in front of ECAL

  int id = 5; //FIemmi: undamaged crystal
  for(int i=0; i<23; i++){
    LCE0_EB_[i] = 0;
    LCE0_EE_[i] = 0;
  }

  //load undamaged LCE models
  TH1D *hEB = (TH1D*)models_file_->Get(Form("LCE_NonDamaged_smooth/h_LCE_at_zero_EB_%d_smooth",id));
  for(int ib=1; ib<=hEB->GetNbinsX(); ib++){
    LCE0_EB_[ib-1] = hEB->GetBinContent(ib);
  }
  TH1D *hEE = (TH1D*)models_file_->Get(Form("LCE_NonDamaged_smooth/h_LCE_at_zero_EE_%d_smooth",id));
  for(int ib=1; ib<=hEE->GetNbinsX(); ib++){
    LCE0_EE_[ib-1] = hEE->GetBinContent(ib);
  }
  //load shower profiles
  //no preshower
  for(int i=0; i<nEne_; i++){
    TH1D *h = (TH1D*)models_file_->Get(Form(SP_path + "h_gamma_%d_gev",int(energies_[i])));
    h->Rebin(10);
    for(int ib=1; ib<=23; ib++){
      gProfile_[i][ib-1] = h->GetBinContent(ib);
    }
  }
  for(int i=0; i<nEne_; i++){
    TH1D *h = (TH1D*)models_file_->Get(Form(SP_path + "h_ele_%d_gev",int(energies_[i])));
    h->Rebin(10);
    for(int ib=1; ib<=23; ib++){
      eProfile_[i][ib-1] = h->GetBinContent(ib);
    }
  }
  //preshower
  for(int i=0; i<nEne_; i++){
    TH1D *h = (TH1D*)models_file_->Get(Form(SP_preshower_path + "h_gamma_%d_gev",int(energies_[i])));
    h->Rebin(10);
    for(int ib=1; ib<=23; ib++){
      gProfile_preshower_[i][ib-1] = h->GetBinContent(ib);
    }
  }
  for(int i=0; i<nEne_; i++){
    TH1D *h = (TH1D*)models_file_->Get(Form(SP_preshower_path + "h_ele_%d_gev",int(energies_[i])));
    h->Rebin(10);
    for(int ib=1; ib<=23; ib++){
      eProfile_preshower_[i][ib-1] = h->GetBinContent(ib);
    }
  }

  //load LCE models
  for(int i=0; i<nLumi_; i++){
    TH2D *h = (TH2D*)models_file_->Get("LCE_normalized_vs_Lsim_smooth/h_LCE_vs_lsim_" + slumi_[i] + "_smooth");
    for(int j=0; j<nRings_; j++){
      TH1D *hp = (TH1D*)h->ProjectionX("hp",j+1,j+1);
      for(int ib=1; ib<=23; ib++){
	LCE_[i][j][ib-1] = hp->GetBinContent(ib);
      }
    }
  }

  //Dump values of double ratio into TGraph2D to interpolate the values for whatever particle energy and equivalent lumi
  this->DRatio();
  this->DRatio_preshower();      
  
  for(int ir=0; ir<nRings_; ir++){
    F_g_[ir] = new TGraph2D();
    F_g_[ir]->SetNpx(500);
    F_g_[ir]->SetNpy(500);
    F_preshower_g_[ir] = new TGraph2D();
    F_preshower_g_[ir]->SetNpx(500);
    F_preshower_g_[ir]->SetNpy(500);
    
    int np = 0;
    for(int ie=0; ie<nEne_; ie++){
      for(int il=0; il<nLumi_; il++){
	F_g_[ir]->SetPoint( np, energies_[ie], lumi_[il], F_[ie][il][ir] );
	F_preshower_g_[ir]->SetPoint( np, energies_[ie], lumi_[il], F_preshower_[ie][il][ir] );
	np++;
      }
    }
  }
  
}//end constructor


FNUFScaleCorrection::~FNUFScaleCorrection(){}

double FNUFScaleCorrection::scaleCorr(double runNo, double eta, double R9, double energy, double R9thresh, double munat_corr, bool debug) {
  if (debug) std::cout << "++++ Printing parameters of scaleCorr method ++++" << std::endl;
  if (debug) std::cout << Form("Inputs: runNo = %f; eta = %f; R9 = %f; energy = %f", runNo, eta, R9, energy) << std::endl;
  double corr = 1.; //default value
  if (R9 <= R9thresh) corr = munat_corr; //low R9: constant correction
  else { //high R9: do machinery
    int iring = getRing(eta);
    if (debug) std::cout << Form("iring = %d", iring) << std::endl;
    double runmin = TMath::MinElement(RR0_vs_run_[iring]->GetN(), RR0_vs_run_[iring]->GetX()); 
    double runmax = TMath::MaxElement(RR0_vs_run_[iring]->GetN(), RR0_vs_run_[iring]->GetX());
    if (debug) std::cout << Form("runmin = %f; runmax = %f", runmin, runmax) << std::endl;
    if (runNo >= runmin && runNo <= runmax) {
      if (debug) std::cout << "Passed the run-number protection" << std::endl;
      double RR0 = RR0_vs_run_[iring]->Eval(runNo);
      if (debug) std::cout << Form("R/R0 = %f", RR0) << std::endl;
      double RR0min = TMath::MinElement(grLvsR_[iring]->GetN(), grLvsR_[iring]->GetX());
      double RR0max = TMath::MaxElement(grLvsR_[iring]->GetN(), grLvsR_[iring]->GetX());
      if (debug) std::cout << Form("RR0min = %f; RR0max = %f", RR0min, RR0max) << std::endl;
      if (RR0 >= RR0min && RR0 <= RR0max) {
	if (debug) std::cout << "Passed the R/R0 protection" << std::endl;
        double Lsim = GetLsim(iring, RR0);
	if (debug) std::cout << Form("Lsim = %f", Lsim) << std::endl;
	if(iring < 15  || iring > 25) corr = F_g_[iring]->Interpolate(energy, Lsim);
	else corr = F_preshower_g_[iring]->Interpolate(energy, Lsim);
	if (debug) std::cout << Form("1./corr = %f", 1./corr) << std::endl;
      }
    }
  }
  return 1./corr;
}

double FNUFScaleCorrection::scaleCorrUncert(double corr, double percent) {

  return corr * percent;
  
}

double FNUFScaleCorrection::GetLsim(int iring, double rr0) {
  if(iring > 29) iring = 29;
  double y = grLvsR_[iring]->Eval(rr0);
  if(y<0) y=0;
  return y;
}

void FNUFScaleCorrection::DRatio() {
  // Calculate double ratio and subtract 1
  for(int ie=0; ie<nEne_; ie++){
    for( int il=0; il<nLumi_; il++){
      for(int ir=0; ir<nRings_; ir++){
	double sumG1 = 0.;
	double sumE1 = 0.;
	double sumG0 = 0.;
	double sumE0 = 0.;
	int zMax = 23;
	if( ir >= 15 ) zMax = 22;
	//Compute ECAL response to photons and electrons
	for(int iz=0; iz<zMax; iz++){ //Integrate over crystal depth
	  double lce0 = LCE0_EB_[iz];
	  if( ir >= 15 ){
	    lce0 = LCE0_EE_[iz]; 
	  }
	  sumG1 += gProfile_[ie][iz] * LCE_[il][ir][iz] * lce0; //Do integral
	  sumE1 += eProfile_[ie][iz] * LCE_[il][ir][iz] * lce0; //Do integral
	  sumG0 += gProfile_[ie][iz];
	  sumE0 += eProfile_[ie][iz];
	}
	F_[ie][il][ir] = (sumG1 / sumG0) / (sumE1 / sumE0 );
      }
    }
  }
}

void FNUFScaleCorrection::DRatio_preshower() {
  // Calculate double ratio and subtract 1
  for(int ie=0; ie<nEne_; ie++){
    for( int il=0; il<nLumi_; il++){
      for(int ir=0; ir<nRings_; ir++){
	double sumG1 = 0.;
	double sumE1 = 0.;
	double sumG0 = 0.;
	double sumE0 = 0.;
	int zMax = 23;
	if( ir >= 15 ) zMax = 22;
	//Compute ECAL response to photons and electrons
	for(int iz=0; iz<zMax; iz++){ //Integrate over crystal depth
	  double lce0 = LCE0_EB_[iz];
	  if( ir >= 15 ){
	    lce0 = LCE0_EE_[iz]; 
	  }
	  sumG1 += gProfile_preshower_[ie][iz] * LCE_[il][ir][iz] * lce0; //Do integral
	  sumE1 += eProfile_preshower_[ie][iz] * LCE_[il][ir][iz] * lce0; //Do integral
	  sumG0 += gProfile_preshower_[ie][iz];
	  sumE0 += eProfile_preshower_[ie][iz];
	}
	F_preshower_[ie][il][ir] = (sumG1 / sumG0) / (sumE1 / sumE0 );
      }
    }
  }
}

int FNUFScaleCorrection::getRing (double eta) {
    eta = abs(eta);
    int iRing = int(eta*10);
    if(eta<1.479 && iRing>14) iRing = 14;
    if(eta>1.479 && iRing<15) iRing = 15;
    if(iRing>29) iRing = 29;
    return iRing;      
}
