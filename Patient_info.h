#ifndef PATINFO_H
#define PATINFO_H


#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
#include <chrono>
#include <random>

#include "QEP_norm.h"

using namespace std;

// INITIALISATION OF THE RANDOM NUMBER GENERATOR AND DISTRIBUTION

//unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();     // PROVIDES A SEED FOR THE GENERATION OF PSEUDO-RANDOM SEQUENCES
//default_random_engine rand_gen (seed1);                                        // GENERATOR OF PSEUDO RANDOM SEQUENCES WITH PROVIDED SEED
//uniform_real_distribution<double> rand_real(0.0, 1.0);                     // CONSTRUCTS A UNIFROM REAL DISTRIBUTION

struct dyn_params{
    double max_time;
    double tstep;
    long int num_of_trajs;
};

class parvalues{
    private:
        unsigned int num;
        double minval;
        double maxval;
        double step;
    public:
        bool well_def;

        parvalues(){
            num = 0;
            minval = 0;
            maxval = 0;
        }

        parvalues(unsigned int num7, double minval7, double maxval7, bool positivity_required){
            if (positivity_required && (minval7 < 0 || maxval7 < 0)){
                cout << "WARNING! Changed sign to negative values!\n";
                minval7 = abs(minval7);
                maxval7 = abs(maxval7);
            }

            if (num7 <= 0){
                cout << "ERROR! At least one point must be taken!\n";
                well_def = 0;
            }
            else{
                if (num7 == 1){
                    num = num7;
                    minval = minval7;
                    maxval = minval7;
                    well_def = 1;
                }
                else{
                    num = num7;
                    if (minval7 != maxval7){
                        minval = min(minval7, maxval7);
                        maxval = max(minval7, maxval7);
                        well_def = 1;
                    }
                    else{
                        cout << "WARNING! There is no meaning in taking more than one point for an interval which contains only one value!\n";
                        cout << "Number of points reduced to 1!\n";
                        num = 1;
                        minval = minval7;
                        maxval = minval7;
                        well_def = 1;
                    }
                }
            }
            if (well_def){
                if (num == 1){
                    step = 0;
                }
                else{
                    step = (maxval - minval) / (num - 1);
                }
            }
        }

        unsigned int what_num(){
            return num;
        }

        double what_min(){
            return minval;
        }

        double what_max(){
            return maxval;
        }

        double what_step(){
            return step;
        }

};



class system_parameters{
    private:
        int N_thresh;
        double gammas[5];
        double Gms[5];
        double current_time;
    public:
        double time_thresh;
        int LX, LY;
        long int tot_size;
        double gd, gi, ohm;

        int neighbours[4][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};

        system_parameters(int LX0 = 1, int LY0 = 1, double gd0 = 1, double gi0 = 0, double ohm0 = 1, double time_thresh0 = 0.005){
            LX = LX0;
            LY = LY0;
            gd = gd0;
            gi = gi0;
            ohm = ohm0;
            tot_size = LX * LY;
            if(time_thresh0 > 0){
                time_thresh = time_thresh0;
            }
            else{
                time_thresh = -1;
                cout << "ERROR! Time precision should be > 0! \n";
            }

            bool omega_accept = 0;
            double aux_var;
            N_thresh = -1;
            while(! omega_accept){
                omega_accept = 1;
                for(int i = 0; i <= 4; i++){
                        aux_var = (gd + gi * i) / 4;
                        if(aux_var == ohm){
                            ohm += 0.001 * (ohm + gi);
                            cout << "WARNING! Omega value slightly increased to " << ohm << "\n";
                            omega_accept = 0;
                        }
                        else if((ohm < aux_var) && (omega_accept)){
                            N_thresh = i;
                            break;
                        }
                }
            }
            if(N_thresh < 0){
                N_thresh = 5;
            }
            produce_gammas();
            produce_Gms();
            current_time = 0.0;
        }


