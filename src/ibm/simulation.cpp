#include <cassert>
#include <iterator>
#include <random>
#include <iostream>
#include <sstream>
#include "group.hpp"
#include "simulation.hpp"


Simulation::Simulation(Parameters const &params) :
    rd{}, // initialize random device (with which to choose a random seed)
    seed{rd()}, // get the random seed 
    rng_r{seed}, // now use the random seed to initialize a random number generator
    par{params}, // copy over the parameters
    data_file{par.file_name}, // initialize the data file to write output to
    data_file_dynamics{par.file_name + "_dynamics"}, // initialize the data file to write output to
    uniform{0.0,1.0}, // initialize the uniform distribution 
    metapopulation(par.n_group, 
        Group(par.init_n_per_group, params)), // initialize all individuals
    p_nest_predation(par.init_n_per_group + 1, 0.0) // 0, 1, ..., n individuals that can stay home
{
    init_nest_predation();
}

void Simulation::reset_across_season_stats()
{
    // reset all the stats
    mean_nests_predated_per_timestep = 0.0;
    var_nests_predated_per_timestep = 0.0;
    mean_foraging_per_group = 0.0;

    total_nests_predated_season = 0;
}

void Simulation::calculate_across_season_stats()
{
    // calculate total time steps across all groups
    // as this is what averages will be based on
    unsigned total_group_time{par.max_time_season * par.n_group};

    mean_nests_predated_per_timestep /= total_group_time;
    var_nests_predated_per_timestep = 
        var_nests_predated_per_timestep/ total_group_time - 
        mean_nests_predated_per_timestep * mean_nests_predated_per_timestep;

    mean_foraging_per_group /= total_group_time;

} // end calculate_across_season_stats


// forage (or not) for resources
void Simulation::forage(
        unsigned const t,
        bool const write_data_foraging)
{
    unsigned group_size;

    double p_forage;

    // averages of other individuals
    double average_action_previous_others{};
    double average_quality_others{};

    // aux variable to keep 
    // track of this individual's quality
    bool quality{};

    double sum_quality_group{};

    unsigned n_foraging{};
    
    std::vector <std::string> output_vector(
            par.init_n_per_group, "");

    // go through all groups
    for (auto group_iter{metapopulation.begin()};
            group_iter != metapopulation.end();
            ++group_iter)
    {

        // ok group is dead
        if (group_iter->group_is_dead)
        {
            continue;
        }

        group_size = static_cast<unsigned>(group_iter->members.size());


        // reset stats  for each group
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
            // look at actions of other individuals
            // however, we can only do this when looking
            // at the previous time step. If we would do this
            // at the current time step then who would
            // know the information first and how would 
            // individuals respond to this??
            // 
            // Given that individuals thus consider the
            // action at the previous time step
            // this only works when t > 0
            if (t > 0) 
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
                        group_iter->members[individual2_idx].foraging_previous;

                    average_quality_others += 
                        group_iter->members[individual2_idx].quality;
                } // end for individual2
       
                if (group_size > 1)
                {
                    average_action_previous_others /= group_size - 1;
                    average_quality_others /= group_size - 1;
                }
            }
            
            quality = group_iter->members[individual_idx].quality;

            // and calculate prob to forage
            p_forage = group_iter->members[individual_idx].prob_forage(
                group_iter->resources / par.max_resources,
                static_cast<double>(t) / par.max_time_season,
                static_cast<double>(quality),
                average_quality_others,
                average_action_previous_others);

            // ok individual goes out to forage
            if (uniform(rng_r) < p_forage)
            {
                sum_quality_group += par.quality_weighting[quality];
                ++n_foraging;
                ++mean_foraging_per_group;

                group_iter->members[individual_idx].foraging_current = true;

//                // now build in predation while foraging
//                if (
//
//
            } 
            else
            {
                group_iter->members[individual_idx].foraging_current = false;
            }

            if (write_data_foraging)
            {
                std::stringstream output{};

                // position in group
                output 
                    << generation << ";"
                    << t << ";"
                    << std::distance(
                        std::begin(metapopulation),
                        group_iter) << ";"
                    << individual_idx << ";"
                    << group_size << ";"
                    << group_iter->resources / par.max_resources << ";"
                    << quality << ";"
                    << average_quality_others << ";"
                    << average_action_previous_others << ";"
                    << p_forage << ";"
                    << group_iter->members[individual_idx].foraging_current << ";";

                output_vector[individual_idx] = output.str();
            }
        } // end for individual_idx

        // spend resources on growth
        group_iter->resources -= par.ac;

        // now update resources for this group
        if (uniform(rng_r) < 
                1.0 - std::exp(-par.epsilon * sum_quality_group))
        {
            // without group augmentation we have
            //group_iter->resources += par.R / group_size;
            //
            // however with group augmentation we have
            group_iter->resources += par.R; 
        } 

        // restrict resources to max
        if (group_iter->resources > par.max_resources)
        {
            group_iter->resources = par.max_resources;
        }

        assert(group_size >= n_foraging);
            
