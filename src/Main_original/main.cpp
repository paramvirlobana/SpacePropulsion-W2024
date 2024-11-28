#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>
#include <cmath>


class GravitySource
{
	sf::Vector2f pos;
	float strength;
	sf::CircleShape s;
	sf::Texture texture_earth;
	
public:
	GravitySource(float pos_x, float pos_y, float strength)
	{
		pos.x = pos_x;
		pos.y = pos_y;
		this->strength = strength;


		texture_earth.loadFromFile("resources/earth_texture.jpg");

		s.setTexture(&texture_earth);
		s.setPosition(pos);
		//s.setFillColor(sf::Color::Red);
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


class Particle
{
	sf::CircleShape s;
	std::vector<sf::Vector2f> path;

public:
	sf::Vector2f pos;
	sf::Vector2f vel;
	Particle(float pos_x, float pos_y, float vel_x, float vel_y)
	{
		pos.x = pos_x;
		pos.y = pos_y;
		vel.x = vel_x;
		vel.y = vel_y;


		s.setPosition(pos);
		s.setFillColor(sf::Color::White);
		s.setRadius(6);


	}

	void render(sf::RenderWindow& wind)
	{
		// Render the current position
		s.setPosition(pos);
		wind.draw(s);

		// Render the path as dotted lines
		for (size_t i = 1; i < path.size(); ++i)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(path[i - 1], sf::Color::White),
				sf::Vertex(path[i], sf::Color::White)
			};
			wind.draw(line, 2, sf::Lines);
		}
	}

	void set_color(sf::Color col)
	{
		s.setFillColor(col);
	}

	void update_physics(GravitySource& s)
	{
		float distance_x = s.get_pos().x - pos.x;
		float distance_y = s.get_pos().y - pos.y;

		float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

		float inverse_distance = 1.f / distance;

		float normalized_x = inverse_distance * distance_x;
		float normalized_y = inverse_distance * distance_y;

		float inverse_square_dropoff = inverse_distance * inverse_distance;;

		float acceleration_x = normalized_x * s.get_strength() * inverse_square_dropoff;
		float acceleration_y = normalized_y * s.get_strength() * inverse_square_dropoff;

		vel.x += acceleration_x;
		vel.y += acceleration_y;


		pos.x += vel.x;
		pos.y += vel.y;


		path.push_back(pos);
		// Limit the size of the path to prevent it from growing indefinitely
		if (path.size() > 600)
			path.erase(path.begin()); // Remove the oldest position
	}

};



