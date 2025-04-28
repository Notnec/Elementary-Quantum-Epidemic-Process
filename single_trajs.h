#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <random>

#include "jumps.h"
#include "visual_aids.h"
#include "file_management.h"
#include "observables.h"
#include "QEP_filenames.h"
#include "initialisations.h"
#include "checks.h"

#include <stdlib.h>




long int count_class_pop(system_parameters spar5, patient** pat_tab5, char type){
    long int type_count = 0;
    for (int i = 0; i < spar5.LX; i++){
        for (int j = 0; j - spar5.LY; j++){
            if (pat_tab5[i][j].class_state == type){
                type_count += 1;
            }
        }
    }
    return (type_count);
}

long int steps_to_next_jump(system_parameters spar5, double jump_time5, double dt5, long int current_tick){
    double time_from_last_tick = spar5.what_time() - dt5*current_tick;
    if (time_from_last_tick < 0){
        cout << "ERROR! The system's time should always exceed the time of the last tick!\n";
        return -1;
    }
    else{
        return (long int) ((jump_time5 + time_from_last_tick) / dt5);
    }

}

double adjust_max_time(double max_time5, double dt5){
    return max_time5 + (dt5 / 10000.);
}



void run_one_traj_upto_njump(system_parameters &spar5, long int max_jnum){

    // LATTICE CREATION
    patient patient_lattice5[spar5.LX][spar5.LY];
    patient* lattice_ptr5[spar5.LX];
    for (int i = 0; i < spar5.LX; i++){
        lattice_ptr5[i] = patient_lattice5[i];
    }
    // END OF LATTICE CREATION

    // INITIALISATION OF OBSERVABLE ARRAYS
    double dt5 = 0.1;
    double max_time5 = 5.;
    max_time5 = adjust_max_time(max_time5, dt5);
    long int max_steps5 = ((long int) (max_time5 / dt5)) + 1;

    QEP_dyn_filenames Qff(spar5, 1, 1);

    lattice_dyn latdyn(spar5, &Qff, max_steps5, max_time5, dt5);

    vector<infected_info> inf_list5 = {};  // CREATION OF THE LIST OF INFECTED
    GEP_usual_init(spar5, lattice_ptr5, inf_list5);    // DEFINITION OF THE INITIAL STATE

    class_tally SCD_counts;

    SCD_counts.S = count_class_pop(spar5, lattice_ptr5, 'S');
    SCD_counts.C = count_class_pop(spar5, lattice_ptr5, 'I');
    SCD_counts.D = count_class_pop(spar5, lattice_ptr5, 'D');

    latdyn.add_config(spar5, lattice_ptr5, inf_list5, 0, SCD_counts);

    cout << "\n";
    show_configuration(spar5, lattice_ptr5);
    cout << "\n";

    long int ticks5 = 0, nticks5;
    long int nn = 1;
    while (nn <= max_jnum && inf_list5.size() > 0){
        jump_details jump5 = compute_next_jump(spar5, inf_list5);
        display_jump_properties(spar5, jump5, nn);
        nn++;

        inf_list5[0].disclose_info();
        inf_list5[0].display_records(spar5);

        nticks5 = steps_to_next_jump(spar5, jump5.time, dt5, ticks5);
        if (ticks5 + nticks5 > max_steps5 - 1){
            nticks5 = max_steps5 - ticks5 - 1;
        }
        if (nticks5 > 0){
            latdyn.add_on_several_steps(spar5, lattice_ptr5, inf_list5, ticks5 + 1, ticks5 + nticks5, SCD_counts);
            ticks5 += nticks5;
        }
        cout << "\n" << "At time t = " << dt5 * ticks5 << ":\n";
        latdyn.show_lattice_pop(spar5, 'I', ticks5);
        cout << "\n";

        if (jump5.type == 'i'){
            SCD_counts.S -= 1;
            SCD_counts.C += 1;
        }
        else{
            SCD_counts.C -= 1;
            SCD_counts.D += 1;
        }

        jumping_after_data_coll(spar5, lattice_ptr5, inf_list5, jump5);
        cout << "\n";
        show_configuration(spar5, lattice_ptr5);
        cout << "\n";
    }

    if (ticks5 < max_steps5 - 1){
        latdyn.add_on_several_steps(spar5, lattice_ptr5, inf_list5, ticks5 + 1, max_steps5 - 1, SCD_counts);
    }

    latdyn.save_to_files(spar5);

}




