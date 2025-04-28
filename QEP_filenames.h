#ifndef QEPF_H
#define QEPF_H

#include "Patient_info.h"
#include "file_management.h"
#include "Inputs.h"
#include "Phase_diag_boundaries.h"


string instance_fname = "Instances.txt";


class QEP_dyn_filenames{
    private:
        string base_params_fname = "Cpp_Parameters.txt";
        string base_popmovie_fname = "Cpp_Lattice_map.txt";
        string base_globalpop_evol_fname = "Cpp_tot_pop.txt";
//        string base_squarepop_fname = "Cpp_square_pop.txt";

        string base_classifier = "batch";
        string base_traj_identifier = "t";
        string current_classifier;

        string popmovie_folder = "Density_movies/";
        string globalpop_folder = "Population_evolution/";
//        string finalpop_folder = "Stationary_populations/";

        bool all_names_set = 0;
        bool overwrite_last;
    public:
        string params_fname, popmovie_fnames[4], globalpop_evol_fnames[4];   //, squarepop_fnames[5];

        QEP_dyn_filenames(system_parameters spar7, long int n_trajs7, bool overwrite_last0 = 0){
            overwrite_last = overwrite_last0;
            params_fname = add_pars_to_fname(base_params_fname, spar7.LX, spar7.LY, base_traj_identifier, n_trajs7);
            current_classifier = determine_classifier(params_fname, base_classifier, overwrite_last);
            params_fname = insert_before_ext(params_fname, "_" + current_classifier);

            distinguish_pops_everywhere();

            for (int i = 0; i < 4; i++){
                popmovie_fnames[i] = popmovie_folder + fully_classify(popmovie_fnames[i], spar7, n_trajs7);
                globalpop_evol_fnames[i] = globalpop_folder + fully_classify(globalpop_evol_fnames[i], spar7, n_trajs7);
//                squarepop_fnames[i] = finalpop_folder + fully_classify(squarepop_fnames[i], spar7, n_trajs7);
            }
            all_names_set = 1;
        }

        QEP_dyn_filenames(){
            all_names_set = 0;
        }

        bool fine_to_proceed(){
            return all_names_set;
        }

        string fully_classify (string base_fname, system_parameters spar7, long int n_trajs7){
            string aux_fname = add_pars_to_fname(base_fname, spar7.LX, spar7.LY, base_traj_identifier, n_trajs7);
            return insert_before_ext(aux_fname, "_" + current_classifier);
        }

        void distinguish_pops(string base_fname, string str_array[4]){
            for (int i = 0; i < 4; i++){
                str_array[i] = insert_before_ext(base_fname, int_to_str_pop(i));
            }
        }

        void distinguish_pops_everywhere(){
            distinguish_pops(base_popmovie_fname, popmovie_fnames);
            distinguish_pops(base_globalpop_evol_fname, globalpop_evol_fnames);
//            distinguish_pops(base_finalpop_fname, finalpop_fnames);
        }

        void save_parameters(system_parameters spar7, long int n_trajs7, double max_time7 = -1, double dt7 = -1){
            ofstream pfile;
            pfile.open(params_fname);
            pfile << "LX: " << spar7.LX << "\n";
            pfile << "LY: " << spar7.LY << "\n";
            pfile << "gd: " << spar7.gd << "\n";
            pfile << "gi: " << spar7.gi << "\n";
            pfile << "Omega: " << spar7.ohm << "\n";
            if (max_time7 > 0 && dt7 > 0){
                pfile << "Max time: " << max_time7 << "\n";
                pfile << "dt: " << dt7 << "\n";
            }
            pfile << "num of trajectories: " << n_trajs7;

            pfile.close();
        }
};



class QEP_PD_filenames{
    private:
        string fundamental_incipit = "PD_";
        string fundamental_slice_constg = "Slice_gfixed_";
        string fundamental_slice_consto = "Slice_ofixed_";
        string fundamental_point = "PDPoint_";

        string fund_base_params_fname = "Cpp_Parameters.txt";
        string fund_base_par_pairs_list_fname = "Cpp_PDpoints.txt";
        string fund_base_ID_times_fname = "Cpp_IDtimes.txt";
        string fund_base_globalD_fname = "Cpp_Dpop.txt";

