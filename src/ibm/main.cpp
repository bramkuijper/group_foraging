#include <string>
#include "simulation.hpp"

int main(int argc, char **argv)
{
    Parameters params{}; // start parameters with all the default settings
    
    // then override defaults with command-line args
    params.file_name = argv[1]; 
    params.mu_a_resource = std::stod(argv[2]); 
    params.mu_b_resource = std::stod(argv[3]); 
    params.mu_a_t = std::stod(argv[4]); 
    params.mu_b_t = std::stod(argv[5]); 
    params.mu_a_qown = std::stod(argv[6]); 
    params.mu_b_qown = std::stod(argv[7]); 
    params.mu_a_qother = std::stod(argv[8]); 
    params.mu_b_qother = std::stod(argv[9]); 
    params.mu_a_action_other = std::stod(argv[10]); 
    params.mu_b_action_other = std::stod(argv[11]); 
    params.p_high_quality = std::stod(argv[12]); 
    params.init_resources = std::stod(argv[13]); 
    params.nest_pred_baseline = std::stod(argv[14]); 
    params.nest_pred_scale = std::stod(argv[15]); 
    params.epsilon = std::stod(argv[16]); 
    params.max_generation = static_cast<unsigned>(std::stoul(argv[17]));

    Simulation sim{params};

    sim.run();

    return 0;
}
