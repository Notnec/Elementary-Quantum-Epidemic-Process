Simulations that address the stationary limit can take a really long time, since they generally perform a scanning of the parameter space, trying to collect
several stochastic trajectories for every parameter pair (\gamma_I, \Omega). It is thus quite natural to subdivide the load on different machines, or to make
a few copies of the C++ code and have them run independently. The Python code in the "Stationary" folder is designed to collate all different simulations
run for the same system, as long as they are collected in one or two folders.
