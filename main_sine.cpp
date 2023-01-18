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
#include <algorithm>

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

double degToRad(double deg) 
{ 
    //Converting degrees to radians 
    return (deg * M_PI / 180.0); 
}

int main(int argc, char** argv) 
{
	vector<double> x,y,ex,ey;
	double min, max;

	Options Opt;

	SineType Data;

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
	
    Data = read_sine(Opt.path.c_str());
	vector<double>::iterator it;
	it = max_element(Data.AngleCorr.begin(), Data.AngleCorr.end());

	if (Opt.m_opt)
		min = Opt.minVal;
	else
		min = 0;
	if (Opt.M_opt)
		max = Opt.maxVal;
	else 
		max = *it;

    // for (auto &point : values)
    // {
    //     cout << "X = " << point.first << "\tY = " << point.second << endl; 
    // }

    // TGraph *gr = new TGraph();
	
	// min = 1010; // 0
	// max = 1058; //values.size();

	min = 4.8;
	max = 6.5;
    for (int i = 0; i < Data.AngleCorr.size(); i++)
    {
        //if ((degToRad(Data.AngleCorr[i]) >= min) && (degToRad(Data.AngleCorr[i]) <= max)) {
            x.push_back(degToRad(Data.AngleCorr[i]));
            y.push_back(Data.Ratio[i]);
			ex.push_back (degToRad(2));
			double dP = 3;
			double dRatio = ( (1/Data.Pin[i]) + (Data.Pout[i]/(Data.Pin[i]*Data.Pin[i])) )*dP;
            ey.push_back(dRatio);
		//}
    }

	TGraphErrors *gr = new TGraphErrors(x.size(), &x[0], &y[0], &ex[0], &ey[0]);
	// TGraph *gr = new TGraph(x.size(), &x[0], &y[0]);

	if (Opt.t_opt)
		gr->SetTitle(Opt.GetTile());
	else
		gr->SetTitle("Sine");

    // gr->SetTitle("EA 132 CI");

	gr->SetLineWidth(2);
	gr->SetLineColor(4);
	// gr->SetMarkerColor(4);
	// gr->SetMarkerStyle(20);
	gr->GetXaxis()->CenterTitle();
	gr->GetXaxis()->SetTitle("Angle of #lambda/2 [rad]");
	gr->GetYaxis()->SetTitle("P_{out} / P_{in}");

	if (Opt.f_opt) {
		TF1* func = new TF1("func","[0]+[1]*sin([2] + [3]*x)"); //  [0]+[1]*exp( -0.5*( (x-[2])/[3] )**2 )
		func->SetParNames("Offset", "Amplitude","Phase", "Frequency");
		func->SetParameters(0.5, 0.5, 0, 3);
		gr->Fit(func);
		// func->Draw();
	}
	// [3] + [0]*exp(-0.5*pow((x-[1])/[2],2))
	// p0*exp(-0.5*((x-p1)/p2)^2)

	gr->Draw("AP");
	// TF1* func = new TF1("f", "0.5+0.5*sin(2*x)", 0, 6);
	// func->Draw();

    c->Update();
	// c->Draw();

	string name = Opt.GetTile();
	if (Opt.d_opt) {
		string auxDir = Opt.dir;
		auxDir.append(name);
		// c->SaveAs(((auxDir.append(".png")).c_str()));
		printf("Saved in %s\n", &auxDir[0]);
	}
	else if (Opt.d_opt == 0) {
		// default save
		string defDir = "bin/";
		defDir.append(name);
		defDir.append(".png");
		// c->SaveAs((defDir.c_str()));
	}
	else {
		cout << "Output not saved" << endl;
	}
	// c->SaveAs((name.append("_lorentz.png").c_str()));

	TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
	rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
	app.Run("true");
}