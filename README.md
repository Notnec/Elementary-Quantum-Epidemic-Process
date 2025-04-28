The main file is "Scan_parameters.cpp". It contains two primary instructions:
1) dyndata_collection()    which produces dynamical data for trajectories up to a maximum time
2) PDdata_collection()     which produces data in the stationary limit, stopping each trajectory when it reaches an absorbing state.

The code is currently set on "dyndata_collection()". To switch, comment the latter and uncomment "PDdata_collection()".

In the same folder where the main file is located four additional folders must be prepared: "Density_Movies", "PD_final_population", "PD_ID_times" and "Population_evolution".

In order to pass parameters to the programme, two txt files are necessary: "dyninput.txt" and "PD_input.txt". Two example files are attached together with the code.
