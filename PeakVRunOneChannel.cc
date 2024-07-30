#include "TCanvas.h"
#include "DFCALGeometry.h"
#include "TFile.h"
#include "TTree.h"
#include <stdio.h>
#include <iostream>

using namespace std;

void led_mod() {
	
    //Get file
    int ff;
    
    cout << "File?" << endl << "1. /work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2019-01-ver01.root" << endl;
    cout << "2. /work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2021-08-ver01.root" << endl;
    cout << "3. /work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2022-05-ver01.root" << endl;
    cout << "4. /work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2022-08-ver00.root" << endl;
    
    cin >> ff;
    
    TFile * file;
    TString FileTitle = "_at_";
    
    if (ff==1) {
    	file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2019-01-ver01.root");
    	FileTitle = FileTitle + "2019-01_LED_";
    }
    else if (ff==2) {
    	file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2021-08-ver01.root");
    	FileTitle = FileTitle + "2021-08_LED_";
    }
    else if (ff==3) {
    	file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2022-05-ver01.root");
    	FileTitle = FileTitle + "2022-05_LED_";
    }
    else if (ff==4) {	
    	file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2022-08-ver00.root");
    	FileTitle = FileTitle + "2022-08_LED_";
    }
    else {
    	cout << "Invalid number, no file loaded" << endl;
    	abort();
    }
    
    
    //Declaring used variables
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
    
    //Getting tree and filling variables
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
    
    Tree->GetEntry(5);
    cout << m_peak[30][4] << endl; 
    
    Long64_t nentries = Tree->GetEntries();
    
    
    
    DFCALGeometry *geo = new DFCALGeometry();
    
    //Get desired channel and LED
    int X,Y,LED;
    cout << "X: ";
    cin >> X;
    cout << endl << "Y: ";
    cin >> Y;
    cout << endl << "LED: ";
    cin >> LED;
    cout << endl;
    
    int C = geo->channel(Y,X);
    
    //Create variables to graph with
    double m_peak_entries[nentries];
    double m_rnb_entries[nentries];
    
    for (Long64_t ii = 0; ii < nentries; ii ++) {
      
      Tree->GetEntry(ii);
      if (m_rnb != 0) {
      	m_peak_entries[ii] = m_peak[C][LED-1];
      	m_rnb_entries[ii] = m_rnb;
      }
    }
    
    //Naming canvas
    TString CanvasTitle = "Channel_";
    CanvasTitle = CanvasTitle + C;
    CanvasTitle = CanvasTitle + "_at_";
    CanvasTitle = CanvasTitle + X;
    CanvasTitle = CanvasTitle + ",";
    CanvasTitle = CanvasTitle + Y;
    CanvasTitle = CanvasTitle + FileTitle;
    CanvasTitle = CanvasTitle + LED;
    
    //Create canvas and graph
    TCanvas *c1 = new TCanvas("c1",CanvasTitle,700,500);
    TGraphErrors *channelpeakVrun = new TGraphErrors(nentries-1,m_rnb_entries,m_peak_entries,nullptr,nullptr);
    
    //Drawing graph
    channelpeakVrun->SetTitle("Channel Peak V Run");
    channelpeakVrun->GetXaxis()->SetNoExponent(true);
    channelpeakVrun->GetYaxis()->SetNoExponent(true);
    channelpeakVrun->Draw("A*");
    channelpeakVrun->SetMarkerSize(0.5);
    c1->Print();
    
    //Saving graph
    int SC;
    cout << "Save?: ";
    cin >> SC;
    cout << endl;
    
    
    if (SC == 1) {
    	CanvasTitle = CanvasTitle + ".png";
    	c1->SaveAs(CanvasTitle);
    }
}