int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 1000), "Your Satellite");
	window.setFramerateLimit(120);
	sf::View view = window.getView();

	// Initialize sources and particles
	std::vector<GravitySource> sources;
	sources.push_back(GravitySource(800, 500, 6500));

	std::vector<Particle> particles;
	particles.push_back(Particle(600, 500, 0, sqrt(6500 / 200)));
	//particles.push_back(Particle(500, 800, 4, 0));


	// setup font
		// font check
		sf::Font font;
		if (!font.loadFromFile("arial.ttf")) {
			// Error handling if font loading fails
			std::cerr << "Failed to load font file!\n";
			return EXIT_FAILURE;
		}
		// setup output
		sf::Text velocity;
		sf::Text position;
		sf::Text period;
		sf::Text vis_viva;
		sf::Text plus;
		sf::Text minus;
	// setup t

		// Display value 
			float t = 1.f;
			float V_abs = 0;
			float P_abs = 0;
			float E_cond = 0;
			float P_cond = 0;

	while (window.isOpen())
	{
		

		
		

		// Handle events
		sf::Event event;
		while (window.pollEvent(event))

		{
			
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			// calcuate for tangency 
			float V_x = particles[0].vel.x;
			float V_y = particles[0].vel.y;
			V_abs = sqrt(pow(V_x,2) + pow(V_y, 2));
			


			// Change velocity of the first particle if up or down arrow keys are pressed
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
				// tangent burn
				particles[0].vel.x += 0.1f * V_x / V_abs;
				particles[0].vel.y += 0.1f * V_y / V_abs;
				}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				particles[0].vel.x -= 0.1f * V_x / V_abs;
				particles[0].vel.y -= 0.1f * V_y / V_abs;
			}
			/*
				// Change velocity of the second particle if left or right arrow keys are pressed
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				particles[1].vel.x -= 0.1f;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				particles[1].vel.x += 0.1f;
			*/
		
			

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				particles[0].pos = sf::Vector2f(600, 500);
				particles[0].vel = sf::Vector2f(0, sqrt(6500/200));
				//particles[1].pos = sf::Vector2f(500, 800);
				//particles[1].vel = sf::Vector2f(4, 0);
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

		// Update physics
		for (auto& source : sources)
		{
			for (auto& particle : particles)
			{
				particle.update_physics(source);
			}
		}

		// Render
		
		window.clear();
		for (auto& source : sources)
		{
			source.render(window);
		}
		for (auto& particle : particles)
		{
			particle.render(window);
		}

		// Display readings 

		std::string t_string;
		t_string = std::to_string(t);

		// Display Pos

		float P_x = particles[0].pos.x;
		float P_y = particles[0].pos.y;
		P_abs = sqrt(pow(P_x-800.f, 2) + pow(P_y - 500.f, 2));
		std::string P_abs_string;
		P_abs_string = std::to_string(P_abs);

		position.setFont(font);
		position.setString("position: " + P_abs_string + " unit");
		position.setCharacterSize(10);
		position.setFillColor(sf::Color::Green);
		position.setPosition(1400, 800);

		// Display Vel

		float V_x = particles[0].vel.x;
		float V_y = particles[0].vel.y;
		V_abs = sqrt(pow(V_x, 2) + pow(V_y, 2));
		std::string V_abs_string;
		V_abs_string = std::to_string(V_abs);

		velocity.setFont(font);
		velocity.setString("velocity:  " + V_abs_string + " unit/frame" );
		velocity.setCharacterSize(10);
		velocity.setFillColor(sf::Color::Green);
		velocity.setPosition(1400, 820);
			// setup velocity indication 
			plus.setFont(font);
			plus.setString("Prograde");
			plus.setCharacterSize(12);
			plus.setFillColor(sf::Color::Red);
			plus.setPosition(1340, 819);
			 
			minus.setFont(font);
			minus.setString("Retrograde");
			minus.setCharacterSize(12);
			minus.setFillColor(sf::Color::Red);
			minus.setPosition(1340, 819);
		// Display period

		float a = 0;
		a = 1/((2.0f / P_abs) - (pow(V_abs, 2) / 6500.0f)); 
		P_cond = ((2 * 3.1415926) / sqrt(6500.0f)) * pow(a,1.5);
		std::string P_cond_string;
		P_cond_string = std::to_string(P_cond);
		period.setFont(font);
		period.setString("period:   " + P_cond_string + " frames");
		period.setCharacterSize(10);
		period.setFillColor(sf::Color::Green);
		period.setPosition(1400, 840);

		// display vis_viva

		E_cond = pow(V_abs, 2) * 0.5f - 6500.0f/P_abs;
		std::string E_cond_string;
		E_cond_string = std::to_string(E_cond);

		vis_viva.setFont(font);
		vis_viva.setString("vis viva:  " + E_cond_string + " unit");
		vis_viva.setCharacterSize(10);
		if (E_cond<0)
			vis_viva.setFillColor(sf::Color::Green);
		else 
			vis_viva.setFillColor(sf::Color::Red);
		vis_viva.setPosition(1400, 860);


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			window.draw(plus);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			window.draw(minus);
		

		window.draw(velocity);
		window.draw(position);
		window.draw(period);
		window.draw(vis_viva);

		window.display();

		t = t + 1.f;
	}

	return 0;
}