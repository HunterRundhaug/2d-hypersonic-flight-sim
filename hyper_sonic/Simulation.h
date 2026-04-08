#pragma once
#include "Vehicle.h"
#include "World.h"

class Simulation {
private:
	double dt;
	Vehicle vehicle;
	World world;

public:
	Simulation();

	Vehicle& getVehicle();

	void update();
	void run(int iterations, int write_csv_flag);
	void updateRotation(double speed);
	void updateTranslation(double speed);

	double getDeltaTime() const;
};