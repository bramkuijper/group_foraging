#include <cmath>
#include "group_forage_dynamic.hpp"

GroupForageDP::GroupForageDP(Parameters const &params) :
    par{params},
    Wnext(par.tmax + 1, 
            std::vector < std::vector < double > >(
                par.xmax + 1, std::vector < double >(par.fmax + 1, 0.0)
                )
     ),
    W(par.tmax + 1, 
            std::vector < std::vector < double > >(
                par.xmax + 1, std::vector < double >(par.fmax + 1, 0.0)
                )
     ),
    Wopt(par.xmax + 1, 
            std::vector < double >(par.fmax + 1, 0.0)
                ),
    f(par.xmax + 1, 
            std::vector < double >(par.fmax + 1, 0.0)
                ),
    data_file{par.file_name}
{
    final_fitness();
}

void GroupForageDP::final_fitness()
{
    for (unsigned t_idx = 0; t_idx <= par.tmax; ++t_idx)
    {
        for (unsigned x = 0; x <= par.xmax; ++x)
        {
            for (unsigned f = 0; f <= par.fmax; ++f)
            {
                Wnext[t_idx][x][f] = 1.0 - std::exp(-par.k * (x - par.xmin));
            }
        }
    } // end for t
} // end final fitness


// the food encounter probability, see Mangel 1990 Anim Behav 39: 1163
double GroupForageDP::food_encounter_prob(double const y1, double const y2) 
{
    return(1.0 - std::exp(-par.epsilon * (y1 + y2)));
}

// probability of nest predation, which depends on the number of defenders
// could make this probably quality dependent too, but let's just focus 
// on number of workers staying at home for now
// the more folks stay at home the lower the predation probability
double GroupForageDP::ph(int const n_at_home)
{
    return(par.phmax * (1.0 - std::pow(n_at_home / par.n_at_home_max, alpha_ph)));
}


void GroupForageDP::optimal_decision()
{
    double fitness;
    for (int t{0}; t <= par.tmax; ++t)
    {
        for (int x{0}; x <= par.xmax; ++x)
        {
            Wopt[t][x] = 0.0;
            f[t][x] = 0; // probability of whether to forage

            for (int fval = 0; fval <= par.fmax; ++fval)
            {
                fitness = Wnext[std::min(par.tmax,t+1)][x][fval];

                if (fitness > Wopt[t][x])
                {
                    Wopt[t][x] = fitness;
                    f[t][x] = fval;
                }
            } // end for f
        } // end for x 
    } // end for t

    double f1_dbl,f2_dbl;

    for (int t{0}; t <= par.tmax; ++t)
    {
        for (int x{0}; x <= par.xmax; ++x)
        {
            for (int f1{0}; f1 <= par.fmax; ++f1)
            {
                f1_dbl = static_cast<double>(f1)/fmax;

                for (int f2{0}; f2 <= par.fmax; ++f2)
                {
                    f2_dbl = static_cast<double>(f2)/fmax;

                    W[t][x][f1][f2] = f1_dbl * f2_dbl * (1.0 - par.pf) * (1.0 - par.pf) *
                        (1.0 - ph(0)) * 
                }
            }
        }
    }
} // end optimal_decision fucntion

void GroupForageDP::write_data()
{
}

void GroupForageDP::write_parameters()
{
}

