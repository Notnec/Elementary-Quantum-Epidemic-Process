
// From MAIN

//    ofstream fff1;
//    fff1.open("to_erase.txt", ios_base::app);
//    fff1 << "To Erase." << " ";
//    fff1.close();
//
//    string ooo = "to_erase.txt";
//    string nnn = "to_erase_anyways.txt";
//
//    // Here system_clock is wall clock time from
//    // the system-wide realtime clock
//    auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
//
//    cout << ctime(&timenow) << endl;
//
//    struct tm * timeinfo;
//    char buffer [80];
//
//    timeinfo = localtime(&timenow);
//
//    strftime (buffer, 80, "On %d-%m-%Y at %H:%M: ", timeinfo);
//    cout <<  buffer << endl;




// From single_trajs.h



//void run_one_traj_until_maxtime_and_save(system_parameters &spar5, double max_time5){
//    if (spar5.what_time() <= max_time5){
//
//        // LATTICE CREATION
//        patient patient_lattice5[spar5.LX][spar5.LY];
//        patient* lattice_ptr5[spar5.LX];
//        for (int i = 0; i < spar5.LX; i++){
//            lattice_ptr5[i] = patient_lattice5[i];
//        }
//        // END OF LATTICE CREATION
//
//        // INITIALISATION OF OBSERVABLE ARRAYS
//        double dt5 = 0.1;
//        max_time5 = adjust_max_time(max_time5, dt5);
//
//
//        dyn_single_obs cl_populations[3];
//        for (int i = 0; i < 3; i++){
//            cl_populations[i] = dyn_single_obs(max_time5, dt5);
//        }
//        long int max_steps5 = cl_populations[0].get_n_points();
//        // 0 is "S"
//        // 1 is "C"
//        // 2 is "D"
//        // REMEMBER THAT "C" STANDS FOR "I" + "B"
//
//        QEP_dyn_filenames Qff(spar5, 1, 1);
//
//        lattice_dyn latdyn(spar5, Qff, max_steps5, max_time5, dt5);
//
//        vector<infected_info> inf_list5 = {};  // CREATION OF THE LIST OF INFECTED
//        GEP_usual_init(spar5, lattice_ptr5, inf_list5);    // DEFINITION OF THE INITIAL STATE
//
//
//        long int Scount = count_class_pop(spar5, lattice_ptr5, 'S');
//        long int Ccount = count_class_pop(spar5, lattice_ptr5, 'I');
//        long int Dcount = count_class_pop(spar5, lattice_ptr5, 'D');
//
//        cl_populations[0].add_at_step(0, (double) Scount / spar5.tot_size);
//        cl_populations[1].add_at_step(0, (double) Ccount / spar5.tot_size);
//        cl_populations[2].add_at_step(0, (double) Dcount / spar5.tot_size);
//        latdyn.add_config(spar5, lattice_ptr5, inf_list5, 0);
//
//        long int ticks5 = 0, nticks5;
//        while (spar5.what_time() <= max_time5 && inf_list5.size() > 0){
//            jump_details jump5 = compute_next_jump(spar5, inf_list5);
//
//            nticks5 = steps_to_next_jump(spar5, jump5.time, dt5, ticks5);
//            if (ticks5 + nticks5 > max_steps5 - 1){
//                nticks5 = max_steps5 - ticks5 - 1;
//            }
//            if (nticks5 > 0){
//                cl_populations[0].add_several(ticks5 + 1, ticks5 + nticks5, (double) Scount / spar5.tot_size);
//                cl_populations[1].add_several(ticks5 + 1, ticks5 + nticks5, (double) Ccount / spar5.tot_size);
//                cl_populations[2].add_several(ticks5 + 1, ticks5 + nticks5, (double) Dcount / spar5.tot_size);
//                latdyn.add_on_several_steps(spar5, lattice_ptr5, inf_list5, ticks5 + 1, ticks5 + nticks5);
//                ticks5 += nticks5;
//            }
//
//            if (jump5.type == 'i'){
//                Scount -= 1;
//                Ccount += 1;
//            }
//            else{
//                Ccount -= 1;
//                Dcount += 1;
//            }
//
//            jumping_after_data_coll(spar5, lattice_ptr5, inf_list5, jump5);
//        }
//
//        if (ticks5 < max_steps5 - 1){
//            cl_populations[0].add_several(ticks5 + 1, max_steps5 - 1, (double) Scount / spar5.tot_size);
//            cl_populations[1].add_several(ticks5 + 1, max_steps5 - 1, (double) Ccount / spar5.tot_size);
//            cl_populations[2].add_several(ticks5 + 1, max_steps5 - 1, (double) Dcount / spar5.tot_size);
//            latdyn.add_on_several_steps(spar5, lattice_ptr5, inf_list5, ticks5 + 1, max_steps5 - 1);
//        }
//
//        cl_populations[0].save_to_file("Test_Spop.txt");
//        cl_populations[1].save_to_file("Test_Cpop.txt");
//        cl_populations[2].save_to_file("Test_Dpop.txt");
//
//        latdyn.save_to_files(spar5);
//        produce_sum_of_dyn_obs("Cpp_Lattice_map_I_3_x_3.txt", "Cpp_Lattice_map_B_3_x_3.txt");
//    }
//}





