#pragma once

class World {
private:
	double gravity;
	double base_air_density; // kg/m^3: air density at sea level
	double base_tempurature; // Kelvin: tempurature at sea level

public:
	World();
	double getGravity() const;
	double getAirDensityAtHeight(int height) const;
	double getTempuratureAtHeight(int height) const;
};