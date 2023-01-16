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

#include <sys/stat.h>

#include "TF1.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "TApplication.h"
#include "TGraphErrors.h"

#include "src/ReadLVM.h"
#include "src/GetOptions.h"



int main(int argc, char** argv) 
{
	vector<double> x, y, fit; // ex,ey;
	double min, max;

	Options Opt;

    AutoCorrelation Values;

	if (Opt.getOptions(argc,argv) < 0 || Opt.h_opt == 1) {
        printf("%s\n", &Opt.help_message[0]);
        return -1;
    }
	if (!Opt.p_opt) {
		fprintf(stderr, "**Input file directory is mandatory. Use option '-p' <path/to/file>\n");
		return -1;
	}

	cout << "In main Title " << Opt.title << " Dir " << Opt.dir << endl;
	
    TApplication app("app", NULL, NULL);
	TCanvas *c = new TCanvas();

    // vector<pair<double,double>> values;
    // vector<pair<double,double>> errors;
	
    // values = read_samples(Opt.path.c_str());
    Values = read_txt(Opt.path.c_str());

	if (Opt.m_opt)
		min = Opt.minVal;
	else 
		min = 0;
	if (Opt.M_opt)
		max = Opt.maxVal;
	else 
		max = Values.Delay.size();

    // for (auto &point : values)
    // {
    //     cout << "X = " << point.first << "\tY = " << point.second << endl; 
    // }

    // TGraph *gr = new TGraph();
	
	// min = 1010; // 0
	// max = 1058; //values.size();

    for (int i = 0; i < Values.Delay.size(); i++)
    {
        if ((Values.Delay[i] >= min) && (Values.Delay[i] <= max)) {
            x.push_back(Values.Delay[i]);
            y.push_back(Values.Intensity[i]);
            fit.push_back(Values.Fit[i]);
			// ex.push_back (0.5);
            // ey.push_back(sqrt(Values.Intensity[i]));
		}
    }

    TMultiGraph *mg = new TMultiGraph();
	// TGraphErrors *gr = new TGraphErrors(x.size(), &x[0], &y[0], &ex[0], &ey[0]);
    TGraph *gr = new TGraph(Values.Delay.size(), &Values.Delay[0], &Values.Intensity[0]);

	if (Opt.t_opt)
		gr->SetTitle(Opt.GetTile());
	else
		gr->SetTitle(" ");

    // gr->SetTitle("EA 132 CI");

	// gr->SetLineWidth(2);
	// gr->SetLineColor(4);
	gr->SetMarkerColor(4);
	gr->SetMarkerStyle(7);
	gr->GetXaxis()->CenterTitle();
	gr->GetXaxis()->SetTitle("Delay [ps]");
	gr->GetYaxis()->SetTitle("Intensity [arb.u.]");

	if (Opt.f_opt) {
        TGraph *grfit = new TGraph(Values.Delay.size(), &Values.Delay[0], &Values.Intensity[0]);
        grfit->Draw("AC");
        mg->Add(grfit);
		// TF1* func = new TF1("func","[0]+[1]*exp( -0.5*( (x-[2])/[3] )**2 )"); //  [0]+[1]*exp( -0.5*( (x-[2])/[3] )**2 )
		// func->SetParNames("C1", "Amplitude", "Mean","Sigma");
		// func->SetParameters(0,300,1030, 5);
		// gr->Fit(func);
	}
	// [3] + [0]*exp(-0.5*pow((x-[1])/[2],2))
	// p0*exp(-0.5*((x-p1)/p2)^2)

	gr->Draw("AP");
    mg->Add(gr);

    c->Update();
	c->Draw();

	string name = Opt.GetTile();
	if (Opt.d_opt) {
		string auxDir = Opt.dir;
		auxDir.append(name);
		c->SaveAs(((auxDir.append(".png")).c_str()));
		printf("Saved in %s\n", &auxDir[0]);
	}
	else if (Opt.d_opt == 0) {
		// default save
		string defDir = "bin/";
		defDir.append(name);
		defDir.append(".png");
		c->SaveAs((defDir.c_str()));
	}
	else {
		cout << "Output not saved" << endl;
	}
	// c->SaveAs((name.append("_lorentz.png").c_str()));

	TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
	rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
	app.Run("true");
}