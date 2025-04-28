#ifndef VISUALAIDS_H
#define VISUALAIDS_H

#include "Patient_info.h"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>



//struct col{
//    string red = "\033[91m";
//    string yellow = "\033[93m";
//    string blue = "\033[94m";
//    string green = "\033[92m";
//    string white = "\033[37m";
//};



void show_configuration(system_parameters spar6, patient** pat_tab6){
    int x_size = spar6.LX;
    int y_size = spar6.LY;
    for (int i = 0; i < x_size; i++){
        for (int j = 0; j < y_size; j++){
            char cstate = pat_tab6[i][j].class_state;
            if (cstate == 'S'){
                cout << "\033[92m" << "* ";
            }
            else if (cstate == 'I'){
                cout << "\033[91m" << "* ";
            }
            else{
                cout << "\033[93m" << "* ";
            }
        }
        cout << "\n";
    }
    cout << "\033[37m";

}





#endif // VISUALAIDS_H
