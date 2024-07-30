#include "TCanvas.h"
#include "DFCALGeometry.h"
#include "TFile.h"
#include "TTree.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

using namespace std;

void led_mod(int ff,int ll,int ll2) {
    
    //Declaring first LED and second LED used to make plots
    ll=ll-1;
    ll2=ll2-1;


    //Obtaining and extracting file corresponding to run number.
    TString rFile = "/work/halld/home/ijaegle/FCAL-LED/RunPeriod-2022-08/ver00/root-files/led-hist-RunPeriod-2022-08-run-nb-";
    rFile = rFile + ff;
    rFile = rFile + ".root";
    TFile * file = new TFile(rFile);
    rFile = "gains_run";
    rFile = rFile + ff;
    rFile = rFile + ".txt";
    TString FileTitle = "Ring_Hist_gains_";
    ifstream fileContents; double items; vector<double> gainData;
    fileContents.open(rFile);
    while (fileContents >> items) {
    	gainData.push_back(items);
    }
    fileContents.close();

    
    FileTitle = FileTitle + "2022-08_";
    

    //Declaring variables used extract data from trees
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
    
    TTree *Tree = (TTree*)file->Get("LED");

    //Getting data from trees
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
    
    FileTitle = FileTitle + "LED_";
    FileTitle = FileTitle + (ll+1);
    FileTitle = FileTitle + "by";
    FileTitle = FileTitle + (ll2+1);
    FileTitle = FileTitle + "_rnb_";
    FileTitle = FileTitle + ff;
    
    //Declaring canvas and peak histograms for each ring
    TCanvas *c1 = new TCanvas("c1",FileTitle,700,500);
    TH1F *peakHist[28];
    for (int ii=0; ii < 28; ii++) {
    	TString temp = "Ring_";
    	temp = temp + (ii+1);
    	temp = temp + "_scaled_by-run_";
    	temp = temp + ff;
    	peakHist[ii] = new TH1F(temp,temp,75,0,3000);
    }
    TH1F *peakHist2[9];
    for (int ii=0; ii < 9; ii++) {
    	TString temp = "2_Ring_";
    	temp = temp + (ii+1);
    	temp = temp + "_scaled_by-run_";
    	temp = temp + ff;
    	peakHist2[ii] = new TH1F(temp,temp,75,0,3000);
    }
    TH1F *peakHistScale = new TH1F("scale","scale",75,0,3000);
    
    //Declaring variables used to get ring number of each channel
    DFCALGeometry *geo = new DFCALGeometry();
    const double blocksize = 4.0157;
    const double inring = sqrt(2)*blocksize;
    int X,Y;
    double X2,Y2,R;
    
    Tree->GetEntry(0);
    
    //Getting ring number and filling histograms
    for (int jj = 0; jj < 2800; jj++) {
      
      X = geo->column(jj);   
      Y = geo->row(jj);
      X2 = (X-29)*blocksize;
      Y2 = (Y-29)*blocksize;
      R = sqrt(X2*X2+Y2*Y2);
      
      for (double RR = 1.; RR < 29.; RR++) {
      	   int RRR = int (RR);
      	   if ( ( R < ( (120.471-inring)*(RR/28.) )+inring ) && ( R >= ( (120.471-inring)*((RR-1)/28.) )+inring ) ) {
      	   	//cout << X2 << " " << Y2 << " " << R << endl;
      	   	//if ( ( X == 52 ) && (Y == 11) ) {
      	   	
      	   	
      	   	if ( RRR > 9 ) {
      		    peakHist[RRR-1]->Fill(m_peak[jj][ll]/gainData[jj]);
      		  }
      		  else {
      		    peakHist[RRR-1]->Fill(m_peak[jj][ll]/gainData[jj]);
      		    peakHist2[RRR-1]->Fill(m_peak[jj][ll2]/gainData[jj]);
      		  }
      		  if ( RRR == 14) {
      		    peakHistScale->Fill(m_peak[jj][ll2]/gainData[jj]);
      		  }
      	  }
      	}
     }
    
    
    //Getting mean of fits
    //gStyle->SetPalette(68,0);
    double gausFitMeans[28];
    double ringNums[28];
    TF1 *gausFit[28];
    for (int ii = 0; ii < 28; ii++) {
    	TString temp = "gaus fit ";
    	temp = temp + ii+1;
   	  gausFit[ii] = new TF1(temp,"gaus(0)",0.,3000.);
   	  peakHist[ii]->Fit(gausFit[ii]);
   	  gausFitMeans[ii] = gausFit[ii]->GetParameter(1);
   	  ringNums[ii] = double(ii) + 1.;
    }
    double gausFitMeans2[9];
    double ringNums2[9];
    TF1 *gausFit2[9];
    for (int ii = 0; ii < 9; ii++) {
    	TString temp = "2 gaus fit ";
    	temp = temp + ii+1;
   	  gausFit2[ii] = new TF1(temp,"gaus(0)",0.,3000.);
   	  peakHist2[ii]->Fit(gausFit2[ii]);
   	  gausFitMeans2[ii] = gausFit2[ii]->GetParameter(1);
   	  ringNums2[ii] = double(ii) + 1.;
   	  cout << ringNums2[ii] << " " << gausFitMeans2[ii] << endl;
    }
    
    TF1 *scaleFit = new TF1("scaleFit","gaus(0)",0.,3000.);
    peakHistScale->Fit(scaleFit);
    double sFactor = ((scaleFit->GetParameter(1))/(gausFitMeans[13]));
    for (int ii = 0; ii < 9; ii++) {
    	gausFitMeans2[ii] = gausFitMeans2[ii]/sFactor;
    }

    
    //Creating a scatterplot of means for each ring number    
    //
    TGraphErrors *meanFit = new TGraphErrors(28,ringNums,gausFitMeans,nullptr,nullptr);
    TGraphErrors *meanFit2 = new TGraphErrors(9,ringNums2,gausFitMeans2,nullptr,nullptr);
    
    
    //Cutting graphs
    double gausFitMeans3[28];
    
    for (int ii=0; ii < 28; ii++) {
    	if (ii < 9) {
    		gausFitMeans3[ii] = gausFitMeans2[ii];
    	}
    	else {
    		gausFitMeans3[ii] = gausFitMeans[ii];
    	}
    }
    
    TGraphErrors *meanFit3 = new TGraphErrors(28,ringNums,gausFitMeans3,nullptr,nullptr);
    
    int numNum = 27;
    int numNum2 = 27;
    
    for(int ii=27; ii > -1; ii--) {
    	if ( (gausFitMeans[ii] < 0 ) ) {
    		cout << meanFit->GetPointX(ii) << " " << meanFit->GetPointY(ii) << endl;
    		meanFit->RemovePoint(ii);
    		meanFit3->RemovePoint(ii);
    		numNum--;
    		numNum2--;
    	}
    	if (gausFitMeans2[ii] < 0) {
    		cout << meanFit2->GetPointX(ii) << " " << meanFit2->GetPointY(ii) << endl;
    		meanFit2->RemovePoint(ii);
    		meanFit3->RemovePoint(ii);
    		numNum2--;
    	}
    }
    
    TF1 *coolfit = new TF1("cool fit","[0]+[1]*x",0.,28.);
    meanFit->Fit(coolfit);
    
    for(int ii=numNum; ii > -1; ii--) {
    	if ( abs( ( coolfit->GetParameter(0)+meanFit->GetPointX(ii)*coolfit->GetParameter(1) ) - meanFit->GetPointY(ii) ) > 150 ) {
    		meanFit->RemovePoint(ii);
    	}
    }
    for(int ii=numNum2; ii > -1; ii--) {
    	if ( abs( ( coolfit->GetParameter(0)+meanFit->GetPointX(ii)*coolfit->GetParameter(1) ) - meanFit->GetPointY(ii) ) > 150 ) {
    		if ( meanFit3->GetPointX(ii) > 9 ) {
    			meanFit3->RemovePoint(ii);
    		}
    	}
    }
    
    //Drawing graphs
    TF1 *betterfit = new TF1("better fit","[0]+[1]*x+[2]*x*x",10.,28.);
    meanFit->Fit(betterfit,"","",10.,25.);
    
    TF1 *bluefit = new TF1("blue fit","[0]+[1]*x+[2]*x*x",0.,25.);
    meanFit3->Fit(bluefit,"","",0.,25.);
    //c1->SetLogz();
    TMultiGraph *meanfits = new TMultiGraph();
    meanfits->SetTitle("Fit Means of peak for Ring Position");
    meanfits->GetXaxis()->SetTitle("Ring # starting from closest to beamline");
    meanfits->GetYaxis()->SetTitle("Mean peak energy for a gaussian fit of all channels in the ring");
    //meanFit->SetStats(0);
    //meanFit2->SetStats(0);
    meanFit->SetMarkerColor(kRed);
    meanFit2->SetMarkerColor(kBlue);
    bluefit->SetLineColor(kBlue);
    
    meanfits->Add(meanFit);
    meanfits->Add(meanFit2);
    
    meanfits->Draw("A*");
    
    gPad->Modified();
    meanfits->GetXaxis()->SetLimits(0.,30.);
    //meanfits->SetMinimum(0.);
    //meanfits->SetMaximum(1000.);
    meanfits->GetYaxis()->SetRangeUser(0,1000);
    //peakHist[1]->Draw();
    
    bluefit->Draw("Same");
    betterfit->Draw("Same");
    
    c1->Print();
    
    
    //Saving graphs
    int Save;
    cout << "Save?: ";
    cin >> Save;
    cout << endl;
    
    
    if (Save == 1) {
    	FileTitle = FileTitle + ".png";
    	c1->SaveAs(FileTitle);
    }
}
