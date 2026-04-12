#include "SimulationApp.h"


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

void SimulationApp::initText(sf::Text& text, std::string text_value, unsigned int size,
    sf::Color color, sf::Text::Style style, sf::Vector2f position) 
{
    text.setString(text_value);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setStyle(style);
    text.setPosition(position);
}

void SimulationApp::startSimulation() {
    if (record_csv) {
        csv_writer = std::make_unique<CSVWriter>(csv_file_name);
        csv_writer->writeHeader({
            "time",
            "x",
            "y",
            "speed",
            "dir_x",
            "dir_y",
            "angular_velocity",
            "temperature"
            });
    }

    simulation_running = true;
}

void SimulationApp::updateSimulation() {
    if (!simulation_running) {
        return;
    }

    simulation.update();

    if (record_csv && csv_writer) {
        csv_writer->writeRow({
            std::to_string(simulation.getTime()),
            std::to_string(simulation.getVehicle().getPosition().x),
            std::to_string(simulation.getVehicle().getPosition().y),
            std::to_string(simulation.getVehicle().getSpeed()),
            std::to_string(simulation.getVehicle().getDirection().x),
            std::to_string(simulation.getVehicle().getDirection().y),
            std::to_string(simulation.getVehicle().getAngularVelocity()),
            std::to_string(simulation.getVehicle().getTemperature())
            });
    }
}

void SimulationApp::stopSimulation() {
    simulation_running = false;

    if (csv_writer) {
        csv_writer->closeFile();
        csv_writer.reset();
    }
}

float SimulationApp::getTopAltitude() {
    return origin_y / pixels_pre_meter;
}

// Main Constructor
SimulationApp::SimulationApp(): 
    vehicle_texture("Assets/rocket_icon.png"),
    vehicle_sprite(vehicle_texture),
    simulation(),
    window(sf::VideoMode({ WIDTH, HEIGHT }), "SFML works!"),
    speed_value_text(font),
    speed_text_static(font),
    veh_x_pos_text(font),
    veh_x_text_static(font),
    veh_y_pos_text(font),
    veh_y_text_static(font),
    veh_ang_velo_text_static(font),
    veh_ang_velo_text(font),
    air_psr_at(font),
    air_pst_static(font),
    veh_temp(font),
    veh_temp_static(font),
    top_altitude_text(font),
    physicsDt(simulation.getDeltaTime())

{
    // Main constructor body
    vehicle_sprite.setScale({ 0.04f, 0.04f });

    // set sprite rotation point to center
    bounds = vehicle_sprite.getLocalBounds();
    vehicle_sprite.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });

    origin_x = WIDTH / 2; // vehicle sprite origin
    origin_y = HEIGHT - 100;

    sprite_pos = worldToScreen(simulation.getVehiclePosition(), pixels_pre_meter, origin_x, origin_y);
    vehicle_sprite.setPosition(sprite_pos);
    s_angle = getRotationDegree(simulation.getVehicleDirection());
    vehicle_sprite.setRotation(sf::degrees(s_angle));

    // TEXT 
    if (!font.openFromFile("Assets/google_sans.ttf"))
    {
        std::cerr << "Failed to load font!\n";
    }

    // set top alt var
    float topAltitude = getTopAltitude();

    // SFML texts
 
    // speed text variable
    initText(speed_value_text, "0", 24, sf::Color::Green,
        sf::Text::Regular, { 100, (float)HEIGHT - 55.f });

    // static "speed" text
    sf::Color speed_s_color(120, 210, 255);
    initText(speed_text_static, "speed :", 15, speed_s_color,
        sf::Text::Regular, { 30, (float)HEIGHT - 50.f });

    // position x text variable
    initText(veh_x_pos_text, "0", 24, sf::Color::Green,
        sf::Text::Regular, { 100, (float)HEIGHT - 85.f });

    // static "y" text
    initText(veh_x_text_static, "x :", 15, speed_s_color,
        sf::Text::Regular, { 30, (float)HEIGHT - 80.f });

    // position y text variable
    initText(veh_y_pos_text, "0", 24, sf::Color::Green,
        sf::Text::Regular, { 100, (float)HEIGHT - 115.f });

    // static "y" text
    initText(veh_y_text_static, "y/ALT :", 15, speed_s_color,
        sf::Text::Regular, { 30, (float)HEIGHT - 110.f });

    // static angular velo text
    initText(veh_ang_velo_text_static, "ang_velo :", 15, speed_s_color,
        sf::Text::Regular, { 30, (float)HEIGHT - 140.f });

    // angular velo value text
    initText(veh_ang_velo_text, "0", 24, sf::Color::Green,
        sf::Text::Regular, { 110, (float)HEIGHT - 145.f });

    // air pressure variable 
    initText(air_psr_at, "0", 24, sf::Color::Green,
        sf::Text::Regular, { 110, (float)HEIGHT - 175.f });

    // air pressure static
    initText(air_pst_static, "air kg/m^3 :", 15, speed_s_color,
        sf::Text::Regular, { 30, (float)HEIGHT - 170.f });

    // vehicle temperature varaible
    initText(veh_temp, "0", 24, sf::Color::Green,
        sf::Text::Regular, { 150, (float)HEIGHT - 205.f });

    // vehicle temp static
    initText(veh_temp_static, "vehicle temp kelvin :", 15, speed_s_color,
        sf::Text::Regular, { 10, (float)HEIGHT - 200.f });

    // top altitude variable
    initText(top_altitude_text,
        "<--Top Alt : " + std::to_string(static_cast<int>(topAltitude)),
        unsigned int(18), sf::Color::Green,
        sf::Text::Regular, { WIDTH - 230.f, 25.f });
    
}

