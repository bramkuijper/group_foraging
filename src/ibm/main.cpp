#include <string>
#include "simulation.hpp"

int main(int argc, char **argv)
{
    Parameters params{}; // start parameters with all the default settings
    
    // then override defaults with command-line args
    params.file_name = argv[1]; 
    params.mu_a_resource = std::stod(argv[2]); 
    params.mu_b_resource = std::stod(argv[3]); 
    params.p_high_quality = std::stod(argv[4]); 
    params.max_generation = std::stoul(argv[5]);

    Simulation sim{params};

    sim.run();

    return 0;
}
