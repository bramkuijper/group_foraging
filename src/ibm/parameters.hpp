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
        unsigned int n_group{2000};
        unsigned int init_n_per_group{2};

        // probability of having a group with a single individual
        double pr_single{0.5};

        // duration of the simulation
        unsigned int max_generation{10000};
        
        unsigned int max_time_season{50};

        // interval of number of generations 
        // at which data is printed
        unsigned int data_print_interval{10};

        // maximum resources
        double max_resources{30.0};

        // slope of the foraging success function
        double epsilon{1.0};

        double init_resources{10.0};

        // range in the [0,1] probability space that
        // affects nest predation: if nest_pred_scale = 1
        // then individuals with no helpers on the nest
        // undergo full predation
        double nest_pred_scale{0.8};


        // baseline level of predation of a nest, reflecting
        // predation risk when all helpers 
        // are present on the nest
        double nest_pred_baseline{0.1};

        // cost of offspring development in terms of resources
        double ac{1.0};

        // resources gained per time step when foraging
        double R{4.0};
        
        // mutation rates and initial values

        // per-generation probabability that alleles mutate
        // for resource reaction norm
        double mu_a_resource{0.0};
        double mu_b_resource{0.0};
        
        // initial values for resource reaction norm
        double a_resource_init{0.0};
        double b_resource_init{0.0};

        // per-generation probabability that alleles mutate
        // for time of season reaction norm
        double mu_a_t{0.0};
        double mu_b_t{0.0};

        // initial values for time of season reaction norm
        double a_t_init{0.0};
        double b_t_init{0.0};

        double mu_a_qown{0.0};
        double mu_b_qown{0.0};

        double a_qown_init{0.0};
        double b_qown_init{0.0};

        double mu_a_qother{0.0};
        double mu_b_qother{0.0};

        double a_qother_init{0.0};
        double b_qother_init{0.0};
        
        double mu_a_action_other{0.0};
        double mu_b_action_other{0.0};

        double a_action_other_init{0.0};
        double b_action_other_init{0.0};

        // boundaries for the logistic
        double logistic_min_bound{-10.0};
        double logistic_max_bound{10.0};

        // standard deviation of mutational effect size
        double sdmu{0.02};

        // quality variation
        double p_high_quality{0.5};

        // initial value of joining a group
        double init_p_group{0.25};

        std::string file_name{"sim_group_forage.txt"};
};

#endif