void SimulationApp::run() {

    // ImGUI
    if (!ImGui::SFML::Init(window)) {
        std::cerr << "Failed to initialize ImGui-SFML\n";
        return;
    }

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
            updateSimulation();
            accumulator -= physicsDt;
        }

        // Update vehicle Sprite Position
        float s_angle = getRotationDegree(simulation.getVehicleDirection());
        vehicle_sprite.setRotation(sf::degrees(s_angle));
        sprite_pos = worldToScreen(simulation.getVehiclePosition(), pixels_pre_meter, origin_x, origin_y);
        vehicle_sprite.setPosition(sprite_pos);

        // Update text values
        double speed = simulation.getVehicleSpeed();
        speed_value_text.setString(std::to_string(static_cast<int>(speed)));
        veh_x_pos_text.setString(std::to_string(static_cast<int>(simulation.getVehiclePosition().x)));
        veh_y_pos_text.setString(std::to_string(static_cast<int>(simulation.getVehiclePosition().y)));
        veh_ang_velo_text.setString(std::to_string((simulation.getVehicleAngularVelocity())));
        air_psr_at.setString(std::to_string(simulation.getAirPressureAtVehicle()));
        veh_temp.setString(std::to_string(static_cast<int>(simulation.getVehicleTemperature())));

        // update trail vector
        trail.push_back(sf::Vertex{ sprite_pos, sf::Color::Cyan });

        // ImGUI - - - - - - - - - - 
        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::SetNextWindowSize(ImVec2(500, 200));

        // main control panel
        ImGui::Begin("Control");

        ImGui::InputFloat("Sim Speed", &simSpeedInput);

        if (ImGui::Button(paused ? "Start" : "Pause")) {

            if (paused) {
                simSpeed = simSpeedInput;
                simulation_running = true;
                startSimulation();
            }
            else {
                simSpeed = 0.0;
                stopSimulation();
            }
            paused = !paused;
        }

        if (ImGui::Button("reset_vehicle")) {
            simulation.resetVehicle();
            accumulator = 0.0;
            clock.restart();
        }

        if (ImGui::Button("clear trail")) {
            trail.clear();
        }

        if (ImGui::InputFloat("Simulation Scale", &pixels_pre_meter)) {
            top_altitude_text.setString("<--Top Alt : " + std::to_string(static_cast<int>(getTopAltitude())));
        }

        ImGui::End();

        // vehicle state controls
        ImGui::SetNextWindowSize(ImVec2(500, 200));
        ImGui::Begin("Vehicle");

        if (ImGui::SliderFloat("Initial Angle", &directionAngleDeg, -180.0f, 180.0f)) {
            float angleRad = directionAngleDeg * 3.14159265f / 180.0f;
            Vec2 newDir(std::sin(angleRad), std::cos(angleRad));
            simulation.setVehicleDirection(newDir);
        }

        float v_mass = simulation.getVehicleMass();
        if (ImGui::InputFloat("Vehicle Mass", &v_mass)) {
            simulation.setVehicleMass(v_mass);
        }
        float v_moment = simulation.getVehicleMomentOfInertia();
        if (ImGui::InputFloat("Vehicle Moment Of Inertia", &v_moment)) {
            simulation.setVehicleMomentOfInertia(v_moment);
        }
        float v_thrust = simulation.getVehicleThrust();
        if (ImGui::InputFloat("Vehicle Thrust", &v_thrust)) {
            simulation.setVehicleThrust(v_thrust);
        }

        float v_alt = simulation.getVehiclePosition().y;
        if (ImGui::InputFloat("Altitude", &v_alt)) {
            simulation.setVehicleAltitude(v_alt);
            sprite_pos = worldToScreen(simulation.getVehiclePosition(), pixels_pre_meter, origin_x, origin_y);
            vehicle_sprite.setPosition(sprite_pos);
        }

        float v_drag = simulation.getVehicleDragCoefficient();
        if (ImGui::InputFloat("Drag Coefficient", &v_drag)) {
            simulation.setVehicleDragCoefficient(v_drag);
        }

        float v_ref_area = simulation.getVehicleReferenceArea();
        if (ImGui::InputFloat("Drag Reference Area", &v_ref_area)) {
            simulation.setVehicleReferenceArea(v_ref_area);
        }

        ImGui::End();

        // CSV controller
        ImGui::SetNextWindowSize(ImVec2(300, 200));
        ImGui::Begin("File output");

        static char file_name_buffer[128] = "output.csv";
        ImGui::InputText("CSV File", file_name_buffer, sizeof(file_name_buffer));
        csv_file_name = file_name_buffer;
        if (csv_file_name.empty()) {
            csv_file_name = "output.csv";
        }
        ImGui::Checkbox("Record flight data to CSV", &record_csv);

        ImGui::End();

        window.clear();
        window.draw(vehicle_sprite);
        window.draw(speed_value_text);
        window.draw(speed_text_static);
        window.draw(veh_x_pos_text);
        window.draw(veh_x_text_static);
        window.draw(veh_y_pos_text);
        window.draw(veh_y_text_static);
        window.draw(veh_ang_velo_text_static);
        window.draw(veh_ang_velo_text);
        window.draw(air_psr_at);
        window.draw(air_pst_static);
        window.draw(veh_temp);
        window.draw(veh_temp_static);
        window.draw(top_altitude_text);
        if (trail.size() >= 2) {
            window.draw(&trail[0], trail.size(), sf::PrimitiveType::LineStrip);
        }

        ImGui::SFML::Render(window);
        window.display();


    }

    ImGui::SFML::Shutdown();// shutdown ImGUI
}