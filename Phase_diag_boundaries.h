#ifndef PDBOUND_H
#define PDBOUND_H

#include "Patient_info.h"
#include "Inputs.h"

//class parvalues{
//    private:
//        unsigned int num;
//        double minval;
//        double maxval;
//        double step;
//    public:
//        bool well_def;
//
//        parvalues(){
//            num = 0;
//            minval = 0;
//            maxval = 0;
//        }
//
//        parvalues(unsigned int num7, double minval7, double maxval7, bool positivity_required){
//            if (positivity_required && (minval7 < 0 || maxval7 < 0)){
//                cout << "WARNING! Changed sign to negative values!\n";
//                minval7 = abs(minval7);
//                maxval7 = abs(maxval7);
//            }
//
//            if (num7 <= 0){
//                cout << "ERROR! At least one point must be taken!\n";
//                well_def = 0;
//            }
//            else{
//                if (num7 == 1){
//                    num = num7;
//                    minval = minval7;
//                    maxval = minval7;
//                    well_def = 1;
//                }
//                else{
//                    num = num7;
//                    if (minval7 != maxval7){
//                        minval = min(minval7, maxval7);
//                        maxval = max(minval7, maxval7);
//                        well_def = 1;
//                    }
//                    else{
//                        cout << "WARNING! There is no meaning in taking more than one point for an interval which contains only one value!\n";
//                        cout << "Number of points reduced to 1!\n";
//                        num = 1;
//                        minval = minval7;
//                        maxval = minval7;
//                        well_def = 1;
//                    }
//                }
//            }
//            if (well_def){
//                if (num == 1){
//                    step = 0;
//                }
//                else{
//                    step = (maxval - minval) / (num - 1);
//                }
//            }
//        }
//
//        unsigned int what_num(){
//            return num;
//        }
//
//        double what_min(){
//            return minval;
//        }
//
//        double what_max(){
//            return maxval;
//        }
//
//        double what_step(){
//            return step;
//        }
//
//};


class phase_diag{
    private:
        parvalues gi_range, ohm_range;
        unsigned int n_gi, n_ohm, tot_points;
//        int LX, LY;
        PD_aux_pars aux_params;
    public:
        vector<double> gi_values = {}, ohm_values = {};
        bool well_def;
        bool slice[2] = {0, 0};


        phase_diag(PD_aux_pars auxpars7, parvalues ohms7, parvalues gis7){

            bool well_def1 = 0, well_def2 = 0;

            if (ohms7.well_def && gis7.well_def){

                gi_range = gis7;
                ohm_range = ohms7;

                n_gi = gi_range.what_num();
                n_ohm = ohm_range.what_num();
                if (n_gi == 1){
                    slice[0] = 1;
                }
                if (n_ohm == 1){
                    slice[1] = 1;
                }

                for (unsigned int i = 0; i < n_ohm; i++){
                    ohm_values.push_back(ohm_range.what_min() + i * ohm_range.what_step());
                }
                for (unsigned int j = 0; j < n_gi; j++){
                    gi_values.push_back(gi_range.what_min() + j * gi_range.what_step());
                }

                avoid_singular_points();

                tot_points = n_gi * n_ohm;

                well_def1 = 1;

                aux_params = auxpars7;
                well_def2 = check_aux_params();

                well_def = well_def1 && well_def2;
            }

        }


        phase_diag(string filename7){

            well_def = get_PD_params_from_file(ohm_range, gi_range, aux_params, filename7);

            if (well_def){

                n_gi = gi_range.what_num();
                n_ohm = ohm_range.what_num();

                for (unsigned int i = 0; i < n_ohm; i++){
                    ohm_values.push_back(ohm_range.what_min() + i * ohm_range.what_step());
                }
                for (unsigned int j = 0; j < n_gi; j++){
                    gi_values.push_back(gi_range.what_min() + j * gi_range.what_step());
                }

                avoid_singular_points();

                tot_points = n_gi * n_ohm;

                well_def = well_def && check_aux_params();
            }
        }


        bool check_aux_params(){
            bool auxb;
            auxb = (aux_params.LX > 0) && (aux_params.LY > 0) && (aux_params.num_of_trajs >= 0);
            if (aux_params.threshold <= 0 || aux_params.threshold >= 1){
                aux_params.threshold = -1;
            }
            return auxb;
        }


        double calc_step(double lbound, double ubound, unsigned int n0){
            if (n0 == 0){
                return 0;
            }
            else{
                return ((ubound - lbound) / n0);
            }
        }

        void avoid_singular_points(){
            for (unsigned int i = 0; i < n_ohm; i++){
                for (unsigned int j = 0; j < n_gi; j++){
                    system_parameters spar7(3, 3, 1, gi_values[j], ohm_values[i]);
                    ohm_values[i] = spar7.ohm;
                }
            }
        }

        unsigned int gis_number(){
            return n_gi;
        }

        unsigned int ohms_number(){
            return n_ohm;
        }

        unsigned int tot_number(){
            return tot_points;
        }

        void print_pair_number(unsigned int gi_counter, unsigned int ohm_counter){
            if (!(valid_indices(gi_counter, ohm_counter))){

                cout << "ERROR! Invalid indices passed to 'pair_number'!\n";
            }
            else{
                cout << "Parameter pair number " << gi_counter * n_ohm + ohm_counter + 1 << " of " << tot_points << "," << endl;
                cout << "corresponding to gamma_I = " << gi_values[gi_counter] << " and Omega = " << ohm_values[ohm_counter] << "." << endl;
            }
        }

        double extract_gi_step(){
            return gi_range.what_step();
        }

        double read_ohm_step(){
            return ohm_range.what_step();
        }

        int xsize(){
            return aux_params.LX;
        }

        int ysize(){
            return aux_params.LY;
        }

        long int totsize(){
            return aux_params.LX * aux_params.LY;
        }

        double return_gi_min(){
            return gi_values[0];
        }

        double return_gi_max(){
            return gi_values[n_gi - 1];
        }

        double return_ohm_min(){
            return ohm_values[0];
        }

        double return_ohm_max(){
            return ohm_values[n_ohm - 1];
        }

        long int return_ntraj(){
            return aux_params.num_of_trajs;
        }

        void fix_sys_parameters(system_parameters &spar7, unsigned int g7, unsigned int o7){
            if (valid_indices(g7, o7)){
                if (aux_params.threshold <= 0 || aux_params.threshold >= 1){
                    spar7 = system_parameters(aux_params.LX, aux_params.LY, 1, gi_values[g7], ohm_values[o7]);
                }
                else{
                    spar7 = system_parameters(aux_params.LX, aux_params.LY, 1, gi_values[g7], ohm_values[o7], aux_params.threshold);
                }
            }
            else{
                cout << "ERROR! There is something wrong with the counters!\n";
            }
        }

        bool valid_indices(unsigned int gi_counter, unsigned int ohm_counter){
            return (gi_counter >= 0 && ohm_counter >= 0 && gi_counter < n_gi && ohm_counter < n_ohm);
        }

        bool final_indices(unsigned int gi_counter, unsigned int ohm_counter){
            return (gi_counter == n_gi - 1 && ohm_counter == n_ohm - 1);
        }

        void update_traj_num(unsigned int actual_ntraj){
            aux_params.num_of_trajs = actual_ntraj;
        }

};


#endif // PDBOUND_H
