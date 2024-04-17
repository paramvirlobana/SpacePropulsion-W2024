#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>
#include <cmath>


class GravitySource
{
	sf::Vector2f pos;
	sf::Vector2f pos_e;
	float strength;
	sf::CircleShape s;
	sf::Texture texture_earth;

public:
	GravitySource(float pos_x, float pos_y, float strength) {
		pos.x = pos_x;
		pos.y = pos_y;
		this->strength = strength;
		pos_e.x = pos_x - 30;
		pos_e.y = pos_y - 30;

		s.setTexture(&texture_earth);
		s.setPosition(pos_e);
		s.setRadius(30);
	}

	void render(sf::RenderWindow& wind)
	{
		wind.draw(s);
	}
	sf::Vector2f get_pos()
	{
		return pos;
	}
	float get_strength()
	{
		return strength;
	}


};

class Particle {
	sf::CircleShape s;
	std::vector<sf::Vector2f> path;

public:
	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::Vector2f pos_s;

	Particle(float pos_x, float pos_y, float vel_x, float vel_y) {
		pos.x = pos_x;
		pos.y = pos_y;
		pos_s.x = pos.x - 6;
		pos_s.y = pos.y - 6;
		vel.x = vel_x;
		vel.y = vel_y;

		s.setPosition(pos_s);
		s.setFillColor(sf::Color::Red);
		s.setRadius(6);
	}

	void render(sf::RenderWindow& wind) {
		s.setPosition(pos_s);
		wind.draw(s);

		// Render the path as dotted lines
		for (size_t i = 1; i < path.size(); ++i) {
			sf::Vertex line[] = {
				sf::Vertex(path[i - 1], sf::Color::White),
				sf::Vertex(path[i], sf::Color::White)
			};
			wind.draw(line, 2, sf::Lines);
		}
	}

	void set_color(sf::Color col) {
		s.setFillColor(col);
	}

	void update_physics(GravitySource& s, float dt) {
		float distance_x = s.get_pos().x - pos.x;
		float distance_y = s.get_pos().y - pos.y;

		float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

		float inverse_distance = 1.f / distance;

		float normalized_x = inverse_distance * distance_x;
		float normalized_y = inverse_distance * distance_y;

		float inverse_square_dropoff = inverse_distance * inverse_distance;

		float acceleration_x = normalized_x * s.get_strength() * inverse_square_dropoff;
		float acceleration_y = normalized_y * s.get_strength() * inverse_square_dropoff;

		vel.x += acceleration_x * dt;
		vel.y += acceleration_y * dt;

		pos.x += vel.x * dt;
		pos.y += vel.y * dt;
		pos_s.x = pos.x - 6;
		pos_s.y = pos.y - 6;
		path.push_back(pos);
		// Limit the size of the path to prevent it from growing indefinitely
		if (path.size() > 20000)
			path.erase(path.begin()); // Remove the oldest position
	}
};



