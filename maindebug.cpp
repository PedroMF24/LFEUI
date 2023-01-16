#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <fstream>

using namespace std;

typedef struct
{
    vector<double> Delay;
    vector<double> Intensity;
    vector<double> Fit;
} AutoCorrelation;

double SciNot(string val) {
    int exp;
    double mantissa;
    double doubleVal = stod(val);
    mantissa = frexp(doubleVal, &exp);
    return ldexp(mantissa, exp);
}

AutoCorrelation read_txt(const string &filename) {

    AutoCorrelation Data;
    Data.Delay.clear();
    Data.Fit.clear();
    Data.Intensity.clear();

    // Open APE file
    ifstream txt_file(filename);

    // Check if the file was successfully opened
    if (!txt_file.is_open()) {
        cerr << "Error: Unable to open LabVIEW Measurement file." << endl;
        exit(0);
    }

    string line = "Deez nuts";

    getline(txt_file, line);
    // Split line
    // cout << line;
    
    while(line.compare("Delay[ps]\tIntensity [arb.u.]\tFit [arb.u.]\r") != 0) {
        getline(txt_file, line);
        cout << line << endl;
    }
    getline(txt_file, line); // Discard separator '# =========='

        stringstream ss(line);
        string delay;
        string intensity;
        string fit;

    // Read values
    while(getline(txt_file, line)) {
        stringstream ss(line);
        getline(ss, delay,'\t');
        getline(ss, intensity, '\t');
        getline(ss, fit, '\t');
        Data.Delay.push_back(stod(delay));
        Data.Intensity.push_back(stod(intensity));
        Data.Fit.push_back(stod(fit));
        cout << stod(delay) << " " << stod(intensity) << " " << stod(fit) << endl;

    }
    txt_file.close();

    return Data;
}

int main() {
    read_txt("data/Sessao2/Autocorrelacao_tarde.txt");
    return 0;
}