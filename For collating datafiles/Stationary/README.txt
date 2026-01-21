Using the C++ files to generate data from simulations of the eQEP you may have obtained several different files related to the same parameter choices. For instance,
if you run the code twice on two different occasions with the same "PD_input.txt" then it will produce two different files, both of which containing relevant
stochastic information about the same system.

The Python file "Processing_PD_files.py" allows you to collate files into "batches" relating to the same system. Note, however, that it will only recognise them
as being of the same sort if all parameters except the number of trajectories are the same, i.e., same bounds on \gamma_I and \Omega and same number of parameter
points.

Please insert in line 8 the folder where the "PD_Cpp_Parameters..." files are stored. You can insert an extra folder of the same kind in line 7, in case you have, e.g., 
run two instances of the code in parallel or imported data from a different machine.