        double calculate_Gm(int NS){

            double g0;
            if(NS < 5 && NS >= 0){
                g0 = gammas[NS];
            }
            else{
                cout << "ERROR! The number of S neighbours MUST be between 0 and 4!" << "\n";
                g0 = 0;
            }

            double aux_v = ohm * ohm - (g0 * g0 / 16.0);

            return sqrt(abs(aux_v));
        }


        void produce_gammas(){

            for(int i = 0; i < 5; i++){
                gammas[i] = gd + gi * i;
            }
        }

        void produce_Gms(){
            for(int i = 0; i < 5; i++){
                Gms[i] = calculate_Gm(i);
            }
        }

        void time_passed(double dt){
            current_time += dt;
        }

        double what_time(){
            return current_time;
        }

        void reset_time(){
            current_time = 0.0;
        }


        bool is_inside(int x, int y){
            if(x >= 0 && x < LX && y >= 0 && y < LY){
                return 1;
            }
            else{
                return 0;
            }
        }


        double get_gamma(int NS){
            if(NS >= 0 && NS < 5){
                return gammas[NS];
            }
            else{
                cout << "ERROR! NS must always be between 0 and 4!" << "\n";
                return -1.0;
            }
        }


        double get_Gm(int NS){
            if(NS >= 0 && NS < 5){
                return Gms[NS];
            }
            else{
                cout << "ERROR! NS must always be between 0 and 4!" << "\n";
                return -1.0;
            }
        }

        bool above_threshold(int N0){
            return (N0 >= N_thresh);
        }

        bool is_classical(){
            return (ohm == 0.);
        }

        void display_parameters(){
            cout << "LX = " << LX << endl;
            cout << "LY = " << LY << endl;
            cout << "gamma_D = " << gd << endl;
            cout << "gamma_I = " << gi << endl;
            cout << "Omega = " << ohm << endl;
        }

        double dist_from_centre(int x0, int y0){
            if (is_inside(x0, y0)){
                int dx = (LX / 2) - x0, dy = (LY / 2) - y0;
                double sq_dist = dx * dx + dy * dy;
                return sqrt(sq_dist);
            }
            else{
                cout << "ERROR! This patient at " << x0 << ", " << y0 << " appears to be outside the lattice!\n";
                return -1;
            }
        }

};


class patient{
    protected:
        int NS;
        double time_of_infection = -1., time_of_death = -1.;
        int x, y;
    public:
        char class_state;
        int approx_dist_from_orig;
        double dist_from_orig;

        patient (system_parameters spar0, char class_state0, int x0, int y0, int NS0 = 0){

            if(class_state0 == 'S' || class_state0 == 'D'){
                class_state = class_state0;
            }
            else if(class_state0 == 'I'){
                class_state = class_state0;
            }
            else{
                cout << "ERROR! Wrong classical state type! It must be S, I or D!" << "\n";
            }

            if(spar0.is_inside(x0, y0)){
                x = x0;
                y = y0;
            }
            else{
                cout << "ERROR! This patient is not inside the lattice!" << "\n";
            }

//            NS = 0;

            if(NS0 >= 0 && NS0 <= 4){
                NS = NS0;
            }
            else{
                cout << "ERROR! Impossible number of S neighbours!" << "\n";
            }
        }


        patient (){
            class_state = 'S';
            x = -1;
            y = -1;
            NS = 0;
        }

        void pat_NS_up(){
            NS += 1;
        }

        void init_coords(system_parameters spar, int x0, int y0){
            if (x0 < 0 || y0 < 0){
                cout << "ERROR! You cannot change the coordinates of a patient after the initialisation!\n";
            }
            else{
                if (!(spar.is_inside(x0, y0))){
                    cout << "ERROR! You cannot assign coordinates outside the lattice!\n";
                }
                else{
                    x = x0;
                    y = y0;
                    dist_from_orig = spar.dist_from_centre(x, y);
                    approx_dist_from_orig = (int) dist_from_orig;
                    if (approx_dist_from_orig < 0){
                        approx_dist_from_orig = 0;
                    }
                }
            }
        }


        void pat_NS_down(){
            NS -= 1;
        }

