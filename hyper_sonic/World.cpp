#include "World.h"
#include <cmath>

// default values probably should be changed 
World::World(): 
	gravity{ 9.81 },
	base_air_density{1.225},
	base_tempurature{ 288.0 } {
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

double World::getTempuratureAtHeight(int height) const {
	// TODO return a dynamic tempurature
	return base_tempurature;
}
