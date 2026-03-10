#ifndef _SIMULATION_HPP_
#define _SIMULATION_HPP_

// class that sets up the simulation

#include <random>
#include <fstream>
#include "parameters.hpp"
#include "individual.hpp"

class Simulation
{
    private:

        // a random device needed to initialize
        // the random number sequence (otherwise always
        // the same random number sequence among replicates)
        std::random_device rd;

        // store the seed used to initialize the random
        // number sequence, so that we can 'replay' the 
        // same simulation for debugging, visualisation purposes
        unsigned int seed;

        // the random number generator itself
        std::mt19937 rng_r;

        // object with all the parameters of this simulation
        Parameters par;

        // the file to write statistics to 
        std::ofstream data_file;

        // uniform distribution
        std::uniform_real_distribution<double> uniform;

        // population of males, females and offspring
        std::vector <Group> metapopulation{};

        // current generation
        unsigned int generation{0};

        void reproduce();
        void write_data();
        void write_parameters();
        void write_data_headers();

    public:
        // initialize the simulation
        Simulation(Parameters const &parms);

        // run the thing
        void run();
};


#endif
