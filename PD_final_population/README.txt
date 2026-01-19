Every time the code is run with the function PDdata_collection() three files are generated in this folder, named
  "PD_Cpp_D_by_traj_<LX>_x_<LY>_t<number of trajectories>_A1_batch<identification number>.txt"
  "PD_Cpp_Dpop_<LX>_x_<LY>_t<number of trajectories>_A1_batch<identification number>.txt"
  "PD_Cpp_SCDpop_<LX>_x_<LY>_t<number of trajectories>_A1_batch<identification number>.txt"
where all quantities within <> brackets are numbers. For instance,
  "PD_Cpp_D_by_traj_5_x_15_t28_A1_batch1.txt"
  "PD_Cpp_Dpop_5_x_15_t28_A1_batch1.txt"
  "PD_Cpp_SCDpop_5_x_15_t28_A1_batch1.txt"
ould be produced by a simulation on a lattice of size 5 x 15 collecting 28 stochastic trajectories. "A1_batch" is just a way to avoid overriding the files if
another simulation is run on a lattice of the same size and for an equal number of trajectories. A more detailed explanation is included in the README of the main folder.

All files include exclusively data taken in the stationary limit t \to \infty, in the sense described in the main text of our paper https://arxiv.org/pdf/2512.22950.
Additionally, they all have the same number of lines, each corresponding to a given parameter point (\gamma_I, \Omega).

In the first file, "PD_Cpp_D_by_traj...", each line contains a number of entries equal to
<number of trajectories> + 2. The first two entries in each line are
  <value of \gamma_I> <value of \Omega>
What follows is a list of values of the global final density of dead sites, summed (not averaged) over all sites, for each trajectories. In other words, for each trajectory 
we have a count of all the final dead sites. For instance, an entry "1" corresponds to a trajectory where the origin died before being able to infect any other site.
The arithmetic average over one such line, excluding of course the first two entries, divided by the system size N, would yield the global average density of dead sites n_D (t \to \infty).

In the second file, "PD_Cpp_Dpop...", each line contains exactly three entries:
  <value of \gamma_I> <value of \Omega> <stochastic average of the stationary global dead density>
The last entry is the stochastic average of the sum over the stationary dead densities of each site. In terms of the quantities described in our paper https://arxiv.org/pdf/2512.22950,
it would correspond to N x n_D (t \to \infty). As explained above, it corresponds to the arithmetic average of the corresponding line in the first file (excluding the first two entries).

In the third file, "PD_Cpp_SCDpop...", each line contains exactly five entries:
  <value of \gamma_I> <value of \Omega> <stochastic average of the stationary global S density> <stochastic average of the stationary global I+B density> <stochastic average of the stationary global D density>
The final entry corresponds to the third entry of the second file, i.e., the system size N times the average density of dead sites. The second entry is the corresponding average global density of infected and
bedridden sites in the statinoary state. It is included as a sanity check. It MUST always be 0, since otherwise it would imply that the system has not reached an absorbing state.
The third, middle entry corresponds to the average global density of susceptible sites. This has been included to perform a consistency check: the sum of this entry with the last entry, in each line, must yield, within 
numerical approximation, the number of sites (the system size N).
