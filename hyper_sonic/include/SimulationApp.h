#pragma once
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>
#include "Simulation.h"
#include "Vec2.h"
#include "CSVWriter.h"

class SimulationApp {

public:
	SimulationApp();
	void run();
	void startSimulation();
	void updateSimulation();
	void stopSimulation();

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
	sf::Text top_altitude_text;

	// CSV writer vars
	std::string csv_file_name = "output.csv";
	bool record_csv = false;
	bool simulation_running = false;
	std::unique_ptr<CSVWriter> csv_writer;

	void initText( sf::Text& text, std::string text_value, unsigned int size,
		sf::Color color, sf::Text::Style style, sf::Vector2f position);

	float getTopAltitude();
};