int main() {

	std::cout << "MIT License\n"
		<< "\n"
		<< "Copyright (c) 2024 | Paramvir Singh Lobana | Jian Jiao\n"
		<< "\n"
		<< "Permission is hereby granted, free of charge, to any person obtaining a copy\n"
		<< "of this software and associated documentation files (the \"Software\"), to deal\n"
		<< "in the Software without restriction, including without limitation the rights\n"
		<< "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
		<< "copies of the Software, and to permit persons to whom the Software is\n"
		<< "furnished to do so, subject to the following conditions:\n"
		<< "\n"
		<< "The above copyright notice and this permission notice shall be included in all\n"
		<< "copies or substantial portions of the Software.\n"
		<< "\n"
		<< "====================================================================================== \n"
		<< "INSTRUCTIONS\n"
		<< "-------------------------------------------------------------------------------------- \n"
		<< "Mode 1  \n"
		<< "Up arrow key to increase the orbit radius.\n"
		<< "Down arrow key to decrease the orbit radius.\n"
		<< "Press T to initiate orbit transfer in Mode 1 \n"
		<< "-------------------------------------------------------------------------------------- \n"
		<< "Mode 2  \n"
		<< "Left arrow key \n"
		<< "Right arrow key \n"
		<< "Press the spacebar key to normalize the orbit in Model 2 \n"
		<< "-------------------------------------------------------------------------------------- \n"
		<< "Ctrl + scroll wheel for zooming in and zooming out. \n"
		<< "====================================================================================== \n";


	// GUI Values
	float win_x = 1600;
	float win_y = 1000;
	float info_x = win_x * 0.05;
	float info_y = win_y * 0.7;
	float info_char_size = 14 + 4;

	sf::RenderWindow window(sf::VideoMode(win_x, win_y), "OrbitSim Desktop");
	window.setFramerateLimit(60);
	sf::View view = window.getView();

	// Initialize sources and particles
	std::vector<GravitySource> sources;
	sources.push_back(GravitySource(win_x / 2, win_y / 2, 6500));


	float particle_x_start = win_x / 2 + 200;
	float particle_y_start = win_y / 2;
	float particle_start = sqrt(pow(200, 2) + pow(0, 2));
	std::vector<Particle> particles;
	particles.push_back(Particle(particle_x_start, particle_y_start, (particle_y_start - win_y / 2) / particle_start * sqrt(6500 / particle_start), -(particle_x_start - win_x / 2) / particle_start * sqrt(6500 / particle_start)));

	sf::Font font;
	if (!font.loadFromFile("resources\\arial.ttf")) {
		std::cerr << "Failed to load font file!\n";
		return EXIT_FAILURE;
	}
	sf::Font font_orb;
	if (!font_orb.loadFromFile("resources\\retro_gaming.ttf")) {
		std::cerr << "Failed to load font file!\n";
		return EXIT_FAILURE;
	}

	// setup output
	sf::Text orbitsim;
	sf::Text status_string;
	sf::Text velocity;
	sf::Text position2; sf::Text position1; sf::Text Radius;
	sf::Text period;
	sf::Text vis_viva;
	sf::Text plus;
	sf::Text minus;
	sf::Text tsf;
	sf::Text Target_Orbit;

	// Display value
	// float P_x = particles[0].pos.x;
	float P_x = 0;
	float P_y = 0;
	float t = 1.f;
	float V_abs = 0;
	float Dv_1 = 0;
	float Dv_2 = 0;
	float P_abs = 0;
	float P_abs_tf = 0;
	float E_cond = 0;
	float P_cond = 0;
	float Target_O = 300;
	int i = 3;


	float timeScale = 20.0f; // Defines how fast the particle is running

	sf::Clock clock;
	const float fixedTimeStep = 1.0f / 5000.0f; // 60 FPS


	// Add program icon
	sf::Image icon;
	icon.loadFromFile("resources\\orb.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	//define key parameters
	float key_pos_x = 0.825; float key_pos_y = 5;
	float key_scale_val = 0.6;

	// Add key image
	sf::Text m_1; m_1.setFont(font); m_1.setString("Mode 1 - Hohmann Transfer (Circular)"); m_1.setCharacterSize(14); m_1.setPosition(win_x * key_pos_x, win_y * 0.375 - 25);
	sf::Text m_11; m_11.setFont(font); m_11.setString("+ Orbit Radius"); m_11.setCharacterSize(16); m_11.setPosition(win_x * key_pos_x + 75, win_y * 0.375 + 20);
	sf::Text m_12; m_12.setFont(font); m_12.setString("- Orbit Radius"); m_12.setCharacterSize(16); m_12.setPosition(win_x * key_pos_x + 75, win_y * 0.45 + 20);
	sf::Texture keys_up; sf::Sprite up_key;
	if (!keys_up.loadFromFile("resources\\key_up.png")) std::cout << "Error loading the image ... " << std::endl;
	up_key.setTexture(keys_up); up_key.setPosition(sf::Vector2f(win_x * key_pos_x, win_y * 0.375)); up_key.setScale(key_scale_val, key_scale_val);


	sf::Texture keys_down; sf::Sprite down_key;
	if (!keys_down.loadFromFile("resources\\key_down.png")) std::cout << "Error loading the image ... " << std::endl;
	down_key.setTexture(keys_down); down_key.setPosition(sf::Vector2f(win_x * key_pos_x, win_y * 0.45)); down_key.setScale(key_scale_val, key_scale_val);

	sf::Text m_2; m_2.setFont(font); m_2.setString("Mode 2 - User Controlled Burn"); m_2.setCharacterSize(14); m_2.setPosition(win_x * key_pos_x, win_y * 0.55 - 25);
	sf::Text m_21; m_21.setFont(font); m_21.setString("Add Burn"); m_21.setCharacterSize(16); m_21.setPosition(win_x * key_pos_x + 75, win_y * 0.55 + 20);
	sf::Text m_22; m_22.setFont(font); m_22.setString("Remove Burn"); m_22.setCharacterSize(16); m_22.setPosition(win_x * key_pos_x + 75, win_y * 0.625 + 20);
	sf::Texture keys_l; sf::Sprite left_key;
	if (!keys_l.loadFromFile("resources\\key_left.png")) std::cout << "Error loading the image ... " << std::endl;
	left_key.setTexture(keys_l); left_key.setPosition(sf::Vector2f(win_x * key_pos_x, win_y * 0.55)); left_key.setScale(key_scale_val, key_scale_val);

	sf::Texture keys_r; sf::Sprite right_key;
	if (!keys_r.loadFromFile("resources\\key_right.png")) std::cout << "Error loading the image ... " << std::endl;
	right_key.setTexture(keys_r); right_key.setPosition(sf::Vector2f(win_x * key_pos_x, win_y * 0.625)); right_key.setScale(key_scale_val, key_scale_val);

	sf::Texture logo; sf::Sprite logo_orb;
	if (!logo.loadFromFile("resources\\orb.png")) std::cout << "Error loading the image ... " << std::endl;
	logo_orb.setTexture(logo); logo_orb.setPosition(sf::Vector2f(15, win_y * 0.05 + 15)); logo_orb.setScale(0.7, 0.7);

	while (window.isOpen()) {
		sf::Time deltaTime = clock.restart();
		float accumulator = deltaTime.asSeconds();
		float dt = std::min(accumulator, fixedTimeStep);

		while (accumulator > 0) {

			// Handle events
			sf::Event event;

			P_x = particles[0].pos.x;
			P_y = particles[0].pos.y;
			P_abs = sqrt(pow(P_x - win_x / 2, 2) + pow(P_y - win_y / 2, 2));
			// calcuate for tangency 
			float V_x = particles[0].vel.x;
			float V_y = particles[0].vel.y;
			V_abs = sqrt(pow(V_x, 2) + pow(V_y, 2));

			while (window.pollEvent(event))

			{
				if (event.type == sf::Event::Closed)
					window.close();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					window.close();




				// Manual input of control
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && i != 0 && i != 1)
				{
					// tangent burn
					particles[0].vel.x += 0.1f * V_x / V_abs;
					particles[0].vel.y += 0.1f * V_y / V_abs;
					i = 10;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && i != 0 && i != 1)
				{
					particles[0].vel.x -= 0.1f * V_x / V_abs;
					particles[0].vel.y -= 0.1f * V_y / V_abs;
					i = 10;
				}

				// Nornalize based on altitude

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					particles[0].vel = sf::Vector2f((P_y - win_y / 2) / P_abs * sqrt(6500 / P_abs), -(P_x - win_x / 2) / P_abs * sqrt(6500 / P_abs));
					i = 3;

				}


				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && i != 0 && i != 1)


				{
					if (Target_O < 350)
						Target_O++;
				}


				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && i != 0 && i != 1)


				{
					if (Target_O > 150)
						Target_O--;
				}


				// Change velocity of the first particle if T
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::T) && i == 3)
				{

					i = 0;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				{
					particles[0].pos = sf::Vector2f(win_x / 2 - 200, win_y / 2);
					particles[0].vel = sf::Vector2f((particles[0].pos.y - win_y / 2) / sqrt(pow(200, 2)) * sqrt(6500 / pow(200, 2)), -(particles[0].pos.x - win_x / 2) / sqrt(pow(200, 2)) * sqrt(6500 / sqrt(pow(200, 2))));
					i = 3;

				}

				// Zoom in

				if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
					{
						if (event.mouseWheelScroll.delta > 0)
							view.zoom(0.9f);
						else if (event.mouseWheelScroll.delta < 0)
							view.zoom(1.1f);

						window.setView(view);
					}
				}


			}

			// moved outside event loop to fig mouse bug

			if (i == 0)
			{
				P_abs_tf = P_abs;
				Dv_1 = 1.0001 * sqrt(6500 * (2 / P_abs_tf - 2 / (P_abs_tf + Target_O)));
				Dv_2 = sqrt(6500 / Target_O);
				particles[0].vel.x = Dv_1 * V_x / V_abs;
				particles[0].vel.y = Dv_1 * V_y / V_abs;
				i = 1;
			}
			if (-0.2 < P_abs - Target_O && P_abs - Target_O < 0.2 && i == 1)
			{
				particles[0].vel.x = Dv_2 * V_x / V_abs;
				particles[0].vel.y = Dv_2 * V_y / V_abs;
				i = 2;
			}

			if (-0.001 < P_abs - Target_O && P_abs - Target_O < 0.001 && i == 2)
			{
				particles[0].vel = sf::Vector2f((P_y - win_y / 2) / P_abs * sqrt(6500 / P_abs), -(P_x - win_x / 2) / P_abs * sqrt(6500 / P_abs));
				i = 3;

			}






			// Update physics
			for (auto& source : sources) {
				for (auto& particle : particles) {
					particle.update_physics(source, dt * timeScale);
				}
			}
			accumulator -= dt;
			// Render
		}

		window.clear();
		for (auto& source : sources)
		{
			source.render(window);
		}
		for (auto& particle : particles)
		{
			particle.render(window);
		}

		// ==================================== PRINT SOFTWARE INFO =============================================
		orbitsim.setFont(font_orb);
		orbitsim.setString("OrbitSim");
		orbitsim.setCharacterSize(50);
		orbitsim.setFillColor(sf::Color::White);
		orbitsim.setPosition(win_x * 0.05, win_y * 0.05);

		status_string.setFont(font);
		status_string.setString("Running okay // | See command line for instructions.");
		status_string.setCharacterSize(info_char_size + 4);
		status_string.setFillColor(sf::Color::White);
		status_string.setPosition(win_x * 0.05, win_y * 0.05 + 75);


		sf::VertexArray lines(sf::LinesStrip, 2);
		lines[0].position = sf::Vector2f(win_x * 0.05, win_y * 0.11);
		lines[1].position = sf::Vector2f(win_x * 1, win_y * 0.11);

		sf::RectangleShape rect(sf::Vector2f(450.f, 75.f));
		rect.setPosition(sf::Vector2f(info_x - 10, 650 - 10)); rect.setFillColor(sf::Color::Transparent); rect.setOutlineColor(sf::Color::White); rect.setOutlineThickness(2);

		sf::RectangleShape rect2(sf::Vector2f(450.f, 150.f));
		rect2.setPosition(sf::Vector2f(info_x - 10, 760 - 10)); rect2.setFillColor(sf::Color::Transparent); rect2.setOutlineColor(sf::Color::White); rect2.setOutlineThickness(2);
		// ==================================== END OF SOFTWARE INFO =============================================
		//===================================== START OF DATA PRINTING ==========================================
		std::string t_string;
		t_string = std::to_string(t);

		// Display Pos

		std::string P_abs_string;
		P_abs_string = std::to_string(P_abs);
		std::string pos_x_string;
		pos_x_string = std::to_string(P_x - win_x / 2);
		std::string pos_y_string;
		pos_y_string = std::to_string(P_y - win_y / 2);


		position1.setFont(font);
		position1.setString("position x: " + pos_x_string);
		position1.setCharacterSize(info_char_size);
		position1.setFillColor(sf::Color::Green);
		position1.setPosition(info_x, 800);

		position2.setFont(font);
		position2.setString("position y : " + pos_y_string);
		position2.setCharacterSize(info_char_size);
		position2.setFillColor(sf::Color::Green);
		position2.setPosition(info_x + 220, 800);



		// Display Time Period
		//position.setFont(font);
		//position.setString("Time: " + t_string + " s");
		//position.setCharacterSize(info_char_size);
		//position.setFillColor(sf::Color::Green);
		//position.setPosition(info_x, 800);

		// Display Vel

		float V_x = particles[0].vel.x;
		float V_y = particles[0].vel.y;
		V_abs = sqrt(pow(V_x, 2) + pow(V_y, 2));
		std::string V_abs_string;
		V_abs_string = std::to_string(V_abs);

		velocity.setFont(font);
		velocity.setString("velocity:  " + V_abs_string + " unit/frame");
		velocity.setCharacterSize(info_char_size);
		velocity.setFillColor(sf::Color::Green);
		velocity.setPosition(info_x, 820);
		// setup velocity indication 
		plus.setFont(font);
		plus.setString("Prograde");
		plus.setCharacterSize(info_char_size + 2);
		plus.setFillColor(sf::Color::Red);
		plus.setPosition(info_x + 300, 820);

		minus.setFont(font);
		minus.setString("Retrograde");
		minus.setCharacterSize(info_char_size + 2);
		minus.setFillColor(sf::Color::Red);
		minus.setPosition(info_x + 300, 820);

		// Display period
		float a = 0;
		a = 1 / ((2.0f / P_abs) - (pow(V_abs, 2) / 6500.0f));
		P_cond = ((2 * 3.1415926) / sqrt(6500.0f)) * pow(a, 1.5);
		std::string P_cond_string;
		P_cond_string = std::to_string(P_cond);
		period.setFont(font);
		period.setString("period:   " + P_cond_string + " frames");
		period.setCharacterSize(info_char_size);
		period.setFillColor(sf::Color::Green);
		period.setPosition(info_x, 840);

		// display vis_viva
		E_cond = pow(V_abs, 2) * 0.5f - 6500.0f / P_abs;
		std::string E_cond_string;
		E_cond_string = std::to_string(E_cond);

		vis_viva.setFont(font);
		vis_viva.setString("vis viva:  " + E_cond_string + " unit");
		vis_viva.setCharacterSize(info_char_size);
		if (E_cond < 0)
			vis_viva.setFillColor(sf::Color::Green);
		else
			vis_viva.setFillColor(sf::Color::Red);
		vis_viva.setPosition(info_x, 860);


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && i != 0 && i != 1)
			window.draw(plus);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && i != 0 && i != 1)
			window.draw(minus);

		// Display target Orb
		std::string Target_O_string;
		Target_O_string = std::to_string(Target_O);
		Target_Orbit.setFont(font);
		Target_Orbit.setString("Target Radius: " + Target_O_string);
		Target_Orbit.setCharacterSize(info_char_size + 4);
		Target_Orbit.setFillColor(sf::Color::Yellow);
		Target_Orbit.setPosition(info_x, 650);

		Radius.setFont(font);
		Radius.setString("Radius " + P_abs_string);
		Radius.setCharacterSize(info_char_size + 4);
		Radius.setFillColor(sf::Color::Green);
		Radius.setPosition(info_x, 680);

		// display tsf
		tsf.setFont(font);
		if (i == 0)
		{
			tsf.setString("Burn 1");
			tsf.setFillColor(sf::Color::Red);
		}
		if (i == 1)
		{
			tsf.setString("Burn 2 Pending");
			tsf.setFillColor(sf::Color::Red);
		}
		if (i >= 2 && i < 3)
		{
			tsf.setString("Trim adjustment Pending");
			tsf.setFillColor(sf::Color::Yellow);
		}

		if (i == 3)
		{
			tsf.setString("Normalized");
			tsf.setFillColor(sf::Color::Green);
		}

		if (i == 10)
		{
			tsf.setString("Cannot transfer; Normalize");
			tsf.setFillColor(sf::Color::Red);
		}
		tsf.setCharacterSize(30);

		tsf.setPosition(info_x, 760);
		// Other info to be printed


		//====================================== DRAW INFORMATION ==========================================
		window.draw(orbitsim);
		window.draw(status_string);
		window.draw(rect); window.draw(rect2);
		window.draw(logo_orb);
		window.draw(up_key); window.draw(down_key); window.draw(right_key); window.draw(left_key);
		window.draw(m_1); window.draw(m_2); window.draw(m_11); window.draw(m_12); window.draw(m_21); window.draw(m_22);
		window.draw(lines);
		window.draw(tsf);
		window.draw(velocity);
		window.draw(position2); window.draw(position1); window.draw(Radius); window.draw(Target_Orbit);
		window.draw(period);
		window.draw(vis_viva);
		//===================================== END OF DATA PRINTING ==========================================


		window.display();

		t += dt * timeScale;
	}

	return 0;
}