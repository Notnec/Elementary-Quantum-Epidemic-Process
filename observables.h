#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <typeinfo>

#include <random>

#include "jumps.h"
#include "visual_aids.h"
#include "file_management.h"
#include "QEP_filenames.h"

#include <stdlib.h>

bool ovrwrt5 = 0;

struct class_tally{
    long int S = 0;
    long int C = 0;
    long int D = 0;
};

class QEP_PD_tally{
    private:
        class_tally tally;
        vector<long int> D_tally_by_traj = {};
    public:

        void add_to_tally(class_tally to_add){
            tally.S += to_add.S;
            tally.C += to_add.C;
            tally.D += to_add.D;
        }

        void initialise(){
            tally.S = 0;
            tally.C = 0;
            tally.D = 0;
            D_tally_by_traj = {};
        }

        void add_traj_to_tally(class_tally to_add){
            D_tally_by_traj.push_back(to_add.D);
            add_to_tally(to_add);
        }

        void save_tallies_by_fnames(phase_diag PD5, unsigned int gi_counter, unsigned int ohm_counter, string globalD_fname, string globalSCD_fname, string by_traj_fname){
            if(!(PD5.valid_indices(gi_counter, ohm_counter))){
                cout << "ERROR! Invalid indices passed to 'save_tallies'!\n";
            }
            else{
                ofstream glob_SCD_f, glob_D_f, by_traj_f;
                glob_D_f.open(globalD_fname, ios_base::app);
                glob_SCD_f.open(globalSCD_fname, ios_base::app);
                by_traj_f.open(by_traj_fname, ios_base::app);
                glob_SCD_f << PD5.gi_values[gi_counter] << " " << PD5.ohm_values[ohm_counter] << " ";
                glob_D_f << PD5.gi_values[gi_counter] << " " << PD5.ohm_values[ohm_counter] << " ";
                by_traj_f << PD5.gi_values[gi_counter] << " " << PD5.ohm_values[ohm_counter];

                glob_SCD_f << (double) tally.S / PD5.return_ntraj() << " " << (double) tally.C / PD5.return_ntraj() << " ";
                glob_SCD_f << (double) tally.D / PD5.return_ntraj();
                glob_D_f << (double) tally.D / PD5.return_ntraj();

                for (unsigned int k = 0; k < D_tally_by_traj.size(); k++){
                    by_traj_f << " " << D_tally_by_traj[k];
                }

                if (!(PD5.final_indices(gi_counter, ohm_counter))){
                    glob_SCD_f << endl;
                    glob_D_f << endl;
                    by_traj_f << endl;
                }

                glob_SCD_f.close();
                glob_D_f.close();
                by_traj_f.close();
            }
        }

        void save_tallies(phase_diag PD5, unsigned int gi_counter, unsigned int ohm_counter, QEP_PD_filenames Qff5){
            if(!(PD5.valid_indices(gi_counter, ohm_counter))){
                cout << "ERROR! Invalid indices passed to 'save_tallies'!\n";
            }
            else{
                ofstream glob_SCD_f, glob_D_f, by_traj_f;
                glob_D_f.open(Qff5.globalD_fname, ios_base::app);
                glob_SCD_f.open(Qff5.globalSCD_fname, ios_base::app);
                by_traj_f.open(Qff5.D_bytraj_fname, ios_base::app);
                glob_SCD_f << PD5.gi_values[gi_counter] << " " << PD5.ohm_values[ohm_counter] << " ";
                glob_D_f << PD5.gi_values[gi_counter] << " " << PD5.ohm_values[ohm_counter] << " ";
                by_traj_f << PD5.gi_values[gi_counter] << " " << PD5.ohm_values[ohm_counter];

                glob_SCD_f << (double) tally.S / PD5.return_ntraj() << " " << (double) tally.C / PD5.return_ntraj() << " ";
                glob_SCD_f << (double) tally.D / PD5.return_ntraj();
                glob_D_f << (double) tally.D / PD5.return_ntraj();

                for (unsigned int k = 0; k < D_tally_by_traj.size(); k++){
                    by_traj_f << " " << D_tally_by_traj[k];
                }

                if (!(PD5.final_indices(gi_counter, ohm_counter))){
                    glob_SCD_f << endl;
                    glob_D_f << endl;
                    by_traj_f << endl;
                }

                glob_SCD_f.close();
                glob_D_f.close();
                by_traj_f.close();
            }
        }



};


