import numpy as np
import os

dont_skip = True
order_of_magn = 10000

Cpp_param_data_dir = "D:/Another folder if present/Otherwise a folder where no parameter files are stored/"
Cpp_param_original_dir = "C:/Folder where parameter files are stored/"
# These folders are the so-called "main" folders of the C++ code

Cpp_density_dir = "PD_Final_Population/"
Cpp_times_dir = "PD_ID_times/"

processing_dir = "D:/Where the collated files should appear/"
# New folder

# DATA FILE INCIPITS
param_dfi = "PD_Cpp_Parameters_"
PD_values_dfi = "PD_Cpp_PDpoints"
Dval1_dfi = "PD_Cpp_D_by_traj"
Dval2_dfi = "PD_Cpp_Dpop"
SCDval_dfi = "PD_Cpp_SCDpop"
ID_times_dfi = "PD_Cpp_IDtimes"

param_dfi_len = len(param_dfi)
PD_values_dfi_len = len(PD_values_dfi)
Dval1_dfi_len = len(Dval1_dfi)
Dval2_dfi_len = len(Dval2_dfi)
SCDval_dfi_len = len(SCDval_dfi)

filext = ".txt"

# PROCESSED FILE INCIPITS
param_pfi = "C_PD_parameters"

base_identifier = "_parbatch"


class PD_params:

    def __init__(self, fname):
        self.LX = 0
        self.LY = 0
        self.gimin = 0
        self.gimax = 0
        self.ngi = 0
        self.ohmmin = 0
        self.ohmmax = 0
        self.nohm = 0
        self.ntraj = 0

        shortened_fname = fname[(fname.rfind("/") + 1):]

        is_a_processed_file = shortened_fname.startswith(param_pfi)

        if not (is_a_processed_file):
            file1 = open(fname, "r")
            one_line = file1.readline()
            self.LX = self.extract_parvalue_from_line(one_line, True)
            one_line = file1.readline()
            self.LY = self.extract_parvalue_from_line(one_line, True)
            one_line = file1.readline()
            self.gimin = self.extract_parvalue_from_line(one_line, False)
            one_line = file1.readline()
            self.gimax = self.extract_parvalue_from_line(one_line, False)
            one_line = file1.readline()
            self.ngi = self.extract_parvalue_from_line(one_line, True)
            one_line = file1.readline()
            self.ohmmin = self.extract_parvalue_from_line(one_line, False)
            one_line = file1.readline()
            self.ohmmax = self.extract_parvalue_from_line(one_line, False)
            one_line = file1.readline()
            self.nohm = self.extract_parvalue_from_line(one_line, True)
            one_line = file1.readline()
            self.ntraj = self.extract_parvalue_from_line(one_line, True)
            file1.close()
        else:
            file1 = open(fname, "r")
            one_line = file1.readline()
            self.LX = self.extract_parvalue_from_line(one_line, True)
            one_line = file1.readline()
            self.LY = self.extract_parvalue_from_line(one_line, True)
            one_line = file1.readline()
            self.gimin = self.extract_parvalue_from_line(one_line, False)
            one_line = file1.readline()
            self.gimax = self.extract_parvalue_from_line(one_line, False)
            one_line = file1.readline()
            self.ngi = self.extract_parvalue_from_line(one_line, True)
            one_line = file1.readline()
            self.ohmmin = self.extract_parvalue_from_line(one_line, False)
            one_line = file1.readline()
            self.ohmmax = self.extract_parvalue_from_line(one_line, False)
            one_line = file1.readline()
            self.nohm = self.extract_parvalue_from_line(one_line, True)
            one_line = file1.readline()
            self.ntraj = self.extract_parvalue_from_line(one_line, True)
            file1.close()

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

    def print_values(self):
        print(f"LX = {self.LX}")
        print(f"LY = {self.LY}")
        print(f"gimin = {self.gimin}")
        print(f"gimax = {self.gimax}")
        print(f"ngi = {self.ngi}")
        print(f"ohmmin = {self.ohmmin}")
        print(f"ohmmax = {self.ohmmax}")
        print(f"nohm = {self.nohm}")
        print(f"ntraj = {self.ntraj}")

    def within_thresh(self, value1, value2, thresh=0.001):
        return (abs((value2 - value1) / (value2 + value1)) < thresh)

    def same_pars(self, other_pars, thresh=0.001):
        bool1 = self.LX == other_pars.LX
        if bool1:
            bool1 = bool1 and (self.LY == other_pars.LY)
            if bool1:
                bool1 = bool1 and self.within_thresh(self.gimin, other_pars.gimin, thresh)
                if bool1:
                    bool1 = bool1 and self.within_thresh(self.gimax, other_pars.gimax, thresh)
                    if bool1:
                        bool1 = bool1 and self.ngi == other_pars.ngi
                        if bool1:
                            bool1 = bool1 and self.within_thresh(self.ohmmin, other_pars.ohmmin, thresh)
                            if bool1:
                                bool1 = bool1 and self.within_thresh(self.ohmmax, other_pars.ohmmax, thresh)
                                if bool1:
                                    bool1 = bool1 and self.nohm == other_pars.nohm
        return bool1


