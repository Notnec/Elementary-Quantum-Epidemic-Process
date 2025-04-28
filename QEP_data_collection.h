#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include <random>
#include<chrono>


using namespace std;


#include "trajectories.h"
#include "file_management.h"
#include "visual_aids.h"
#include "Inputs.h"
#include "Phase_diag_boundaries.h"


bool distinguish_def_undef(system_parameters spar8, dyn_params dpar8){

    bool again = 0, repeat8 = 0;
    char c_one_more, c_same_pars, just_checking;

    if (dpar8.num_of_trajs == 0){

        do{
            cout << "Starting simulation without a defined number of trajectories.\n";
            cout << "To stop it from running, place a 'end.txt' file in the main folder.\n";
            cout << "Stopping it manually will result in no data being saved.\n";

            run_undef_trajs_up_to_maxtime(spar8, dpar8.max_time, dpar8.tstep);

            cout << endl << "Would you like to launch another simulation (y/n)? ";
            cin.clear();
            cin.sync();
            cin >> c_one_more;
            if (c_one_more == 'y'){
                cout << endl << "Would you like to use the same parameters as before (y/n)? ";
                cin >> c_same_pars;
                if (c_same_pars == 'y'){
                    if (file_already_exists("end.txt")){
                        cout << "WARNING! 'end.txt' file detected. If you still wish to proceed (or once you wish to), enter 'y': ";
                        cin >> just_checking;
                        if (just_checking == 'y'){
                            again = 1;
                            repeat8 = 0;
                        }
                        else{
                            again = 0;
                            repeat8 = 0;
                        }
                    }
                    else{
                        again = 1;
                        repeat8 = 0;
                    }
                }
                else{
                    repeat8 = 1;
                    again = 0;
                }
            }
            else{
                again = 0;
                repeat8 = 0;
            }
        }
        while(again);
    }
    else{
        do{
            cout << "Starting simulation for " << dpar8.num_of_trajs << " trajectories.\n";
            cout << "A percentage progression will be displayed.\n";

            run_def_trajs_up_to_maxtime(spar8, dpar8.max_time, dpar8.tstep, dpar8.num_of_trajs);

            cout << endl << "Would you like to launch another simulation (y/n)? ";
            cin.clear();
            cin.sync();
            cin >> c_one_more;
            if (c_one_more == 'y'){
                cout << endl << "Would you like to use the same parameters as before (y/n)? ";
                cin >> c_same_pars;
                if (c_same_pars == 'y'){
                    again = 1;
                    repeat8 = 0;
                }
                else{
                    repeat8 = 1;
                    again = 0;
                }
            }
            else{
                again = 0;
                repeat8 = 0;
            }
        }
        while(again);
    }
    return repeat8;
}


void dyndata_collection(){

    system_parameters syspars;
    dyn_params dynpars;
    char choice;

    bool got_parameters, repeat = 0;

    cout << "Starting the procedure for the collection of dynamical data.\n";

    do{
        cout << "Please enter 'f' or 'F' if you wish to define the parameters through 'dyn_input.txt'.\n";
        cout << "Enter 'm' or 'M' for manually inserting the parameters.\n";
        cout << "Any other key will terminate this instance.\n";
        cout << "Your choice: ";
        cin >> choice;


        switch(choice){
            case 'm': case 'M':
                got_parameters = typing_in_parameters(syspars, dynpars);
                if (got_parameters){
                    cout << "Parameters successfully acquired.\n";
                    repeat = distinguish_def_undef(syspars, dynpars);
                }
                break;
            case 'f': case 'F':
                got_parameters = get_dyn_params_from_file(syspars, dynpars, "dyn_input.txt");
                if (got_parameters){
                    cout << "Parameters successfully acquired.\n";
                    repeat = distinguish_def_undef(syspars, dynpars);
                }
                break;
            default:
                break;
        }

    }
    while(repeat);
}


void PDdata_collection(){

    phase_diag PD_pars("PD_input.txt");

    if (!(PD_pars.well_def)){
        cout << "ERROR! The parameters provided are either invalid or have not been defined yet. Please check 'PD_input.txt'.\n";
    }
    else{
        cout << "Valid parameters acquired. ";
        if (PD_pars.return_ntraj() == 0){
            cout << "Number of trajectories undefined. Remember to create a file 'end.txt' to terminate the simulation.\n";
            cout << "Do remember that once the file is found the programme will bring its ongoing parameter sweep to its end (1 trajectory per point\n";
            cout << "in parameter space). Hence, it will not stop immediately. A procedure has been added to provide estimates of the remaining time.\n\n";
            PD_scan_for_undef_traj_num(PD_pars);
        }
        else{
            cout << "Number of trajectories requested: " << PD_pars.return_ntraj() << "; starting simulation now.\n\n";
            PD_scan_for_def_traj_num(PD_pars, PD_pars.return_ntraj());
        }
    }
}
