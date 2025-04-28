#ifndef INPUTS_H
#define INPUTS_H

#include <iostream>
#include <cmath>

#include "Patient_info.h"
#include "file_management.h"

col colour;


void typing_in_parameters0(system_parameters &spar6, dyn_params &dpar6){
    double gd6, gi6, ohm6;
    int LX6, LY6;
    double max_time6, dt6;
    long int n_trajs6 = -1;
    double t_thresh6;
    char answer;
    bool proceed = 0;

    while (!proceed){
        n_trajs6 = -1;
        cout << "Initiating parameter acquisition.\n" << "Please enter the desired values and press enter.\n";
        cout << "Take care to provide " + colour.green + "integer" + colour.white + " values for " + colour.green + "integer" + colour.white + " variables.\n" << "A reminder will be provided.\n\n";

        do{
            cout << "Enter the horizontal " + colour.green + "integer" + colour.white + " size of the lattice: ";
            cin >> LX6;
        }
        while (LX6 <= 0);

        do{
            cout << endl << "Enter the vertical " + colour.green + "integer" + colour.white + " size of the lattice: ";
            cin >> LY6;
        }
        while (LY6 <= 0);

        cout << endl << "Insert gamma_D (recommended value 1): ";
        cin >> gd6;
        cout << endl << "Insert gamma_I: ";
        cin >> gi6;
        cout << endl << "Insert Omega: ";
        cin >> ohm6;
        cout << endl;

        cout << "Now enter the final time for the simulations: ";
        cin >> max_time6;
        cout << endl << "and the time step you wish to employ: ";
        cin >> dt6;

        cout << endl << endl;

        while (n_trajs6 < 0){
            cout << "The number of trajectories should, once again, be a positive " + colour.green + "integer" + colour.white + ".\n";
            cout << "Remember that inserting 0 means that the code will continue running until a file is created/renamed in the main folder called " + colour.yellow + "'end.txt'." + colour.white << endl;
            cout << "So, how many trajectories? ";
            cin >> n_trajs6;
        }

        cout << endl << "Would you like to change the precision threshold set for the decimation process (y/n)?";
        cout << endl << "If unsure, please select 'no': ";
        cin >> answer;
        if (answer == 'y'){
            while (t_thresh6 <= 0 || t_thresh6 >= 1){
                cout << endl << "Its value should be between 0 and 1. Enter it now: ";
                cin >> t_thresh6;
                cout << endl;
            }
            spar6 = system_parameters(LX6, LY6, gd6, gi6, ohm6, t_thresh6);
        }
        else{
            cout << endl;
            spar6 = system_parameters(LX6, LY6, gd6, gi6, ohm6);
        }
        dpar6.max_time = max_time6;
        dpar6.num_of_trajs = n_trajs6;
        dpar6.tstep = dt6;

        cout << endl << endl;
        cout << "Excellent. You are about to start a simulation for:\n\n";
        cout << "A lattice of size " + colour.green << LX6 << " x " << LY6 << colour.white + " with parameters\n";
        cout << "gamma_D = " << colour.red << gd6 << colour.white;
        cout << ", gamma_I = " << colour.red << gi6 << colour.white;
        cout << " and Omega = " << colour.red << gd6 << colour.white << ".\n";
        cout << "The simulation will run up to time " << colour.yellow << max_time6 << colour.white << " and data will be collected at steps of ";
        cout << colour.yellow << dt6 << colour.white << ".\n";
        cout << "PLEASE CONFIRM.\n";
        cout << "(entering any other key than 'n' will lock in these choices)\n";
        cin >> answer;
        if (answer != 'n'){
            proceed = 1;
        }
    }
}

template<typename T1>
bool read_from_input(T1 &var){
    string aux_s = "";
    cin.clear();
    cin.sync();
    getline(cin, aux_s);
    bool aux_b = (istringstream(aux_s) >> var);
    return aux_b;
}


