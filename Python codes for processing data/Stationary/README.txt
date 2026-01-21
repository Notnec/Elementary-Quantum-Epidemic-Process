Using the C++ files to generate data from simulations of the eQEP you may have obtained several different files related to the same parameter choices. For instance,
if you run the code twice on two different occasions with the same "PD_input.txt" then it will produce two different files, both of which containing relevant
stochastic information about the same system.

The Python file "Processing_PD_files.py" allows you to collate files into "batches" relating to the same system. Note, however, that it will only recognise them
as being of the same sort if all parameters except the number of trajectories are the same, i.e., same bounds on \gamma_I and \Omega and same number of parameter
points.

Please insert in line 8 the folder where the "PD_Cpp_Parameters..." files are stored. You can insert an extra folder of the same kind in line 7, in case you have, e.g., 
run two instances of the code in parallel or imported data from a different machine.
In line 15 insert the folder where you wish the collated files to appear. This folder should contain the two subfolders
  "PD_Final_Population/" and
  "PD_ID_times/".

In the selected folder a number of files will appear equal to the number of different systems simulated, in the sense reported above. For instance, if you have run
dozens of independent simulations all with the same "PD_input.txt", only one file will appear here.
These files are named
  "C_PD_parameters_parbatch<identification number>.txt",
where "identification number" is an index distinguishing different system parameters.
The internal structure of these files is very similar to that of the "PD_input.txt" and should be self-explanatory if you have read the README for the C++ code.


In the subfolder "PD_Final_Population/" as many files will appear as have been produced in its parent folder.
These are called
  "Dpop_by_traj_parbatch<identification number>.txt".
Each of them will contain a number of lines equal to the number of parameter points requested in the corresponding "PD_input.txt". For instance, if a total of
5 \gamma_I values and 7 \Omega values have been simmulated, these files will have 35 lines, one per parameter pair.
The two entries of each line are
  <\gamma_I value> <\Omega value>
The remaining entries on each line (from the third onwards) are values of the stationary dead population summed over all lattice sites obtained in each trajectory.
Equivalently, you can think of these numbers as the final number of dead sites in each trajectory. 
The number of entries in each line, minus 2, will be equal to the TOTAL number of trajectories run for the same parameters. For instance, if you run the C++ code 20
times with the same "PD_input.txt", each time with 15 trajectories, then you will get 20*15 = 300 entries (plus 2) in each line.


The subfolder "PD_ID_times/" will probably collect a very large number of files, since they will be distinguished by parameter pair (\gamma_I, \Omega). In fact, two files
per parameter pair will be produced, called
  (a) "IDtimes_<LX>_x_<LY>_for_g<floor(10000 * \gamma_I value)>_o<floor(10000 * \Omega value)>_parbatch<identification number>.txt"
and
  (b) "IDtimes_<LX>_x_<LY>_for_g<floor(10000 * \gamma_I value)>_o<floor(10000 * \Omega value)>_parbatch<identification number>_feat.txt".
For instance, a file
  "IDtimes_5_x_15_for_g2000_o100_parbatch3.txt"
will correspond to (potentially several) simulations of a system of size 5 x 15 for values \gamma_= = 0.2 and \Omega = 0.01. The system simulated in this case
correspond to the third batch of parameters identified by the Python code, meaning that there are at least two other ones (say, at least two simulations run with
two different "PD_input.txt" files).

Each (a) file will be organised in (potentially) very long lines. Each line will correspond to an integer distance from the origin, starting from 0 and increasing
by 1 at every new line. The entries in each line are in the form
  <t_I> <t_D> <t_I> <t_D> <t_I> <t_D> <t_I> <t_D> .....
with t_I a recorded infection time at that distance and the following t_D the death time of the same site.
These files will not cover the entire lattice. They will instead collect data for distances (from the origin) up to the first time the boundaries of the system are hit.
For instance, for a 101x101 lattice the vertices of the square lie at distance floor(50 * sqrt(2)) = 70, but the files will stop at the 52nd line, corresponding to distance
d = 51.
Note that the length of each line actually depends on the number of stochastic events recorded at each given distance and can therefore fluctuate wildly. However, by
construction the number of entries will always be even.

Each (b) file will have the same number of lines of the corresponding (a) file and will report a limited amount of statistical information about the data recorded in (a).
In order of appearance, a line in a (b) file will provide: the distance from the origin, the minimum t_I, the maximum t_D, the number of t_I t_D pair in the corresponding
line in (a), the mean of t_I values of that line, the mean of t_I^2, the standard deviation of the t_Is and then the same three quantities for t_D.


