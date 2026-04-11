#pragma once
#include "Vec2.h"


class Vehicle {
private:
	Vec2 position;
	Vec2 velocity;
	Vec2 direction;

	// origin variables for resetting
	Vec2 origin_position;
	Vec2 origin_velocity;
	Vec2 origin_direction;
	double origin_angular_velocity;

	double mass; 
	double thrust; 

	double angular_velocity;
	double moment_of_inertia;
	
	double drag_coefficient;
	double reference_area;

	double pressure_limit;
	double fuel_left;

	double temperature;

public:
	Vehicle();

	Vec2 getPosition() const;
	Vec2 getVelocity() const;
	Vec2 getDirection() const;

	double getTemperature() const;
	double getMass() const;
	double getThrust() const;
	double getFuelLeft() const;
	double getDrag() const;
	double getReferenceArea() const;
	double getAngularVelocity() const;
	double getMomentOfInertia() const;

	double getSpeed() const;

	// setters
	void setTemperature(double newTemp);
	void setNewPosition(Vec2 new_position);
	void setNewVelocity(Vec2 new_velocity);
	void setAngularVelocity(double new_ang_velo);
	void setDirection(Vec2 new_dir);

	void setVehicleMass(double newMass);
	void setVehicleMomentOfInertia(double moment);
	void setVehicleThrust(double new_thrust);

	void resetVehicle();
};