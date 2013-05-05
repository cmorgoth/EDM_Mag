#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "TFile.h"
#include "TMath.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TStyle.h"

using namespace std;

int main(){
  
  ifstream file;
  //file.open( "../autoscan-2013-05_03-07-24.txt" );
  //file.open( "../autoscan-2013-05_03-09-35.txt" );
  //file.open("../autoscan-2013-04-01_10-06-32.txt");
  file.open("../autoscan-2013-04-01_13-34-29.txt");
  //file.open( "../autoscan-2013-05_03-10-41.txt");
  //file.open( "../autoscan-2013-05_03-08-30.txt");

  std::vector<double> Omega;
  std::vector<double> Amp1;
  std::vector<double> Amp2;
  std::vector<double> Diff;
  std::vector<double> Sum;
  std::vector<double> Error;
  

  string line[7] ;
  //double Omega, Amp[2], buff[4];
  
  for(int i = 0; i < 4; i++)getline(file, line[0]);//skipping header
  
  if( file.is_open() ){
    while ( file.good() ){
      //file >> Omega >> Amp[0] >> Amp[1] >> buff[0] >> buff[1] >> buff[2] >> buff[3];
      file >> line[0] >> line[1] >> line[2] >> line[3] >> line[4] >> line[5] >> line[6];
      if(file.eof())break;
      //if(line[0].find('E') != 0)line[0].replace(line[0].find('E'),1,1,'e'); 
      for(int i = 0; i < 6; i++)line[i].replace(line[i].find('E'),1,1,'e'); 
      Omega.push_back( atof( line[0].c_str() ) );
      Amp1.push_back( atof( line[1].c_str() ) );
      Amp2.push_back( atof( line[2].c_str() ) );
      //std::cout << atof( line[0].c_str() ) << "  " << atof( line[1].c_str() ) << "  " <<  atof( line[2].c_str() ) << std::endl;
    }
  }else{
    std::cout << "Unable to Read/Open the file" << std::endl;
  }
  
  for(int j = 0; j < Amp1.size(); j++){
    Diff.push_back(Amp2[j]-Amp1[j]);
    Sum.push_back(Amp1[j]+Amp2[j]);
  }
  
  TGraph* gr = new TGraph( Omega.size(), &(Omega[0]), &(Amp1[0]) );//s1
  TGraph* gr1 = new TGraph( Omega.size(), &(Omega[0]), &(Amp2[0]) );//s2
  TGraph* gr2 = new TGraph( Omega.size(), &(Omega[0]), &(Diff[0]) );//diff
  TGraph* gr3 = new TGraph( Omega.size(), &(Omega[0]), &(Sum[0]) );//sum
  
  TFile* f = new TFile("plots.root", "RECREATE");
  
  //TF1* f1 = new TF1("f1","[2]*((cos([0])*[1]/2.)/((x-14000)*(x-14000)+[1]*[1]/4)-(sin([0])*(x-14000)*[1]/2.)/((x-14000)*(x-14000)+[1]*[1]/4))",11500,15500);
  
  //TF1* f1 = new TF1("f1","[2]*((cos([0])*[1]/2.)/((x-[3])*(x-[3])+[1]*[1]/4.)+(sin([0])*([3]-x)]*[1]/2.)/((x-[3])*(x-[3])+[1]*[1]/4.))",11200,16000);
  
  TF1* f1 = new TF1("f1","[2]*((cos([0])*[1]/2.)/((x-[3])*(x-[3])+[1]*[1]/4.)+(sin([0])*([3]-x))/((x-[3])*(x-[3])+[1]*[1]/4.))+pol5(4)",11200,16000);
  
  f1->SetParNames("phi","Gamma","Amp","Nu0");
  //f1->SetParLimits(3, 11200, 16000);
  //f1->SetParLimits(2, 0, 100000);
  //f1->SetParLimits(2, 0, 11200);
  //f1->SetParLimits(0, -4*3.1416, 4*3.1416);
  //f1->SetParLimits(1, 0, 1000000);
  f1->SetParameter(0, 0.361193);
  f1->SetParameter(1, 610.815);
  f1->SetParameter(2, 0.0375757);
  f1->SetParameter(3, 13531);
  
  f1->SetParameter(4, 0.0);
  f1->SetParameter(5, 0.0);
  f1->SetParameter(6, 0.0);
  f1->SetParameter(7, 0.0);
  f1->SetParameter(8, 0.0);
  f1->SetParameter(9, 0.0);
  
  //f1->SetParameter(0, .0);
  //f1->SetParameter(1, 280.);
  //f1->SetParameter(2, .0);
  //f1->SetParameter(3,14000);
  
  
  TF1* f2 = new TF1("f2","[2]*((-1*sin([0])*[1]/2.)/((x-[3])*(x-[3])+[1]*[1]/4.)+(cos([0])*([3]-x))/((x-[3])*(x-[3])+[1]*[1]/4.))+pol5(4)",11200,16000);
  
  f2->SetParNames("phi","Gamma","Amp","Nu0");
  
  f2->SetParameter(0, .001);
  f2->SetParameter(1, 280.);
  f2->SetParameter(2, .01);
  f2->SetParameter(3,14000);
  
  //TF1* f3 = new TF1("f3","[2]*(((cos([0])-sin([4]))*[1]/2.)/((x-[3])*(x-[3])+[1]*[1]/4.)+((sin([0])+cos([4]))*([3]-x))/((x-[3])*(x-[3])+[1]*[1]/4.))+pol5(5)+[11]",11000,15900);
  TF1* f3 = new TF1("f3","[2]*(((cos([0])-sin([0]))*[1]/2.)/((x-[3])*(x-[3])+[1]*[1]/4.)+((sin([0])+cos([0]))*([3]-x))/((x-[3])*(x-[3])+[1]*[1]/4.))+[4]+[5]",11000,16000);
  
  f3->SetParNames("phi","Gamma","Amp","Nu0");
  
  f3->SetParameter(0, .001);
  f3->SetParameter(1, 280.);
  f3->SetParameter(2, .01);
  //f3->SetParameter(5, .01);
  f3->SetParameter(3,14000);

  f3->SetParameter(4, 0.0);
  f3->SetParameter(5, 0.0);
  //f3->SetParameter(6, 0.0);
  //f3->SetParameter(7, 0.0);
  //f3->SetParameter(8, 0.0);
  //f3->SetParameter(9, 0.0);
  //f3->SetParameter(10, 0.0);
  
  //TF1* f4 = new TF1("f4","([2]*((cos([0])+sin([4]))*[1]/2.)/((x-[3])*(x-[3])+[1]*[1]/4.)-[5]*((sin([0])-cos([4]))*([3]-x))/((x-[3])*(x-[3])+[1]*[1]/4.))+pol1(6)+[12]",11000,15900);
  
  //TF1* f4 = new TF1("f4","([2]*((cos([0])+sin([4]))*[1]/2.)/((x-[3])*(x-[3])+[1]*[1]/4.)-[5]*((sin([0])-cos([4]))*([3]-x))/((x-[3])*(x-[3])+[1]*[1]/4.)) + pol5(6)",11000,15900);

  TF1* f4 = new TF1("f4","([2]*((cos([0])+sin([0]))*[1]/2.)/((x-[3])*(x-[3])+[1]*[1]/4.)-[4]*((sin([0])-cos([0]))*([3]-x))/((x-[3])*(x-[3])+[1]*[1]/4.)) + pol5(6)",11000,15900);
  
  f4->SetParNames("phi","Gamma","Amp","Nu0","Amp2");
  
  f4->SetParameter(0, .001);
  f4->SetParameter(4, .001);
  f4->SetParameter(1, 280.);
  f4->SetParameter(2, .01);
  //f4->SetParameter(5, .01);
  f4->SetParameter(3,14000);
  
  f4->SetParameter(5, 0.0);
  f4->SetParameter(6, 0.0);
  f4->SetParameter(7, 0.0);
  f4->SetParameter(8, 0.0);
  f4->SetParameter(9, 0.0);
  f4->SetParameter(10, 0.0);
  //f4->SetParameter(12, 0.0);
  
  //TF1* pol = new TF1("pol","expo(0)+[2]+[3]*x+[4]*x*x",11100,12800);
  TF1* pol = new TF1("pol","pol3",11100,12800);
  
  pol->SetParameter(0, 0.0);
  pol->SetParameter(1, 0.0);
  pol->SetParameter(2, 0.0);
  pol->SetParameter(3, 0.0);
  pol->SetParameter(4, 0.0);
  //pol->SetParameter(5, 0.0);
  //pol->SetParameter(6, 0.0);
  
  double w = 600;
  double h = 600;
  TCanvas* c = new TCanvas("c", "c", w, h);
  c->cd();
  f1->Draw();
  c->SaveAs("f1.pdf");
  c->SaveAs("f1.png");
  
  TCanvas* c1 = new TCanvas("c1", "c1", w, h);
  
  c1->cd();
  gr->SetMarkerSize(0.1);
  gr->SetMarkerStyle(24);
  gr->SetMarkerColor(4);
  //gr->GetYaxis()->SetRangeUser(-0.0002, 0.0002);
  gr->Draw("AP");
  gr->Fit(f1, "WREMV");
  //gr->Fit(f1, "REV");
  
  c1->SaveAs("fit1.png");
  c1->SaveAs("fit1.pdf");
  
  gr1->SetMarkerStyle(24);
  gr1->SetMarkerColor(2);
  gr1->SetMarkerSize(0.1);
  gr1->Draw("AP");
  gr1->Fit(f2, "REMVI");
  
  c1->SaveAs("fit2.png");
  c1->SaveAs("fit2.pdf");
  
  gr2->SetMarkerStyle(24);
  gr2->SetMarkerColor(2);
  gr2->SetMarkerSize(0.1);
  gr2->Draw("AP");
  f3->SetLineColor(4);
  
  TFitResultPtr fitR = gr2->Fit(pol, "REMVIS");
  TFitResult* fit = fitR.Get();
  
  c1->SaveAs("fit3_1A_1Phi_pol0_0_0.png");
  c1->SaveAs("fit3_1A_1Phi_pol0_0_0.pdf");
  
  double par[4];
  
  par[0] = fit->Value(0);
  par[1] = fit->Value(1);
  par[2] = fit->Value(2);
  par[3] = fit->Value(3);
  
  std::cout << "parameter: " << par[0] << " " << par[1] << " " << par[2] << " " << par[3] << std::endl;
  
  double diff2 = .0;
  int ctr = 0;
  for(int j = 0; j < Omega.size(); j++){
    if( Omega[j] > 11100 && Omega[j] < 12800 ){
      diff2 += pow( Diff[j] - ( par[0]+par[1]*Omega[j]+par[2]*pow(Omega[j],2)+par[3]*pow(Omega[j],3)), 2);
      ctr++;
    }
  }
  
  double err = sqrt((double)diff2/(ctr-1));
  std::cout << "Standard Deviation: " << err << std::endl;
  std::vector<double> ErrY;
  std::vector<double> ErrX;
  for(int j = 0; j < Omega.size(); j++){
    ErrY.push_back(err);
    ErrX.push_back(0.0);
  }
  
  TGraphErrors* GE = new TGraphErrors(Omega.size(), &(Omega[0]), &(Diff[0]), &(ErrX[0]), &(ErrY[0]));

  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(1111);
  GE->SetMarkerStyle(24);
  GE->SetMarkerColor(2);
  GE->SetMarkerSize(0.1);
  GE->Draw("AP");
  GE->Fit(f4,"REMVI");
  c1->SaveAs("graph_err_4_pol5_v2_scan2.png");
  c1->SaveAs("graph_err_4_pol5_v2_scan2.pdf");
  

  gr3->SetMarkerStyle(24);
  gr3->SetMarkerColor(2);
  gr3->SetMarkerSize(0.1);
  gr3->Draw("AP");
  f4->SetLineColor(4);
  //gr3->Fit(f4, "REMVI");
  
  c1->SaveAs("fit4.png");
  c1->SaveAs("fit4.pdf");
  
  gr->Write("g1");
  gr1->Write("g2");
  gr2->Write("g3");
  gr3->Write("g4");
  
  f->Close();
  
  return 0;
}