def within_precision(value1, value2, thresh=0.001):
    return (abs((value2 - value1) / (value2 + value1)) < thresh)


def prepare_par_fname_list():
    init_list = os.listdir(Cpp_param_data_dir)
    jj = 0
    while jj < len(init_list):
        if not os.path.isfile(Cpp_param_data_dir + init_list[jj]):
            del init_list[jj]
        elif not (init_list[jj].startswith(param_dfi)):
            del init_list[jj]
        else:
            init_list[jj] = Cpp_param_data_dir + init_list[jj]
            jj += 1

    init_list2 = os.listdir(Cpp_param_original_dir)
    jj = 0
    while jj < len(init_list2):
        if not os.path.isfile(Cpp_param_original_dir + init_list2[jj]):
            del init_list2[jj]
        elif not (init_list2[jj].startswith(param_dfi)):
            del init_list2[jj]
        else:
            init_list2[jj] = Cpp_param_original_dir + init_list2[jj]
            jj += 1

    return init_list + init_list2


def prepare_Cpar_fname_list():
    init_list = os.listdir(processing_dir)
    jj = 0
    while jj < len(init_list):
        if not os.path.isfile(processing_dir + init_list[jj]):
            del init_list[jj]
        elif not (init_list[jj].startswith(param_pfi)):
            del init_list[jj]
        else:
            init_list[jj] = processing_dir + init_list[jj]
            jj += 1

    return init_list


def find_equal_pars_to(pars, pars_list):
    """This compares all parameters EXCEPT the number of trajectories, as it should. It does so with those recorded in a
    growing sequence of parameter lists to check whether the same combination has been touched already, in which case
    this element should join the previous ones"""
    ii = 0
    while ii < len(pars_list):
        if pars.same_pars(pars_list[ii]):
            return ii
        else:
            ii += 1
    return -1


def find_same_par_files(fname_list):
    if len(fname_list) > 0:
        fname_table = [[fname_list[0]]]
        auxpos = fname_list[0].rindex("/" + param_dfi)
        endfname_table = [[fname_list[0][(auxpos + param_dfi_len):-4]]]
        folder_table = [[fname_list[0][:auxpos + 1]]]
        pars_list = [PD_params(fname_list[0])]
        aux_trajn = pars_list[0].ntraj
        traj_list = [[aux_trajn]]
        # pars_list[0] = pars_list[0][:-1]
        # syssizes = ["_" + str(int(pars_list[0][0])) + "_x_" + str(int(pars_list[0][1]))]
        for jj in range(1, len(fname_list)):
            current_pars = PD_params(fname_list[jj])
            index = find_equal_pars_to(current_pars, pars_list)
            if index < 0:
                pars_list.append(current_pars)
                fname_table.append([fname_list[jj]])
                auxpos = fname_list[jj].rindex("/" + param_dfi)
                endfname_table.append([fname_list[jj][(auxpos + param_dfi_len):-4]])
                folder_table.append([fname_list[jj][:auxpos + 1]])
                traj_list.append([current_pars.ntraj])
                # syssizes.append("_" + str(int(current_pars[0])) + "_x_" + str(int(current_pars[1])))
            else:
                fname_table[index].append(fname_list[jj])
                traj_list[index].append(current_pars.ntraj)
                auxpos = fname_list[jj].rindex("/" + param_dfi)
                endfname_table[index].append(fname_list[jj][(auxpos + param_dfi_len):-4])
                folder_table[index].append(fname_list[jj][:auxpos + 1])

        return fname_table, pars_list, traj_list, endfname_table, folder_table  # , syssizes
    else:
        return [], [], [], [], []  # , []


