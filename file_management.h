#ifndef FILEMAN_H
#define FILEMAN_H

#include <stdio.h>
#include <string>
#include <sstream>


bool file_already_exists(string fname){
    ifstream file5;
    file5.open(fname);
    if (file5){
        file5.close();
        return 1;
    }
    else{
        file5.close();
        return 0;
    }
}

void remove_previous_file(string fname){
    if (file_already_exists(fname)){
        remove(&(fname[0]));
    }
}

void rename_file(string previous, string new_name){
    char* prev0 = &(previous[0]);
    char* new0 = &(new_name[0]);
    if (file_already_exists(previous)){
        if (file_already_exists(new_name)){
            remove(new0);
        }
        rename(prev0, new0);
    }
    else{
        cout << "#### WARNING! The following file has not been found: " << previous << " ####" << endl;
    }
}


string insert_before_ext(string orig_fname, string insertion){

    string orig_wo_ext = orig_fname;
    orig_wo_ext.erase(orig_wo_ext.end() - 4, orig_wo_ext.end());
    int ls = orig_fname.length();
    string fext = orig_fname.substr(ls - 4, 4);

    return orig_wo_ext + insertion + fext;
}

string do_not_overwrite(string fname){
    if (!file_already_exists(fname)){
        return fname;
    }
    else{
        string fname_wo_ext = fname;
        fname_wo_ext.erase(fname_wo_ext.end() - 4, fname_wo_ext.end());
        bool available = 0;
        int ii = 0;
        while (!available){
            fname = fname_wo_ext + "_" + to_string(ii) + ".txt";
            if (!file_already_exists(fname)){
                available = 1;
            }
            ii++;
        }
        return fname;
    }
}





string determine_classifier(string base_fname, string grouping, bool ovrwrt = 0){
    bool available = 0;
    int ii = 1;
    string candidate_fname;
    while (!available){
        candidate_fname = insert_before_ext(base_fname, "_" + grouping + to_string(ii));
        if (!file_already_exists(candidate_fname)){
            available = 1;
        }
        else{
            ii++;
        }
    }
    if (!ovrwrt || ii == 1){
        return grouping + to_string(ii);
    }
    else{
        return grouping + to_string(ii - 1);
    }
}




string add_size_fo_fname(string fname, int LX0, int LY0){
    string str_size = "_" + to_string(LX0) + "_x_" + to_string(LY0);
    return insert_before_ext(fname, str_size);
}

string add_pars_to_fname(string fname, int LX0, int LY0, string traj_id, long int n_traj5){
    fname = add_size_fo_fname(fname, LX0, LY0);
    fname = insert_before_ext(fname, "_" + traj_id + to_string(n_traj5));
    return fname;
}

string add_pars_to_fname(string fname, int LX0, int LY0){
    fname = add_size_fo_fname(fname, LX0, LY0);
    return fname;
}

string choose_fname(string fname, bool ovrwrt){
    if (ovrwrt){
        return fname;
    }
    else{
        return do_not_overwrite(fname);
    }
}

bool fname_starts_with(string fname, string seq){
    if (fname.length() < seq.length()){
        return 0;
    }
    else if(fname.substr(0, seq.length()) == seq){
        return 1;
    }
    else{
        return 0;
    }
}


string erase_from_start(string fname, string seq){
    if (fname.length() < seq.length()){
        return "";
    }
    else if(fname.substr(0, seq.length()) == seq){
        return fname.substr(seq.length());
    }
    else{
        return "";
    }
}




void save_dyn_data_with_time(vector<double> arr0, double dt0, string namefile0){
    long int ll0 = arr0.size();
    ofstream twocol_f;
    twocol_f.open(namefile0);
    for (int i = 0; i < ll0; ++i){
        twocol_f << dt0 * i << " " << arr0[i] << endl;
    }
    twocol_f.close();
}


void rename_endfile(){
    if (file_already_exists("end.txt")){
        rename_file("end.txt", "endnot.txt");
    }
}


#endif // FILEMAN_H
