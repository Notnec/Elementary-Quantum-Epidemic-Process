import numpy as np
import matplotlib.pyplot as plt
import cmath
import keyboard
import os

name_init = "Cpp_Lattice_map_"
par_init = "Cpp_Parameters_"

init_length = len(name_init)
pop_types = ["S", "I", "B", "D"]

distinguisher = "batch"
len_d = len(distinguisher)

params_folder = "FOLDER WHERE THE Cpp_Parameters FILES ARE LOCATED"
maps_folder = "FOLDER WHERE THE CPP_Lattice_map FILES ARE LOCATED"


class list_of_available_params:

    def __init__(self, folder = params_folder):

        self.all_flist = initialise_fname_list(folder, par_init)
        self.full_detailed_list = []
        self.detailed_list = []

        self.initialise_detlist()

    def initialise_detlist(self):

        for fname in self.all_flist:
            self.full_detailed_list.append(parfile_params(fname))
        self.assign_indices(self.full_detailed_list)
        self.detailed_list = self.full_detailed_list.copy()

    def assign_indices(self, detlist):
        for jj in range(len(detlist)):
            detlist[jj].parfile_index = jj

    def reset_detlist(self):
        self.detailed_list = self.full_detailed_list.copy()
        self.assign_indices(self.detailed_list)

    def print_detlist(self, detlist):
        if (len(detlist) == 0) or (not isinstance(detlist[0], parfile_params)):
            print("ERROR! We have no list of parameter details!")
        else:
            for elem in detlist:
                elem.print_for_choosing()
                # print("\n")
        print("\n")

    def filter(self, lx = "", ly = "", gd = "", gi = "", om = "", trajnum = "", tstep = "", tmax = "", batch = ""):
        if not (isinstance(lx, str)):
            print("ERROR when acquiring filtering values.")
        elif not (isinstance(ly, str)):
            print("ERROR when acquiring filtering values.")
        elif not (isinstance(gd, str)):
            print("ERROR when acquiring filtering values.")
        elif not (isinstance(gi, str)):
            print("ERROR when acquiring filtering values.")
        elif not (isinstance(om, str)):
            print("ERROR when acquiring filtering values.")
        elif not (isinstance(trajnum, str)):
            print("ERROR when acquiring filtering values.")
        elif not (isinstance(tstep, str)):
            print("ERROR when acquiring filtering values.")
        elif not (isinstance(tmax, str)):
            print("ERROR when acquiring filtering values.")
        elif not (isinstance(batch, str)):
            print("ERROR when acquiring filtering values.")
        else:
            filtering_lx, lx0 = self.good_filtering_par(lx, True)
            filtering_ly, ly0 = self.good_filtering_par(ly, True)
            filtering_gd, gd0 = self.good_filtering_par(gd)
            filtering_gi, gi0 = self.good_filtering_par(gi)
            filtering_om, om0 = self.good_filtering_par(om)
            filtering_trajnum, tn0 = self.good_filtering_par(trajnum, True)
            filtering_tstep, tstep0 = self.good_filtering_par(tstep)
            filtering_tmax, tmax0 = self.good_filtering_par(tmax)
            filtering_batch, batch0 = self.good_filtering_par(batch, True)

            jj = 0
            while (jj < len(self.detailed_list)):
                elem = self.detailed_list[jj]
                erasing = False
                if (not erasing) and filtering_lx:
                    if lx0 != int(elem.LX):
                        erasing = True
                if (not erasing) and filtering_ly:
                    if ly0 != int(elem.LY):
                        erasing = True
                if (not erasing) and filtering_gd:
                    if not within_thresh(gd0, float(elem.gd)):
                        erasing = True
                if (not erasing) and filtering_gi:
                    if not within_thresh(gi0, float(elem.gi)):
                        erasing = True
                if (not erasing) and filtering_om:
                    if not within_thresh(om0, float(elem.omega)):
                        erasing = True
                if (not erasing) and filtering_trajnum:
                    if tn0 != int(elem.ntraj):
                        erasing = True
                if (not erasing) and filtering_tstep:
                    if not within_thresh(tstep0, float(elem.dt)):
                        erasing = True
                if (not erasing) and filtering_tmax:
                    if not within_thresh(tmax0, float(elem.max_t)):
                        erasing = True
                if (not erasing) and filtering_batch:
                    if batch0 != int(elem.batch):
                        erasing = True

                if erasing:
                    del self.detailed_list[jj]
                else:
                    jj += 1

            self.assign_indices(self.detailed_list)


    def good_filtering_par(self, val, is_integer = False):
        try:
            if is_integer:
                val_num = int(val)
                filter_by = True
            else:
                val_num = float(val)
                filter_by = True
        except ValueError:
            val_num = -1
            filter_by = False

        return filter_by, val_num

    def return_map_fnames(self, aux_index):
        if aux_index < 0 or aux_index >= len(self.detailed_list):
            print("ERROR! Index not within permissible bounds!")
            return ["", "", "", ""]
        else:
            parfile0 = self.detailed_list[aux_index]
            if not parfile0.viable_file:
                print("ERROR! Bad file!")
                return ["", "", "", ""]
            else:
                fname_ending = parfile0.filename[len(par_init):]
                aux_4list = []
                for ptype in pop_types:
                    aux_4list.append(name_init + ptype + "_" + fname_ending)
                return aux_4list

    def return_map_fnames_and_props(self, aux_index):
        aux_4list = self.return_map_fnames(aux_index)
        if aux_4list[0] != "":
            elem = self.detailed_list[aux_index]
            return aux_4list, elem
        else:
            return None, None

