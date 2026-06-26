#ifndef _GROUP_HPP_
#define _GROUP_HPP_

#include <vector>
#include "individual.hpp"
#include "parameters.hpp"

class Group
{
    public:
        std::vector<Individual> members;
        std::vector<Individual> juveniles;

        double resources{0.0};

        // initiation constructor
        Group(
                int const nmembers, 
                Parameters const &params
                );

        // 
        Group(Group const &other);

        void operator=(Group const &other);
};

#endif