// From initialisations.h




//template <int rows, int cols> void init_allS_square_lattice(system_parameters spar0, patient ptab0[rows][cols]){
//
//    for (int i = 0; i < rows; i++){
//        for (int j = 0; j < cols; j++){
//            ptab0[i][j].init_coords(spar0, i, j);
//        }
//    }
//}


//patient** create_patients(system_parameters spar0){
//    patient patient_lattice5[spar0.LX][spar0.LY];
//    patient* lattice_ptr5[spar0.LX];
//    for (int i = 0; i < spar0.LX; i++){
//        lattice_ptr5[i] = patient_lattice5[i];
//    }
//    return lattice_ptr5;
//}





// From file_management.h




//string determine_new_file_end(string fname, string grouping){
//    // The argument grouping should be something identifying all files produced by a given simulation, something like
//    // "batch" or "group" or "set".
//    if (!file_already_exists(fname)){
//        return "";
//    }
//    else{
//        string fname_wo_ext = fname;
//        fname_wo_ext.erase(fname_wo_ext.end() - 4, fname_wo_ext.end());
//        bool available = 0;
//        int ii = 0;
//        while (!available){
//            fname = fname_wo_ext + "_" + grouping + to_string(ii) + ".txt";
//            if (!file_already_exists(fname)){
//                available = 1;
//            }
//            else{
//                ii++;
//            }
//        }
//        return grouping + to_string(ii);
//    }
//}


//string classify_filename(string base_fname, int LX0, int LY0, long int n_traj5, string grouping){
//    string fname5 = insert_before_ext(base_fname, "_" + to_string(LX0) + "_x_" + to_string(LY0) + "_t" + to_string(n_traj5));
//    string classifier = determine_classifier(fname5, grouping);
//    return insert_before_ext(fname5, classifier);
//}












// FROM Inputs.h



//bool get_dyn_params_from_file0(system_parameters &spar6, dyn_params &dpar6, string fname){
//    ifstream pfile;
//    bool valid_params = 0;
//    if (file_already_exists(fname)){
//        pfile.open(fname);
//        int num_of_lines = sizeof(dyn_labels) / sizeof(dyn_labels[0]);
//        bool acquired[num_of_lines];
//        for (int i = 0; i < num_of_lines; i++){
//            acquired[i] = 0;
//        }
//
//        double gd6 = -1, gi6 = -1, ohm6 = -1;
//        int LX6 = -1, LY6 = -1;
//        double max_time6 = -1, dt6 = -1;
//        long int n_trajs6 = -1;
//        double t_thresh6 = -1;
//        bool thresh_encountered = 0;
//
//        string aux_line;
//        int aux_pos;
//        while(getline(pfile, aux_line)){
//            aux_pos = find_corresponding_param(aux_line, num_of_lines);
//            if (aux_pos >= 0){
//                if (!acquired[aux_pos]){
//                    switch(aux_pos){
//                        case 0:
//                            LX6 = pluck_value<int>(aux_pos, aux_line);
//                            if (LX6 > 0){
//                                acquired[0] = 1;
//                            }
//                            break;
//                        case 1:
//                            LY6 = pluck_value<int>(aux_pos, aux_line);
//                            if (LY6 > 0){
//                                acquired[1] = 1;
//                            }
//                            break;
//                        case 2:
//                            gd6 = pluck_value<double>(aux_pos, aux_line);
//                            if (gd6 > 0){
//                                acquired[2] = 1;
//                            }
//                            break;
//                        case 3:
//                            gi6 = pluck_value<double>(aux_pos, aux_line);
//                            if (gi6 > 0){
//                                acquired[3] = 1;
//                            }
//                            break;
//                        case 4:
//                            ohm6 = pluck_value<double>(aux_pos, aux_line);
//                            if (!(isnan(ohm6))){
//                                acquired[4] = 1;
//                            }
//                            break;
//                        case 5:
//                            max_time6 = pluck_value<double>(aux_pos, aux_line);
//                            if (max_time6 > 0){
//                                if (acquired[6]){
//                                    if (max_time6 >= dt6){
//                                        acquired[5] = 1;
//                                    }
//                                }
//                                else{
//                                    acquired[5] = 1;
//                                }
//                            }
//                            break;
//                        case 6:
//                            dt6 = pluck_value<double>(aux_pos, aux_line);
//                            if (dt6 > 0){
//                                if (acquired[5]){
//                                    if (max_time6 >= dt6){
//                                        acquired[6] = 1;
//                                    }
//                                }
//                                else{
//                                    acquired[6] = 1;
//                                }
//                            }
//                            break;
//                        case 7:
//                            n_trajs6 = pluck_value<long int>(aux_pos, aux_line);
//                            if (n_trajs6 >= 0){
//                                acquired[7] = 1;
//                            }
//                            break;
//                        case 8:
//                            t_thresh6 = pluck_value<double>(aux_pos, aux_line);
//                            if (t_thresh6 > 0 && t_thresh6 < 1){
//                                thresh_encountered = 1;
//                            }
//                            break;
//                    }
//                }
//            }
//        }
//        pfile.close();
//        cout << "Input file now closed. It can be changed without risk.\n";
//        valid_params = check_alltrue(acquired, 8);
//        if (valid_params){
//            if (!thresh_encountered){
//                spar6 = system_parameters(LX6, LY6, gd6, gi6, ohm6);
//                dpar6.max_time = max_time6;
//                dpar6.num_of_trajs = n_trajs6;
//                dpar6.tstep = dt6;
//            }
//            else{
//                spar6 = system_parameters(LX6, LY6, gd6, gi6, ohm6, t_thresh6);
//                dpar6.max_time = max_time6;
//                dpar6.num_of_trajs = n_trajs6;
//                dpar6.tstep = dt6;
//            }
//        }
//    }
//    else{
//        cout << "The input file was not found. A new one is being created now.\n";
//        create_empty_dyn_inputfile(fname);
//        cout << "Done. You can now insert some parameters and relaunch the code.\n";
//        cout << "The file is named " + fname + ".\n";
//    }
//    return valid_params;
//}