struct class_pops{
    double S;
    double C;
    double D;
};



class dyn_single_obs{
    private:
    public:
        vector<double> value_sequence;
        double tmax, dt;
        long int n_points;

        dyn_single_obs(double tmax0, double dt0){
            n_points = ((long int) (tmax0 / dt0)) + 1;
            for (long int i = 0; i < n_points; i++){
                value_sequence.push_back(0.);
            }
            tmax = tmax0;
            dt = dt0;
        }

        dyn_single_obs(){

        }

        long int get_n_points(){
            return n_points;
        }

        void add_at_step(long int step, double value5){
            if (step >= 0 && step < n_points){
                value_sequence[step] += value5;
            }
            else{
                cout << "ERROR! Data collection step outside of boundaries!\n";
            }
        }

        void add_several(long int step_i, long int step_f, double value5){
            if (step_i >= 0 && step_i < n_points && step_f >= 0 && step_f >= step_i){
                if (step_f < n_points){
                    for (long int i = step_i; i <= step_f; i++){
                        value_sequence[i] += value5;
                    }
                }
                else{
                    for (long int i = step_i; i < n_points; i++){
                        value_sequence[i] += value5;
                    }
                }

            }
            else{
                cout << "ERROR! Data collection outside of boundaries!\n";
            }
        }

        void divide_by_traj_num(long int n_trajs5){
            for (long int i = 0; i < n_points; i++){
                value_sequence[i] /= n_trajs5;
            }
        }

        void save_to_file(string fname){
            fname = choose_fname(fname, ovrwrt5);
            ofstream file5;
            file5.open(fname);
            if (file5.is_open()){
                for (long int i = 0; i < n_points; i++){
                    file5 << value_sequence[i] << " ";
                }
                file5 << "\n";
            }
        }

};


class lattice_dyn{
    private:
        vector<vector<dyn_single_obs>> lat_map[4] = {};
        dyn_single_obs pop_squares[5];  // Element 4 corresponds to the I + B one, which we record for convenience
        QEP_dyn_filenames* ptr_Qfnames;
    public:
        double tmax, dt;
        long int n_steps, n_trajs;


        lattice_dyn(system_parameters spar5, QEP_dyn_filenames* ptr_Qfnames0, long int n0, double tmax0, double dt0, long int n_trajs0 = 1){

            ptr_Qfnames = ptr_Qfnames0;
            n_trajs = n_trajs0;

            for (int tp = 0; tp < 4; tp++){
                pop_squares[tp] = dyn_single_obs(tmax0, dt0);
                for (int i = 0; i < spar5.LX; i++){
                    vector<dyn_single_obs> aux_vec;
                    for (int j = 0; j < spar5.LY; j++){
                        aux_vec.push_back(dyn_single_obs(tmax0, dt0));
                    }
                    lat_map[tp].push_back(aux_vec);
                }
            }
            pop_squares[4] = dyn_single_obs(tmax0, dt0);

            tmax = tmax0;
            dt = dt0;
            n_steps = ((long int) (tmax0 / dt0)) + 1;
            if (n_steps != n0){
                cout << "ERROR! The total number of steps does not match the parameters!\n";
            }
        }

        void set_SCD_squares(system_parameters spar5, class_tally ct05, int step){
            double auxd = (double) ct05.S / spar5.tot_size;
            pop_squares[0].value_sequence[step] += auxd * auxd;
            auxd = (double) ct05.D / spar5.tot_size;
            pop_squares[3].value_sequence[step] += auxd * auxd;
            auxd = (double) ct05.C / spar5.tot_size;
            pop_squares[4].value_sequence[step] += auxd*auxd;
        }