bool typing_in_parameters(system_parameters &spar6, dyn_params &dpar6){
    double gd6, gi6, ohm6;
    int LX6, LY6;
    double max_time6, dt6;
    long int n_trajs6 = -1;
    double t_thresh6;
    char answer;
    bool proceed = 0, get_past = 0;

    while (!proceed){
        n_trajs6 = -1;
        cout << "Initiating parameter acquisition.\n" << "Please enter the desired values and press enter.\n";
        cout << "Take care to provide " + colour.green + "integer" + colour.white + " values for " + colour.green + "integer" + colour.white + " variables.\n" << "A reminder will be provided.\n\n";

        get_past = 0;
        do{
            cout << "Enter the horizontal " + colour.green + "integer" + colour.white + " size of the lattice: ";
            get_past = read_from_input<int>(LX6);
        }
        while (LX6 <= 0 || !get_past);

        get_past = 0;
        do{
            cout << endl << "Enter the vertical " + colour.green + "integer" + colour.white + " size of the lattice: ";
            get_past = read_from_input<int>(LY6);
        }
        while (LY6 <= 0 || !get_past);

        get_past = 0;
        do{
            cout << endl << "Insert gamma_D (recommended value 1): ";
            get_past = read_from_input<double>(gd6);
        }
        while (!get_past);

        get_past = 0;
        do{
            cout << endl << "Insert gamma_I: ";
            get_past = read_from_input<double>(gi6);
        }
        while(!get_past);

        get_past = 0;
        do{
            cout << endl << "Insert Omega: ";
            get_past = read_from_input<double>(ohm6);
        }
        while(!get_past);

        cout << endl;

        get_past = 0;
        do{
            cout << "Now enter the final time for the simulations: ";
            get_past = read_from_input<double>(max_time6);
        }
        while(!get_past);

        get_past = 0;
        do{
            cout << endl << "and the time step you wish to employ: ";
            get_past = read_from_input<double>(dt6);
        }
        while(!get_past);


        cout << endl << endl;

        get_past = 0;
        do{
            cout << "The number of trajectories should, once again, be a positive " + colour.green + "integer" + colour.white + ".\n";
            cout << "Remember that inserting 0 means that the code will continue running until a file is created/renamed in the main folder called " + colour.yellow + "'end.txt'." + colour.white << endl;
            cout << "So, how many trajectories? ";
            get_past = read_from_input<long int>(n_trajs6);
        }
        while (n_trajs6 < 0 || !get_past);


        cout << endl << "Would you like to change the precision threshold set for the decimation process (y/n)?";
        cout << endl << "If unsure, please select 'n': ";
        cin >> answer;
        if (answer == 'y'){
            get_past = 0;
            do{
                cout << endl << "Its value should be between 0 and 1. Enter it now: ";
                get_past = read_from_input<double>(t_thresh6);
                cout << endl;
            }
            while (t_thresh6 <= 0 || t_thresh6 >= 1 || !get_past);

            spar6 = system_parameters(LX6, LY6, gd6, gi6, ohm6, t_thresh6);
        }
        else{
            cout << endl;
            spar6 = system_parameters(LX6, LY6, gd6, gi6, ohm6);
        }
        dpar6.max_time = max_time6;
        dpar6.num_of_trajs = n_trajs6;
        dpar6.tstep = dt6;

        cout << endl << endl;
        cout << "Excellent. You are about to start a simulation for:\n\n";
        cout << "A lattice of size " + colour.green << LX6 << " x " << LY6 << colour.white + " with parameters\n";
        cout << "gamma_D = " << colour.red << gd6 << colour.white;
        cout << ", gamma_I = " << colour.red << gi6 << colour.white;
        cout << " and Omega = " << colour.red << ohm6 << colour.white << ".\n";
        cout << "The simulation will run up to time " << colour.yellow << max_time6 << colour.white << " and data will be collected at steps of ";
        cout << colour.yellow << dt6 << colour.white << ".\n";
        cout << "PLEASE CONFIRM.\n";
        cout << "(entering 'x' will abort the code; 'n' will clear the current parameters and restart the sequence above)\n";
        cout << "(any other key will start the simulation with these parameters)\n";
        cin >> answer;
        if (answer == 'x'){
            break;
        }
        else if (answer != 'n'){
            proceed = 1;
        }
    }
    return proceed;
}

