#ifndef __GETOPTIONS__
#define __GETOPTIONS__

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <vector>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

class Options {
    public:
        Options();
        int getOptions(int argc, char **argv);
        int isString(string str);
        void SetDir(string newDir);
        void SetPath(string newPath);
        void SetTitle(string newTitle);

        const char* GetTile();

    // private:
    int h_opt, d_opt, t_opt, m_opt, M_opt, p_opt, f_opt;
    string dir, title, path, help_message;
    double minVal, maxVal;
};



#endif