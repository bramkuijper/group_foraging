#ifndef _INDIVIDUAL_HPP_
#define _INDIVIDUAL_HPP_

#include <random>
#include "parameters.hpp"

class Individual
{
    public:

        // diploid loci for probability 
        double p_group[2]{0.0,0.0};

        bool quality{false};

        // default constructor, used when we initialize
        // the simulation at t = 0
        Individual(Parameters const &parms);

        // copy constructor
        Individual(Individual const &other);

        // birth constructor
        Individual(Individual const &mother,
                Individual const &father,
                std::mt19937 &rng_r,
                Parameters const &parms
                );

        void operator=(Individual const &other);

};


#endif