string dyn_labels[9] = {"LX: ", "LY: ", "gd: ", "gi: ", "Omega: ", "Max time: ", "dt: ", "num of trajectories: ", "threshold: "};

string PD_labels[10] = {"LX: ", "LY: ", "gi first bound: ", "gi second bound: ", "num of gis: ", "Omega first bound: ",
"Omega second bound: ", "num of Omegas: ", "num of trajectories: ", "threshold: "};


bool line_starts_with(string line, string seq){
    if (line.length() < seq.length()){
        return 0;
    }
    else if(line.substr(0, seq.length()) == seq){
        return 1;
    }
    else{
        return 0;
    }
}


void create_empty_dyn_inputfile(string fname){
    if (file_already_exists(fname)){
        cout << "IMPOSSIBLE! There is already a file with this name!\n";
    }
    else{
        ofstream pfile;
        int num_of_lines = sizeof(dyn_labels) / sizeof(dyn_labels[0]);
        pfile.open(fname);
        for (int i = 0; i < num_of_lines; i++){
            pfile << dyn_labels[i];
            if (i < num_of_lines - 1){
                pfile << "\n";
            }
        }
        pfile.close();
    }
}


void create_empty_PD_inputfile(string fname){
    if (file_already_exists(fname)){
        cout << "IMPOSSIBLE! There is already a file with this name!\n";
    }
    else{
        ofstream pfile;
        int num_of_lines = sizeof(PD_labels) / sizeof(PD_labels[0]);
        pfile.open(fname);
        for (int i = 0; i < num_of_lines; i++){
            pfile << PD_labels[i];
            if (i < num_of_lines - 1){
                pfile << "\n";
            }
        }
        pfile.close();
    }
}


int find_corresponding_param(string line6, int labels_size){
    for (int i = 0; i < labels_size; i++){
        if (line_starts_with(line6, dyn_labels[i])){
            return i;
        }
    }
    return -1;
}

int find_corresponding_line(string line6, string lines[], int labels_size){
    for (int i = 0; i < labels_size; i++){
        if (line_starts_with(line6, lines[i])){
            return i;
        }
    }
    return -1;
}


template<typename Tv>
Tv pluck_value(int pos, string line6){
    int l6 = dyn_labels[pos].length();
    Tv aux_var;
    string aux_sub = line6.substr(l6);
    istringstream(aux_sub) >> aux_var;
    return aux_var;
}

template<typename Tv>
bool pluck_value(int pos, string line6, Tv &aux_var, string data_type = "dyn"){
    int l6;
    if (data_type == "dyn"){
        l6 = dyn_labels[pos].length();
    }
    else{
        l6 = PD_labels[pos].length();
    }

    string aux_sub = line6.substr(l6);
    while (aux_sub[0] == ' '){
        aux_sub.erase(aux_sub.begin());
    }
    if (aux_sub == ""){
        return 0;
    }
    else{
        istringstream(aux_sub) >> aux_var;
        return 1;
    }
}


bool check_alltrue(bool blist6[], int num_of_elems){
    bool auxb = 1;
    for (int i = 0; i < num_of_elems; i++){
        auxb = auxb && blist6[i];
    }
    return auxb;
}