void run_one_traj_until_maxtime(system_parameters &spar5, double max_time5, double dt5, long int max_steps5, lattice_dyn &latdyn5){
    if (spar5.what_time() <= max_time5){

        // LATTICE CREATION
        patient patient_lattice5[spar5.LX][spar5.LY];
        patient* lattice_ptr5[spar5.LX];
        for (int i = 0; i < spar5.LX; i++){
            lattice_ptr5[i] = patient_lattice5[i];
        }
        // END OF LATTICE CREATION

        // INITIALISATION OF OBSERVABLE ARRAYS

        vector<infected_info> inf_list5 = {};  // CREATION OF THE LIST OF INFECTED
        GEP_usual_init(spar5, lattice_ptr5, inf_list5);    // DEFINITION OF THE INITIAL STATE

        class_tally SCD_counts;

        SCD_counts.S = count_class_pop(spar5, lattice_ptr5, 'S');
        SCD_counts.C = count_class_pop(spar5, lattice_ptr5, 'I');
        SCD_counts.D = count_class_pop(spar5, lattice_ptr5, 'D');

        latdyn5.add_config(spar5, lattice_ptr5, inf_list5, 0, SCD_counts);

        long int ticks5 = 0, nticks5;
        while (spar5.what_time() <= max_time5 && inf_list5.size() > 0){
            jump_details jump5 = compute_next_jump(spar5, inf_list5);

            nticks5 = steps_to_next_jump(spar5, jump5.time, dt5, ticks5);
            if (ticks5 + nticks5 > max_steps5 - 1){
                nticks5 = max_steps5 - ticks5 - 1;
            }
            if (nticks5 > 0){
                latdyn5.add_on_several_steps(spar5, lattice_ptr5, inf_list5, ticks5 + 1, ticks5 + nticks5, SCD_counts);
                ticks5 += nticks5;
            }

            if (jump5.type == 'i'){
                SCD_counts.S -= 1;
                SCD_counts.C += 1;
            }
            else if (jump5.type == 'd'){
                SCD_counts.C -= 1;
                SCD_counts.D += 1;
            }

            jumping_after_data_coll(spar5, lattice_ptr5, inf_list5, jump5);
        }

        if (ticks5 < max_steps5 - 1){
            latdyn5.add_on_several_steps(spar5, lattice_ptr5, inf_list5, ticks5 + 1, max_steps5 - 1, SCD_counts);
        }

    }
}


class_tally conclude_one_traj(system_parameters &spar5){

    // LATTICE CREATION
    patient patient_lattice5[spar5.LX][spar5.LY];
    patient* lattice_ptr5[spar5.LX];
    for (int i = 0; i < spar5.LX; i++){
        lattice_ptr5[i] = patient_lattice5[i];
    }
    // END OF LATTICE CREATION

    // INITIALISATION OF OBSERVABLE ARRAYS

    vector<infected_info> inf_list5 = {};  // CREATION OF THE LIST OF INFECTED
    GEP_usual_init(spar5, lattice_ptr5, inf_list5);    // DEFINITION OF THE INITIAL STATE

    long int Scount = count_class_pop(spar5, lattice_ptr5, 'S');
    long int Ccount = count_class_pop(spar5, lattice_ptr5, 'I');
    long int Dcount = count_class_pop(spar5, lattice_ptr5, 'D');

    while (inf_list5.size() > 0){
        jump_details jump5 = compute_next_jump(spar5, inf_list5);

        if (jump5.type == 'i'){
            Scount -= 1;
            Ccount += 1;
        }
        else{
            Ccount -= 1;
            Dcount += 1;
        }

        jumping_after_data_coll(spar5, lattice_ptr5, inf_list5, jump5);
    }

    class_tally aux_ct;
    aux_ct.S = Scount;
    aux_ct.C = Ccount;
    aux_ct.D = Dcount;
    if (Ccount != 0){
        cout << "ERROR! The model cannot be stationary if there are still Is around!\n";
    }
    return aux_ct;
}


class_tally conclude_one_traj_for_PD(system_parameters &spar5, string ID_times_fname){

    // LATTICE CREATION
    patient patient_lattice5[spar5.LX][spar5.LY];
    patient* lattice_ptr5[spar5.LX];
    for (int i = 0; i < spar5.LX; i++){
        lattice_ptr5[i] = patient_lattice5[i];
    }

    // END OF LATTICE CREATION

    // INITIALISATION OF OBSERVABLE ARRAYS

    vector<infected_info> inf_list5 = {};  // CREATION OF THE LIST OF INFECTED
    GEP_usual_init(spar5, lattice_ptr5, inf_list5);    // DEFINITION OF THE INITIAL STATE

    long int Scount = count_class_pop(spar5, lattice_ptr5, 'S');
    long int Ccount = count_class_pop(spar5, lattice_ptr5, 'I');
    long int Dcount = count_class_pop(spar5, lattice_ptr5, 'D');

    while (inf_list5.size() > 0){
        jump_details jump5 = compute_next_jump(spar5, inf_list5);

        if (jump5.type == 'i'){
            Scount -= 1;
            Ccount += 1;
        }
        else{
            Ccount -= 1;
            Dcount += 1;
        }

        jumping_after_data_coll(spar5, lattice_ptr5, inf_list5, jump5);
    }

    // RECORDING INFECTION AND DEATH TIMES FOR THIS TRAJECTORY
    ID_times_radial IDT5;
    IDT5.record_from_config(spar5, lattice_ptr5);
    IDT5.append_values(ID_times_fname);

    // RECORDING THE TOTAL FINAL NUMBER OF S, C AND D POPULATIONS FOR THIS TRAJECTORY.
    // REMEMBER THAT C IS I + B
    class_tally aux_ct;
    aux_ct.S = Scount;
    aux_ct.C = Ccount;
    aux_ct.D = Dcount;
    if (Ccount != 0){
        cout << "ERROR! The model cannot be stationary if there are still Is around!\n";
    }
    return aux_ct;
}
