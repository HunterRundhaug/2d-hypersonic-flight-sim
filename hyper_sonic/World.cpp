#include "World.h"
#include <cmath>

// default values probably should be changed 
World::World(): 
	gravity{ 9.81 },
	base_air_density{1.225},
	base_temperature{ 288.0 } {
}

double World::getGravity() const {
	return gravity;
}

double World::getAirDensityAtHeight(int height) const {
	double scale_height = 8500;
	double exponent = -height / scale_height;
	double air_dense_at_height = base_air_density * std::exp(exponent);
	return air_dense_at_height;
}

double World::getTemperatureAtHeight(int height) const {
	// Simple troposphere-style lapse rate model
	// Temperature drops about 0.0065 C per meter of altitude

	double lapse_rate = 0.0065;
	double temp = base_temperature - lapse_rate * height;

	// Optional clamp so it does not go unrealistically low
	if (temp < -56.5) {
		temp = -56.5;
	}

	return temp;
}