def compute_tot_traj(traj_list):
    for trajs in traj_list:
        aux_sum = 0
        for i in range(len(trajs)):
            aux_sum += trajs[i]
        trajs.append(aux_sum)


def avoid_overwriting_file(filename):
    fname_wout_ext = filename[:-4]
    fname_ext = filename[-4:]
    if not os.path.exists(filename):
        return filename
    else:
        i = 0
        found = False
        while not found:
            aux_fname = fname_wout_ext + "_" + str(i) + fname_ext
            if not os.path.exists(aux_fname):
                found = True
            else:
                i += 1
        return aux_fname


def avoid_overwriting_param_file(filename):
    fname_wout_ext = filename[:-4]
    fname_ext = filename[-4:]
    i = 0
    found = False
    while not found:
        aux_fname = fname_wout_ext + "_" + str(i) + fname_ext
        if not os.path.exists(aux_fname):
            found = True
        else:
            i += 1
    return aux_fname


def extract_identifier_num_from_Cpar_fname(Cfname1):
    aux_fn = Cfname1[:-4]
    aux_pos = aux_fn.rfind(base_identifier)
    return int(aux_fn[(aux_pos + len(base_identifier)):])


def extract_identifier_from_Cpar_fname(Cfname1):
    aux_fn = Cfname1[:-4]
    aux_pos = aux_fn.rfind(base_identifier)
    return aux_fn[(aux_pos):]


def write_numlist_excluding_some(list_length, to_exclude_list):
    counter1 = 1
    auxlist = []
    while len(auxlist) < list_length:
        if not counter1 in to_exclude_list:
            auxlist.append(counter1)
        counter1 += 1
    return auxlist


def recording_Cparam_files(parlist1, trlist1, base_Cpar_fname=processing_dir + param_pfi):
    already_processed = prepare_Cpar_fname_list()
    Cpars_list = [PD_params(aux_ff) for aux_ff in already_processed]
    already_taken_IDnums = [extract_identifier_num_from_Cpar_fname(aux_ff) for aux_ff in already_processed]

    aux_len = len(parlist1)
    available_nums = write_numlist_excluding_some(aux_len, already_taken_IDnums)

    identifier_list1 = ["" for jj in range(aux_len)]
    corresponding_filenums = [-1 for jj in range(aux_len)]

    counter0 = 0
    to_process_list1 = []
    for jj in range(aux_len):
        corresponding_filenums[jj] = find_equal_pars_to(parlist1[jj], Cpars_list)
        if corresponding_filenums[jj] >= 0:
            identifier_list1[jj] = extract_identifier_from_Cpar_fname(already_processed[corresponding_filenums[jj]])
            if Cpars_list[corresponding_filenums[jj]].ntraj == trlist1[jj][-1]:
                to_process_list1.append(False)
                if not dont_skip:
                    print(f"Not adding to: {identifier_list1[jj]}")
            else:
                to_process_list1.append(True)
                print(f"Adding to: {identifier_list1[jj]}")
        else:
            identifier_list1[jj] = base_identifier + str(available_nums[counter0])
            counter0 += 1
            to_process_list1.append(True)
            print(f"Creating anew: {identifier_list1[jj]}")
    print("\n\n")

    for jj in range(aux_len):
        # identifier_list1.append(base_identifier + str(jj + 1))
        if to_process_list1[jj]:
            auxf1 = open(base_Cpar_fname + identifier_list1[jj] + ".txt", "w")
            auxf1.write(f"LX: {parlist1[jj].LX}\n")
            auxf1.write(f"LY: {parlist1[jj].LY}\n")
            auxf1.write(f"gi min value: {parlist1[jj].gimin}\n")
            auxf1.write(f"gi max value: {parlist1[jj].gimax}\n")
            auxf1.write(f"number of gis: {parlist1[jj].ngi}\n")
            auxf1.write(f"ohm min value: {parlist1[jj].ohmmin}\n")
            auxf1.write(f"ohm max value: {parlist1[jj].ohmmax}\n")
            auxf1.write(f"number of ohms: {parlist1[jj].nohm}\n")
            auxf1.write(f"tot number of trajs: {trlist1[jj][-1]}")

            auxf1.close()

    return identifier_list1, to_process_list1


