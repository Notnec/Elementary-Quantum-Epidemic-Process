#ifndef JUMPS_H
#define JUMPS_H


#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include <random>

#include "decimation.h"
//#include "checks.h"


// This library aims to collect all functions & procedure dealing with performing all the necessary updates from the moment a jump has
// been selected to the point where it has been performed and the time has come again to determine the next jump.

jump_details compute_next_jump(system_parameters spar2, vector<infected_info> &inf_list2){
    jump_details jump2;
    if (spar2.is_classical()){
        classical_decimation(spar2, inf_list2, jump2);
    }
    else{
        decimation_game(spar2, inf_list2, jump2);
    }
    return jump2;
}


/* First we need to update the qvecs to Jtime-, then we move to Jtime+ and update the NSs, after which we can rewrite the records
 and recalculate the time pairs.  */

void update_to_death_event(system_parameters &spar2, vector<infected_info> &inf_list2, jump_details jump2){
    spar2.time_passed(jump2.time);
    inf_list2[jump2.pos].death_occurred(spar2.what_time());
    inf_list2.erase(inf_list2.begin() + jump2.pos);

    unsigned int l2 = inf_list2.size();
    // First, we update the surviving qvecs
    for (unsigned int i = 0; i < l2; i++){
        inf_list2[i].update_vec_and_u_after_death(spar2, jump2.time);
    }
}

void update_after_death(system_parameters spar2, vector<infected_info> &inf_list2){
    // Here we move to immediately after the death event; we need to write the records and recalculate the time pairs,
    // at least in the case ohm != 0. Remember that inf_list2 here is already missing the dead site.

    if (!spar2.is_classical()){
        unsigned int l2 = inf_list2.size();
        for (unsigned int i = 0; i < l2; i++){
            inf_list2[i].write_records(spar2);
            inf_list2[i].set_pair_of_time_bounds(spar2);
        }
    }
}

void which_neighbour(system_parameters spar2, patient** pat_tab, int xcoord, int ycoord, int n_crd[2]){
    vector<int> candidates = {0, 1, 2, 3};
    bool found = 0;
    int x1 = -1, y1 = -1;
    while (!found){
        unsigned int aux_l = candidates.size();
        uniform_int_distribution<unsigned int> rand_int(0, aux_l - 1);
        unsigned int aux_pos = rand_int(rand_gen);
        x1 = xcoord + spar2.neighbours[candidates[aux_pos]][0];
        y1 = ycoord + spar2.neighbours[candidates[aux_pos]][1];
        if (spar2.is_inside(x1, y1)){
            if (pat_tab[x1][y1].class_state == 'S'){
                found = 1;
            }
            else{
                candidates.erase(candidates.begin() + aux_pos);
            }
        }
        else{
            candidates.erase(candidates.begin() + aux_pos);
        }
    }
    if (x1 < 0 || y1 < 0){
        cout << "ERROR! An infection has been requested for a site without S neighbours!\n";
    }
    else{
        n_crd[0] = x1;
        n_crd[1] = y1;
    }
}

void update_NSs(system_parameters spar2, patient** pat_tab, int new_inf_x, int new_inf_y){
    int x1, y1;
    for (unsigned int i = 0; i < 4; i++){
        x1 = new_inf_x + spar2.neighbours[i][0];
        y1 = new_inf_y + spar2.neighbours[i][1];
        if (spar2.is_inside(x1, y1)){
            pat_tab[x1][y1].pat_NS_down();
        }
    }
}


void update_to_inf_event(system_parameters &spar2, patient** pat_table, vector<infected_info> &inf_list2, jump_details jump2){
    spar2.time_passed(jump2.time);
    int n_coord[2];
    which_neighbour(spar2, pat_table, inf_list2[jump2.pos].extract_x(), inf_list2[jump2.pos].extract_y(), n_coord);
    unsigned int aux_l = inf_list2.size();
    inf_list2.push_back(infected_info(spar2, &(pat_table[n_coord[0]][n_coord[1]])));
    inf_list2.back().inf_occurred(spar2.what_time());
    /* We now have a vector of infected; the last element is the new one, which needs no update of qvec. Hence we only
    perform the update for elements between 0 and the next-to-last element, corresponding to position aux_l - 1.  */
    // UPDATING THE qvecs for all old elements

    for (unsigned int i = 0; i < aux_l; i++){
        inf_list2[i].update_vec_and_u_after_infection(spar2, jump2.time, i == jump2.pos);
    }
    // NOW WE NEED TO UPDATE THE VALUES OF NS FOR ALL INVOLVED SITES
    update_NSs(spar2, pat_table, n_coord[0], n_coord[1]);
}

void update_after_infection(system_parameters spar2, vector<infected_info> &inf_list2){
    if (!spar2.is_classical()){
        unsigned int aux_l = inf_list2.size();
        for (unsigned int i = 0; i < aux_l; i++){
            inf_list2[i].write_records(spar2);
            inf_list2[i].set_pair_of_time_bounds(spar2);
        }
    }
}


void complete_a_jump(system_parameters &spar2, patient** pat_table, vector<infected_info> &inf_list2){

    jump_details jump1 = compute_next_jump(spar2, inf_list2);

    // HERE IS WHERE THE CALCULATION OF OBSERVABLES SHOULD TAKE PLACE.
    // HENCE, THIS FUNCTION IS FOR TESTING ONLY. RECORDING OF OBSERVABLES IS MORE CONVENIENTLY DONE IN trajectories.h

    if (jump1.type == 'd'){
        update_to_death_event(spar2, inf_list2, jump1);
        update_after_death(spar2, inf_list2);
    }
    else if(jump1.type == 'i'){
        update_to_inf_event(spar2, pat_table, inf_list2, jump1);
        update_after_infection(spar2, inf_list2);
    }
    else if(jump1.type == 'n'){
        cout << "ERROR! Jump not recognised!\n";
    }
    else{
        cout << "ERROR! Jump type not even 'n'!!!" << "\n";
    }
}

void jumping_after_data_coll(system_parameters &spar2, patient** pat_table, vector<infected_info> &inf_list2, jump_details jump1){

    if (jump1.type == 'd'){
//        cout << "Death taking place at (" << inf_list2[jump1.pos].extract_x() << ", " << inf_list2[jump1.pos].extract_y() << ").\n\n";
        update_to_death_event(spar2, inf_list2, jump1);
        update_after_death(spar2, inf_list2);
    }
    else if(jump1.type == 'i'){
        update_to_inf_event(spar2, pat_table, inf_list2, jump1);
        update_after_infection(spar2, inf_list2);
    }
    else if(jump1.type == 'n'){
        cout << "ERROR! Jump not recognised!\n";
    }
    else{
        cout << "ERROR! Jump type not even 'n'!!!" << "\n";
    }
}

#endif // JUMPS_H
