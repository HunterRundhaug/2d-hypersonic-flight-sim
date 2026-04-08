#include <iostream>
#include "Simulation.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <imgui.h>
#include <imgui-SFML.h>
#include "Vec2.h"

sf::Vector2f worldToScreen(const Vec2& worldPos, float pixelsPerMeter, float originX, float originY) {
    float screenX = originX + static_cast<float>(worldPos.x) * pixelsPerMeter;
    float screenY = originY - static_cast<float>(worldPos.y) * pixelsPerMeter;
    return { screenX, screenY };
}

float getRotationDegree(Vec2 vehicle_direction) {
    double angle_rad = std::atan2(vehicle_direction.x, vehicle_direction.y);
    float angle_deg = (float)(angle_rad * 180 / 3.1415926535897323846);
    return angle_deg;
}

int main()
{
    std::cout << "Initializing Program...\n";

    // Simulation initialization
    Simulation* simulation = new Simulation();
    Vehicle& vehicle = simulation->getVehicle();
    int write_csv_flag = 0;

    // sprite init
    unsigned int WIDTH = 1920;
    unsigned int HEIGHT = 1080;
    sf::Texture texture;
    if (!texture.loadFromFile("Assets/rocket_icon.png")) {
        std::cerr << "Failed to load rocket texture\n";
    }
    sf::Sprite vehicle_sprite(texture);
    vehicle_sprite.setScale({ 0.04f, 0.04f });
    sf::FloatRect bounds = vehicle_sprite.getLocalBounds();
    vehicle_sprite.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });

    float origin_x = WIDTH / 2; // vehicle origin
    float origin_y =  HEIGHT - 100;

    float pixels_pre_meter = 0.1f; // determines screen space size (make smaller to view larger area)

    sf::Vector2f sprite_pos = worldToScreen(vehicle.getPosition(), pixels_pre_meter, origin_x, origin_y);
    vehicle_sprite.setPosition(sprite_pos);
    float s_angle = getRotationDegree(vehicle.getDirection());
    vehicle_sprite.setRotation(sf::degrees(s_angle));


    // WINDOW
    sf::RenderWindow window(sf::VideoMode({ WIDTH, HEIGHT }), "SFML works!");

    // ImGUI
    if (!ImGui::SFML::Init(window)) {
        std::cerr << "Failed to initialize ImGui-SFML\n";
        return 1;
    }

    sf::Clock deltaClock;

    // TEXT 
    sf::Font font;
    if (!font.openFromFile("Assets/google_sans.ttf"))
    {
        std::cerr << "Failed to load font!\n";
    }

    // speed text variable
    sf::Text speed_value_text(font);
    speed_value_text.setString("0");
    speed_value_text.setCharacterSize(24);
    speed_value_text.setFillColor(sf::Color::Green);
    speed_value_text.setStyle(sf::Text::Regular);
    speed_value_text.setPosition({ 100, (float)HEIGHT - 55.f });

    // static "speed" text
    sf::Text speed_text_static(font);
    speed_text_static.setString("speed :");
    speed_text_static.setCharacterSize(15);
    sf::Color speed_s_color(120, 210, 255);
    speed_text_static.setFillColor(speed_s_color);
    speed_text_static.setStyle(sf::Text::Regular);
    speed_text_static.setPosition({ 30, (float)HEIGHT - 50.f });

    // position x text variable
    sf::Text veh_x_pos_text(font);
    veh_x_pos_text.setString("0");
    veh_x_pos_text.setCharacterSize(24);
    veh_x_pos_text.setFillColor(sf::Color::Green);
    veh_x_pos_text.setStyle(sf::Text::Regular);
    veh_x_pos_text.setPosition({ 100, (float)HEIGHT - 85.f });

    // static "y" text
    sf::Text veh_x_text_static(font);
    veh_x_text_static.setString("x :");
    veh_x_text_static.setCharacterSize(15);
    veh_x_text_static.setFillColor(speed_s_color);
    veh_x_text_static.setStyle(sf::Text::Regular);
    veh_x_text_static.setPosition({ 30, (float)HEIGHT - 80.f });

    // position y text variable
    sf::Text veh_y_pos_text(font);
    veh_y_pos_text.setString("0");
    veh_y_pos_text.setCharacterSize(24);
    veh_y_pos_text.setFillColor(sf::Color::Green);
    veh_y_pos_text.setStyle(sf::Text::Regular);
    veh_y_pos_text.setPosition({ 100, (float)HEIGHT - 115.f });

    // static "y" text
    sf::Text veh_y_text_static(font);
    veh_y_text_static.setString("y :");
    veh_y_text_static.setCharacterSize(15);
    veh_y_text_static.setFillColor(speed_s_color);
    veh_y_text_static.setStyle(sf::Text::Regular);
    veh_y_text_static.setPosition({ 30, (float)HEIGHT - 110.f });

    // vehicle trail
    std::vector<sf::Vertex> trail;



    // Time
    sf::Clock clock;
    double accumulator = 0.0;
    double physicsDt = simulation->getDeltaTime(); // Need to be same as dt in simulation
    double simSpeed = 0.0;
    float simSpeedInput = 1.0f;

    // ImGUI variables
    bool paused = true;
    float directionAngleDeg = 0.0f; 

    // Animation Loop
    while (window.isOpen()) {
        double frameTime = clock.restart().asSeconds();
        accumulator += frameTime * simSpeed;

        while (const std::optional event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        while (accumulator >= physicsDt) {
            // run a single simulation step
            simulation->update();
            accumulator -= physicsDt;
        }

        // Update vehicle Sprite Position
        float s_angle = getRotationDegree(vehicle.getDirection());
        vehicle_sprite.setRotation(sf::degrees(s_angle));
        sf::Vector2f sprite_pos = worldToScreen(vehicle.getPosition(), pixels_pre_meter, origin_x, origin_y);
        vehicle_sprite.setPosition(sprite_pos);
    
        // Update text values
        double speed = vehicle.getSpeed();
        speed_value_text.setString(std::to_string(static_cast<int>(speed)));
        veh_x_pos_text.setString(std::to_string(static_cast<int>(vehicle.getPosition().x)));
        veh_y_pos_text.setString(std::to_string(static_cast<int>(vehicle.getPosition().y)));
        
        // update trail vector
        trail.push_back(sf::Vertex { sprite_pos, sf::Color::Cyan });

        // ImGUI - - - - - - - - - - 
        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::SetNextWindowSize(ImVec2(500, 200));

        // main control panel
        ImGui::Begin("Control");

        ImGui::InputFloat("Sim Speed", &simSpeedInput);

        if (ImGui::Button( paused ? "Start" : "Pause")) {
            
            paused ? simSpeed = simSpeedInput : simSpeed = 0.0;
            paused = !paused;
        }

        if (ImGui::Button("reset_vehicle")) {
            vehicle.resetVehicle();
            accumulator = 0.0;
            clock.restart();
        }

        if (ImGui::Button("clear trail")) {
            trail.clear();
        }

        ImGui::InputFloat("Simulation Scale", &pixels_pre_meter);

        ImGui::End();

        // vehicle state controls
        ImGui::SetNextWindowSize(ImVec2(500, 200));
        ImGui::Begin("Vehicle");

        if (ImGui::SliderFloat("Initial Angle", &directionAngleDeg, -180.0f, 180.0f)) {
            float angleRad = directionAngleDeg * 3.14159265f / 180.0f;
            Vec2 newDir(std::sin(angleRad), std::cos(angleRad));
            vehicle.setDirection(newDir);
        }

        float v_mass = vehicle.getMass();
        if (ImGui::InputFloat("Vehicle Mass", &v_mass)) {
            vehicle.setVehicleMass(v_mass);
        }
        float v_moment = vehicle.getMomentOfInertia();
        if (ImGui::InputFloat("Vehicle Moment Of Inertia", &v_moment)) {
            vehicle.setVehicleMomentOfInertia(v_moment);
        }
        float v_thrust = vehicle.getThrust();
        if (ImGui::InputFloat("Vehicle Thrust", &v_thrust)) {
            vehicle.setVehicleThrust(v_thrust);
        }

        ImGui::End();

        window.clear();
        window.draw(vehicle_sprite);
        window.draw(speed_value_text);
        window.draw(speed_text_static);
        window.draw(veh_x_pos_text);
        window.draw(veh_x_text_static);
        window.draw(veh_y_pos_text);
        window.draw(veh_y_text_static);
        if (trail.size() >= 2) {
            window.draw(&trail[0], trail.size(), sf::PrimitiveType::LineStrip);
        }

        ImGui::SFML::Render(window);
        window.display();


    }

    ImGui::SFML::Shutdown();// shutdown ImGUI

    /*
    Simulation* simulation = new Simulation();

    // steps * dt = total simulation time in seconds
    // default dt = 0.01
    int steps = 10000; 

    int write_csv_flag = 1;

    simulation->run(steps, write_csv_flag);
    */
}





