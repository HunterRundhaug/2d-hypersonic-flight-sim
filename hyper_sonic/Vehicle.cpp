#include "Vehicle.h"

Vehicle::Vehicle()
	: position{ 0.0, 0.0 },
	velocity{ 0.0, 0.0 },
	direction{ 0.05, 0.9 }, 
	mass(1000.0),
	angular_velocity{0},
	moment_of_inertia{5},
	thrust(15000.0), 
	fuel_left(0.0),
	drag_coefficient(0.4),
	pressure_limit(0.0),
	reference_area(0.5){
}

// Getters 
Vec2 Vehicle::getPosition() const {
	return position;
}
Vec2 Vehicle::getVelocity() const {
	return velocity;
}
Vec2 Vehicle::getDirection() const {
	return direction;
}

double Vehicle::getMass() const {
	return mass;
}
double Vehicle::getThrust() const {
	return thrust;
}
double Vehicle::getFuelLeft() const {
	return fuel_left;
}
double Vehicle::getDrag() const {
	return drag_coefficient;
}
double Vehicle::getReferenceArea() const {
	return reference_area;
}
double Vehicle::getSpeed() const {
	double speed = velocity.getLength();
	return speed;
}
double Vehicle::getAngularVelocity() const {
	return angular_velocity;
}
double Vehicle::getMomentOfInertia() const {
	return moment_of_inertia;
}

// Setters
void Vehicle::setNewPosition(Vec2 new_position) {
	position = new_position;
}

void Vehicle::setNewVelocity(Vec2 new_velocity) {
	velocity = new_velocity;
}

void Vehicle::setAngularVelocity(double new_ang_velo) {
	angular_velocity = new_ang_velo;
}

void Vehicle::setDirection(Vec2 new_dir) {
	direction = new_dir;
}
