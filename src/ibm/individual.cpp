#include <random>
#include "parameters.hpp"
#include "individual.hpp"

Individual::Individual(Parameters const &parms) :
    p_group{parms.init_p_group,parms.init_p_group},
{}

Individual::Individual(Individual const &other) :
    p_group{other.p_group[0],other.p_group[1]},
    quality{other.quality}
{}

Individual::Individual(Individual const &mother,
        Individual const &father,
        std::mt19937 &rng_r,
        Parameters const &params) 
{
    // set up bernoulli distribution to 
    // randomly sample parental alleles
    std::bernoulli_distribution random_segregator{0.5};
    std::uniform_real_distribution<double> uniform{0.0,1.0};
    std::normal_distribution<double> normal{0.0,1.0};

    p_group[0] = mother.p_group[random_segregator(rng_r)];
    p_group[1] = father.p_group[random_segregator(rng_r)];

    // cycle through alleles
    for (unsigned int allele_idx{0}; allele_idx < 2; 
            ++allele_idx)
    {
        if (uniform(rng_r) < params.mu_p_group)
        {
            p_group[allele_idx] += params.sdmu * normal(rng_r);

            // set bounds between 0, 1
            p_group[allele_idx] = p_group[allele_idx] < 0.0 ? 
                0.0 : (p_group[allele_idx] > 1.0 ? 1.0 : p_group[allele_idx]);
        }
    }

    // determine quality
    quality = uniform(rng_r) < params.p_high_quality;
} // end birth constructor

void Individual::operator=(Individual const &other)
{
    for (unsigned int allele_idx{0};
            allele_idx < 2;
            ++allele_idx)
    {
        p_group[allele_idx] = other.p_group[allele_idx];
    }

    quality = other.quality;
} 
