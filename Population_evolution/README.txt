Every time the code is run with a decommented 
  dyndata_collection()
four files will be produced in this folder, with names 
  "Cpp_tot_pop_<population type>_<LX>_x_<LY>_t<number of trajectories>_batch<identification number>.txt",
where all quantities within <> brackets but the very first one are numbers. <population type> distinguishes the four separate files and takes the values S, B, I and D.
An example is the following:
  "Cpp_tot_pop_S_7_x_11_t24_batch1.txt"
  "Cpp_tot_pop_B_7_x_11_t24_batch1.txt"
  "Cpp_tot_pop_I_7_x_11_t24_batch1.txt"
  "Cpp_tot_pop_D_7_x_11_t24_batch1.txt"
corresponding to a simulation of a lattice of size 7 x 11 and to the collection of 24 stochastic trajectories. "batch" is just a way to avoid overriding the files if
another simulation is run on a lattice of the same size and for an equal number of trajectories. A more detailed explanation is included in the README of the main folder.

Each file is filled with <number of timesteps> lines, each of which has 4 entries in the following order
  <time> <stochastic average of the global population> <stochastic average of the global population squared> <stochastic standard deviation of the global population>.
To better understand, consider the quantity in our paper https://arxiv.org/pdf/2512.22950 defined in Eq. (108). That would be the second entry of each line of the file
  "Cpp_tot_pop_I_7_x_11_t24_batch1.txt"
An analgous consideration holds for the other population types (S,B,D).
Now, the very same quantity can be expressed as the stochastic average of a random variable m_I. Our algorithm produces <number of trajectories> samples of this random variable.
The third entry of each line would be the average, over this list, of m_I^2 and the fourth and last entry is the square root of the third entry minus the square of the second.
