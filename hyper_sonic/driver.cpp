#include <iostream>

#include "SimulationApp.h"

#include "Simulation.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <imgui.h>
#include <imgui-SFML.h>
#include "Vec2.h"


int main()
{
    std::cout << "Initializing Program...\n";

    SimulationApp simApp = SimulationApp();
    simApp.run();
    

    /*
    Simulation* simulation = new Simulation();

    // steps * dt = total simulation time in seconds
    // default dt = 0.01
    int steps = 10000; 

    int write_csv_flag = 1;

    simulation->run(steps, write_csv_flag);
    */
}