        string fund_base_globalSCD_fname = "Cpp_SCDpop.txt";
        string fund_base_Dcount_by_traj_fname = "Cpp_D_by_traj.txt";

        string base_params_fname = "PD_Cpp_Parameters.txt";
        string base_par_pairs_list_fname = "PD_Cpp_PDpoints.txt";
        string base_ID_times_fname = "PD_Cpp_IDtimes.txt";
        string base_globalD_fname = "PD_Cpp_Dpop.txt";

        string base_globalSCD_fname = "PD_Cpp_SCDpop.txt";
        string base_Dcount_by_traj_fname = "PD_Cpp_D_by_traj.txt";


        string base_classifier = "A1_batch";
        string base_traj_identifier = "t";
        string base_gi_classifier = "g";
        string base_ohm_classifier = "o";
        string current_classifier;

        string IDtimes_folder = "PD_ID_times/";
        string globalD_folder = "PD_Final_population/";

        bool all_names_set = 0;
        bool overwrite_last;
    public:
        string params_fname, par_pairs_fname, globalD_fname, globalSCD_fname, D_bytraj_fname, aux_IDt_str, aux_str;
        vector<string> ID_times_fnames = {};

        QEP_PD_filenames(phase_diag PD7, long int n_trajs7, bool overwrite_last0 = 0){

            if (PD7.slice[0] && PD7.slice[1]){
                string base_params_fname = fundamental_point + fund_base_params_fname;
                string base_par_pairs_list_fname = fundamental_point + fund_base_par_pairs_list_fname;
                string base_ID_times_fname = fundamental_point + fund_base_ID_times_fname;
                string base_globalD_fname = fundamental_point + fund_base_globalD_fname;
                string base_globalSCD_fname = fundamental_point + fund_base_globalSCD_fname;
                string base_Dcount_by_traj_fname = fundamental_point + fund_base_Dcount_by_traj_fname;
            }
            else if (PD7.slice[0]){
                string base_params_fname = fundamental_slice_constg + fund_base_params_fname;
                string base_par_pairs_list_fname = fundamental_slice_constg + fund_base_par_pairs_list_fname;
                string base_ID_times_fname = fundamental_slice_constg + fund_base_ID_times_fname;
                string base_globalD_fname = fundamental_slice_constg + fund_base_globalD_fname;
                string base_globalSCD_fname = fundamental_slice_constg + fund_base_globalSCD_fname;
                string base_Dcount_by_traj_fname = fundamental_slice_constg + fund_base_Dcount_by_traj_fname;
            }
            else if (PD7.slice[1]){
                string base_params_fname = fundamental_slice_consto + fund_base_params_fname;
                string base_par_pairs_list_fname = fundamental_slice_consto + fund_base_par_pairs_list_fname;
                string base_ID_times_fname = fundamental_slice_consto + fund_base_ID_times_fname;
                string base_globalD_fname = fundamental_slice_consto + fund_base_globalD_fname;
                string base_globalSCD_fname = fundamental_slice_consto + fund_base_globalSCD_fname;
                string base_Dcount_by_traj_fname = fundamental_slice_consto + fund_base_Dcount_by_traj_fname;
            }

            if (file_already_exists(instance_fname)){
                unsigned int auxi;
                ifstream auxf1(instance_fname);
                auxf1 >> auxi;
                auxf1.close();
                if (auxi > 0){
                    auxi--;
                }
                ofstream auxf2(instance_fname);
                auxf2 << auxi;
                auxf2.close();
            }

            overwrite_last = overwrite_last0;
            params_fname = add_pars_to_fname(base_params_fname, PD7.xsize(), PD7.ysize(), base_traj_identifier, n_trajs7);
            current_classifier = determine_classifier(params_fname, base_classifier, overwrite_last);
            params_fname = insert_before_ext(params_fname, "_" + current_classifier);

            par_pairs_fname = fully_classify(base_par_pairs_list_fname, PD7, n_trajs7);
            globalD_fname = globalD_folder + fully_classify(base_globalD_fname, PD7, n_trajs7);
            globalSCD_fname = globalD_folder + fully_classify(base_globalSCD_fname, PD7, n_trajs7);
            D_bytraj_fname = globalD_folder + fully_classify(base_Dcount_by_traj_fname, PD7, n_trajs7);
//            aux_IDt_str = IDtimes_folder + fully_classify(base_ID_times_fname, PD7, n_trajs7);
            aux_IDt_str = IDtimes_folder + add_pars_to_fname(base_ID_times_fname, PD7.xsize(), PD7.ysize(), base_traj_identifier, n_trajs7);

            int num_gis7, num_ohms7;
            num_gis7 = PD7.gis_number();
            num_ohms7 = PD7.ohms_number();
            for (int i = 0; i < num_gis7; i++){
                for (int j = 0; j < num_ohms7; j++){
                    aux_str = distinguish_par_pairs(aux_IDt_str, base_gi_classifier, i, num_gis7);
                    aux_str = distinguish_par_pairs(aux_str, base_ohm_classifier, j, num_ohms7);
                    aux_str = insert_before_ext(aux_str, "_" + current_classifier);
                    ID_times_fnames.push_back(aux_str);
                }
            }

            all_names_set = 1;
        }

