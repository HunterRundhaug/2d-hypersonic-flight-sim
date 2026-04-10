#pragma once
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>
#include "Simulation.h"
#include "Vec2.h"

class SimulationApp {

public:
	SimulationApp();
	void run();

private:

	unsigned int WIDTH = 1920;
	unsigned int HEIGHT = 1080;

	// origin point for vehicle sprite
	float origin_x;
	float origin_y;

	// determines screen space size (make smaller to view larger area)
	float pixels_pre_meter = 0.1f;

	sf::Texture vehicle_texture;
	sf::Sprite vehicle_sprite;

	sf::FloatRect bounds;

	sf::Vector2f sprite_pos;

	float s_angle;

	Simulation simulation;

	Vehicle& vehicle;

	sf::RenderWindow window;

	sf::Clock deltaClock;

	sf::Clock clock;
	double accumulator = 0.0;
	double physicsDt;
	double simSpeed = 0.0;
	float simSpeedInput = 1.0f;
	bool paused = true;
	float directionAngleDeg = 0.0f;

	std::vector<sf::Vertex> trail;

	sf::Font font;
	
	// SFML texts
	sf::Text speed_value_text;
	sf::Text speed_text_static;
	sf::Text veh_x_pos_text;
	sf::Text veh_x_text_static;
	sf::Text veh_y_pos_text;
	sf::Text veh_y_text_static;

};