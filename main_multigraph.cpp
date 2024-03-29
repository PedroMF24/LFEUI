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

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <vector>
#include <cmath>

#include <dirent.h>
#include <sys/types.h>

#include <sys/stat.h>

#include "TF1.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "TApplication.h"
#include "TMultiGraph.h"
#include "TLegend.h"

using namespace std;

string removeLvm(string str) 
{ 
    int n = str.length(); 
    // Check if string has ".lvm" at 
    // the end 
    if (n >= 4 && str.substr(n - 4) == ".lvm") 
        str.erase(n - 4, 4); 
    return str;
}

vector<pair<double, double>> read_samples(const string &filename) { // double* integ_time, 

    vector<pair<double,double>> values;

    // Open the LabVIEW Measurement file
    ifstream lvm_file(filename);

    // Check if the file was successfully opened
    if (!lvm_file.is_open()) {
        cerr << "Error: Unable to open LabVIEW Measurement file." << endl;
        exit(0);
    }

    string line = "Deez nuts";

    getline(lvm_file, line);
    // Split line
    // cout << line;
    
    while(line.compare("X_Value,Wavelengths,Spectral intensity,Comment\r") != 0)
        getline(lvm_file, line);
    getline(lvm_file, line); // Discard X values

        stringstream ss(line);
        string index;
        string wavelength;
        string intensity;
        string integ_time;

        getline(ss, index,',');
        getline(ss, wavelength, ',');
        getline(ss, intensity, ',');
        getline(ss, integ_time, ',');
        values.emplace_back(stod(wavelength), stod(intensity));
        // cout << "Integ time: " << integ_time << endl;

    // Read values
    while(getline(lvm_file, line)) {
        stringstream ss(line);
        getline(ss, index,',');
        getline(ss, wavelength, ',');
        getline(ss, intensity, ',');
        values.emplace_back(stod(wavelength), stod(intensity));
    }
    lvm_file.close();

    return values;
}


// #include "src/ReadLVM.h"
// #include "src/GetOptions.h"


vector<string> list_dir(const char *path) {
    vector<string> AllFilesName;
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        fprintf(stderr, "**Given directory is empty\n");
        exit(0);
    }

    //readdir return a pointer to the entry of a folder (could be any type not only .txt)

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            AllFilesName.push_back(entry->d_name);
            cout << entry->d_name << endl;
        }
    // if (entry->d_name == "." || entry->d_name == "..")
    //     continue;
    // AllFilesName.push_back(entry->d_name);
    //push the name of every file
    }
    closedir(dir);
    return AllFilesName; 
}

