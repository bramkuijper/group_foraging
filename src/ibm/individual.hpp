#ifndef _INDIVIDUAL_HPP_
#define _INDIVIDUAL_HPP_

#include <random>
#include "parameters.hpp"

class Individual
{
    public:

        // diploid loci for cues
        //
        // how much is decision to stay home dependent on 
        // resource level of the brood
        double a_resource[2]{0.0,0.0};
        double b_resource[2]{0.0,0.0};

        // how much is decision to stay home dependent on 
        // time of season
        double a_t[2]{0.0,0.0};
        double b_t[2]{0.0,0.0};

        // how much is decision to stay home dependent on 
        // one's own quality
        double a_qown[2]{0.0,0.0};
        double b_qown[2]{0.0,0.0};

        // how much is decision to stay home dependent on 
        // the other helper's quality
        double a_qother[2]{0.0,0.0};
        double b_qother[2]{0.0,0.0};

        // how much is decision to stay home dependent on 
        // the other helper's quality
        double a_action_other[2]{0.0,0.0};
        double b_action_other[2]{0.0,0.0};

        // an individual's own quality
        bool quality{false};

        bool foraging_previous{false}; 
        bool foraging_current{false}; 

        // default constructor, used when we initialize
        // the simulation at t = 0
        Individual(Parameters const &parms);

        // copy constructor
        Individual(Individual const &other);

        // birth constructor
        Individual(Individual const &mother,
                std::mt19937 &rng_r,
                Parameters const &parms
                );

        void operator=(Individual const &other);

        void mutate(
            std::mt19937 &rng_r,
            Parameters const &params);

        // probability to forage 
        // which depends on cues
        double prob_forage(
            double const resources,
            double const time_cue,
            double const qown,
            double const qother,
            double const action_other);
};


#endif
