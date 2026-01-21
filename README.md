The C++ files were edited with CodeBlocks, the Python files with PyCharm.

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
In particular, 


(1) dyndata_collection() is going to produce files in the main folder, in "Density_Movies" and in "Population_evolution";
  In the main folder, one file will be produced every time the code is run, called
    "Cpp_parameters_<LX>_x_<LY>_t<num of trajectories>_batch<identifier number>.txt",
  where all part within < > brackets should be interpreted as integer numbers like in the example below:
    "Cpp_parameters_101_x_101_t53_batch2.txt".
  which corresponds to a simulation for a lattice of 101 x 101 sites and a collection of 53 stochastic trajectories per parameter point visited. The batch number is just an identifier which prevents
  subsequent simulations at the same size and number of trajectories to override one another. In this example, a simulation on a lattice of the same size which collected 53 trajectories had already 
  been run previously, producing a file
    "Cpp_parameters_101_x_101_t53_batch1.txt".
  This was therefore the second time that the code ended having produced 53 trajectories (per parameter point) on a square 101 x 101 in size.
  The third time will produce a file
    "Cpp_parameters_101_x_101_t53_batch3.txt"
  and so on.

  The file itself includes the following lines:
    LX: <LX>
    LY: <LY>
    gd: <value of \gamma_D>
    gi: <value of \gamma_I>
    Omega: <value of \Omega>
    Max time: <End time t_{end} at which each trajectory is terminated>
    dt: <time step at which data has been collected>
    num of trajectories: <actual number of trajectories run per parameter point>

As already mentioned, the other files are described in the READMEs inside the "Density_Movies" and "Population_evolution" folders.


(2) PDdata_collection() is going to produce files in the main folder, in "PD_Final_population" and in "PD_ID_times";
  In the main folder, two files will be produced every time the code is run, called
    "PD_Cpp_parameters_<LX>_x_<LY>_t<num of trajectories>_A1_batch<identifier number>.txt",
  and
    "PD_Cpp_PDpoints_<LX>_x_<LY>_t<num of trajectories>_A1_batch<identifier number>.txt"
  where all part within < > brackets should be interpreted as integer numbers like in the example below:
    "PD_Cpp_parameters_13_x_13_t77_A1_batch2.txt"
  and
    "PD_Cpp_PDpoints_13_x_13_t77_A1_batch2.txt".
  which corresponds to a simulation for a lattice of 13 x 13 sites and a collection of 77 stochastic trajectories per parameter point visited. The batch number is just an identifier which prevents
  subsequent simulations at the same size and number of trajectories to override one another. In this example, a simulation on a lattice of the same size which collected 77 trajectories had already 
  been run previously, producing a file
    "PD_Cpp_parameters_13_x_13_t77_A1_batch1.txt".
  This was therefore the second time that the code ended having produced 77 trajectories (per parameter point) on a square 13 x 13 in size.
  The third time will produce a file
    "PD_Cpp_parameters_13_x_13_t77_A1_batch3.txt"
  and so on.
  The only reason why there is an "A1" in front of "batch" is because we needed to distinguish also between files run on different computers. There is no need to worry about it
  if this code is not made into different copies to be run in parallel.

  The file "PD_Cpp_parameters..." includes the following lines:
    LX: <LX>
    LY: <LY>
    gi first bound: <value of the lower bound for \gamma_I>
    gi second bound: <value of the higher bound for \gamma_I>
    num of gis: <number of equally spaced \gamma_I points considered bewteen the two bounds above>
    Omega first bound: <value of the lower bound for \Omega>
    Omega second bound: <value of the higher bound for \Omega>
    num of Omegas: <number of equally spaced \Omega points considered bewteen the two bounds above>
    num of trajectories: <actual number of trajectories collected>

  The file "PD_Cpp_PDpoints..." is a list of all parameter points simulated; each line has the following format:
    [g<index of \gamma_I value>/<total number of \gamma_I values>, o<index of \Omega value>/<total number of \Omega values>] <corresponding \gamma_I value> <corresponding \Omega value>
  For instance, a hypothetical line
    [g2/9, o3/11] 1.1 2.01
  would signify that there aree 9 different \gamma_I values and 11 different \Omega values being simulated. The second \gamma_I value and third \Omega value (in increasing order) are
  1.1 and 2.01, respectively.
  There will be one and only one line per parameter point, meaning that the file will include <total number of \gamma_I values> x <total number of \Omega values> lines; in the example above,
  that would be 9 x 11 = 99 lines.

  As already mentioned, the other files are described in the READMEs inside the "PD_Final_population" and "PD_ID_times" folders.
