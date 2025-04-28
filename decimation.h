#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include <random>

#include "patient_info.h"

struct jump_details{
    double time;
    char type;
    unsigned int pos;
};


bool decimation_round(system_parameters spar1, vector<infected_info> &list_inf, vector<int> &part_numbers, double &t_king){
    bool worth_continuing = 0;

    if (t_king < 0){
        t_king = list_inf[part_numbers[0]].larger_time();
    }

    if (!(part_numbers.size() > 0 && part_numbers.size() <= list_inf.size())){
        cout << "ERROR! The recorded participant numbers are wrong!" << "\n";
    }

    unsigned int j = 0;
    while (j < part_numbers.size()){
        if (list_inf[part_numbers[j]].smaller_time() > t_king){
            part_numbers.erase(part_numbers.begin() + j);
        }
        else if (list_inf[part_numbers[j]].larger_time() < t_king){
            t_king = list_inf[part_numbers[j]].larger_time();
            list_inf[part_numbers[j]].Newt_update_bounds(spar1);
            worth_continuing = worth_continuing || (!(list_inf[part_numbers[j]].small_enough_interval(spar1.time_thresh)));
            j++;
        }
        else{
            list_inf[part_numbers[j]].Newt_update_bounds(spar1);
            worth_continuing = worth_continuing || (!(list_inf[part_numbers[j]].small_enough_interval(spar1.time_thresh)));
            j++;
        }
    }

    return worth_continuing;
}

void create_participant_list(int l0, vector<int> &part_nums){
    for (int i = 0; i < l0; i++){
        part_nums.push_back(i);
    }
}

unsigned int pick_position(unsigned int l2){
    if (l2 == 1){
        return 0;
    }
    else{
        uniform_int_distribution<unsigned int> rand_int(0, l2 - 1);
        return rand_int(rand_gen);
    }
}


void decimation_game(system_parameters spar1, vector<infected_info> &list_inf0, jump_details &jump1){
    unsigned int l1 = list_inf0.size();
    unsigned int p_number;
    if (l1 == 0){
        cout << "ERROR! No decimation game can take place without infected!" << "\n";
        jump1.time = -1.;
    }
    else{
        vector<int> part_nums1 = {};
        create_participant_list(l1, part_nums1);
        double jump_time = -1.;
        bool worth_continuing1 = 1;
        while (part_nums1.size() > 1 && worth_continuing1){
            worth_continuing1 = decimation_round(spar1, list_inf0, part_nums1, jump_time);
        }
        if (worth_continuing1){
            p_number = part_nums1[0];
            while (!(list_inf0[p_number].small_enough_interval(spar1.time_thresh))){
                list_inf0[p_number].Newt_update_bounds(spar1);
            }
        }
        else{
            unsigned int p1 = pick_position(part_nums1.size());
            p_number = part_nums1[p1];
        }

        jump_time = list_inf0[p_number].midtime();

        jump1.pos = p_number;
        jump1.time = jump_time;
        jump1.type = list_inf0[p_number].select_jump_type(spar1);
    }
}


void classical_decimation(system_parameters spar1, vector<infected_info> &list_inf0, jump_details &jump1){
    unsigned int l1 = list_inf0.size();
    vector<double> temp_times = {};
    vector<int> temp_pos = {};
    if (l1 == 0){
        cout << "ERROR! No classical decimation game can take place without infected!" << "\n";
        jump1.time = -1.;
    }
    else{
        for (unsigned int i = 0; i < l1; i++){
            temp_pos.push_back(i);
            temp_times.push_back(list_inf0[i].class_get_jumptime(spar1));
        }
        while (temp_pos.size() > 1){
            if (temp_times[1] >= temp_times[0]){
                temp_pos.erase(temp_pos.begin() + 1);
                temp_times.erase(temp_times.begin() + 1);
            }
            else{
                temp_pos.erase(temp_pos.begin());
                temp_times.erase(temp_times.begin());
            }
        }
        jump1.time = temp_times[0];
        jump1.pos = temp_pos[0];
        jump1.type = list_inf0[jump1.pos].select_jump_type(spar1);
    }
}