bool get_dyn_params_from_file(system_parameters &spar6, dyn_params &dpar6, string fname){
    ifstream pfile;
    bool valid_params = 0;
    if (file_already_exists(fname)){
        pfile.open(fname);
        int num_of_lines = sizeof(dyn_labels) / sizeof(dyn_labels[0]);
        bool acquired[num_of_lines];
        for (int i = 0; i < num_of_lines; i++){
            acquired[i] = 0;
        }

        double gd6 = -1, gi6 = -1, ohm6 = -1;
        int LX6 = -1, LY6 = -1;
        double max_time6 = -1, dt6 = -1;
        long int n_trajs6 = -1;
        double t_thresh6 = -1;
        bool thresh_encountered = 0;

        string aux_line;
        int aux_pos;
        while(getline(pfile, aux_line)){
            aux_pos = find_corresponding_param(aux_line, num_of_lines);
            if (aux_pos >= 0){
                if (!acquired[aux_pos]){
                    switch(aux_pos){
                        case 0:
                            if (pluck_value<int>(aux_pos, aux_line, LX6)){
                                if (LX6 > 0){
                                    acquired[0] = 1;
                                }
                            }
                            break;
                        case 1:
                            if (pluck_value<int>(aux_pos, aux_line, LY6)){
                                if (LY6 > 0){
                                acquired[1] = 1;
                                }
                            }
                            break;
                        case 2:
                            if (pluck_value<double>(aux_pos, aux_line, gd6)){
                                if (gd6 > 0){
                                    acquired[2] = 1;
                                }
                            }
                            break;
                        case 3:
                            if (pluck_value<double>(aux_pos, aux_line, gi6)){
                                if (gi6 > 0){
                                    acquired[3] = 1;
                                }
                            }
                            break;
                        case 4:
                            if (pluck_value<double>(aux_pos, aux_line, ohm6)){
                                acquired[4] = 1;
                            }
                            break;
                        case 5:
                            if (pluck_value<double>(aux_pos, aux_line, max_time6)){
                                if (max_time6 > 0){
                                    if (acquired[6]){
                                        if (max_time6 >= dt6){
                                            acquired[5] = 1;
                                        }
                                    }
                                    else{
                                        acquired[5] = 1;
                                    }
                                }
                            }
                            break;
                        case 6:
                            if (pluck_value<double>(aux_pos, aux_line, dt6)){
                                if (dt6 > 0){
                                    if (acquired[5]){
                                        if (max_time6 >= dt6){
                                            acquired[6] = 1;
                                        }
                                    }
                                    else{
                                        acquired[6] = 1;
                                    }
                                }
                            }
                            break;
                        case 7:
                            if (pluck_value<long int>(aux_pos, aux_line, n_trajs6)){
                                if (n_trajs6 >= 0){
                                    acquired[7] = 1;
                                }
                            }
                            break;
                        case 8:
                            if (pluck_value<double>(aux_pos, aux_line, t_thresh6)){
                                if (t_thresh6 > 0 && t_thresh6 < 1){
                                    thresh_encountered = 1;
                                }
                            }
                            break;
                    }
                }
            }
        }
        pfile.close();
        cout << colour.blue + "Input file now closed. It can be changed without risk." + colour.white + "\n" ;
        valid_params = check_alltrue(acquired, 8);
        if (valid_params){
            if (!thresh_encountered){
                spar6 = system_parameters(LX6, LY6, gd6, gi6, ohm6);
                dpar6.max_time = max_time6;
                dpar6.num_of_trajs = n_trajs6;
                dpar6.tstep = dt6;
            }
            else{
                spar6 = system_parameters(LX6, LY6, gd6, gi6, ohm6, t_thresh6);
                dpar6.max_time = max_time6;
                dpar6.num_of_trajs = n_trajs6;
                dpar6.tstep = dt6;
            }
        }
    }
    else{
        cout << "The input file was not found. A new one is being created now.\n";
        create_empty_dyn_inputfile(fname);
        cout << "Done. You can now insert some parameters and relaunch the code.\n";
        cout << "The file is named '" + fname + "'.\n";
    }
    return valid_params;
}


struct PD_aux_pars{
    long int num_of_trajs;
    int LX, LY;
    double threshold;
};

