#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

#include <random>

#include "jumps.h"
#include "visual_aids.h"
#include "file_management.h"
//#include "checks.h"
#include "single_trajs.h"
#include "QEP_filenames.h"

#include <stdlib.h>


void display_trajectory_progress(unsigned int tt5, unsigned long int n_traj5, int counting_freq){
    unsigned int auxi = 100 * (tt5 + 1) / n_traj5, auxi2 = 100 * (tt5) / n_traj5;
    if (auxi / counting_freq != auxi2 / counting_freq){
        cout << auxi << "% ... ";
    }
    if (auxi == 100){
        cout << endl;
    }
}




void run_def_trajs_up_to_maxtime(system_parameters spar5, double max_time5, double dt5, unsigned long int n_traj5){

    max_time5 = adjust_max_time(max_time5, dt5);
    long int max_steps5 = ((long int) (max_time5 / dt5)) + 1;

    QEP_dyn_filenames Qff(spar5, n_traj5, 0);

    lattice_dyn latdyn05(spar5, &Qff, max_steps5, max_time5, dt5, n_traj5);

    for (unsigned int tt = 1; tt <= n_traj5; ++tt){
        spar5.reset_time();
        run_one_traj_until_maxtime(spar5, max_time5, dt5, max_steps5, latdyn05);
        display_trajectory_progress(tt, n_traj5, 10);
    }

    if (Qff.fine_to_proceed()){
        Qff.save_parameters(spar5, n_traj5, max_time5, dt5);
        latdyn05.save_to_files(spar5);
        latdyn05.save_aggregate_pops(spar5, n_traj5);
    }

}


void run_undef_trajs_up_to_maxtime(system_parameters spar5, double max_time5, double dt5){

    max_time5 = adjust_max_time(max_time5, dt5);
    long int max_steps5 = ((long int) (max_time5 / dt5)) + 1;

    QEP_dyn_filenames Qff;

    lattice_dyn latdyn05(spar5, &Qff, max_steps5, max_time5, dt5);

    long int n_trajs5 = 0;

    do
    {
        spar5.reset_time();
        run_one_traj_until_maxtime(spar5, max_time5, dt5, max_steps5, latdyn05);
        n_trajs5++;
    }
    while (!file_already_exists("end.txt"));

    cout << "Simulation ended at " << n_trajs5 << " trajectories.\n";

    Qff = QEP_dyn_filenames(spar5, n_trajs5, 0);
    latdyn05.n_trajs = n_trajs5;

    if (Qff.fine_to_proceed()){
        Qff.save_parameters(spar5, n_trajs5, max_time5, dt5);
        latdyn05.save_to_files(spar5);
        latdyn05.save_aggregate_pops(spar5, n_trajs5);
    }
    else{
        cout << "WARNING! Trouble with the filenames!\n";
    }
}


void PD_scan_for_def_traj_num(phase_diag PD5, unsigned long int n_traj5){

    QEP_PD_filenames Qff(PD5, n_traj5, 0); /* We can define the filenames now since we already know the tot number of trajectories */

    system_parameters spar5;

    QEP_PD_tally final_pop_count5;

    if (PD5.well_def){
        Qff.save_parameters(PD5, n_traj5);
        int num_gis5, num_ohms5;
        num_gis5 = PD5.gis_number();
        num_ohms5 = PD5.ohms_number();
        for (int i = 0; i < num_gis5; i++){  // Loop on gamma_Is
            for (int j = 0; j < num_ohms5; j++){  // Loop on Omegas
                final_pop_count5.initialise();
                PD5.print_pair_number(i, j);
                PD5.fix_sys_parameters(spar5, i, j);
                for (unsigned long int tt = 0; tt < n_traj5; ++tt){  // Loop on fixed number of trajectories
                    spar5.reset_time(); // We fix t = 0 for the initial state for every trajectory
                    final_pop_count5.add_traj_to_tally(conclude_one_traj_for_PD(spar5, Qff.find_corresponding_filename(PD5, i, j)));
                    display_trajectory_progress(tt, n_traj5, 10);
                }
                final_pop_count5.save_tallies(PD5, i, j, Qff);
            }
        }
    }
    else{
        cout << "ERROR! Trying to launch trajectories without properly defined parameters.\n";
    }
}



class comput_record{
    private:
        vector<long unsigned int> time_sums;
        vector<long unsigned int> max_times;
    public:

        vector<long unsigned int> cumulative_times;
        vector<long unsigned int> cumulative_max;

        comput_record(phase_diag PD5){
            unsigned int num_of_components = PD5.gis_number();
            for (unsigned int i = 0; i < num_of_components; ++i){
                time_sums.push_back(0);
                max_times.push_back(0);

                cumulative_times.push_back(0);
                cumulative_max.push_back(0);
            }
        }

        void new_time(unsigned int elaps5, unsigned int gi_counter){
            time_sums[gi_counter] += elaps5;
            if (max_times[gi_counter] <= elaps5){
                max_times[gi_counter] = elaps5;
            }
        }

