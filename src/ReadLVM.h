#ifndef __READLVM__
#define __READLVM__

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <vector>
#include <cmath>

using namespace std;

typedef struct
{
    vector<double> Delay;
    vector<double> Intensity;
    vector<double> Fit;
} AutoCorrelation;

typedef struct
{
    vector<double> Pin;
    vector<double> Pout;
    vector<double> Ratio;
    vector<double> AngleCorr;
} SineType;



double SciNot(string val);

// double SciNot(double val);

vector<pair<double, double>> read_samples(const string &filename); 

AutoCorrelation read_txt(const string &filename);

SineType read_sine(const string &filename);

#endif