def reconstruct_pop_fnames(fold_list1, endf_list1):
    auxl_Dtraj = [fold_list1[0] + Cpp_density_dir + Dval1_dfi + endf_list1[0] + ".txt"]
    auxl_D = [fold_list1[0] + Cpp_density_dir + Dval2_dfi + endf_list1[0] + ".txt"]
    auxl_SCD = [fold_list1[0] + Cpp_density_dir + SCDval_dfi + endf_list1[0] + ".txt"]
    for i in range(1, len(fold_list1)):
        auxl_Dtraj.append(fold_list1[i] + Cpp_density_dir + Dval1_dfi + endf_list1[i] + ".txt")
        auxl_D.append(fold_list1[i] + Cpp_density_dir + Dval2_dfi + endf_list1[i] + ".txt")
        auxl_SCD.append(fold_list1[i] + Cpp_density_dir + SCDval_dfi + endf_list1[i] + ".txt")

    return auxl_Dtraj, auxl_D, auxl_SCD


def reconstruct_IDtimes_fn(folder1, endf1, gi_count, gi_num, ohm_count, ohm_num):
    auxl = folder1 + Cpp_times_dir + ID_times_dfi + endf1
    auxpart = "_g" + str(gi_count) + "_" + str(gi_num)
    auxpart += "_o" + str(ohm_count) + "_" + str(ohm_num)
    aux_index = auxl.index("_batch")
    # We have to check now whether we are working with a file from a different machine.
    # These do not end in _batch, but rather _An_batch with n an integer
    temp_str = auxl[:aux_index]
    aux_index2 = temp_str.rfind("_")
    if aux_index2 == -1:
        print("ERROR! There should be at least another '_' character in this string!")
    elif aux_index2 == len(temp_str) - 1:
        print("ERROR! Why are there two consecutive '_' characters?")
    else:
        if temp_str[aux_index2 + 1] == "A":
            # then we are working with an alternate file
            try:
                def_int = int(temp_str[(aux_index2 + 2):])
                got_an_int = True
            except:
                got_an_int = False
                print("WARNING! Alternate filename not written according to specifications!")
            if got_an_int:
                aux_index = aux_index2

    auxl = auxl[:aux_index] + auxpart + auxl[aux_index:]
    auxl += ".txt"
    return auxl


def prepare_IDfn_onebatch_table(fold2, endf2, PDpoints2):
    x2 = len(PDpoints2)
    y2 = len(PDpoints2[0])
    auxtt = []
    for i in range(x2):
        auxll = []
        for j in range(y2):
            auxll.append(reconstruct_IDtimes_fn(fold2, endf2, i + 1, x2, j + 1, y2))
        auxtt.append(auxll)
    return auxtt


def initialise_IDfn_firstbatch_table(fold2, endf2, PDpoints2):
    x2 = len(PDpoints2)
    y2 = len(PDpoints2[0])
    auxtt = []
    for i in range(x2):
        auxll = []
        for j in range(y2):
            auxll.append([reconstruct_IDtimes_fn(fold2, endf2, i + 1, x2, j + 1, y2)])
        auxtt.append(auxll)
    return auxtt


