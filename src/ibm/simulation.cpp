#include <cassert>
#include <random>
#include <iostream>
#include "simulation.hpp"


Simulation::Simulation(Parameters const &params) :
    rd{}, // initialize random device (with which to choose a random seed)
    seed{rd()}, // get the random seed 
    rng_r{seed}, // now use the random seed to initialize a random number generator
    par{params}, // copy over the parameters
    data_file{par.file_name}, // initialize the data file to write output to
    uniform{0.0,1.0}, // initialize the uniform distribution 
    metapopulation{par.n_group, Group(par.init_n_per_group, params)} // initialize all males
{}



void Simulation::forage()
{
    unsigned group_size;

    // normally a Poisson process has mean 1/(1/lambda) = lambda, however a delay
    // Poisson process has mean 1.0 / (h + 1/lambda). See Mangel & Clark 1988, pp. 37, 38
    
    std::array <std::poisson_distribution, 2> number_encounters{
        std::poisson_distribution<>{1.0 / (par.h[0] + 1.0 / par.lambda[0])},
        std::poisson_distribution<>{1.0 / (par.h[0] + 1.0 / par.lambda[0])}
        };

    for (auto group_iter{metapopulation.begin()};
            group_iter != metapopulation.end();
            ++group_iter)
    {
        group_size = static_cast<unsigned>(group_iter->members.size());

        for (auto individual_iter{group_iter->members.begin()},
                    individual_iter != group_iter->members.end(),
                    ++individual_iter)
        {
            q = individual_iter->quality;
        }
    }
}

void Simulation::run()
{
    write_data_headers();

    for (generation = 0; 
            generation <= par.max_generation; 
            ++generation)
    {
        forage();
        learn();




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

// ok individuals group forage or not 
void Simulation::forage()
{
    std::vector <unsigned> in_group{};

    for (auto ind_iter{individuals.begin(0)};
            ind_iter != individuals.end();
            ++ind_iter)
    {

    }
} // end forage()



void Simulation::reproduce()
{
    if (individuals.size() < 1)
    {
        write_data();
        write_parameters();
        exit(1);
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
        << "max_generation;" << par.max_generation << ";" << std::endl
        << "n_males_sampled;" << par.n_males_sampled << ";" << std::endl
        << "mu_t;" << par.mu_t << ";" << std::endl
        << "mu_p;" << par.mu_p << ";" << std::endl
        << "max_mut_p;" << par.max_mut_p << ";" << std::endl
        << "max_mut_t;" << par.max_mut_t << ";" << std::endl
        << "biast;" << par.biast << ";" << std::endl
        << "a;" << par.a << ";" << std::endl
        << "b;" << par.b << ";" << std::endl
        << "c;" << par.c << ";" << std::endl
        << "only_positive;" << par.only_positive << ";" << std::endl
        << "bias_negative;" << par.bias_negative << ";" << std::endl
        << "init_t;" << par.init_t << ";" << std::endl
        << "init_p;" << par.init_p << ";" << std::endl;
}
