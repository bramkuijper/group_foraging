#include <random>
#include <algorithm>
#include "parameters.hpp"
#include "individual.hpp"

Individual::Individual(Parameters const &parms) :
    a_resource{0.5 * parms.a_resource_init, 0.5 * parms.a_resource_init},
    b_resource{0.5 * parms.b_resource_init, 0.5 * parms.b_resource_init},
    a_t{0.5 * parms.a_t_init, 0.5 * parms.a_t_init},
    b_t{0.5 * parms.b_t_init, 0.5 * parms.b_t_init},
    a_qown{0.5 * parms.a_qown_init, 0.5 * parms.a_qown_init},
    b_qown{0.5 * parms.b_qown_init, 0.5 * parms.b_qown_init},
    a_qother{0.5 * parms.a_qother_init, 0.5 * parms.a_qother_init},
    b_qother{0.5 * parms.b_qother_init, 0.5 * parms.b_qother_init},
    a_action_other{0.5 * parms.a_action_other_init, 0.5 * parms.a_action_other_init},
    b_action_other{0.5 * parms.b_action_other_init, 0.5 * parms.b_action_other_init}
{}

Individual::Individual(Individual const &other) :
    a_resource{other.a_resource[0],other.a_resource[1]},
    b_resource{other.b_resource[0],other.b_resource[1]},
    a_t{other.a_t[0],other.a_t[1]},
    b_t{other.b_t[0],other.b_t[1]},
    a_qown{other.a_qown[0],other.a_qown[1]},
    b_qown{other.b_qown[0],other.b_qown[1]},
    a_qother{other.a_qother[0],other.a_qother[1]},
    b_qother{other.b_qother[0],other.b_qother[1]},
    a_action_other{other.a_action_other[0],other.a_action_other[1]},
    b_action_other{other.b_action_other[0],other.b_action_other[1]},
    quality{other.quality},
    action_previous{other.action_previous}
{}

// calculate probability an 
// individual will go and forage
double Individual::prob_forage(
        double const resources,
        double const time_cue,
        double const qown,
        double const qother,
        double const action_other)
{
    double exponent{
        0.5 * (b_resource[0] + b_resource[1]) *
            (
                resources - 0.5 * (a_resource[0] + a_resource[1])
            )
        + 0.5 * (b_t[0] + b_t[1]) *
            (
                time_cue - 0.5 * (a_t[0] + a_t[1])
            )
        + 0.5 * (b_qown[0] + b_qown[1]) *
            (
                qown - 0.5 * (a_qown[0] + a_qown[1])
            )
        + 0.5 * (b_qother[0] + b_qother[1]) *
            (
                qother - 0.5 * (a_qother[0] + a_qother[1])
            )
        + 0.5 * (b_qother[0] + b_qother[1]) *
            (
                qother - 0.5 * (a_qother[0] + a_qother[1])
            )
        + 0.5 * (b_action_other[0] + b_action_other[1]) *
            (
                action_other - 0.5 * (a_action_other[0] + a_action_other[1])
            )
    };

    return(1.0 / (1.0 + std::exp(-exponent)));
} // end prob_forage