def IDfn_multibatch_join(matrix_of_lists, matrix_of_fns, PDpoints2):
    x1 = len(matrix_of_lists)
    x2 = len(matrix_of_fns)
    auxtt = [[]]
    if x1 != x2:
        print("   ERROR! The two matrices of ID filenames should have an equal number of rows!")
    else:
        y1 = len(matrix_of_lists[0])
        y2 = len(matrix_of_fns[0])
        if y1 != y2:
            print("   ERROR! The two matrices of ID filenames should have an equal number of columns!")
        else:
            if (x1 != len(PDpoints2) or y1 != len(PDpoints2[0])):
                print("   ERROR! The size of these matrices should match the number")
            else:
                auxtt = [[matrix_of_lists[ii][jj].append(matrix_of_fns[ii][jj]) for jj in range(y1)] for ii in
                         range(x1)]
    return auxtt


def reconstruct_IDtimes_fnames(fold_list1, endf_list1, PDpoints1):
    aux_mat = initialise_IDfn_firstbatch_table(fold_list1[0], endf_list1[0], PDpoints1)
    auxlen = len(fold_list1)
    if not auxlen == len(endf_list1):
        print("ERROR! The lengths of the folders list and the endfile parts list should match!")
        return [[[]]]
    else:
        for jj in range(1, auxlen):
            IDfn_multibatch_join(aux_mat, prepare_IDfn_onebatch_table(fold_list1[jj], endf_list1[jj], PDpoints1),
                                 PDpoints1)
        return aux_mat


def form_listpars(PDpoints_fname):
    aux_file = open(PDpoints_fname, "r")
    auxl = []
    lines1 = aux_file.readlines()
    for i in range(len(lines1)):
        auxi = lines1[i].index("]")
        aux_line = lines1[i][(auxi + 2):]
        auxi2 = aux_line.index(" ")
        auxl.append([float(aux_line[:auxi2]), float(aux_line[(auxi2 + 1):])])

    aux_file.close()
    return auxl


def form_tabpars(PDpoints_fname, pars):
    ng = pars.ngi
    no = pars.nohm
    auxt = np.zeros((ng, no, 2))

    aux_file = open(PDpoints_fname, "r")
    lines1 = aux_file.readlines()
    for i in range(len(lines1)):
        auxp1 = lines1[i].index("[") + 1
        auxp2 = lines1[i].index("/")
        if not lines1[i][auxp1] == "g":
            print("PROBLEM! Wrong format when trying to retrieve the gi datapoint!")
        else:
            counter_gi = int(lines1[i][(auxp1 + 1):auxp2]) - 1
            auxp3 = lines1[i].index(", ")
            ng_check = int(lines1[i][(auxp2 + 1): auxp3])
            if not ng_check == ng:
                print("PROBLEM! Inconsistent number of gamma_Is!")
            else:
                aux_line = lines1[i][(auxp3 + 2):]
                if aux_line[0] != "o":
                    print("PROBLEM: Wrong format when trying to retrieve the ohm datapoint!")
                else:
                    auxp4 = aux_line.index("/")
                    counter_ohm = int(aux_line[1:auxp4]) - 1
                    auxp5 = aux_line.index("]")
                    no_check = int(aux_line[(auxp4 + 1): auxp5])
                    if not no_check == no:
                        print("PROBLEM! Inconsistent number of Omegas!")
                    else:
                        aux_line = aux_line[(auxp5 + 1):]
                        while aux_line[0] == " ":
                            aux_line = aux_line[1:]
                        auxi2 = aux_line.index(" ")
                        # auxl.append([float(aux_line[:auxi2]), float(aux_line[(auxi2 + 1):])])
                        auxt[counter_gi, counter_ohm] = [float(aux_line[:auxi2]), float(aux_line[(auxi2 + 1):])]

    aux_file.close()
    return auxt


def SCD_extract_from_line(line2):
    auxl = np.zeros(5)
    for i in range(5):
        i0 = line2.find(" ")
        if (i0 >= 0):
            auxl[i] = float(line2[:i0])
            line2 = line2[(i0 + 1):]
        else:
            auxl[i] = float(line2)
    return auxl


def D_extract_from_line(line2):
    auxl = np.zeros(3)
    for i in range(3):
        i0 = line2.find(" ")
        if (i0 >= 0):
            auxl[i] = float(line2[:i0])
            line2 = line2[(i0 + 1):]
        else:
            auxl[i] = float(line2)
    return auxl