//        std::cout << "group size: " << group_size 
//                << " n forage: " << n_foraging 
//                << " group size min forage: " << (group_size - n_foraging) 
//                << " nest pred size: " << p_nest_predation.size() << " " << std::endl;

        // calculate predation of the nest
        if (uniform(rng_r) < 
                p_nest_predation[n_foraging])
        {
            // group dead
            group_iter->resources = 0;

            assert(!group_iter->group_is_dead);

            group_iter->group_is_dead = true;
            ++total_nests_predated_season;
        }

        // now that decisions have been made let's update action previous
        for (unsigned int individual_idx{0};
                individual_idx < group_size;
                ++individual_idx)
        {
            if (write_data_foraging)
            {
                data_file_dynamics << output_vector[individual_idx]
                    << group_iter->group_is_dead << ";"
                    << p_nest_predation[n_foraging] << ";"
                    << sum_quality_group << ";"
                    << group_iter->members[individual_idx].foraging_previous << ";"
                    << group_iter->members[individual_idx].foraging_current << ";"
                    << std::endl;
            }

            group_iter->members[individual_idx].foraging_previous = 
                group_iter->members[individual_idx].foraging_current;
        }
    } // end for group
} // end forage()


// calculate survival rate of the nest
// in the presence of n_defenders
void Simulation::init_nest_predation()
{
    for (unsigned n_foraging_idx{0}; n_foraging_idx <= par.init_n_per_group; ++n_foraging_idx)
    {
        p_nest_predation[n_foraging_idx] = par.nest_pred_baseline 
            + par.nest_pred_scale * 
                static_cast<double>(n_foraging_idx) / par.init_n_per_group;
    }
}

void Simulation::run()
{
    write_data_headers();

    for (generation = 0; 
            generation <= par.max_generation; 
            ++generation)
    {
        // every generation start with an initial amount 
        // of resources
        // and also all groups are alive
        for (auto group_iter{metapopulation.begin()};
                group_iter != metapopulation.end();
                ++group_iter)
        {
            group_iter->resources = par.init_resources;
            group_iter->group_is_dead = false;
        }

        // reset all the seasonal stats
        reset_across_season_stats();

        for (time_of_season = 0;
                time_of_season <= par.max_time_season;
                ++time_of_season)
        {
            forage(time_of_season, generation == par.max_generation);
        }

        calculate_across_season_stats();
        
        // replace the current generation
        reproduce();

        // write statistics to file
        if (generation % par.data_print_interval == 0)
        {
            write_data();
        }

    } // end for unsigned int generation
   
    write_parameters();
        
} // end run_simulation()