// From QEP_data_collection.h



//
//
//void dyndata_collection(){
//
//
//    system_parameters syspars;
//    dyn_params dynpars;
//    char choice;
//
//    bool got_parameters, repeat = 0;
//
//    cout << "Starting the procedure for the collection of dynamical data.\n";
//
//    do{
//        cout << "Please enter 'f' or 'F' if you wish to define the parameters through 'dyn_input.txt'.\n";
//        cout << "Enter 'm' or 'M' for manually inserting the parameters.\n";
//        cout << "Any other key will terminate this instance.\n";
//        cout << "Your choice: ";
//        cin >> choice;
//
//
//        switch(choice){
//            case 'm': case 'M':
//                got_parameters = typing_in_parameters(syspars, dynpars);
//                if (got_parameters){
//                    cout << "Parameters successfully acquired.\n";
//
//                        if (dynpars.num_of_trajs == 0){
//
//                            do{
//                                cout << "Starting simulation without a defined number of trajectories.\n";
//                                cout << "To stop it from running, place a 'end.txt' file in the main folder.\n";
//                                cout << "Stopping it manually will result in no data being saved.\n";
//
//                                run_undef_trajs_up_to_maxtime(syspars, dynpars.max_time, dynpars.tstep);
//
//                                cout << "Would you like to launch another simulation (y/n)? ";
//                                cin >> c_one_more;
//                                if (c_one_more == 'y'){
//                                    cout << endl << "Would you like to use the same parameters as before (y/n)? ";
//                                    cin >> c_same_pars;
//                                    if (c_same_pars == 'y'){
//                                        again = 1;
//                                        repeat = 0;
//                                    }
//                                    else{
//                                        repeat = 1;
//                                        again = 0;
//                                    }
//                                }
//                                else{
//                                    again = 0;
//                                    repeat = 0;
//                                }
//                            }
//                            while(again);
//                        }
//                        else{
//                            do{
//                                cout << "Starting simulation for " << dynpars.num_of_trajs << " trajectories.\n";
//                                cout << "A percentage progression will be displayed.\n";
//
//                                run_def_trajs_up_to_maxtime(syspars, dynpars.max_time, dynpars.tstep, dynpars.num_of_trajs);
//
//                                cout << "Would you like to launch another simulation (y/n)? ";
//                                cin >> c_one_more;
//                                if (c_one_more == 'y'){
//                                    cout << endl << "Would you like to use the same parameters as before (y/n)? ";
//                                    cin >> c_same_pars;
//                                    if (c_same_pars == 'y'){
//                                        again = 1;
//                                        repeat = 0;
//                                    }
//                                    else{
//                                        repeat = 1;
//                                        again = 0;
//                                    }
//                                }
//                                else{
//                                    again = 0;
//                                    repeat = 0;
//                                }
//                            }
//                            while(again);
//                        }
//                }
//                break;
//            case 'f': case 'F':
//                got_parameters = get_dyn_params_from_file(syspars, dynpars, "dyn_input.txt");
//                if (got_parameters){
//                    cout << "Parameters successfully acquired.\n";
//
//                        if (dynpars.num_of_trajs == 0){
//
//                            cout << "Starting simulation without a defined number of trajectories.\n";
//                            cout << "To stop it from running, place a 'end.txt' file in the main folder.\n";
//                            cout << "Stopping it manually will result in no data being saved.\n";
//
//                            run_undef_trajs_up_to_maxtime(syspars, dynpars.max_time, dynpars.tstep);
//                        }
//                        else{
//                            cout << "Starting simulation for " << dynpars.num_of_trajs << " trajectories.\n";
//                            cout << "A percentage progression will be displayed.\n";
//                            run_def_trajs_up_to_maxtime(syspars, dynpars.max_time, dynpars.tstep, dynpars.num_of_trajs);
//                        }
//                }
//                break;
//            default:
//                break;
//        }
//
//    }
//    while(repeat);
//
//}
