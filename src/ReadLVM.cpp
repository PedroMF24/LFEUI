#include "ReadLVM.h"

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
        // cout << stod(delay) << " " << stod(intensity) << " " << stod(fit) << endl;

    }
    txt_file.close();

    return Data;
}

SineType read_sine(const string &filename) {

    SineType Data;

    ifstream txt_file(filename);

    // Check if the file was successfully opened
    if (!txt_file.is_open()) {
        cerr << "Error: Unable to open LabVIEW Measurement file." << endl;
        exit(0);
    }

    string line = "Deez nuts";

    getline(txt_file, line); // Discard separator Header

        stringstream ss(line);
        string angle;
        string angleCorr;
        string ratio;
        string Pin;
        string Pout;

    // Read values
    while(getline(txt_file, line)) {
        stringstream ss(line);
        getline(ss, angle,'\t');
        getline(ss, angleCorr, '\t');
        getline(ss, ratio, '\t');
        getline(ss, Pin, '\t');
        getline(ss, Pout, '\t');

        Data.AngleCorr.push_back(stod(angleCorr));
        Data.Ratio.push_back(stod(ratio));
        Data.Pin.push_back(stod(Pin));
        Data.Pout.push_back(stod(Pout));

        // cout << stod(delay) << " " << stod(intensity) << " " << stod(fit) << endl;
    }
    txt_file.close();

    return Data;
}