def within_thresh(a, b, thresh = 0.000001):
    try:
        if np.abs((a-b)/(a+b)) <= thresh:
            return True
        else:
            return False
    except ValueError:
        return False

class parfile_params:

    def __init__(self, fname, folder = params_folder):
        self.LX = 0
        self.LY = 0
        self.gd = 0
        self.gi = 0
        self.omega = 0
        self.ntraj = 0
        self.max_t = 0
        self.dt = 0

        self.filename = ""
        self.batch = 0

        self.viable_file = False

        self.parfile_index = -1

        file1 = open(folder + fname, "r")
        one_line = file1.readline()
        self.LX = self.extract_parvalue_from_line(one_line, True)
        one_line = file1.readline()
        self.LY = self.extract_parvalue_from_line(one_line, True)
        one_line = file1.readline()
        self.gd = self.extract_parvalue_from_line(one_line, False)
        one_line = file1.readline()
        self.gi = self.extract_parvalue_from_line(one_line, False)
        one_line = file1.readline()
        self.omega = self.extract_parvalue_from_line(one_line, False)
        one_line = file1.readline()
        self.max_t = self.extract_parvalue_from_line(one_line, False)
        one_line = file1.readline()
        self.dt = self.extract_parvalue_from_line(one_line, False)
        one_line = file1.readline()
        self.ntraj = self.extract_parvalue_from_line(one_line, True)
        file1.close()

        pt, lx1, ly1, ntr1, batch1 = readpar_from_fname(folder + fname, par_init)
        if pt in pop_types:
            print("ERROR! There shouldn't be a population type here!")
        elif int(lx1) != self.LX:
            print(f"ERROR! LX mismatch! {lx1}, {self.LX}")
        elif int(ly1) != self.LY:
            print("ERROR! LY mismatch!")
        elif int(ntr1) != self.ntraj:
            print("ERROR! Trajectory number mismatch!")
        else:
            self.batch = batch1
            self.filename = fname
            self.viable_file = True

    def assign_index(self, ind):
        if isinstance(ind, int) and ind >= 0:
            self.parfile_index = ind
        else:
            print("Please provide a valid index")

    def return_index(self):
        return self.parfile_index


    def extract_parvalue_from_line(self, one_line, is_int, separator=":"):
        index = one_line.find(separator)
        if not one_line[index + 1] == " ":
            print("ERROR! Trouble with reading parameter lines from file!")
        else:
            if (is_int):
                par_value = int(one_line[(index + 2):])
            else:
                par_value = float(one_line[(index + 2):])
        return par_value

    def print_for_choosing(self):
        print(f"({self.parfile_index}) {self.LX} x {self.LY} lattice, gamma_D = {self.gd}, gamma_I = {self.gi}, Omega = {self.omega}, {self.ntraj} trajectories, time step {self.dt} up to t = {self.max_t}. Batch {self.batch}.")