        string distinguish_par_pairs(string base_fname, string base_classifier7, int par_counter, int par_number){
            return insert_before_ext(base_fname, "_" + base_classifier7 + to_string(par_counter + 1) + "_" + to_string(par_number));
        }

        QEP_PD_filenames(){
            all_names_set = 0;
        }

        bool fine_to_proceed(){
            return all_names_set;
        }

        string fully_classify (string base_fname, phase_diag PD7, long int n_trajs7){
            string aux_fname = add_pars_to_fname(base_fname, PD7.xsize(), PD7.ysize(), base_traj_identifier, n_trajs7);
            return insert_before_ext(aux_fname, "_" + current_classifier);
        }


        void save_parameters(phase_diag PD7, long int n_trajs7){

            if (all_names_set){
                ofstream pfile;

                int num_gis7 = PD7.gis_number();
                int num_ohms7 = PD7.ohms_number();

                pfile.open(params_fname);
                pfile << PD_labels[0] << PD7.xsize() << "\n";
                pfile << PD_labels[1] << PD7.ysize() << "\n";
                pfile << PD_labels[2] << PD7.return_gi_min() << "\n";
                pfile << PD_labels[3] << PD7.return_gi_max() << "\n";
                pfile << PD_labels[4] << num_gis7 << "\n";
                pfile << PD_labels[5] << PD7.return_ohm_min() << "\n";
                pfile << PD_labels[6] << PD7.return_ohm_max() << "\n";
                pfile << PD_labels[7] << num_ohms7 << "\n";
                pfile << PD_labels[8] << n_trajs7;
                pfile.close();

                pfile.open(par_pairs_fname);
                for (int i = 0; i < num_gis7; i++){
                    for (int j = 0; j < num_ohms7; j++){
                        pfile << "[" + base_gi_classifier + to_string(i + 1) + "/" + to_string(num_gis7) + ", ";
                        pfile << base_ohm_classifier + to_string(j + 1) + "/" + to_string(num_ohms7) + "] ";
                        pfile << PD7.gi_values[i] << " " << PD7.ohm_values[j];
                        if (i != num_gis7 - 1 || j != num_ohms7 - 1){
                            pfile << endl;
                        }
                    }
                }
                pfile.close();
            }
            else{
                cout << "ERROR! Cannot save ill-defined parameters!\n";
            }
        }

        void print_all_fnames(){
            cout << endl << endl;
            cout << colour.red;
            cout << params_fname << endl << par_pairs_fname << endl << globalD_fname << endl;
            cout << globalSCD_fname << endl << D_bytraj_fname << endl;
            for (unsigned int i = 0; i < ID_times_fnames.size(); ++i){
                cout << ID_times_fnames[i] << endl;
            }
            cout << colour.white << endl;
        }


        string find_corresponding_filename(phase_diag PD7, int gi_counter, int ohm_counter){
            int num_gis7, num_ohms7, aux_pos;
            num_gis7 = PD7.gis_number();
            num_ohms7 = PD7.ohms_number();
            if (all_names_set){
                if (gi_counter >= 0 && gi_counter < num_gis7 && ohm_counter >= 0 && ohm_counter < num_ohms7){
                    aux_pos = num_ohms7 * gi_counter + ohm_counter;
                    return ID_times_fnames[aux_pos];
                }
                else{
                    cout << "ERROR! Unrecognised counters for the parameters!\n";
                    return "";
                }
            }
            else{
                cout << "ERROR! Names note defined yet!\n";
                return "";
            }

        }

};