def Dtraj_extract_from_line(line2):
    auxl = np.array([])
    i0 = 0
    while i0 >= 0:
        i0 = line2.find(" ")
        if (i0 >= 0):
            auxl = np.append(auxl, float(line2[:i0]))
            line2 = line2[(i0 + 1):]
        else:
            auxl = np.append(auxl, float(line2))
    return auxl


def write_strlist_to_file(lst1, fname1, counter, last_val):
    auxi1 = len(lst1)
    if counter == 0:
        mode = "w"
    else:
        mode = "a"
    auxf1 = open(fname1, mode)
    auxf1.write(str(lst1[0]))
    for i in range(1, auxi1):
        auxf1.write(" " + str(lst1[i]))
    if counter != last_val:
        auxf1.write("\n")
    auxf1.close()


def check_IDfname_matrix(mat2, checklen):
    aux1 = len(mat2)
    aux2 = len(mat2[0])
    auxi = len(mat2[0][0])
    if not auxi == checklen:
        print("   WARNING! Wrong length of the IDtimes matrix elements!")
    for i in range(aux1):
        for j in range(aux2):
            if len(mat2[i][j]) != auxi:
                print("   WARNING! The matrix for the IDtimes filenames has elements of different lengths!")

            for mm in range(auxi):
                pos_g = mat2[i][j][mm].rfind("_g" + str(i + 1))
                if pos_g < 0:
                    print("   WARNING! gamma_I value not found in the filename!")

                pos_o = mat2[i][j][mm].rfind("_o" + str(j + 1))
                if pos_o < 0:
                    print("   WARNING! Omega value not found in the filename!")


def IDT_extract_from_line(auxline):
    pos1 = auxline.index(" ")
    auxint = int(auxline[:pos1])
    auxll = auxline[(pos1 + 1):]
    pos2 = auxll.index(" ")
    auxf1 = float(auxll[:pos2])
    auxll = auxll[(pos2 + 1):]
    auxf2 = float(auxll)

    return auxint, auxf1, auxf2


def create_empty_2list(len2):
    auxl = []
    for il in range(len2):
        auxl.append([])
    return auxl


