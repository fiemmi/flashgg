#ifndef FNUFScaleCorrection_H
#define FNUFScaleCorrection_H

class FNUFScaleCorrection {
 public:
  FNUFScaleCorrection();
  FNUFScaleCorrection(std::string RR0maps_name, std::string models_name);
  ~FNUFScaleCorrection();
  int nE()              { return nEne_; };
  double energy(int i)  { return energies_[i]; };
  int nL()              { return nLumi_; };
  double lumi(int i)    { return lumi_[i]; };
  int nR()              { return nRings_; };  
  double getLsim(int, double);
  double getClosest(double x, std::vector<double>);
  void DRatio();
  void DRatio_preshower();
  //double F(int i, int j, int k) { return F_[i][j][k]; };
  //double F_preshower(int i, int j, int k) { return F_preshower_[i][j][k]; };
  int getRing(double eta);
  double scaleCorr(bool isData, double runNo, double eta, double R9, double energy, double R9thresh, double munat_corr, bool debug);
  double scaleCorrUncert(double corr, double percent, bool debug);
  
 private:
  static const int nEne_ = 33;
  static const int nLumi_ = 16;
  static const int nRings_ = 30;
  double lumi_[nLumi_] = { 1e-1, 2e-1, 5e-1, 1e+0, 2e+0, 5e+0, 1e+1, 2e+1, 5e+1, 10e+1, 15e+1, 20e+1, 25e+1, 30e+1, 40e+1, 50e+1 };
  std::vector<double> lumi_v_{ 1e-1, 2e-1, 5e-1, 1e+0, 2e+0, 5e+0, 1e+1, 2e+1, 5e+1, 10e+1, 15e+1, 20e+1, 25e+1, 30e+1, 40e+1, 50e+1 };
  TString slumi_[nLumi_] = { "1e-1", "2e-1", "5e-1", "1e+0", "2e+0", "5e+0", "1e+1", "2e+1", "5e+1", "10e+1", "15e+1", "20e+1", "25e+1", "30e+1", "40e+1", "50e+1" };
  //double energies_[nEne_] = { 10, 15, 20, 30, 50, 75, 100, 150, 200, 300, 500, 750, 1000 };
  double energies_[nEne_] = { 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 225, 250, 275, 300, 325, 350, 375, 400, 450, 500, 600, 700, 800, 1000 };
  std::vector<double> energies_v_{ 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 225, 250, 275, 300, 325, 350, 375, 400, 450, 500, 600, 700, 800, 1000 };
  TFile * RR0maps_file_;
  TFile * models_file_;
  double gProfile_[nEne_][23]; //23: number of bins in crystal depth
  double eProfile_[nEne_][23];
  double gProfile_preshower_[nEne_][23]; //23: number of bins in crystal depth
  double eProfile_preshower_[nEne_][23];
  TGraph * RR0_vs_run_[nRings_];
  double avgRR0[nRings_]; //average (on a per-year basis) R/R0 in a given ring. Used in MC to get uncertainty
  TGraph * grLvsR_[nRings_];
  //double F_[nEne_][nLumi_][nRings_];
  TGraph2D * F_[nRings_];
  double F_preshower_[nEne_][nLumi_][nRings_]; 
  TGraph2D * F_g_[nRings_]; //TGraph2D to be interpolated
  TGraph2D * F_preshower_g_[nRings_]; //TGraph2D to be interpolated 
  double LCE_[nLumi_][nRings_][23];
  double LCE0_EB_[23];
  double LCE0_EE_[23];
  TString path;
  
};

#endif
