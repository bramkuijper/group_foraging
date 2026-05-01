#ifndef _PARAMETERS_HPP_
#define _PARAMETERS_HPP_

#include <string>


class Parameters {

    public:
        std::string file_name{"sim_dynprog_group_forage"};

        double k{0.5}; // slope of terminal fitness function
        double xmin{1}; // minimum value of x before fitness can commence (minimum investment)

        int xmax{100}; // values of x - resources to larvae
        int tmax{100}; // maximum number of time steps during the day

        double y1{0.5}; // quality of focal in terms of foraging success
        double y2{1.0}; // quality of non-focal in terms of foraging success

        double epsilon{1.0}; // how fast individuals should find food per timestep
        double pf{0.1}; // probability with which individuals will get predated upon when foraging

        double phmax{0.25}; // max probability of nest predation when all adults are foraging
        int n_at_home_max{2}; // max number of adults remaining at home
        double alpha_ph{1.0}; // whether relation between individuals staying home and predation risk of nest is concave, linear or convex
        double fmax{100}; // maximum value of the grid to evaluate 
        
        double ac{0.1}; // decrement due to larval development
        double R{1.0}; // increment upon finding food
};


#endif 