        void produce_cumulative_sums(unsigned int gi_counter, unsigned int n_traj5){
            unsigned long int auxt = 0, auxt2 = 0;
            unsigned int auxs = time_sums.size();

            for (unsigned int i = auxs - 1; i >= gi_counter; --i){
                auxt += time_sums[i] / n_traj5;
                auxt2 += max_times[i];
                cumulative_times[i] = auxt;
                cumulative_max[i] = auxt2;
            }
        }


        unsigned int return_sum_element(unsigned int gi_counter, unsigned int n_traj5){
            return (long unsigned int) (time_sums[gi_counter] / n_traj5);
        }

        unsigned int return_max_element(unsigned int gi_counter){
            return max_times[gi_counter];
        }
};


void PD_scan_for_undef_traj_num(phase_diag PD5){
    long int n_traj5 = 0;
    system_parameters spar5;

    int num_gis5, num_ohms5;
    num_gis5 = PD5.gis_number();
    num_ohms5 = PD5.ohms_number();

    time_t init, endt, vart;
    unsigned long int elapst;
    struct tm * timeinfo;
    char buffer [80];

    QEP_PD_tally final_pop_count5[num_gis5][num_ohms5];

    QEP_PD_temp_filenames temp_Qff(PD5);

    comput_record c_rec(PD5);

//    rename_endfile();

    do
    {
        if (n_traj5 == 0){
            cout << "First sweep initiated. Estimates on duration will only be available once the next one starts.\n";
        }
        for (int i = 0; i < num_gis5; i++){  // Loop on gamma_Is

            init = chrono::system_clock::to_time_t(chrono::system_clock::now());

            if (file_already_exists("end.txt")){
                cout << "Termination file 'end.txt' detected. Proceeding to complete the current sweep.\n";
                cout << "Please note that deleting/renaming the file before said completion will cancel the termination request and let the code continue.\n";
                if (n_traj5 > 0){
                    c_rec.produce_cumulative_sums(i, n_traj5);

                    timeinfo = localtime(&init);
                    strftime (buffer, 80, "on %d-%m-%Y at %H:%M", timeinfo);

                    cout << "Producing the " << i + 1 << "-th gamma_I value data of " << num_gis5 << ". ";
                    cout << "Starting " << buffer << ".\n";

                    if (i < num_gis5 - 1){
                        vart = init + c_rec.return_sum_element(i, n_traj5);
                        timeinfo = localtime(&vart);
                        strftime (buffer, 80, "%d-%m-%Y at %H:%M", timeinfo);

                        cout << "The next gamma_I value will probably start around " << buffer << endl;

                        vart = init + c_rec.return_max_element(i);
                        timeinfo = localtime(&vart);
                        strftime (buffer, 80, "%d-%m-%Y at %H:%M", timeinfo);

                        cout << "or, if unlucky, probably not beyond " << buffer << "." << endl;
                    }

                    vart = init + c_rec.cumulative_times[i];
                    timeinfo = localtime(&vart);
                    strftime (buffer, 80, "%d-%m-%Y at %H:%M", timeinfo);

                    cout << endl << colour.blue << "Estimated ending time: " << buffer << ".\n";

                    vart = init + c_rec.cumulative_max[i];
                    timeinfo = localtime(&vart);
                    strftime (buffer, 80, "%d-%m-%Y at %H:%M", timeinfo);

                    cout << "Estimated upper bound: " << buffer << ".\n";
                    cout << "Estimates have been built upon the times it took for the code to go through the previous loops." << endl << "Please note that they are subject to error.\n";
                    cout << endl << colour.white << endl;
                }
            }

            for (int j = 0; j < num_ohms5; j++){  // Loop on Omegas
                PD5.fix_sys_parameters(spar5, i, j);
                final_pop_count5[i][j].add_traj_to_tally(conclude_one_traj_for_PD(spar5, temp_Qff.find_corresponding_filename(PD5, i, j)));
            }
            endt = chrono::system_clock::to_time_t(chrono::system_clock::now());
            elapst = endt - init;
            c_rec.new_time(elapst, i);

        }
        n_traj5++;
        if (n_traj5 == 1){
            cout << "First sweep completed (1 trajectory per PD point).\n";
        }
    }
    while (!file_already_exists("end.txt"));

    PD5.update_traj_num(n_traj5);
    QEP_PD_filenames Qff(PD5, n_traj5);
    Qff.save_parameters(PD5, n_traj5);

    for (int i = 0; i < num_gis5; i++){  // Loop on gamma_Is
        for (int j = 0; j < num_ohms5; j++){  // Loop on Omegas

            rename_file(temp_Qff.find_corresponding_filename(PD5, i, j), Qff.find_corresponding_filename(PD5, i, j));
            final_pop_count5[i][j].save_tallies(PD5, i, j, Qff);
        }
    }
}
