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
        bool group_is_dead{false};

        // initiation constructor
        Group(
                unsigned const nmembers, 
                Parameters const &params
                );

        // 
        Group(Group const &other);

        void operator=(Group const &other);
};

#endif