        void set_inf_time(double t0){
            time_of_infection = t0;
        }

        void set_d_time(double t0){
            time_of_death = t0;
        }

        void count_NS(system_parameters spar0, patient** list_pat){
            for(int i = 0; i < 4; i++){
                int neighbour_x = x + spar0.neighbours[i][0];
                int neighbour_y = y + spar0.neighbours[i][1];
                if(spar0.is_inside(neighbour_x, neighbour_y)){
                    if(list_pat[neighbour_x][neighbour_y].class_state == 'S'){
                        pat_NS_up();
                    }
                }
            }
        }


        double return_inf_time(){
            return time_of_infection;
        }

        double return_d_time(){
            return time_of_death;
        }

        int return_NS(){
            return NS;
        }

        int xpos(){
            return x;
        }

        int ypos(){
            return y;
        }

        void print_xy(){
            cout << "We have x = " << x << " and y = " << y << "\n";
        }

        void disclose_all(){
            string colour;
            if (class_state == 'S'){
                colour = "\033[92m";
            }
            else if (class_state == 'I'){
                colour = "\033[91m";
            }
            else{
                colour = "\033[93m";
            }
            cout << "This patient is at position (" << colour << x << "\033[37m" << ", " << colour << y << "\033[37m" << ") and is in state " << class_state << ".\n";
            cout << "It has counted " << colour << NS << "\033[37m" << " S nearest neighbours and its recorded time of infection is " << colour << time_of_infection << "\033[37m" << ".\n";
            cout << "It died at t = " << colour << time_of_death << "\033[37m" << ".\n";
        }

        void fall_ill(double ti){
            class_state = 'I';
            set_inf_time(ti);
        }


        void pass_away(double td){
            if (class_state != 'I'){
                cout << "ERROR! Only I sites can die!\n";
            }
            else{
                class_state = 'D';
                set_d_time(td);
            }
        }

        void init_kill(double td){
            class_state = 'D';
            set_d_time(td);
        }

};



class infected_info
{
    private:
        double times[2];
        double u;
        patient* to_pat;
    public:
        double qvec[2];
        vector<double> records;


        infected_info(system_parameters spar0, patient* to_pat0){
            to_pat = to_pat0;
            if (!((*to_pat).class_state == 'I')){
                (*to_pat).class_state = 'I';
            }

            generate_new_u();
            set_pair_of_time_bounds(spar0); // When a new infected is created its NS does NOT change!
            qvec[0] = 1.;
            qvec[1] = 1.;
            write_records(spar0);
        }

        void generate_new_u(){
            u = rand_real(rand_gen);
        }

        int extract_x(){
            return ((*to_pat).xpos());
        }

        int extract_y(){
            return ((*to_pat).ypos());
        }

        void set_pair_of_time_bounds(system_parameters spar0){
            if (!(u >= 0 && u <= 1)){
                cout << "ERROR! The probability must have a value between 0 and 1!" << "\n";
            }
            else{
                int NS1 = (*to_pat).return_NS();
                double gamma0 = spar0.get_gamma(NS1);
                double Gm0 = spar0.get_Gm(NS1);
                double t_small, t_large;

                if (spar0.above_threshold(NS1)){ // Omega small
                    double base_coeff = (4. * Gm0) / gamma0;
                    double coeff = base_coeff * base_coeff;
                    t_small = -log(u / coeff) / ((gamma0 / 2.) + (2. * Gm0));
                    double t_small2 = - log(u) / gamma0;
                    t_small = max(t_small, t_small2);
                    t_large = -log(u * coeff) / (gamma0 / 2. - 2. * Gm0);
                }
                else{    // Omega large
                    double coeff2 = spar0.ohm - gamma0 / 4.;
                    double coeff3 = spar0.ohm + gamma0 / 4.;
                    double coeff4 = coeff2 / coeff3;
                    double coeff5 = M_PI / Gm0;
                    t_small = -2. * log(u / coeff4) / gamma0;
                    double t_small2 = coeff5 * floor(-2. * log(u) / (gamma0 * coeff5) );
                    double t_small3 = - log(u) / gamma0;
                    t_small = max( max(t_small, t_small2), t_small3);
                    t_large = - 2. * log(u * coeff4) / gamma0;
                    double t_large2 = t_small2 + coeff5;
                    t_large = min(t_large2, t_large);
                }
                times[0] = t_small;
                times[1] = t_large;
            }
        }