def initialise_fname_list(dir_name, incipit = name_init):
    if not isinstance(dir_name, str):
        print("ERROR! Directory name not in string format!")
        return []
    else:
        if dir_name[-1] != "/":
            dir_name = dir_name + "/"
        init_list = os.listdir(dir_name)
        jj = 0
        while jj < len(init_list):
            if not os.path.isfile(dir_name + init_list[jj]):
                del init_list[jj]
            elif not (init_list[jj].startswith(incipit)):
                del init_list[jj]
            else:
                jj += 1

        return init_list

def readpar_from_fname(fname, incipit = name_init):
    slash_pos = fname.rfind("/")
    if slash_pos >= 0:
        aux_name = fname[slash_pos + 1:]
    else:
        aux_name = fname

    if not aux_name.startswith(incipit):
        print("ERROR! Filename not starting with the specified incipit!")
    else:
        aux_len = len(incipit)
        aux_name = aux_name[aux_len:]

        if incipit == name_init:
            if not aux_name[0] in pop_types:
                print(aux_name)
                print("ERROR! Invalid population type!")
            else:
                ptype = aux_name[0]
                aux_name = aux_name[2:]
        else:
            ptype = ""
            # aux_name = aux_name[1:]

        aux_p1 = aux_name.find("_")
        LX1 = aux_name[:aux_p1]
        aux_name = aux_name[aux_p1 + 3:]

        aux_p2 = aux_name.find("_")
        LY1 = aux_name[:aux_p2]
        aux_name = aux_name[aux_p2 + 2:]

        aux_p3 = aux_name.find("_")
        ntr = aux_name[:aux_p3]
        aux_name = aux_name[aux_p3 + len_d + 1:]

        aux_p4 = aux_name.find(".")
        batch = aux_name[:aux_p4]

        # print(ptype)
        # print(LX1)
        # print(LY1)
        # print(ntr)
        # print(batch)

        return ptype, LX1, LY1, ntr, batch

    return "", "", "", "", ""



def import_maps_from_txt(fname1, sys_props):
    sizex = sys_props.LX
    sizey = sys_props.LY
    arr0 = np.zeros((sizex, sizey))
    arr = np.array([])
    arr_defined = False
    with open(fname1, "r") as file1:
        j = 0
        for line1 in file1:
            if len(line1.split()) == 0:
                j = 0
                if not (arr_defined):
                    arr = [np.transpose(arr0)]
                    arr_defined = True
                else:
                    arr = np.append(arr, [np.transpose(arr0)], axis = 0)
            else:
                arr0[j] = line1.split()
                j += 1
    return arr


def display_density_evol(time_step, av_obs_evol):
    S_evol = av_obs_evol
    fig1, ax1 = plt.subplots(1)
    im = ax1.imshow(S_evol[0], origin='lower', vmin=0, vmax=1)
    up_limit = len(S_evol)
    for jj in range(up_limit):
        im.set_data(S_evol[jj])
        plt.pause(time_step)
    plt.close(fig1)