// reproduction of each colony
void Simulation::reproduce()
{
    std::vector <double> reproduction_vector{};

    for (unsigned int group_idx{0};
            group_idx < metapopulation.size();
            ++group_idx)
    {
        reproduction_vector.push_back(std::exp(metapopulation[group_idx].resources));
    } // end for group_idx

    std::discrete_distribution <unsigned> group_reproduction_sampler(
            reproduction_vector.begin(),
            reproduction_vector.end());

    unsigned int group_origin_idx;
    unsigned int parent_idx;

    std::uniform_int_distribution <unsigned> 
        parent_sampler{0, par.init_n_per_group - 1};

    // TODO: allow for linear increases in offspring production
    // see Field et al 2000 Nature --
    //
    // reset after every t time steps
    
    unsigned target_group_size{0};

    for (auto group_iter{metapopulation.begin()};
            group_iter != metapopulation.end();
            ++group_iter)
    {
        target_group_size = uniform(rng_r) < par.pr_single ?
            1 : par.init_n_per_group;
        
        // remove any pre-existing juveniles in this group
        // which are basically left over from recruitment from
        // previous generations
        group_iter->juveniles.clear();

        for (unsigned newborn_idx{0};
                newborn_idx < target_group_size;
                ++newborn_idx)
        {
            group_origin_idx = group_reproduction_sampler(rng_r);

            // always use first parent of a local group...
            // unless group size > 1
            parent_idx = 0;

            if (metapopulation[group_origin_idx].members.size() ==
                    par.init_n_per_group)
            {
                parent_idx = parent_sampler(rng_r);
            }

            group_iter->juveniles.push_back(
                    Individual(
                        metapopulation[group_origin_idx].members[parent_idx],
                        rng_r,
                        par));
        } // end for newborn idx
    }

    // now replace members with juveniles
    for (auto group_iter{metapopulation.begin()};
            group_iter != metapopulation.end();
            ++group_iter)
    {
        // clear existing members
        group_iter->members.clear();

        // replace members with juveniles
        group_iter->members = group_iter->juveniles;
    }
} // end reproduce()

