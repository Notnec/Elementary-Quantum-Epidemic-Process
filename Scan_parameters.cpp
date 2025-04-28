#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include <random>
#include<chrono>


using namespace std;

struct col{
    string red = "\033[91m";
    string yellow = "\033[93m";
    string blue = "\033[94m";
    string green = "\033[92m";
    string white = "\033[37m";
};


// INITIALISATION OF THE RANDOM NUMBER GENERATOR AND DISTRIBUTION

unsigned seed = chrono::system_clock::now().time_since_epoch().count();     // PROVIDES A SEED FOR THE GENERATION OF PSEUDO-RANDOM SEQUENCES
default_random_engine rand_gen (seed);                                        // GENERATOR OF PSEUDO RANDOM SEQUENCES WITH PROVIDED SEED
uniform_real_distribution<double> rand_real(0.0, 1.0);                     // CONSTRUCTS A UNIFROM REAL DISTRIBUTION


#include "QEP_data_collection.h"


///////// MAIN STARTS HERE ////////////
///////////////////////////////////////
///////////////////////////////////////

int main(){

    dyndata_collection();


//    PDdata_collection();

    return 0;
}