class QEP_PD_temp_filenames{
    private:
        string base_ID_times_fname = "Aux_PD_Cpp_IDtimes.txt";
        string base_gi_classifier = "g";
        string base_ohm_classifier = "o";
        string IDtimes_folder = "PD_ID_times/";
        bool all_names_set = 0;
        unsigned int instance_number = 0;
    public:
        vector<string> ID_times_fnames = {};
        string aux_IDt_str, aux_str;

        QEP_PD_temp_filenames(phase_diag PD7){

            instance_number = determine_instance();

            aux_IDt_str = IDtimes_folder + fully_classify(base_ID_times_fname, PD7);

            int num_gis7, num_ohms7;
            num_gis7 = PD7.gis_number();
            num_ohms7 = PD7.ohms_number();
            for (int i = 0; i < num_gis7; i++){
                for (int j = 0; j < num_ohms7; j++){
                    aux_str = distinguish_par_pairs(aux_IDt_str, base_gi_classifier, i, num_gis7);
                    aux_str = distinguish_par_pairs(aux_str, base_ohm_classifier, j, num_ohms7);
                    aux_str = insert_before_ext(aux_str, "_" + to_string(instance_number));
                    ID_times_fnames.push_back(aux_str);
                }
            }

            all_names_set = 1;
        }


        unsigned int determine_instance(){
            unsigned int auxi;
            if (!(file_already_exists(instance_fname))){
                ofstream tempf(instance_fname);
                tempf << 1;
                tempf.close();
                return 1;
            }
            else{
                ifstream tempf(instance_fname);
                tempf >> auxi;
                tempf.close();
                auxi++;
                ofstream tempf2(instance_fname);
                tempf2 << auxi;
                tempf2.close();
                return auxi;
            }
        }

        string fully_classify (string base_fname, phase_diag PD7){
            string aux_fname = add_pars_to_fname(base_fname, PD7.xsize(), PD7.ysize());
            return aux_fname;
        }

        string distinguish_par_pairs(string base_fname, string base_classifier7, int par_counter, int par_number){
            return insert_before_ext(base_fname, "_" + base_classifier7 + to_string(par_counter + 1) + "_" + to_string(par_number));
        }

        string find_corresponding_filename(phase_diag PD7, int gi_counter, int ohm_counter){
            int num_gis7, num_ohms7, aux_pos;
            num_gis7 = PD7.gis_number();
            num_ohms7 = PD7.ohms_number();
            if (all_names_set){
                if (gi_counter >= 0 && gi_counter < num_gis7 && ohm_counter >= 0 && ohm_counter < num_ohms7){
                    aux_pos = num_ohms7 * gi_counter + ohm_counter;
                    return ID_times_fnames[aux_pos];
                }
                else{
                    cout << "ERROR! Unrecognised counters for the parameters!\n";
                    return "";
                }
            }
            else{
                cout << "ERROR! Names note defined yet!\n";
                return "";
            }

        }

};


void produce_sum_of_dyn_obs(string fname1, string fname2){
    ifstream f1, f2;
    ofstream f3;
    string fname3;
    double p1, p2, p3;
    if (fname_starts_with(fname1, "Cpp_Lattice_map_I") && fname_starts_with(fname2, "Cpp_Lattice_map_B")){
        fname3 = "Cpp_Lattice_map_C" + erase_from_start(fname1, "Cpp_Lattice_map_I");
        f1.open(fname1);
        f2.open(fname2);
        f3.open(fname3);
        f3.precision(4);
        string line1, line2;
        while(getline(f1, line1) && getline(f2, line2)){
            istringstream iss1(line1), iss2(line2);
            if (line1.length()*line2.length() == 0){
                f3 << "\n";
            }
            else{
                while (!(iss1.eof() || iss2.eof())){
                    iss1 >> p1;
                    iss2 >> p2;
                    p3 = p1 + p2;
                    f3 << p3 << " ";
                }
                f3 << "\n";
            }
        }
        f1.close();
        f2.close();
        f3.close();
    }
}


#endif // QEPF_H