void Simulation::write_data()
{
    // variables to calculate 
    // means and variances
    double mean_a_resource{0.0};
    double ss_a_resource{0.0};
    
    double mean_b_resource{0.0};
    double ss_b_resource{0.0};
    
    double mean_a_t{0.0};
    double ss_a_t{0.0};
    
    double mean_b_t{0.0};
    double ss_b_t{0.0};

    double mean_a_qown{0.0};
    double ss_a_qown{0.0};
    
    double mean_b_qown{0.0};
    double ss_b_qown{0.0};
    
    double mean_a_qother{0.0};
    double ss_a_qother{0.0};
    
    double mean_b_qother{0.0};
    double ss_b_qother{0.0};
    
    double mean_a_action_other{0.0};
    double ss_a_action_other{0.0};
    
    double mean_b_action_other{0.0};
    double ss_b_action_other{0.0};

    // aux variable 
    double x;

    unsigned n{0};

    double mean_resources{0.0};
    double ss_resources{0.0};

    // aux variable to make n_group count in terms of double
    double d_n_metapops{static_cast<double>(metapopulation.size())};

    for (auto group_iter{metapopulation.begin()};
            group_iter != metapopulation.end();
            ++group_iter)
    {
        x = group_iter->resources;
        mean_resources += x;
        ss_resources += x * x;

        for (auto individual_iter{group_iter->members.begin()};
                individual_iter != group_iter->members.end();
                ++individual_iter)
        {
            x = 0.5 * (individual_iter->a_resource[0] + 
                individual_iter->a_resource[1]);

            mean_a_resource += x;
            ss_a_resource += x * x;

            x = 0.5 * (individual_iter->b_resource[0] + 
                individual_iter->b_resource[1]);

            mean_b_resource += x;             
            ss_b_resource += x * x;

            x = 0.5 * (individual_iter->a_t[0] + 
                individual_iter->a_t[1]);

            mean_a_t += x;
            ss_a_t += x * x;

            x = 0.5 * (individual_iter->b_t[0] + 
                individual_iter->b_t[1]);

            mean_b_t += x;
            ss_b_t += x * x;

            x = 0.5 * (individual_iter->a_qown[0] + 
                individual_iter->a_qown[1]);

            mean_a_qown += x;            
            ss_a_qown += x * x;

            x = 0.5 * (individual_iter->b_qown[0] + 
                individual_iter->b_qown[1]);

            mean_b_qown += x;            
            ss_b_qown += x *x;

            x = 0.5 * (individual_iter->a_qother[0] + 
                individual_iter->a_qother[1]);

            mean_a_qother += x;
            ss_a_qother += x * x;

            x = 0.5 * (individual_iter->b_qother[0] + 
                individual_iter->b_qother[1]);

            mean_b_qother += x;             
            ss_b_qother += x * x;

            x = 0.5 * (individual_iter->a_action_other[0] + 
                individual_iter->a_action_other[1]);

            mean_a_action_other += x;
            ss_a_action_other += x * x;

            x = 0.5 * (individual_iter->b_action_other[0] + 
                individual_iter->b_action_other[1]);

            mean_b_action_other += x;
            ss_b_action_other += x * x;

            ++n;
        } // end for individual_iter
    } // end for group_iter

    // variables to calculate 
    // means and variances
    mean_a_resource /= n;

    double var_a_resource{ss_a_resource / n - mean_a_resource * mean_a_resource};

    mean_b_resource /= n;
    double var_b_resource{ss_b_resource / n - mean_b_resource * mean_b_resource};
    
    mean_a_t /= n;
    double var_a_t{ss_a_t / n - mean_a_t * mean_a_t};
    
    mean_b_t /= n;
    double var_b_t{ss_b_t / n - mean_b_t * mean_b_t};

    mean_a_qown /= n;
    double var_a_qown{ss_a_qown / n - mean_a_qown * mean_a_qown};
    
    mean_b_qown /= n;
    double var_b_qown{ss_b_qown / n - mean_b_qown * mean_b_qown};
    
    mean_a_qother /= n;
    double var_a_qother{ss_a_qother / n - mean_a_qother * mean_a_qother};
    
    mean_b_qother /= n;
    double var_b_qother{ss_b_qother / n - mean_b_qother * mean_b_qother};
    
    mean_a_action_other /= n;
    double var_a_action_other{ss_a_action_other / n -
        mean_a_action_other * mean_a_action_other};
    
    mean_b_action_other /= n;
    double var_b_action_other{ss_b_action_other / n -
        mean_b_action_other * mean_b_action_other};

    double mean_n_per_group = static_cast<double>(n) / d_n_metapops;

    mean_resources /= d_n_metapops;

    double var_resources = ss_resources / d_n_metapops - 
        mean_resources * mean_resources;

    data_file << generation << ";" 
        << time_of_season << ";"
        << mean_a_resource << ";"
        << var_a_resource << ";"
        << mean_b_resource << ";"
        << var_b_resource << ";"
        << mean_a_t << ";"
        << var_a_t << ";"
        << mean_b_t << ";"
        << var_b_t << ";"
        << mean_a_qown << ";"
        << var_a_qown << ";"
        << mean_b_qown << ";"
        << var_b_qown << ";" 
        << mean_a_qother << ";"
        << var_a_qother << ";"
        << mean_b_qother << ";"
        << var_b_qother << ";" 
        << mean_a_action_other << ";"
        << var_a_action_other << ";"
        << mean_b_action_other << ";"
        << var_b_action_other << ";" 
        << mean_n_per_group << ";" 
        << mean_resources  << ";" 
        << var_resources  << ";" 
        << mean_foraging_per_group << ";"
        << total_nests_predated_season << ";"
        << std::endl;
} // end write_data()

