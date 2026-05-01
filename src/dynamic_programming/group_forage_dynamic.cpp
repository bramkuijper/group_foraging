#include <cmath>
#include <cassert>
#include "group_forage_dynamic.hpp"

GroupForageDP::GroupForageDP(Parameters const &params) :
    par{params},
    Wnext(par.tmax + 1, 
            std::vector < std::vector < std::vector < double > > >(
                par.xmax + 1, std::vector < std::vector < double > >(
                    par.fmax + 1, std::vector < double >(par.fmax + 1, 0.0)
                )
            )
     ),
    W(par.tmax + 1, 
            std::vector < std::vector < std::vector < double > > >(
                par.xmax + 1, std::vector < std::vector < double > >(
                    par.fmax + 1, std::vector < double >(par.fmax + 1, 0.0)
                )
            )
     ),
    Wopt(par.xmax + 1, 
            std::vector < std::vector < double > >(par.fmax + 1, 
                std::vector < double>(par.fmax + 1, 0.0)
                )
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
                Wnext[t_idx][x][f1][f2] = 1.0 - std::exp(-par.k * (x - par.xmin));
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
    return(par.phmax * (1.0 - std::pow(static_cast<double>(n_at_home) / par.n_at_home_max, alpha_ph)));
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

    double f1_dbl,f2_dbl,xpr, xpr1, xpr2, xprdiff, xprpr, xprpr1,xprpr2, xprprdiff;

    for (int t{0}; t <= par.tmax; ++t)
    {
        for (int x{0}; x <= par.xmax; ++x)
        {
            for (int f1{0}; f1 <= par.fmax; ++f1)
            {
                f1_dbl = static_cast<double>(f1)/par.fmax;

                for (int f2{0}; f2 <= par.fmax; ++f2)
                {
                    f2_dbl = static_cast<double>(f2)/par.fmax;

                    // value of x'
                    // i.e., updated resources in case of finding food
                    xpr = x - par.ac + par.R / 2.0;
                    // linear interpolation of xpr
                    xpr1 = std::floor(xpr);
                    xpr2 = std::ceil(xpr);
                    xprdiff = xpr - xpr1;

                    assert(xprdiff >= 0);
                    assert(xprdiff <= 1.0);


                    // value of x''
                    xprpr = x - par.ac;
                    // linear interpolation of x''
                    xprpr1 = std::floor(xprpr);
                    xprpr2 = std::ceil(xprpr);
                    xprprdiff = xprpr - xprpr1;
                    assert(xprprdiff >= 0);
                    assert(xprprdiff <= 1.0);

                    // event 1: both individuals go out to forage, both survive
                    W[t][x][f1][f2] = f1_dbl * f2_dbl * (1.0 - par.pf) * (1.0 - par.pf) *
                        (1.0 - ph(0)) * food_encounter_prob(par.y1, par.y2) * (
                                (1.0 - xprdiff) * Wopt[t][xpr1][f1][f2] +
                                xprdiff * Wopt[t][xpr2][f1][f2]
                                )
                        +
                        (1.0 - food_encounter_prb(par.y1, par.y2)) * (
                                (1.0 - xprprdiff)  * Wopt[t][xprpr1][f1][f2] +
                                xprprdiff * Wopt[t][xprpr2][f1][f2]);

                    // event 2: both individuals go out to forage, the non-focal y2 individual dies
                    W[t][x][f1][f2] += f1_dbl * f2_dbl * par.pf *(1.0 - par.pf) * (1.0 - ph(0)) *
                        * (food_encounter_prob(par.y1, 0.0) * (
                                (1.0 - xprdiff) * Vopt[t][xpr1][f1] +
                                xprdiff * Vopt[t][xpr2][f1] // focal is now alone
                                    ) +
                                (1.0 - food_encounter_prob(par.y1, 0.0)) * (
                                    (1.0 - xprprdiff) * Vopt[t][xprpr1][f1] +
                                    xprprdiff * Vopt[t][xprpr2][f1]
                                    )
                            );

                    // event 2: both individuals go out to forage, the focal y1 individual dies
                    W[t][x][f1][f2] += f1_dbl * f2_dbl * par.pf * (1.0 - par.pf) * (1.0 - ph(0)) *
                        * (food_encounter_prob(par.y2, 0.0) 

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
