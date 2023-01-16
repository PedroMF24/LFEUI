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

#include "TH1D.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TRootCanvas.h"
#include "TApplication.h"
#include "TMultiGraph.h"

using namespace std;

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

    vector<string> nameOfFiles = list_dir("/home/pmfig/dev/LFEUI/data/Sessao2/C_Iris/Espectros/");

    vector<pair<double,double>> values;

    TApplication app("app", NULL, NULL);
    TCanvas *c = new TCanvas("canvas", "canvas", 1400, 800);

    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle("Spectral Broadening CI");
	
    for (int i = 0; i < nameOfFiles.size(); i++)
    {
        // nameOfFiles[i].erase(nameOfFiles[i].find_last_not_of("\r\n") + 1);
        string FilePath = "data/Sessao2/C_Iris/Espectros/";
        FilePath.append(nameOfFiles[i]);
        values = read_samples(FilePath.c_str());
        
        for (int j = 0; j < values.size(); j++)
        {
            if ((values[j].first >= 1010) && (values[j].first <= 1060)) {
                x.push_back(values[j].first);
                y.push_back(values[j].second);
            }
        }
        TGraph *gr = new TGraph(x.size(), &x[0], &y[0]);
        // gr->SetTitle(" ");
        // gr->SetLineWidth(2);
        // gr->SetLineColor(i);
        gr->SetMarkerColor(i+1);
        gr->SetMarkerStyle(7);
        mg->Add(gr);
        x.clear();
        y.clear();
    }

    mg->GetXaxis()->CenterTitle();
    mg->GetXaxis()->SetTitle("#lambda [nm]");
    mg->GetYaxis()->SetTitle("Counts");
	mg->Draw("AP");
    c->Update();
    c->SaveAs("bin/Spectral Broadening CI.png");

	TRootCanvas *rc = (TRootCanvas *)c->GetCanvasImp();
	rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
	app.Run("true");
    return 0;
}