        double larger_time(){
            return times[1];
        }

        double smaller_time(){
            return times[0];
        }

        double midtime(){
            if (times[0] > 0 && times[1]/times[0] > 10){
                return sqrt(times[0] * times[1]);
            }
            else{
                return ((times[0] + times[1]) / 2.0);
            }
        }

        void Newt_update_bounds(system_parameters spar0){
            double midt = midtime();
            double p0 = calc_prob(spar0, midt);
//            cout << "Before the update the times are " << times[0] << " and " << times[1] << ".\n";
            bool is_positive = ((p0 - u) >= 0);
            if (is_positive){
                times[0] = midt;
            }
            else{
                times[1] = midt;
            }
//            cout << "After the update the times are " << times[0] << " and " << times[1] << ".\n\n";
        }

        double time_interval(){
            return times[1] - times[0];
        }

        bool small_enough_interval(double  thresh){
            return ((time_interval() / times[1]) < thresh);
        }

        void update_qvec(system_parameters spar0, double dt0){
            if (!check_norm()){
                cout << "ERROR! Non-normalised vector fed to update_qvec!" << "\n";
            }
            else{
                encode_updated_qvec(spar0, dt0, qvec);
                normalise_vec();
                }
                /* C++ MISSING: writing records   */
        }


        void update_vec_and_u_after_infection(system_parameters spar0, double J_time, bool is_winner){
            if (is_winner){
                qvec[0] = 1.;
                qvec[1] = 1.;
            }
            else{
                update_qvec(spar0, J_time);
            }
            generate_new_u();
        }

        void update_vec_and_u_after_death(system_parameters spar0, double J_time){
            update_qvec(spar0, J_time);
            generate_new_u();
        }

        double class_get_jumptime(system_parameters spar0){
            if (spar0.ohm != 0){
                cout << "ERROR! The classical functions are meant to be used ONLY for Omega = 0! \n";
                return -1.;
            }
            else{
                int NS1 = (*to_pat).return_NS();
                double loc_gamma = spar0.get_gamma(NS1);
                return (- log(u) / loc_gamma);
            }
        }

        void disclose_info(){
            string colour = "\033[91m";
            string nocolour = "\033[37m";
            cout << "This infected lies at x = " << colour << (*to_pat).xpos() << nocolour << " and y = " << colour << (*to_pat).ypos() << nocolour << ".\n";
            cout << "Its q-vector components are (" << colour << qvec[0] << nocolour << ", " << colour << qvec[1] << nocolour << ").\n";
            cout << "Its stored random element is u = " << colour << u << nocolour << ".\n";
            cout << "Correspondingly, the time bounds are " << colour << times[0] << nocolour << " and " << colour << times[1] << nocolour << ".\n";
            cout << "Its records count a total of " << colour << records.size() << nocolour << ".\n\n";
        }


        double vec_sqnorm(){
            double aux = qvec[0]*qvec[0] + qvec[1]*qvec[1];
            return aux;
        }

        double vec_norm(){
            return sqrt(vec_sqnorm());
        }

        bool check_norm(){
            double aux = vec_sqnorm();
            if (abs(aux - 2) < 0.0001){
                return 1;
            }
            else{
                return 0;
            }
        }

        void normalise_vec(){
            double nrm = vec_norm();
            if (nrm != 0){
                qvec[0] *= sqrt(2) / nrm;
                qvec[1] *= sqrt(2) / nrm;
            }
        }

