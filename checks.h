#ifndef CHECKS_H
#define CHECKS_H


#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include <random>

#include "initialisations.h"
#include "jumps.h"


void display_all_patient_data(system_parameters spar4, patient** pat_tab4){
    for (int i = 0; i < spar4.LX; i++){
        for (int j = 0; j < spar4.LY; j++){
            pat_tab4[i][j].disclose_all();
        }
    }
    cout << "\n\n";
}


void init_state_for_checks(system_parameters spar4, patient **pat_tab4, vector<infected_info> &inf_list4){
    // WE ASSUME THAT inf_list IS PASSED AS AN EMPTY VECTOR
    int central_x = spar4.LX / 2;
    int central_y = spar4.LY / 2;
    init_infect_patient(pat_tab4, 0, 0);
    init_infect_patient(pat_tab4, spar4.LX - 1, spar4.LY - 1);
    init_infect_patient(pat_tab4, central_x, central_y);
    for (int i = 0; i < spar4.LX - 1; i++){
        init_slaughter_patient(pat_tab4, i, spar4.LY - 1);
    }
}

void init_cond1(system_parameters spar4, patient **pat_tab4, vector<infected_info> &inf_list4){
    init_allS_square_lattice(spar4, pat_tab4);
    init_state_for_checks(spar4, pat_tab4, inf_list4);
    inf_list4 = construct_inf_list(spar4, pat_tab4);
    set_init_NSs(spar4, pat_tab4);
}

void display_jump_properties(system_parameters spar4, jump_details jump4, long int num){
    cout << "Jump number " << num << " occurs at t = " << spar4.what_time() + jump4.time << " and is of type " << jump4.type << ".\n";
}


#endif // CHECKS_H
