These files were edited with CodeBlocks.

The main file is "Scan_parameters.cpp". The remaining ones are function and object libraries. "Scan_parameters.cpp" contains two primary instructions:
1) dyndata_collection()    which produces dynamical data for trajectories up to a maximum time
2) PDdata_collection()     which produces data in the stationary limit, stopping each trajectory when it reaches an absorption point.

The code is currently set on "dyndata_collection()". To switch, comment the latter and uncomment "PDdata_collection()".

In the same folder where the main file is located four additional folders must be prepared: "Density_Movies", "PD_Final_population", "PD_ID_times" and "Population_evolution".

In order to pass parameters to the main, the user can either follow the instructions and enter they manually as they are requested on screen OR 
prepare two auxiliary txt files: "dyninput.txt" and "PD_input.txt", the first in order to guide dyndata_collection(), the second for PDdata_collection().
Two example files are attached. Two similar files with a "_with_comments" added to the name detail the meaning of the various entries.

NOTE: In the input files just mentioned, if you select "0" as your desired number of trajectories, then the code will scan all the requested parameter points, running one trajectory for each,
after which it will start another scan, adding one more trajectory per point. The code will continue to run until it reads the presence of a "end.txt" file in the main folder, 
which can be e.g. renamed or copied to signal the code to stop. Once the "end.txt" file presence is read, however, the code will not stop immediately and instead it will finish its current
scan to the last parameter point, so that for each point exactly the same number of trajectories is collected. Be aware that this "final stretch", depending on the size of the lattice and
the magnitude of the parameters (especially \gamma_I), may take hours.

SUGGESTION: run first a few "trial" trajectories, starting at small system sizes, in order to get a rough idea of the timescales taken by this algorithm on your system.

In each of the four folders "Density_Movies", "PD_Final_population", "PD_ID_times" and "Population_evolution" you may find a README file describing the files that the algorithm produces in each.