def movie_cycle(map_fnames, sys_props):
    num_trajs = sys_props.ntraj
    repeat = True
    repeat0 = True
    print("INSTRUCTIONS: you will be required to insert a visualisation timestep. This is the time the system will wait before refreshing\n"
          "the snapshot, or equivalently the time betwween one snapshot of the lattice and the next. Very small values will not actually\n"
          "be effective as the computation time will dominate and end up determining the actual visualisation refresh rate.\n"
          "Should you insert anything but a number the cycle will stop. If instead you enter a valid timestep you will enter another cycle\n"
          "requesting you to choose one of the four populations S, B, I, D. Any input that does not start with one of these four letters\n"
          "(case insensitive) is going to stop the second cycle and bring you back to the timestep choice.\n")
    while repeat0:
        key_press = input("  Insert a snapshot timestep: ")
        try:
            visualisation_tstep = float(key_press)
            repeat = True
        except ValueError:
            print("Exiting the movie cycle.")
            break

        while repeat:
            key_press = input("    Insert a population type: ")
            if key_press[0] == "s" or key_press[0] == "S":
                average_evol = import_maps_from_txt(maps_folder + map_fnames[0], sys_props)/num_trajs
                display_density_evol(visualisation_tstep, average_evol)
            elif key_press[0] == "I" or key_press[0] == "i":
                average_evol = import_maps_from_txt(maps_folder + map_fnames[1], sys_props)/num_trajs
                display_density_evol(visualisation_tstep, average_evol)
            elif key_press[0] == "b" or key_press[0] == "B":
                average_evol = import_maps_from_txt(maps_folder + map_fnames[2], sys_props)/num_trajs
                display_density_evol(visualisation_tstep, average_evol)
            elif key_press[0] == "d" or key_press[0] == "D":
                average_evol = import_maps_from_txt(maps_folder + map_fnames[3], sys_props)/num_trajs
                display_density_evol(visualisation_tstep, average_evol)
            else:
                repeat = False
                break

def filtering_and_choosing(par_list):
    choice_made = False
    safety_count = 0
    while not choice_made:
        safety_count += 1
        if safety_count > 10000:
            choice_made = True
        par_list.print_detlist(par_list.detailed_list)
        print("Please enter a number to choose one of the parameter sets above or 'f' (case insensitive) to  introduce\n"
              "one or more filters to narrow down the choices of 'r' (case insenitive) to remove all filters.\n"
              "Any other choice will exit the programme, including a number not in the correct range.\n")
        key_pressed = input(" Your choice: ")
        if key_pressed[0] == "r" or key_pressed[0] == "R":
            par_list.reset_detlist()
            print("\nFilters reset.\n")
        elif key_pressed[0] == "f" or key_pressed[0] == "F":
            print("You will now be asked to choose values for the system parameters. This should help identify the\n"
                  "data files of interest. If you do not wish to apply a filter for one or more, just insert something\n"
                  "that is not a number (like a string or alphabetic character)")

            lx0 = input("Insert LX: ")
            ly0 = input("Insert LY: ")
            gd0 = input("Insert gamma_D value: ")
            gi0 = input("Insert gamma_I value: ")
            om0 = input("Insert Omega value: ")
            trajnum0 = input("Insert number of trajectories: ")
            tstep0 = input("Insert data recording timestep: ")
            tmax0 = input("Insert maximum time: ")
            batch0 = input("Insert batch number: ")

            par_list.filter(lx0, ly0, gd0, gi0, om0, trajnum0, tstep0, tmax0, batch0)
            print("\nFilters applied.\n")
        else:
            try:
                choice_num = int(key_pressed)
            except ValueError:
                choice_num = -1

            if choice_num >= 0 and choice_num < len(par_list.detailed_list):
                choice_made = True
                return choice_num
            else:
                break
    return -1



selected_num = 0
while selected_num >= 0:
    par_list = list_of_available_params()
    selected_num = filtering_and_choosing(par_list)
    # We now need to pass this number to the function that produces the movies
    if selected_num >= 0:
        map_fnames, system_params = par_list.return_map_fnames_and_props(selected_num)
        movie_cycle(map_fnames, system_params)



