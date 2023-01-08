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
#include "TStyle.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "TApplication.h"
#include "TGraphErrors.h"

#include "src/ReadLVM.h"
#include "src/GetOptions.h"



int main(int argc, char** argv) 
{
	vector<double> x,y,ex,ey;
	double min, max;

	Options Opt;

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

    vector<pair<double,double>> values;
    vector<pair<double,double>> errors;
	
    values = read_samples(Opt.path.c_str());

	if (Opt.m_opt)
		min = Opt.minVal;
	else 
		min = 0;
	if (Opt.M_opt)
		max = Opt.maxVal;
	else 
		max = values.size();

    // for (auto &point : values)
    // {
    //     cout << "X = " << point.first << "\tY = " << point.second << endl; 
    // }

    // TGraph *gr = new TGraph();
	
	// min = 1010; // 0
	// max = 1058; //values.size();

    for (int i = 0; i < values.size(); i++)
    {
        if ((values[i].first >= min) && (values[i].first <= max)) {
            x.push_back(values[i].first);
            y.push_back(values[i].second);
			ex.push_back (0.5);
            ey.push_back(sqrt(values[i].second));
		}
    }

	TGraphErrors *gr = new TGraphErrors(x.size(), &x[0], &y[0], &ex[0], &ey[0]);

	if (Opt.t_opt)
		gr->SetTitle(Opt.GetTile());
	else
		gr->SetTitle(" ");

    // gr->SetTitle("EA 132 CI");

	gr->SetLineWidth(2);
	gr->SetLineColor(4);
	// gr->SetMarkerColor(4);
	// gr->SetMarkerStyle(20);
	gr->GetXaxis()->CenterTitle();
	gr->GetXaxis()->SetTitle("#lambda [nm]");
	gr->GetYaxis()->SetTitle("Counts");

	// TF1* func = new TF1("func","chebyshev"); //  [0]+[1]*exp( -0.5*( (x-[2])/[3] )**2 )
    //func->SetParNames("C1", "C2", "m","s");
	// func->SetParameters(0,300,1030, 8);

	// gr->Fit(func);
	// [3] + [0]*exp(-0.5*pow((x-[1])/[2],2))
	// p0*exp(-0.5*((x-p1)/p2)^2)

	gr->Draw("AP");

    c->Update();
	c->Draw();
	if (Opt.d_opt) {
		string auxDir = Opt.dir;
		c->SaveAs(((auxDir.append("Graph.png")).c_str()));
		printf("Saved in %s\n", &auxDir[0]);
	}
	else if (Opt.d_opt == 0) {
		// default save
		string defDir = "bin/";
		defDir.append("Graph.png");
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