// mutate individual alleles
void Individual::mutate(
            std::mt19937 &rng_r,
            Parameters const &params)
{
    std::uniform_real_distribution<double> uniform{0.0,1.0};
    std::normal_distribution<double> normal{0.0,1.0};

    for (unsigned int allele_idx{0};
            allele_idx < 2; ++allele_idx)
    {
        if (uniform(rng_r) < params.mu_a_resource)
        {
            a_resource[allele_idx] += normal(rng_r) * params.sdmu;

            a_resource[allele_idx] = std::clamp(a_resource[allele_idx], 
                    params.logistic_min_bound, 
                    params.logistic_max_bound);

        }
        
        if (uniform(rng_r) < params.mu_b_resource)
        {
            b_resource[allele_idx] += normal(rng_r) * params.sdmu;

            b_resource[allele_idx] = std::clamp(b_resource[allele_idx], 
                    params.logistic_min_bound, 
                    params.logistic_max_bound);

        }
        
        if (uniform(rng_r) < params.mu_b_resource)
        {
            b_resource[allele_idx] += normal(rng_r) * params.sdmu;

            b_resource[allele_idx] = std::clamp(b_resource[allele_idx], 
                    params.logistic_min_bound, 
                    params.logistic_max_bound);
        }
        
        if (uniform(rng_r) < params.mu_a_t)
        {
            a_t[allele_idx] += normal(rng_r) * params.sdmu;

            a_t[allele_idx] = std::clamp(a_t[allele_idx], 
                    params.logistic_min_bound, 
                    params.logistic_max_bound);
        }
        
        if (uniform(rng_r) < params.mu_b_t)
        {
            b_t[allele_idx] += normal(rng_r) * params.sdmu;

            b_t[allele_idx] = std::clamp(b_t[allele_idx], 
                    params.logistic_min_bound, 
                    params.logistic_max_bound);
        }
        
        if (uniform(rng_r) < params.mu_a_qown)
        {
            a_qown[allele_idx] += normal(rng_r) * params.sdmu;

            a_qown[allele_idx] = std::clamp(a_qown[allele_idx], 
                    params.logistic_min_bound, 
                    params.logistic_max_bound);
        }

        if (uniform(rng_r) < params.mu_b_qown)
        {
            b_qown[allele_idx] += normal(rng_r) * params.sdmu;

            b_qown[allele_idx] = std::clamp(b_qown[allele_idx], 
                    params.logistic_min_bound, 
                    params.logistic_max_bound);
        }
        

        if (uniform(rng_r) < params.mu_a_qother)
        {
            a_qother[allele_idx] += normal(rng_r) * params.sdmu;

            a_qother[allele_idx] = std::clamp(a_qother[allele_idx], 
                    params.logistic_min_bound, 
                    params.logistic_max_bound);
        }

        if (uniform(rng_r) < params.mu_b_qother)
        {
            b_qother[allele_idx] += normal(rng_r) * params.sdmu;

            b_qother[allele_idx] = std::clamp(b_qother[allele_idx], 
                    params.logistic_min_bound, 
                    params.logistic_max_bound);
        }
        
        if (uniform(rng_r) < params.mu_a_action_other)
        {
            a_action_other[allele_idx] += normal(rng_r) * params.sdmu;

            a_action_other[allele_idx] = std::clamp(a_action_other[allele_idx], 
                    params.logistic_min_bound, 
                    params.logistic_max_bound);
        }

        if (uniform(rng_r) < params.mu_b_action_other)
        {
            b_action_other[allele_idx] += normal(rng_r) * params.sdmu;

            b_action_other[allele_idx] = std::clamp(b_action_other[allele_idx], 
                    params.logistic_min_bound, 
                    params.logistic_max_bound);
        }
    } // end for allele_idx
} // end mutate


Individual::Individual(Individual const &mother,
        std::mt19937 &rng_r,
        Parameters const &params) 
{
    // set up bernoulli distribution to 
    // randomly sample parental alleles
    std::bernoulli_distribution random_segregator{0.5};
    std::uniform_real_distribution<double> uniform{0.0,1.0};

    // inheritance
    a_resource[0] = mother.a_resource[random_segregator(rng_r)];
    a_resource[1] = mother.a_resource[random_segregator(rng_r)];

    b_resource[0] = mother.b_resource[random_segregator(rng_r)];
    b_resource[1] = mother.b_resource[random_segregator(rng_r)];

    a_t[0] = mother.a_t[random_segregator(rng_r)];
    a_t[1] = mother.a_t[random_segregator(rng_r)];
    
    b_t[0] = mother.b_t[random_segregator(rng_r)];
    b_t[1] = mother.b_t[random_segregator(rng_r)];
    
    a_qown[0] = mother.a_qown[random_segregator(rng_r)];
    a_qown[1] = mother.a_qown[random_segregator(rng_r)];

    b_qown[0] = mother.b_qown[random_segregator(rng_r)];
    b_qown[1] = mother.b_qown[random_segregator(rng_r)];
    
    a_qother[0] = mother.a_qother[random_segregator(rng_r)];
    a_qother[1] = mother.a_qother[random_segregator(rng_r)];

    b_qother[0] = mother.b_qother[random_segregator(rng_r)];
    b_qother[1] = mother.b_qother[random_segregator(rng_r)];
    
    a_action_other[0] = mother.a_action_other[random_segregator(rng_r)];
    a_action_other[1] = mother.a_action_other[random_segregator(rng_r)];

    b_action_other[0] = mother.b_action_other[random_segregator(rng_r)];
    b_action_other[1] = mother.b_action_other[random_segregator(rng_r)];

    mutate(rng_r, params);
    
    // determine quality
    quality = uniform(rng_r) < params.p_high_quality;
} // end birth constructor

void Individual::operator=(Individual const &other)
{
    for (unsigned int allele_idx{0};
            allele_idx < 2;
            ++allele_idx)
    {
        a_resource[allele_idx] = other.a_resource[allele_idx];
        b_resource[allele_idx] = other.b_resource[allele_idx];
        a_t[allele_idx] = other.a_t[allele_idx];
        b_t[allele_idx] = other.b_t[allele_idx];
        a_qown[allele_idx] = other.a_qown[allele_idx];
        b_qown[allele_idx] = b_qown[allele_idx];
        a_qother[allele_idx] = other.a_qother[allele_idx];
        b_qother[allele_idx] = other.b_qother[allele_idx];
        a_action_other[allele_idx] = other.a_action_other[allele_idx];
        b_action_other[allele_idx] = other.b_action_other[allele_idx];
    }

    quality = other.quality;
    action_previous = other.action_previous;
} 