int main(int argc, char** argv) 
{
	vector<double> x, y;
	double min, max;

    vector<string> nameOfFiles = list_dir("/home/pmfig/dev/LFEUI/data/Sessao3/Espectros/");
    // vector<string> nameOfFiles = list_dir("/home/pmfig/dev/LFEUI/data/Sessao2/C_Iris/Espectros/");
    // vector<string> nameOfFiles = list_dir("/home/pmfig/dev/LFEUI/data/Sessao2/S_Iris/Espectros/");

    vector<pair<double,double>> values;

    TApplication app("app", NULL, NULL);
    TCanvas *c = new TCanvas("canvas", "canvas", 1400, 800);

    TMultiGraph *mg = new TMultiGraph();
    TGraph *grRef = new TGraph();
    TLegend* legend = new TLegend(0.80,0.50,0.9,0.9); // S_iris 0.80,0.65 : C_Iris 0.80,0.6 : 
    mg->SetTitle("Spectral Broadening S3");
    // mg->SetTitle("Spectral Broadening CI S2");
    // mg->SetTitle("Spectral Broadening SI S2");
	
    for (int i = 0; i < nameOfFiles.size(); i++)
    {
        // nameOfFiles[i].erase(nameOfFiles[i].find_last_not_of("\r\n") + 1);

        string FilePath = "data/Sessao3/Espectros/";
        // string FilePath = "data/Sessao2/C_Iris/Espectros/";
        // string FilePath = "data/Sessao2/S_Iris/Espectros/";
        

        FilePath.append(nameOfFiles[i]);
        values = read_samples(FilePath.c_str());
        
        TGraph *gr = new TGraph(x.size(), &x[0], &y[0]);
        if (nameOfFiles[i] == "Espectro_manha.lvm") {
            for (int j = 0; j < values.size(); j++)
            {
                if ((values[j].first >= 1010) && (values[j].first <= 1060)) {
                    // x.push_back(values[j].first/5.3);
                    // y.push_back(values[j].second/5.3);
                    gr->AddPoint(values[j].first, values[j].second/8); // C_Iris/4.7 S_Iris/8
                }
            }
            TF1* func = new TF1("func","gaus");
            func->SetLineColor(kRed);
            func->SetLineWidth(2);
            gr->Fit("func");
            cout << "Got here Manhã - Red\n";
            gr->SetMarkerColor(0);
            gr->SetMarkerStyle(9);
            gr->SetMarkerSize(0.1);
            func->Draw("SAME");
            mg->Add(gr);

        }
        else if (nameOfFiles[i] == "Ref_Spectre.lvm") {
            for (int j = 0; j < values.size(); j++)
            {
                if ((values[j].first >= 1010) && (values[j].first <= 1060)) {
                    // x.push_back(values[j].first/5.3);
                    // y.push_back(values[j].second/5.3);
                    gr->AddPoint(values[j].first, values[j].second*1.70);
                }
            }
            // TF1* func = new TF1("func","gaus");
            // func->SetLineColor(kBlue);
            // func->SetLineWidth(2);
            // gr->Fit("func");
            // cout << "Got here Tarde - Blue\n";
            // gr->SetMarkerColor(0);
            // gr->SetMarkerStyle(9);
            // gr->SetMarkerSize(0.1);
            // func->Draw("SAME");
            gr->SetLineColor(41);
            gr->SetLineWidth(2);
            // gr->Draw("AC");
            legend->AddEntry(gr,"Ref Curve","l");
            grRef = gr;
            // mg->Add(gr);
        }
        else {
            for (int j = 0; j < values.size(); j++)
            {
                if ((values[j].first >= 1010) && (values[j].first <= 1060)) {
                    // x.push_back(values[j].first);
                    // y.push_back(values[j].second);
                    gr->AddPoint(values[j].first, values[j].second);
                }
            }
            if (i < 9)
                gr->SetMarkerColor(i+1);
            else 
                gr->SetMarkerColor(i+30);
            gr->SetMarkerStyle(7);
            legend->AddEntry(gr,removeLvm(nameOfFiles[i]).c_str(), "p");
            mg->Add(gr);
        }
        // gr->SetTitle(" ");
        // gr->SetLineWidth(2);
        // gr->SetLineColor(i);
        // gr->SetMarkerColor(i+1);
        // gr->SetMarkerStyle(7);
        // mg->Add(gr);
        x.clear();
        y.clear();

        
        // if (nameOfFiles[i] == "Espectro_de_referencia.lvm") {
        //     TGraph *gr = new TGraph(x.size(), &x[0], &y[0]);
        //     TF1* func = new TF1("func","gaus");
        //     gr->Fit("func");
        //     func->Draw("SAME");
        //     cout << "Got here\n";
        //     // gr->SetTitle(" ");
        //     // gr->SetLineWidth(2);
        //     // gr->SetLineColor(i);
        //     gr->SetMarkerColor(i+1);
        //     gr->SetMarkerStyle(7);
        //     mg->Add(gr);
        // }
        // x.clear();
        // y.clear();
    }

    mg->GetXaxis()->CenterTitle();
    mg->GetXaxis()->SetTitle("#lambda [nm]");
    mg->GetYaxis()->SetTitle("Counts");
    
	mg->Draw("AP"); // AP
    grRef->Draw("SAME");
    


    legend->SetHeader("#lambda/2 angle [#circ]","C"); // option "C" allows to center the header
    // legend->AddEntry("f1","Function abs(#frac{sin(x)}{x})","l");
    legend->Draw();

    // c->BuildLegend();
    c->Update();

    c->SaveAs("bin/Spectral Broadening S3.png");
    // c->SaveAs("bin/Spectral Broadening CI S2.png");
    // c->SaveAs("bin/Spectral Broadening SI S2.png");

	TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
	rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
	app.Run("true");
    return 0;
}