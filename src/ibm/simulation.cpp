#include <cassert>
#include <random>
#include <iostream>
#include "group.hpp"
#include "simulation.hpp"


Simulation::Simulation(Parameters const &params) :
    rd{}, // initialize random device (with which to choose a random seed)
    seed{rd()}, // get the random seed 
    rng_r{seed}, // now use the random seed to initialize a random number generator
    par{params}, // copy over the parameters
    data_file{par.file_name}, // initialize the data file to write output to
    uniform{0.0,1.0}, // initialize the uniform distribution 
    metapopulation{par.n_group, 
        Group(par.init_n_per_group, params)} // initialize all males
{}


void Simulation::forage(unsigned const t)
{
    unsigned group_size;
    bool q;

    double p_forage;

    // averages of other individuals
    double average_action_previous_others{};
    double average_quality_others{};

    // aux variable to keep 
    // track of this individual's quality
    bool quality{};

    // keep track of the new resources
    double new_group_resources{};

    double sum_quality_group{};

    unsigned n_foraging{};

    unsigned group_ctr_test{0};

    // go through all groups
    for (auto group_iter{metapopulation.begin()};
            group_iter != metapopulation.end();
            ++group_iter)
    {
        group_size = static_cast<unsigned>(group_iter->members.size());

        // reset stats things
        new_group_resources = 0.0;
        average_action_previous_others = 0.0;
        average_quality_others = 0.0;
        sum_quality_group = 0.0;
        n_foraging = 0;

        // cannot use iterators as we need to avoid
        // including the focal individual in calculating the 
        // averages of non-focal individuals
        for (unsigned int individual_idx{0};
                individual_idx < group_size;
                ++individual_idx)
        {
            for (unsigned int individual2_idx{0};
                    individual2_idx < group_size;
                    ++individual2_idx)
            {
                // if this individual is the same as the focal 
                // ignore the remainder as those pertain to stats
                // from others in the group
                if (individual_idx == individual2_idx)
                {
                    continue;
                }

                average_action_previous_others += 
                    group_iter->members[individual2_idx].action_previous;

                average_quality_others += 
                    group_iter->members[individual2_idx].quality;
            } // end for individual2
   
            if (group_size > 1)
            {
                average_action_previous_others /= group_size - 1;
                average_quality_others /= group_size - 1;
            }
            
            quality = group_iter->members[individual_idx].quality;

            // and calculate prob to forage
            p_forage = group_iter->members[individual_idx].prob_forage(
                group_iter->resources / par.max_resources,
                static_cast<double>(t) / par.max_time_season,
                static_cast<double>(quality),
                average_quality_others,
                average_action_previous_others
                    );

            // ok individual goes out to forage
            if (uniform(rng_r) < p_forage)
            {
                sum_quality_group += quality;
                ++n_foraging;


            } // 
        } // end for individual_idx

        group_iter->resources -= par.ac;
   
        // now update resources for this group
        if (uniform(rng_r) < 
                1.0 - std::exp(-par.epsilon * sum_quality_group))
        {
            group_iter->resources += par.R / group_size;
        } 

        assert(group_size >= n_foraging);

        // calculate predation of the nest
        if (uniform(rng_r) < 
                p_nest_predation[group_size - n_foraging])
        {
            // group dead
            group_iter->resources = -10000; 
        }

        std::cout << t << " " << group_ctr_test 
            << " " << group_size 
            << " " << n_foraging
            << " " << group_iter->resources << std::endl;

        ++group_ctr_test;
    } // end for group
} // end forage()

// calculate survival rate of the nest
// in the presence of n_defenders
void Simulation::init_nest_predation()
{
    for (unsigned n_idx{0}; n_idx < par.n_group; ++n_idx)
    {
        p_nest_predation[n_idx] = par.nest_pred_baseline 
            + par.nest_pred_scale * 
            static_cast<double>(
                    par.n_group - n_idx) / par.n_group;
    }
}

void Simulation::run()
{
    write_data_headers();

    for (generation = 0; 
            generation <= par.max_generation; 
            ++generation)
    {
        // start with 0 resources
        for (auto group_iter{metapopulation.begin()};
                group_iter != metapopulation.end();
                ++group_iter)
        {
            group_iter->resources = 0.0;
        }

        for (time_of_season = 0;
                time_of_season <= par.max_time_season;
                ++time_of_season)
        {
            forage(time_of_season);
        }

        // write statistics to file
        if (generation % par.data_print_interval == 0)
        {
            write_data();
        }

        // replace the current generation
        reproduce();

    } // end for unsigned int generation
   
    write_parameters();
        
} // end run_simulation()



void Simulation::reproduce()
{
    std::vector <double> reproduction_vector{};

    double sum_resources{0.0};
    for (auto group_iter{metapopulation.begin()};
            group_iter != metapopulation.end();
            ++group_iter)
    {
        reproduction_vector.push_back(group_iter->resources);
        sum_resources += group_iter->resources;
    }

    std::discrete_distribution group_reproduction_sampler(
            reproduction_vector.begin(),
            reproduction_vector.end());

    for (unsigned int group_idx{0}; 
            group_idx < par.n_group;
            ++group_idx)
    {
        if (uniform(rng_r) < par.pr_single)
        {


        }
    }
} // end reproduce()

void Simulation::write_data()
{
} // end write_data()

void Simulation::write_data_headers()
{
    data_file << "generation;mean_t;mean_p;var_t;var_p;cov_pt;frac_female_survive;frac_male_survive"
        << std::endl;
} // end write_data_headers()

void Simulation::write_parameters()
{
    data_file << std::endl << std::endl
        << "seed;" << seed << ";" << std::endl
        << "n_group;" << par.n_group << ";" << std::endl
        << "init_n_per_group;" << par.init_n_per_group << ";" << std::endl
        << "max_generation;" << par.max_generation << ";" << std::endl;

} // end write_parameters
