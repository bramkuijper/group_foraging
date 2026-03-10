#ifndef _PARAMETERS_HPP_
#define _PARAMETERS_HPP_

// add stop to forage one time step, 
// as long as there is a risk reward trade-off
//
// how to connect this to the empirical side: relationship 
// between variance and mean food returns
//
// stuff on larvae.
//
// When to stop foraging is really important
//
// 1. time steps to take stock when to stop foraging
// 
// 2. reset time that the number of food items are set to 0 again
// which is a night.
//
// something that could get rid of this mean variance relationship, 
// so that variance might be bigger in larger groups
//
// handling time really needs to reflect time rather than rates, because individuals
// that are crappy arrive later, this can influence the dynamic
//
//
//

#include <string>

class Parameters
{
    public:
        // population size, which should be even number
        unsigned int n_group{500};
        unsigned int init_n_per_group{1};

        // duration of the simulation
        unsigned int max_generation{20000};

        // interval of number of generations 
        // at which data is printed
        unsigned int data_print_interval{10};

        // per-generation probabability that p allele mutates
        double mu_p_group{0.01};

        // standard deviation of mutational effect size
        double sdmu{0.02};

        // efficacy of sexual selection (see Pomiankowski et al 1991 Evolution)
        double a{1.0};

        // survival cost of female choice (see Pomiankowski et al 1991 Evolution)
        double b{0.0025};

        // handling time of lq and hq individuals
        double h[2]{0.5,2.0};

        // encounter rate of food
        double lambda[2]{1.5, 1.5};

        // survival cost of male ornamentation
        double c{0.5};

        // quality variation
        double p_high_quality{0.5};

        // initial value of joining a group
        double init_p_group{0.25};

        std::string file_name{"sim_group_joining.txt"};
};

#endif
