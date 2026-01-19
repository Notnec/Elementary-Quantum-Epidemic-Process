Every time the code is run with a decommented 
  dyndata_collection()
four files will be produced in this folder, with names 
  "Cpp_Lattice_map_<population type>_<LX>_x_<LY>_t<number of trajectories>_batch<identification number>.txt",
where all quantities within <> brackets but the very first one are numbers. <population type> distinguishes the four separate files and takes the values S, B, I and D.
An example is the following:
  "Cpp_Lattice_map_S_7_x_11_t24_batch1.txt"
  "Cpp_Lattice_map_B_7_x_11_t24_batch1.txt"
  "Cpp_Lattice_map_I_7_x_11_t24_batch1.txt"
  "Cpp_Lattice_map_D_7_x_11_t24_batch1.txt"
corresponding to a simulation of a lattice of size 7 x 11 and to the collection of 24 stochastic trajectories. "batch" is just a way to avoid overriding the files if
another simulation is run on a lattice of the same size and for an equal number of trajectories. A more detailed explanation is included in the README of the main folder.

Each file contains the sum over all trajectories of the single-site-resolved quantum (not stochastic) averages of the populations \sigma_k^{\mu\mu} 
for \mu = S (susceptible), B (bedridden), I (infected) or D (dead).
The file content is a succession of numerical matrices with LX rows and LY columns separated by empty lines. The total number of matrices in each file is equal to
  <number of time steps at which data has been collected>, including the initial condition at t = 0 which appears at the very top of the file.
In other words, dividing (each entry of) any matrix by the number of trajectories one obtains a snapshot of a given SIBD population over the entire lattice at some point in time.
Here are the first two matrices in "Cpp_Lattice_map_I_7_x_11_t24_batch1.txt", taken from an actual file:

0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 24 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0

0 0 0 0 0 0 0.92646 0 0 0 0
0 0 0 0 0 1.89599 1.9262 0 0 0 0
0 0 0 0 0.920721 0.945022 2.92241 0 0 0 0
0 0 0 0 4.78972 16.2995 2.68174 0 0 0 0
0 0 0 0 1.9166 0.990137 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0

The one on top is the initial condition, where we see that only the origin has some infected component, equal to the number of trajectories since the initial
condition is the same (one central seed) for all.
The second matrix shows the lattice after one time step (t = 0 + 0.2 = 0.2). Dividing each entry by 24 (the number of trajectories) one would get a rough estimate for the
infected (I) population averages over the entire lattice. We see, for instance, that at the origin we have now about 16, leading to an average of about 0.67.
One third of the initial infection has been lost, on average.

If we were to sum the four matrices from the four different files at any given (but fixed and equal for all) time, we would get a matrix where all the entries correspond to the number
ot trajectories, 24 in the example above, as the sum of all \sigma_k^{\mu\mu} yields the identity.
