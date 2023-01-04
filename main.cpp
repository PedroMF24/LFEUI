/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "TF1.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "TApplication.h"
#include "TGraphErrors.h"

#include "src/ReadLVM.h"

int main() {

    string name = "data/RefSpectrum.lvm";

    TApplication app("app", NULL, NULL);
	TCanvas *c = new TCanvas();

    vector<pair<double,double>> values;
    values = read_samples(name.c_str());
    for (auto &point : values)
    {
        cout << "X = " << point.first << "\tY = " << point.second << endl; 
    }

    TGraph *gr = new TGraph();

	double x, y;
	
	double min, max;
	
	min = 0; // 0
	max = values.size(); //values.size();

    for (int i = min; i < max; i++)
    {
        gr->AddPoint(values[i].first, values[i].second);
    }
    
	gr->SetLineWidth(2);
	gr->SetLineColor(4);
	// gr->SetMarkerColor(4);
	// gr->SetMarkerStyle(20);
	gr->GetXaxis()->CenterTitle();
	gr->GetXaxis()->SetTitle("X");
	gr->GetYaxis()->SetTitle("Y");

	gr->Draw();

    c->Update();
	c->Draw();
	c->SaveAs((name.append(".png").c_str()));

	TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
	rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
	app.Run("true");

}