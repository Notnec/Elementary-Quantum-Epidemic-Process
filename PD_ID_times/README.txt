Every time the code is run with the function PDdata_collection() several files will be produced in this folder, one per parameter pair (\gamma_I, \Omega).
Each file will have a name with the following structure:
  "PD_Cpp_IDtimes_<LX>_x_<LY>_t<number of trajectories>_g<index of \gamma_I value>_<total number of \gamma_I values>_o<index of \Omega value>_<total number of \Omega values>_A1_batch<identification number>.txt"
For instance,
  "PD_Cpp_IDtimes_5_x_15_t28_g2_5_o4_5_A1_batch1.txt"
corresponds to the file obtained for a 5 x 15 system for which 28 trajectories were collected and refers to the second of 5 \gamma_I values and the fourth of 5 \Omega values selected to be
investigated. There are therefore 25 different (\gamma_I, \Omega) pairs being considered, and thus in this case this will be one of 25 different files.
"A1_batch" is just a way to avoid overriding the files if another simulation is run on a lattice of the same size and for an equal number of trajectories. A more detailed explanation is included 
in the README of the main folder.

The number of lines in each file is variable. It corresponds to the total number of jumps occurred in all trajectories run for a specific parameter pair, plus the number of trajectories and then divided by 2:
  <number of lines> = ( <total number of jumps in all trajectories> + <number of trajectories> ) / 2.

Each line has three entries:
  <integer distance of a site from the origin> <infection time at that site t_I> <death time at that site t_D>,
where "integer distance" means the integer part of the distance of a given site from the origin (centre) of the lattice. For instance, the next-nearest neighbors of the origin lie at a distance
\sqrt{2} \approx 1.4. The corresponding integer distance is 1.
Having fixed the parameter pair under observation, for each trajectory the infection and death times of every site which experiences a jump are recorded and appended to the file, together with the
corresponding "integer distance".
Note that the lines of a file are not exactly a pure list of stochastic events that took place over the course of many trajectories; every line, in fact, includes the infection and death times
OF THE SAME SITE, which are distinct, albeit not independent, events.