        void add_config(system_parameters spar5, patient** pat_tab5, vector<infected_info> inf_list5, int step, class_tally ct5){
            set_SCD_squares(spar5, ct5, step);

            double aux_comp[2];
            int aux_x, aux_y;
            for (int i = 0; i < spar5.LX; i++){
                for (int j = 0; j < spar5.LY; j++){
                    char c_state = pat_tab5[i][j].class_state;
                    if (c_state == 'S'){
                        lat_map[0][i][j].value_sequence[step] += 1;
                    }
                    else if (c_state == 'D'){
                        lat_map[3][i][j].value_sequence[step] += 1;
                    }
                }
            }
            double auxI = 0, auxB = 0;
            for (unsigned long int j = 0; j < inf_list5.size(); j++){
                inf_list5[j].encode_quantum_components(spar5, step*dt - spar5.what_time(), aux_comp);
                aux_x = inf_list5[j].extract_x();
                aux_y = inf_list5[j].extract_y();
                lat_map[1][aux_x][aux_y].value_sequence[step] += aux_comp[0];
                lat_map[2][aux_x][aux_y].value_sequence[step] += aux_comp[1];
                auxI += aux_comp[0] / spar5.tot_size;
                auxB += aux_comp[1] / spar5.tot_size;
            }
            pop_squares[1].value_sequence[step] += auxI * auxI;
            pop_squares[2].value_sequence[step] += auxB * auxB;
        }


        void add_on_several_steps(system_parameters spar5, patient** pat_tab5, vector<infected_info> inf_list5, int i_step, int f_step, class_tally ct5){
            for (int stp = i_step; stp <= f_step; stp++){
                add_config(spar5, pat_tab5, inf_list5, stp, ct5);
            }
        }

        void save_to_files(system_parameters spar5){

            if ((*ptr_Qfnames).fine_to_proceed()){

                ofstream T_file;
                string fnames[4] = (*ptr_Qfnames).popmovie_fnames;

                for (int tp = 0; tp < 4; tp++){
                    T_file.open(fnames[tp]);
                    for (long int step = 0; step < n_steps; step++){
                        for (int i = 0; i < spar5.LX; i++){
                            for (int j = 0; j < spar5.LY; j++){
                                T_file << lat_map[tp][i][j].value_sequence[step];
                                if (j != spar5.LY - 1){
                                    T_file << " ";
                                }
                            }
                            T_file << "\n";
                        }
                        T_file << "\n";
                    }
                    T_file.close();
                }
            }
            else{
                cout << "WARNING! Filenames not yet determined!\n";
            }
        }

        void set_traj_number(long int n_trajs0){
            n_trajs = n_trajs0;
        }

        void get_sizes(){
            cout << "Sizes: " << lat_map[0].size() << "  " << lat_map[0][0].size() << "  " << lat_map[0][0][0].value_sequence.size() << "\n";
            cout << n_steps << "\n";
        }


        void show_lattice_pop(system_parameters spar5, char ptype, long int step){
            int aux_int = pop_to_int(ptype);

            for (int i = 0; i < spar5.LX; i++){
                for (int j = 0; j < spar5.LY; j++){
                    cout << lat_map[aux_int][i][j].value_sequence[step];
                    if (j != spar5.LY){
                        cout << " ";
                    }
                }
                cout << "\n";
            }
            cout << "\n";
        }


        double average_pop_at_step(system_parameters spar5, char ptype, long int step){
            int tp = pop_to_int(ptype);
            if (step >= 0 && step < n_steps){
                double aux_tot = 0;

                for (int i = 0; i < spar5.LX; i++){
                    for (int j = 0; j < spar5.LY; j++){
                        aux_tot += lat_map[tp][i][j].value_sequence[step];
                    }
                }
                return (aux_tot / spar5.tot_size);
            }
            else{
                return -1.;
            }
        }

        double compute_stdev(double square, double average){
            double auxvar = square - average*average;
            if (auxvar > 0){
                return sqrt(square - average*average);
            }
            else if(auxvar > -0.0000001){
                return 0;
            }
            else{
                cout << "ERROR! Negative variance detected!\n";
                return -1;
            }
        }

