#include "Simulation.h"
#include "Vec2.h"
#include <iostream>
#include "CSVWriter.h"
#include <cmath>

Simulation::Simulation(): 
	dt{0.01},
	vehicle{},
	world{}
{}

void Simulation::update() {
	
	// calculate new direction 
	double speed = vehicle.getSpeed();
	updateRotation(speed);
	updateTranslation(speed);

}

void Simulation::updateRotation(double speed) {
	// calculate new direction 
	double c = 1;
	double angular_damping = c * vehicle.getAngularVelocity();
	double mismatch_angle = vehicle.getDirection().crossProduct(vehicle.getVelocity().normalize());
	double k = 0.0001;
	double aero_torque = 0;
	if (speed > 1e-6) {
		aero_torque = k * world.getAirDensityAtHeight(vehicle.getPosition().y) * speed * speed * mismatch_angle;
	}
	double net_torque = aero_torque - angular_damping;
	double angular_acceleration = net_torque / vehicle.getMomentOfInertia();
	vehicle.setAngularVelocity(vehicle.getAngularVelocity() + angular_acceleration * dt);
	double delta_theta = vehicle.getAngularVelocity() * dt;
	double old_xdir = vehicle.getDirection().x;
	double old_ydir = vehicle.getDirection().y;
	double new_xdir = old_xdir * std::cos(delta_theta) - old_ydir * std::sin(delta_theta);
	double new_ydir = old_xdir * std::sin(delta_theta) + old_ydir * std::cos(delta_theta);
	Vec2 new_dir{ new_xdir, new_ydir };
	new_dir = new_dir.normalize();
	vehicle.setDirection(new_dir);
}

void Simulation::updateTranslation(double speed) {
	Vec2 thrust_force = vehicle.getDirection().normalize() * vehicle.getThrust();
	Vec2 gravity_force{ 0, -vehicle.getMass() * world.getGravity() };

	double rho = world.getAirDensityAtHeight(vehicle.getPosition().y);
	double Cd = vehicle.getDrag();
	double A = vehicle.getReferenceArea();
	double drag_magnitude = 0.5 * rho * speed * speed * Cd * A;
	Vec2 drag_direction = vehicle.getVelocity().normalize() * -1.0;
	Vec2 drag_force = drag_direction * drag_magnitude;

	Vec2 total_force = thrust_force + gravity_force + drag_force;

	Vec2 acceleration = total_force / vehicle.getMass();
	Vec2 new_velocity = vehicle.getVelocity() + (acceleration * dt);

	Vec2 new_position = vehicle.getPosition() + new_velocity * dt;

	// set vehicle's new values
	vehicle.setNewPosition(new_position);
	vehicle.setNewVelocity(new_velocity);
}

void Simulation::run(int iterations, int write_csv_flag) {
	double time = 0;

	// write data to csv 
	CSVWriter* csv = nullptr;
	if (write_csv_flag) {
		csv = new CSVWriter("output.csv");
		csv->writeHeader(std::vector<std::string>{"time", "x", "y", "speed", "dir_x", "dir_y", "angular_velocity"});
	}

	while (iterations > 0) {

		// write data to csv
		if (write_csv_flag) {
			std::string time_str = std::to_string(time);
			std::string x_str = std::to_string(vehicle.getPosition().x);
			std::string y_str = std::to_string(vehicle.getPosition().y);
			std::string speed_str = std::to_string(vehicle.getSpeed());
			std::string vehicle_dir_x = std::to_string(vehicle.getDirection().x);
			std::string vehicle_dir_y = std::to_string(vehicle.getDirection().y);
			std::string angular_velo = std::to_string(vehicle.getAngularVelocity());
			csv->writeRow(std::vector<std::string>{time_str, x_str, y_str, speed_str, vehicle_dir_x, vehicle_dir_y, angular_velo});
		}

		//std::cout << "Position: " << vehicle.getPosition() << " Speed: " << vehicle.getSpeed() <<  std::endl;

		update();
		time += dt;
		iterations--;
	}

	if (write_csv_flag) {
		csv->closeFile();
	}
}

Vehicle& Simulation::getVehicle() {
	return vehicle;
}

double Simulation::getDeltaTime()const {
	return dt;
}