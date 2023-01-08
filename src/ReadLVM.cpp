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