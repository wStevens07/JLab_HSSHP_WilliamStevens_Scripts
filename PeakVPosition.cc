#include "TCanvas.h"
#include "DFCALGeometry.h"
#include "TFile.h"
#include "TTree.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

using namespace std;

void led_mod() {

    //Undo comments to get file.

    //TFile * file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2019-01-ver01.root");
    TFile * file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2021-08-ver01.root");
    //TFile * file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2022-05-ver01.root");
    //TFile * file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2022-08-ver00.root");
    
    //Declaring variables to graph with.
    int m_led[2800][5], m_rnb = 0;
    double m_mean[2800][5], m_rms[2800][5], m_integral[2800][5], m_integral_err[2800][5];
    double m_peak[2800][5], m_width[2800][5], m_yield[2800][5], m_chi2[2800][5];
    double m_peak_err[2800][5], m_width_err[2800][5], m_yield_err[2800][5];
    double peak, width, mean;
    double peak_err;
    double max, xmin, xmax;
    int bxmin, bxmax;
    double ratio_peak = 0, ratio_mean = 0;
    double ratio_peak_err = 0;
    
    //Creating tree and filling variables
    TTree *Tree = (TTree*)file->Get("LED");

    Tree->SetBranchAddress("m_rnb", &m_rnb);
    Tree->SetBranchAddress("m_led", &m_led);
    Tree->SetBranchAddress("m_mean", &m_mean);
    Tree->SetBranchAddress("m_rms", &m_rms);
    Tree->SetBranchAddress("m_integral", &m_integral);
    Tree->SetBranchAddress("m_integral_err", &m_integral_err);
    Tree->SetBranchAddress("m_peak", &m_peak);
    Tree->SetBranchAddress("m_width", &m_width);
    Tree->SetBranchAddress("m_yield", &m_yield);
    Tree->SetBranchAddress("m_peak_err", &m_peak_err);
    Tree->SetBranchAddress("m_width_err", &m_width_err);
    Tree->SetBranchAddress("m_yield_err", &m_yield_err);
    Tree->SetBranchAddress("m_chi2", &m_chi2);
    
    
    Long64_t nentries = Tree->GetEntries();
    
    //Creating canvas
    
    //TCanvas *c1 = new TCanvas("c1","posVpeak ALL plots 2019-01",700,500);
    TCanvas *c1 = new TCanvas("c1","posVpeak ALL plots 2021-08",700,500);
    //TCanvas *c1 = new TCanvas("c1","posVpeak ALL plots 2022-05",700,500);
    //TCanvas *c1 = new TCanvas("c1","posVpeak ALL plots 2022-08",700,500);
    
    
    //Creating histograms
    TH2F *posVpeak[6];
    for (int ii=0; ii < 5; ii++) {
    	TString temp = "Position Vs Peak LED ";
    	temp = temp + (ii+1);
    	posVpeak[ii] = new TH2F(temp,temp,59,0,59,59,0,59);
    }
    
    posVpeak[5] = new TH2F("Position Vs Peak ALL","Position Vs Peak ALL",59,0,59,59,0,59);
    
    DFCALGeometry *geo = new DFCALGeometry();
    
    //Filling histograms
    for (Long64_t ii = 0; ii < nentries; ii ++) {
      
      Tree->GetEntry(ii);


      int X,Y;
      double X2,Y2,R;
      for (int jj = 0; jj < 2800; jj++) {
      
      X = geo->column(jj);   
      Y = geo->row(jj);
      X2 = (X-29)*4.0157;
      Y2 = (Y-29)*4.0157;
      R = sqrt(X2*X2+Y2*Y2);
      
      	for (int ll = 0; ll < 5; ll++) {
      		
      	   
      	   	//cout << X2 << " " << Y2 << " " << R << endl;
      	   	//if ( ( X == 52 ) && (Y == 11) ) {

      		posVpeak[ll]->Fill(X,Y,m_peak[jj][ll]);
      		posVpeak[5]->Fill(X,Y,m_peak[jj][ll]);

      	
      	}
      }
    }
    
    //Drawing histograms
    gStyle->SetPalette(68,0);
    c1->Divide(3,2);
    
    for (int ii=0; ii < 6; ii++) {
    
    c1->cd(ii+1);
    gPad->SetLogz();
    posVpeak[ii]->Scale(1./nentries);
    posVpeak[ii]->SetStats(0);
    posVpeak[ii]->Draw("colz");
    
    }
}