        void write_records(system_parameters spar0){

            int NS1 = (*to_pat).return_NS();

            double loc_gamma = spar0.get_gamma(NS1), loc_Gm = spar0.get_Gm(NS1);

            if (spar0.above_threshold(NS1)){
                double loc_comb = ((loc_gamma / 4) - spar0.ohm);
                double coeff_A = qvec[0] - qvec[1] * loc_comb / loc_Gm;
                double coeff_B = qvec[0] + qvec[1] * loc_comb / loc_Gm;
                double C_plus = (loc_gamma / (16. * loc_comb)) * coeff_A * coeff_A;
                double C_naught = - (spar0.ohm / (2. * loc_comb)) * coeff_B * coeff_A;
                double C_minus = (loc_gamma / (16. * loc_comb)) * coeff_B * coeff_B;

                double aux_array[] = {coeff_A, coeff_B, C_plus, C_naught, C_minus};
                records.assign(aux_array, aux_array + 5);

            }
            else{
                double loc_comb = -((loc_gamma / 4) - spar0.ohm);
                double coeff_A = qvec[0];
                double coeff_B = qvec[1] * loc_comb / loc_Gm;
                double coeff_D0 = 1;
                double coeff_D1 = (loc_gamma / 4) * (((qvec[0] * qvec[0]) / loc_comb) - (qvec[1] * qvec[1]) / ((loc_gamma / 4) + spar0.ohm));
                double coeff_D2 = -loc_gamma / (2 * loc_Gm) * qvec[0] * qvec[1];

                double aux_array[] = {coeff_A, coeff_B, coeff_D0, coeff_D1, coeff_D2};
                records.assign(aux_array, aux_array + 5);
            }
        }

        void display_records(system_parameters spar0){

            int NS1 = (*to_pat).return_NS();
            unsigned int aux_l = records.size();

            if (aux_l <= 0){
                cout << "ERROR! No records have been produced yet!\n";
            }
            else{
                if (spar0.above_threshold(NS1)){
                    cout << "Omega small regime:\n";
                }
                else{
                    cout << "Omega large regime:\n";
                }
                for (unsigned int i = 0; i < aux_l; i++){
                    cout << records[i] << " ";
                }
                cout << endl;
            }
        }


        double calc_prob(system_parameters spar0, double t0){
            if ((*to_pat).class_state != 'I'){
                cout << "ERROR! Cannot calculate probabilities for non-infected sites!" << "\n";
                return -1;
            }
            else{
                int NS1 = (*to_pat).return_NS();
                if (spar0.above_threshold(NS1)){    // Omega small
                    double aux_p_1 = records[2] * exp(-t0 * ((spar0.get_gamma(NS1)/ 2.) - 2. * spar0.get_Gm(NS1)));
                    double aux_p_2 = records[3] * exp(-t0 * (spar0.get_gamma(NS1) / 2.));
                    double aux_P_3 = records[4] * exp(-t0 * ((spar0.get_gamma(NS1) / 2.) + 2. * spar0.get_Gm(NS1)));
                    return (aux_p_1 + aux_p_2 + aux_P_3);
                }
                else{   // Omega large
                    double aux_p_1 = records[2];
                    double aux_sin = sin(t0 * spar0.get_Gm(NS1));
                    double aux_cos = cos(t0 * spar0.get_Gm(NS1));
                    double aux_p_2 = records[3] * aux_sin * aux_sin;
                    double aux_p_3 = records[4] * aux_sin * aux_cos;

                    return exp(-t0 * (spar0.get_gamma(NS1) / 2.)) * (aux_p_1 + aux_p_2 + aux_p_3);
                }
            }
        }