        void save_aggregate_pop_type(system_parameters spar5, char ptype, long int n_traj5){
            ofstream aggr_f;
            string fname = (*ptr_Qfnames).globalpop_evol_fnames[pop_to_int(ptype)];

            aggr_f.open(fname);
            for (long int ss = 0; ss < n_steps; ++ss){
                aggr_f << dt * ss << " " << (average_pop_at_step(spar5, ptype, ss) / n_traj5) << " " << (pop_squares[pop_to_int(ptype)].value_sequence[ss] / n_traj5);
                aggr_f << " " << compute_stdev((pop_squares[pop_to_int(ptype)].value_sequence[ss] / n_traj5), (average_pop_at_step(spar5, ptype, ss) / n_traj5));
                if (ss != n_steps - 1){
                    aggr_f << "\n";
                }
            }
            aggr_f.close();
        }

        void save_aggregate_pops(system_parameters spar5, long int n_traj5){
            if ((*ptr_Qfnames).fine_to_proceed()){
                for (int i = 0; i < 4; i++){
                    save_aggregate_pop_type(spar5, int_to_pop(i), n_traj5);
                }
            }
            else{
                cout << "WARNING! Filenames not yet prepared!\n";
            }
        }

        void save_local_av_pops(system_parameters spar5, int posx, int posy, long int n_traj5){
            if (!spar5.is_inside(posx, posy)){
                cout << "ERROR! Trying to save the local population of a non-existing site!\n";
            }
            else{
                ofstream loc_f;
                string base_fname = "Cpp_pop_at_", fname;
                base_fname = base_fname + "[" + to_string(posx) + ", " + to_string(posy) + "]";
                base_fname += ".txt";
                fname = choose_fname(add_size_fo_fname(base_fname, spar5.LX, spar5.LY), ovrwrt5);

                loc_f.open(fname);
                for (long int ss = 0; ss < n_steps; ++ss){
                    loc_f << dt * ss;
                    for (int i = 0; i < 4; i++){
                        loc_f << " " << lat_map[i][posx][posy].value_sequence[ss] / n_traj5;
                    }
                    if (ss != n_steps - 1){
                        loc_f << endl;
                    }
                }
            }
        }

};




class global_obs{
    private:
    public:
        vector<double> global_values;
        long int n_trajs;


        global_obs(long int n_trajs0 = 1){

            n_trajs = n_trajs0;
            for (long int i = 0; i < n_trajs; i++){
                global_values.push_back(0.);
            }
        }


        void add_value(long int pos, double val0){
            if (pos >= 0 && pos < n_trajs){
                global_values[pos] += val0;
            }
            else{
                cout << "ERROR! Invalid position in the array of values!\n";
            }
        }

        void add_new_value(double val0){
            global_values.push_back(val0);
            n_trajs++;
        }

        bool check_consistency1(){
            return global_values.size() == (unsigned long int) n_trajs;
        }

        void save_to_file(system_parameters spar6, string fname6){
            ofstream loc_f;
            string base_fname = fname6, fname;
            fname = choose_fname(add_size_fo_fname(base_fname, spar6.LX, spar6.LY), ovrwrt5);
            loc_f.open(fname);
            for (long int i = 0; i < n_trajs; i++){
                loc_f << global_values[i];
                if (i != n_trajs - 1){
                    loc_f << " ";
                }
            }
            loc_f.close();
        }
};



class average_densities{
    private:
    public:
        vector<global_obs> class_densities;
        long int n_trajs;

        average_densities(long int n_trajs0){
            for (int j = 0; j < 4; j++){
                class_densities.push_back(global_obs(n_trajs));
            }
        }

        void add_value_of_type(long int pos, double val0, char ptype){
            int aux_p = pop_to_int(ptype);
            if (aux_p >= 0 && aux_p < 4){
                class_densities[aux_p].add_value(pos, val0);
            }
            else{
                cout << "ERROR! Invalid population type in average densities class!\n";
            }
        }