bool get_PD_params_from_file(parvalues &ohms6, parvalues &gis6, PD_aux_pars &auxp6, string fname){
    ifstream pfile;
    bool valid_params = 0;
    if (file_already_exists(fname)){
        pfile.open(fname);
        int num_of_lines = sizeof(PD_labels) / sizeof(PD_labels[0]);
        bool acquired[num_of_lines];
        for (int i = 0; i < num_of_lines; i++){
            acquired[i] = 0;
        }


        bool thresh_encountered = 0;
        int LX6 = -1, LY6 = -1;
        double gi1 = -1, gi2 = -1, ohm1 = -1, ohm2 = -1;
        unsigned int num_gis = -1, num_ohms = -1;
        long int n_trajs6 = -1;
        double t_thresh6 = -1;

        string aux_line;
        int aux_pos;
        while(getline(pfile, aux_line)){
            aux_pos = find_corresponding_line(aux_line, PD_labels, num_of_lines);
            if (aux_pos >= 0){
                if (!acquired[aux_pos]){
                    switch(aux_pos){
                        case 0:
                            if (pluck_value<int>(aux_pos, aux_line, LX6, "PD")){
                                if (LX6 > 0){
                                    acquired[0] = 1;
                                }
                            }
                            break;
                        case 1:
                            if (pluck_value<int>(aux_pos, aux_line, LY6, "PD")){
                                if (LY6 > 0){
                                acquired[1] = 1;
                                }
                            }
                            break;
                        case 2:
                            if (pluck_value<double>(aux_pos, aux_line, gi1, "PD")){
                                if (gi1 > 0){
                                    acquired[2] = 1;
                                }
                            }
                            break;
                        case 3:
                            if (pluck_value<double>(aux_pos, aux_line, gi2, "PD")){
                                if (gi2 > 0){
                                    acquired[3] = 1;
                                }
                            }
                            break;
                        case 4:
                            if (pluck_value<unsigned int>(aux_pos, aux_line, num_gis, "PD")){
                                if (num_gis >= 1){
                                    acquired[4] = 1;
                                }
                            }
                            break;
                        case 5:
                            if (pluck_value<double>(aux_pos, aux_line, ohm1, "PD")){
                                acquired[5] = 1;
                            }
                            break;
                        case 6:
                            if (pluck_value<double>(aux_pos, aux_line, ohm2, "PD")){
                                acquired[6] = 1;
                            }
                            break;
                        case 7:
                            if (pluck_value<unsigned int>(aux_pos, aux_line, num_ohms, "PD")){
                                if (num_ohms >= 1){
                                    acquired[7] = 1;
                                }
                            }
                            break;
                        case 8:
                            if (pluck_value<long int>(aux_pos, aux_line, n_trajs6, "PD")){
                                if (n_trajs6 >= 0){
                                    acquired[8] = 1;
                                }
                            }
                            break;
                        case 9:
                            if (pluck_value<double>(aux_pos, aux_line, t_thresh6, "PD")){
                                if (t_thresh6 > 0 && t_thresh6 < 1){
                                    thresh_encountered = 1;
                                }
                            }
                            break;
                    }
                }
            }
        }
        pfile.close();
        cout << colour.blue + "Input file now closed. It can be changed without risk." + colour.white + "\n" ;
        valid_params = check_alltrue(acquired, 9);
        if (valid_params){
            if (!thresh_encountered){
                ohms6 = parvalues(num_ohms, ohm1, ohm2, 0);
                gis6 = parvalues(num_gis, gi1, gi2, 1);
                auxp6.LX = LX6;
                auxp6.LY = LY6;
                auxp6.num_of_trajs = n_trajs6;
                auxp6.threshold = -1;
                valid_params = valid_params && ohms6.well_def && gis6.well_def;
            }
            else{
                ohms6 = parvalues(num_ohms, ohm1, ohm2, 0);
                gis6 = parvalues(num_gis, gi1, gi2, 1);
                auxp6.LX = LX6;
                auxp6.LY = LY6;
                auxp6.num_of_trajs = n_trajs6;
                auxp6.threshold = t_thresh6;
                valid_params = valid_params && ohms6.well_def && gis6.well_def;
            }
        }
    }
    else{
        cout << "The input file was not found. A new one is being created now.\n";
        create_empty_PD_inputfile(fname);
        cout << "Done. You can now insert some parameters and relaunch the code.\n";
        cout << "The file is named '" + fname + "'.\n";
    }
    return valid_params;
}




#endif // INPUTS_H