        void encode_updated_qvec(system_parameters spar0, double dt0, double aux_vec[2]){
            if (!check_norm()){
                cout << "ERROR! Non-normalised vector fed to return_updated_qvec!" << "\n";
            }
            else{
                int NS1 = (*to_pat).return_NS();
                double gamma0 = spar0.get_gamma(NS1);
                double Gm0 = spar0.get_Gm(NS1);

                if (spar0.above_threshold(NS1)){ // Omega small
                    double a0 = records[0] * exp(dt0 * (Gm0 - (gamma0 / 4.))) + records[1] * exp(-dt0 * (Gm0 + (gamma0 / 4.)));
                    double auxb = records[0] * exp(dt0 * (Gm0 - (gamma0 / 4.))) - records[1] * exp(-dt0 * (Gm0 + (gamma0 / 4.)));
                    double b0 = auxb * (Gm0 / (2.0 * (spar0.ohm - (gamma0 / 4.))));

                    aux_vec[0] = a0 / 2.;
                    aux_vec[1] = b0;
                }
                else{       // Omega large
                    double a0 = exp(dt0 * gamma0 / 4.) * (records[0] * cos(Gm0 * dt0) + records[1] * sin(Gm0 * dt0));
                    double b0 = exp(dt0 * gamma0 / 4.) * (-records[0] * sin(Gm0 * dt0) + records[1] * cos(Gm0 * dt0)) * (Gm0 / (spar0.ohm - (gamma0 / 4.)));
                    aux_vec[0] = a0;
                    aux_vec[1] = b0;
                }
            }
        }

        void encode_quantum_components(system_parameters spar0, double time_since_jump, double qcomp[2]){
            double aux_vec0[2];
            encode_updated_qvec(spar0, time_since_jump, aux_vec0);
            normalisation2(aux_vec0);

            double i0, b0;
            i0 = (aux_vec0[0] + aux_vec0[1]) / 2.;
            b0 = (aux_vec0[0] - aux_vec0[1]) / 2.;
            qcomp[0] = i0 * i0;
            qcomp[1] = b0 * b0;
        }


        char select_jump_type(system_parameters spar0){
            patient pat0 = (*to_pat);
            int NS1 = pat0.return_NS();
            if (pat0.class_state != 'I'){
                cout << "ERROR! Only an infected can select a jump type!" << "\n";
                return 'n';
            }
            else{
                double death_chance = spar0.gd / spar0.get_gamma(NS1);
                double p = rand_real(rand_gen);
                if (p < death_chance){
                    return 'd';
                }
                else{
                    return 'i';
                }
            }
        }

        int extract_NS(){
            return (*to_pat).return_NS();
        }

        void NS_up(){
            (*to_pat).pat_NS_up();
        }

        void NS_down(){
            (*to_pat).pat_NS_down();
        }

        void death_occurred(double td0){
            (*to_pat).pass_away(td0);
        }

        void inf_occurred(double ti0){
            (*to_pat).fall_ill(ti0);
        }

};


vector<infected_info> construct_inf_list(system_parameters spar1, patient** pat_tab){
    vector<infected_info> list_inf1 = {};
    for (int i = 0; i < spar1.LX; i++){
        for (int j = 0; j < spar1.LY; j++){
            if (pat_tab[i][j].class_state == 'I'){
                list_inf1.push_back(infected_info(spar1, &(pat_tab[i][j])));
            }
        }
    }
    return list_inf1;
}


int pop_to_int(char ptype){
    if (ptype == 'S'){
        return 0;
    }
    else if (ptype == 'I'){
        return 1;
    }
    else if (ptype == 'B'){
        return 2;
    }
    else if (ptype == 'D'){
        return 3;
    }
    else{
        cout << "ERROR! Population type not recognised!\n";
        return -1;
    }
}

char int_to_pop(int numtype){
    if (numtype == 0){
        return 'S';
    }
    else if (numtype == 1){
        return 'I';
    }
    else if (numtype == 2){
        return 'B';
    }
    else if (numtype == 3){
        return 'D';
    }
    else{
        cout << "ERROR! Population type not recognised!\n";
        return 'N';
    }
}

string int_to_str_pop(int numtype){
    if (numtype == 0){
        return "_S";
    }
    else if (numtype == 1){
        return "_I";
    }
    else if (numtype == 2){
        return "_B";
    }
    else if (numtype == 3){
        return "_D";
    }
    else{
        cout << "ERROR! Population type not recognised!\n";
        return "_N";
    }
}

#endif // PATINFO_H