void Simulation::write_data_headers()
{
    data_file 
        << "generation" << ";" 
        << "time_of_season" << ";"
        << "mean_a_resource" << ";"
        << "var_a_resource" << ";"
        << "mean_b_resource" << ";"
        << "var_b_resource" << ";"
        << "mean_a_t" << ";"
        << "var_a_t" << ";"
        << "mean_b_t" << ";"
        << "var_b_t" << ";"
        << "mean_a_qown" << ";"
        << "var_a_qown" << ";"
        << "mean_b_qown" << ";"
        << "var_b_qown" << ";" 
        << "mean_a_qother" << ";"
        << "var_a_qother" << ";"
        << "mean_b_qother" << ";"
        << "var_b_qother" << ";" 
        << "mean_a_action_other" << ";"
        << "var_a_action_other" << ";"
        << "mean_b_action_other" << ";"
        << "var_b_action_other" << ";" 
        << "mean_n_per_group" << ";" 
        << "mean_resources" << ";" 
        << "var_resources" << ";" 
        << "mean_foraging_per_group" << ";"
        << "total_nests_predated_season" << ";"
        << std::endl;

    data_file_dynamics 
        << "generation" << ";"
        << "t" << ";"
        << "group_idx" << ";"
        << "individual_idx" << ";"
        << "group_size" << ";"
        << "group_resources" << ";"
        << "quality" << ";"
        << "avg_quality_others" << ";"
        << "avg_action_previous_others" << ";"
        << "p_forage" << ";"
        << "foraging_current" << ";"
        << "group_dead" << ";"
        << "p_nest_predatin" << ";"
        << "sum_quality_group" << ";"
        << "foraging_previous" << ";"
        << "foraging_current" << ";" 
        << std::endl;
        

} // end write_data_headers()

void Simulation::write_parameters()
{
    data_file << std::endl << std::endl
        << "seed;" << seed << ";" << std::endl
        << "n_group;" << par.n_group << ";" << std::endl
        << "init_n_per_group;" << par.init_n_per_group << ";" << std::endl
        << "pr_single;" << par.pr_single << ";" << std::endl
        << "max_generation;" << par.max_generation << ";" << std::endl
        << "max_time_season;" << par.max_time_season << ";" << std::endl
        << "data_print_interval;" << par.data_print_interval << ";" << std::endl
        << "max_resources;" << par.max_resources << ";" << std::endl
        << "epsilon;" << par.epsilon << ";" << std::endl
        << "init_resources;" << par.init_resources << ";" << std::endl
        << "nest_pred_scale;" << par.nest_pred_scale << ";" << std::endl
        << "nest_pred_baseline;" << par.nest_pred_baseline << ";" << std::endl
        << "ac;" << par.ac << ";" << std::endl
        << "R;" << par.R << ";" << std::endl
        << "mu_a_resource;" << par.mu_a_resource << ";" << std::endl
        << "mu_b_resource;" << par.mu_b_resource << ";" << std::endl
        << "a_resource_init;" << par.a_resource_init<< ";" << std::endl
        << "b_resource_init;" << par.a_resource_init << ";" << std::endl
        << "mu_a_t;" << par.mu_a_t << ";" << std::endl
        << "mu_b_t;" << par.mu_b_t << ";" << std::endl
        << "a_t_init;" << par.a_t_init<< ";" << std::endl
        << "b_t_init;" << par.a_t_init << ";" << std::endl
        << "mu_a_qown;" << par.mu_a_qown << ";" << std::endl
        << "mu_b_qown;" << par.mu_b_qown << ";" << std::endl
        << "a_qown_init;" << par.a_qown_init<< ";" << std::endl
        << "b_qown_init;" << par.a_qown_init << ";" << std::endl
        << "mu_a_qother;" << par.mu_a_qother << ";" << std::endl
        << "mu_b_qother;" << par.mu_b_qother << ";" << std::endl
        << "a_qother_init;" << par.a_qother_init<< ";" << std::endl
        << "b_qother_init;" << par.a_qother_init << ";" << std::endl
        << "mu_a_action_other;" << par.mu_a_action_other << ";" << std::endl
        << "mu_b_action_other;" << par.mu_b_action_other << ";" << std::endl
        << "a_action_other_init;" << par.a_action_other_init<< ";" << std::endl
        << "b_action_other_init;" << par.a_action_other_init << ";" << std::endl
        << "logistic_min_bound;" << par.logistic_min_bound << ";" << std::endl
        << "logistic_max_bound;" << par.logistic_max_bound << ";" << std::endl
        << "sdmu;" << par.sdmu << ";" << std::endl
        << "p_high_quality;" << par.p_high_quality << ";" << std::endl
        << "quality_weighting_low;" << par.quality_weighting[0] << ";" << std::endl
        << "quality_weighting_hi;" << par.quality_weighting[1] << ";" << std::endl
        << "init_p_group;" << par.init_p_group << ";" << std::endl
    ;
} // end write_parameters