        void add_new_value_of_type(double val0, char ptype){
            int aux_p = pop_to_int(ptype);
            if (aux_p >= 0 && aux_p < 4){
                class_densities[aux_p].add_new_value(val0);
            }
            else{
                cout << "ERROR! Invalid population type in final densities class!\n";
            }
        }

        void add_new_values(double vals[4]){
            for (int i = 0; i < 4; i++){
                class_densities[i].add_new_value(vals[i]);
            }
            n_trajs++;
        }

        bool consistency_check1(){
            bool auxb = 1;
            for (int i = 1; i < 4; i++){
                auxb = auxb && (class_densities[0].n_trajs == class_densities[i].n_trajs);
            }
            return auxb;
        }

        bool consistency_check2(){
            bool auxb = 1;
            for (int i = 0; i < 4; i++){
                auxb = auxb && class_densities[i].check_consistency1();
            }
            return auxb;
        }

        bool consistency_check3(){
            bool auxb = 1;
            for (int i = 0; i < 4; i++){
                auxb = auxb && (class_densities[i].n_trajs == n_trajs);
            }
            return auxb;
        }

        bool consistency_check_set1(){
            return consistency_check1() && consistency_check2() && consistency_check3();

        }

};


template <class Th>
class histo_on_reals{
    private:
    public:
        int nbins;
        double inf_xlim, sup_xlim;
        vector<double[2]> intervals;
        vector<double> midpoints;
        vector<Th> heights;

        histo_on_reals(int nbins0, double inf_x0, double sup_x0){
            nbins = nbins0;
            if (inf_x0 <= sup_x0){
                inf_xlim = inf_x0;
                sup_xlim = sup_x0;
            }
            else{
                cout << "ERROR! Wrong delimiters for the histogram!\n";
                inf_xlim = 0;
                sup_xlim = 0;
            }

            double auxl, auxr;
            if (typeid(Th).name() == typeid(int).name() || typeid(Th).name() == typeid(long int).name() || typeid(Th).name() == typeid(double).name()){
                for (int i = 0; i < nbins; i++){
                    heights.push_back(0);
                    auxl = left_bound(i);
                    auxr = right_bound(i);
                    intervals.push_back({auxl, auxr});
                    midpoints.push_back((auxr - auxl) / 2.);
                }
            }
            else{
                cout << "ERROR! Histogram class needs a numerical type!\n";
            }
        }

        double left_bound(int bin){
            return inf_xlim + bin*((sup_xlim - inf_xlim) / nbins);
        }

        double right_bound(int bin){
            return inf_xlim + (bin + 1)*((sup_xlim - inf_xlim) / nbins);
        }

        void add_value_at_pos(int pos, Th val0){
            if (pos >= 0 && pos < nbins){
                heights[pos] += val0;
            }
            else{
                cout << "ERROR! Wrong position in the histogram!\n";
            }
        }

};


struct class_event{
    double ti;
    double td;
    int radius;
};

class ID_times_radial{
    private:
        vector<class_event> triplets;
    public:
        ID_times_radial(){
            triplets = {};
        }

        void record_new_point(class_event clev0){
            triplets.push_back(clev0);
        }

        void record_from_config(system_parameters spar5, patient** pat_tab5){
            class_event clev1;
            for (int i = 0; i < spar5.LX; i++){
                for (int j = 0; j < spar5.LY; j++){
                    clev1.radius = pat_tab5[i][j].approx_dist_from_orig;
                    clev1.ti = pat_tab5[i][j].return_inf_time();
                    if (clev1.ti >= 0){
                        clev1.td = pat_tab5[i][j].return_d_time();
                        if (clev1.td > clev1.ti){
                            record_new_point(clev1);
                        }
                    }
                }
            }
        }


        void append_values(string filename){
            long int l0 = triplets.size();
            ofstream tri_file;
            tri_file.open(filename, ios_base::app);
            for (int i = 0; i < l0; ++i){
                tri_file << triplets[i].radius << " ";
                tri_file << triplets[i].ti << " ";
                tri_file << triplets[i].td;
                tri_file << "\n";
            }
            tri_file.close();
        }
};




