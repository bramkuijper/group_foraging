#include <string>
#include "simulation.hpp"

int main(int argc, char **argv)
{
    Parameters params{}; // start parameters with all the default settings

    params.filename = argv[1]; // then override defaults with command-line args

    Simulation sim{params};

    return 0;
}
