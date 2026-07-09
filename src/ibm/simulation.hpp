#ifndef _SIMULATION_HPP_
#define _SIMULATION_HPP_

// class that sets up the simulation

#include <random>
#include <fstream>
#include "parameters.hpp"
#include "group.hpp"

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

        // vector containing the nest predation probabilities
        // for different numbers of defenders
        std::vector <double> p_nest_predation{};

        // current generation
        unsigned int generation{0};
        unsigned int time_of_season{0};

        // some statistics
        //
        // average number of nests predated per time step
        // across a season
        double mean_nests_predated_per_timestep{0.0};

        // sum squares in the number of nests predated per time step
        // across a season
        double var_nests_predated_per_timestep{0.0};

        // total count of nests predated over a season
        unsigned total_nests_predated_season{0};
        
        // mean number of foragers per group
        double mean_foraging_per_group{0.0};
        
        void reproduce();
        void write_data();
        void write_parameters();
        void write_data_headers();

        void forage(unsigned const t);
        void learn();

        void init_nest_predation();
        void reset_across_season_stats();
        void calculate_across_season_stats();

    public:
        // initialize the simulation
        Simulation(Parameters const &parms);

        // run the thing
        void run();
};


#endif