def combine_all_files(parlist1, fold_tab1, endf_tab1, tr_tab1, idfr_list1, to_proc_list1):
    ll1 = len(fold_tab1)
    for i in range(ll1):  # LOOP OVER DIFFERENT PARAMETER SETS
        if (not to_proc_list1[i]) and (not dont_skip):
            print(f"Parameter set {i + 1} of {ll1} has no new data. Skipped.")
        else:
            Dtraj_list1, Dlist1, SCDlist1 = reconstruct_pop_fnames(fold_tab0[i], endf_tab0[i])
            Dvals_by_traj = []
            if (len(Dtraj_list1) == len(Dlist1) and len(Dlist1) == len(SCDlist1)):
                ll2 = len(Dtraj_list1)
            else:
                print("ERROR! The lists of filenames should feature the same lenght!")
            num_gi = parlist1[i].ngi
            num_ohm = parlist1[i].nohm
            LX1 = parlist1[i].LX
            LY1 = parlist1[i].LY
            syssize = LX1 * LY1
            PDpoints_tab1 = form_tabpars(fold_tab1[i][0] + PD_values_dfi + endf_tab1[i][0] + ".txt", parlist1[i])

            sup_dist = int(np.sqrt(((LX1 + 1) / 2) ** 2 + ((LY1 + 1) / 2) ** 2))
            cutoff_dist = min(int((LX1 + 1) / 2), int((LY1 + 1) / 2))
            if cutoff_dist >= sup_dist:
                print("Cutoff NOT OK")

            IDtimes_fnames = reconstruct_IDtimes_fnames(fold_tab1[i], endf_tab1[i], PDpoints_tab1)
            check_IDfname_matrix(IDtimes_fnames, ll2)

            for k in range(ll2):  # LOOP OVER DIFFERENT FILES IN THE SAME PARAMETER SET
                aux_file1 = open(Dtraj_list1[k], "r")
                aux_file2 = open(Dlist1[k], "r")
                aux_file3 = open(SCDlist1[k], "r")
                Dtraj_lines = aux_file1.readlines()
                Dlines = aux_file2.readlines()
                SCDlines = aux_file3.readlines()
                if not (len(Dtraj_lines) == len(Dlines) and len(Dlines) == len(SCDlines) and len(
                        SCDlines) == num_ohm * num_gi):
                    print("ERROR! These files should have the same number of lines!")
                else:
                    ll3 = len(Dtraj_lines)
                    for m in range(
                            ll3):  # LOOP OVER DIFFERENT PARAMETERS gamma_I AND Omega FOR A PARTICULAR FILE (k) IN A PARAMETER SET (i)
                        SCD_line = SCD_extract_from_line(SCDlines[m])
                        D_line = D_extract_from_line(Dlines[m])
                        Dtraj_line = Dtraj_extract_from_line(Dtraj_lines[m])
                        if not within_precision(SCD_line[2] + SCD_line[4], syssize):
                            print("   WARNING! Populations sum different from system size!")
                        if not SCD_line[3] == 0:
                            print("   WARNING! Non-zero I population!")
                        if not (SCD_line[0] == D_line[0] and SCD_line[1] == D_line[1] and SCD_line[-1] == D_line[-1]):
                            print("   WARNING! Discrepancy between the SCD and D files!")
                        if not (len(Dtraj_line) - 2) == tr_tab1[i][k]:
                            print("   WARNING! There should be as many values as there are trajectories!")
                        if not (within_precision(Dtraj_line[0], PDpoints_tab1[int(m / num_ohm)][int(m % num_ohm)][
                            0]) and within_precision(Dtraj_line[1],
                                                     PDpoints_tab1[int(m / num_ohm)][int(m % num_ohm)][1])):
                            print(
                                "   WARNING! The parameters in the file do not match the expected one at this position!")
                        if not (within_precision(np.mean(Dtraj_line[2:]), D_line[2])):
                            print(
                                "   WARNING! The mean of the values by trajectory does not match the one recorded in the D file!")
                        if (k == 0):
                            Dvals_by_traj.append([int(xx) for xx in Dtraj_line[2:].tolist()])
                        else:
                            Dvals_by_traj[m] += [int(xx) for xx in Dtraj_line[2:].tolist()]

                aux_file1.close()
                aux_file2.close()
                aux_file3.close()

            Dfilename = processing_dir + Cpp_density_dir + "Dpop_by_traj" + idfr_list1[i] + ".txt"
            for m in range(ll3):
                temp_list = PDpoints_tab1[int(m / num_ohm)][int(m % num_ohm)].tolist() + Dvals_by_traj[m]
                write_strlist_to_file(temp_list, Dfilename, m, ll3 - 1)

            for g1 in range(num_gi):
                print(
                    f"Processing parameter set {i + 1} of {ll1}. Remaining gamma_I values for these parameters: {num_gi - g1}")
                for o1 in range(num_ohm):
                    IDtimes_data = create_empty_2list(cutoff_dist + 1)

                    mint = np.zeros(
                        cutoff_dist + 1)  # Array of smaller times (one per distance) to construct a histogram
                    aux_arr = np.zeros(
                        cutoff_dist + 1)  # auxiliary array to check that a min has been chosen from the actual set of times
                    maxt = np.zeros(
                        cutoff_dist + 1)  # Array of greater times (one per distance) to construct a histogram
                    meantd = np.zeros(cutoff_dist + 1)
                    meantd2 = np.zeros(cutoff_dist + 1)
                    meanti = np.zeros(cutoff_dist + 1)
                    meanti2 = np.zeros(cutoff_dist + 1)

                    for k in range(ll2):

                        IDtimes_single_file = open(IDtimes_fnames[g1][o1][k], "r")
                        IDtimes_extracted = IDtimes_single_file.readlines()
                        max_dist_recorded = 0

                        for one_line in IDtimes_extracted:
                            try:
                                dist1, ti, td = IDT_extract_from_line(one_line)
                            except:
                                print(f"Troublesome file: {IDtimes_fnames[g1][o1][k]}")
                                break
                            if dist1 > sup_dist:
                                print("   WARNING! Upper bound over distances overtaken!")
                            elif dist1 <= cutoff_dist:
                                max_dist_recorded = max(max_dist_recorded, dist1)
                                IDtimes_data[dist1] += [ti, td]

                                maxt[dist1] = max(maxt[dist1], td)
                                if aux_arr[dist1] == 0:
                                    mint[dist1] = ti
                                    aux_arr[dist1] = 1
                                else:
                                    mint[dist1] = min(mint[dist1], ti)

                                meantd[dist1] += td
                                meantd2[dist1] += td ** 2

                                meanti[dist1] += ti
                                meanti2[dist1] += ti ** 2

                        IDtimes_single_file.close()

                    Rationalised_IDtimes_single_filename = processing_dir + Cpp_times_dir + "IDtimes_"
                    # Rationalised_IDtimes_single_filename += str(g1 + 1) + "of" + str(num_gi) + "_o" + str(
                    #     o1 + 1) + "of" + str(num_ohm)
                    Rationalised_IDtimes_single_filename += str(LX1) + "_x_" + str(LY1) + "_"
                    Rationalised_IDtimes_single_filename += "for_g"
                    Rationalised_IDtimes_single_filename += str(round(order_of_magn*PDpoints_tab1[g1][o1][0]))
                    Rationalised_IDtimes_single_filename += "_o"
                    Rationalised_IDtimes_single_filename += str(round(order_of_magn*PDpoints_tab1[g1][o1][1]))
                    Rationalised_IDtimes_single_filename += idfr_list1[i]
                    feature_file_name = Rationalised_IDtimes_single_filename + "_feat.txt"
                    Rationalised_IDtimes_single_filename += ".txt"

                    R_single_file = open(Rationalised_IDtimes_single_filename, "w")
                    feat_single_file = open(feature_file_name, "w")

                    for dd in range(max_dist_recorded + 1):
                        ll4 = len(IDtimes_data[dd])
                        if not (ll4 % 2 == 0):
                            print("   WARNING! The length of a list of time PAIRS should be EVEN!")
                        else:
                            countpoints = ll4 // 2

                            meanti[dd] = meanti[dd] / countpoints
                            meanti2[dd] = meanti2[dd] / countpoints
                            meantd[dd] = meantd[dd] / countpoints
                            meantd2[dd] = meantd2[dd] / countpoints

                            auxstr1 = f"Dist = {dd}; mintime = {mint[dd]}; maxtime = {maxt[dd]}; numpoints = {countpoints}; "
                            auxstr2 = f"mean ti = {meanti[dd]}; mean ti^2 = {meanti2[dd]}; stdev ti = {np.sqrt(meanti2[dd] - meanti[dd] ** 2)}; "
                            auxstr3 = f"mean td = {meantd[dd]}; mean td^2 = {meantd2[dd]}; stdev td = {np.sqrt(meantd2[dd] - meantd[dd] ** 2)}"
                            # feat_single_file.write(f"Dist = {dd}; mintime = {mint[dd]}; maxtime = {maxt[dd]}; numpoints = {countpoints}; mean ti = {meanti[dd]}; mean ti^2 = {meanti2[dd]}; stdev ti = {sqrt(meanti2[dd] - meanti[dd]**2)}; mean td = {meantd[dd]}; mean td^2 = {meantd2[dd]}; stdev td = {sqrt(meantd2[dd] - meantd[dd]**2)}")
                            feat_single_file.write(auxstr1 + auxstr2 + auxstr3)
                            for tt in range(ll4):
                                R_single_file.write(str(IDtimes_data[dd][tt]))
                                if (tt < ll4 - 1):
                                    R_single_file.write(" ")
                                elif dd < max_dist_recorded:
                                    R_single_file.write("\n")
                                    feat_single_file.write("\n")
                    R_single_file.close()
                    feat_single_file.close()


fname_tab0, parlist0, tr_tab0, endf_tab0, fold_tab0 = find_same_par_files(prepare_par_fname_list())
compute_tot_traj(tr_tab0)
identifier_list0, to_process_list0 = recording_Cparam_files(parlist0, tr_tab0)

combine_all_files(parlist0, fold_tab0, endf_tab0, tr_tab0, identifier_list0, to_process_list0)



