#ifndef INIT_H
#define INIT_H


#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include <random>

#include "Patient_info.h"



void init_allS_square_lattice(system_parameters spar0, patient** ptab0){

    for (int i = 0; i < spar0.LX; i++){
        for (int j = 0; j < spar0.LY; j++){
            ptab0[i][j].init_coords(spar0, i, j);
        }
    }
}

void init_infect_patient(patient** pat_tab, int x0, int y0){
    pat_tab[x0][y0].fall_ill(0.);
}

void init_slaughter_patient(patient** pat_tab, int x0, int y0){
    pat_tab[x0][y0].init_kill(0.);
}

void set_init_NSs(system_parameters spar0, patient** pat_tab){
    for (int i = 0; i < spar0.LX; i++){
        for (int j = 0; j < spar0.LY; j++){
            pat_tab[i][j].count_NS(spar0, pat_tab);
        }
    }
}

void create_1seed_init_state(system_parameters spar0, patient **pat_tab){
    // WE ASSUME THAT inf_list IS PASSED AS AN EMPTY VECTOR
    int central_x = spar0.LX / 2;
    int central_y = spar0.LY / 2;
    init_infect_patient(pat_tab, central_x, central_y);
}

void GEP_usual_init(system_parameters spar0, patient **pat_tab0, vector<infected_info> &inf_list0){
    init_allS_square_lattice(spar0, pat_tab0);
    create_1seed_init_state(spar0, pat_tab0);
    set_init_NSs(spar0, pat_tab0);
    inf_list0 = construct_inf_list(spar0, pat_tab0);
}




#endif // INIT_H
