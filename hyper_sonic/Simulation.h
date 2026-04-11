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

	void setVehicleAltitude(double alt);

	Vec2 getVehiclePosition();
	Vec2 getVehicleDirection();
	double getVehicleSpeed();
	double getVehicleMass();
	double getVehicleMomentOfInertia();
	double getVehicleThrust();
	double getVehicleAngularVelocity();

	void resetVehicle();
	void setVehicleDirection(Vec2 newDir);
	void setVehicleMass(float new_mass);
	void setVehicleMomentOfInertia(float v_moment);
	void setVehicleThrust(float v_thrust);
};