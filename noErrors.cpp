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

int main(int argc, char** argv) {


    string name = argv[1];

    TApplication app("app", NULL, NULL);
	TCanvas *c = new TCanvas();

    vector<pair<double,double>> values;
    values = read_samples(name.c_str());
    for (auto &point : values)
    {
        cout << "X = " << point.first << "\tY = " << point.second << endl; 
    }

    TGraph *gr = new TGraph();
	// TGraphErrors *gr = new TGraphErrors();

	double x, y;
	
	double min, max;
	
	min = 1000; // 0
	max = 1070; //values.size();

    for (int i = 0; i < values.size(); i++)
    {
		if ((values[i].first >= min) && (values[i].first <= max)) {
			gr->AddPoint(values[i].first, values[i].second);
		}
    }
    
	gr->SetLineWidth(2);
	gr->SetLineColor(4);
	// gr->SetMarkerColor(4);
	// gr->SetMarkerStyle(20);
	gr->GetXaxis()->CenterTitle();
	gr->GetXaxis()->SetTitle("X");
	gr->GetYaxis()->SetTitle("Y");

	gr->Draw("AP");

    c->Update();
	c->Draw();
	c->SaveAs((name.append("png").c_str()));

	TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
	rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
	app.Run("true");

}