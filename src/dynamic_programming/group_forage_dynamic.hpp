#ifndef _GROUP_FORAGE_DYN_HPP_
#define _GROUP_FORAGE_DYN_HPP_

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "parameters.hpp"

class GroupForageDP {

    Parameters par{};

    std::vector < std::vector < std::vector < std::vector < double > > > > Wnext{};    
    std::vector < std::vector < std::vector < std::vector < double > > > > W{};    
    std::vector < std::vector < double> > Wopt{};    
    std::vector < std::vector < double> > f{};    // first index t next index x

    std::ofstream data_file{};


    double food_encounter_prob(
            double const y1, 
            double const y2);

    // probability that nest gets predated
    double ph(int const n);

    void write_data();
    void write_parameters();

    public:
        GroupForageDP(Parameters const &params); // the constructor
                                                 //
        void run();

        void final_fitness();
        void optimal_decision();
};

#endif
