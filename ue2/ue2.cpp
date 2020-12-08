#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "Point.h"
#include "Andrews.h"

void Draw();

std::vector<Point> list;
std::vector<Point> hull;
float radius = 5.0;
float sizeMultiplier = 50;
int randomCnt = 10;
float randomMin = -10, randomMax = 10;
int mode = 0;
Andrews a;

char* getOption(char** start, char** end, const std::string& option)
{
	char** iterator = std::find(start, end, option);

	if (iterator != end && ++iterator != end)
	{
		return *iterator;
	}
	return nullptr;

}

bool OptionExists(char** start, char** end, const std::string& option)
{
	return std::find(start, end, option) != end;
}

int main(int argc, char* argv[])
{
		char* filePath = getOption(argv, argv + argc, "--file");
		char* newRandomCnt = getOption(argv, argv + argc, "--random");
		if (newRandomCnt != nullptr)
		{
			randomCnt = atoi(newRandomCnt);
		}
		char* newMode = getOption(argv, argv + argc, "--mode");
		if (newMode != nullptr)
		{
			mode = atoi(newMode);
		}

		if (filePath != nullptr)
		{
			std::ifstream inputfile(filePath);
			std::string line;

			float x, y;

			while (std::getline(inputfile, line))
			{
				std::istringstream line_stream(line);

				if (line_stream >> x && line_stream.get() == ',' && line_stream >> y)
				{
					list.push_back(Point(x, y));
				}
			}
		}

		if (list.empty())
		{
			std::random_device rnd;
			std::mt19937 gen(rnd());
			std::uniform_real_distribution<> dis(randomMin, randomMax);

			for (int i = 0; i < randomCnt; i++)
			{
				list.push_back(Point(dis(gen), dis(gen)));
			}
		}


		if (mode == 0)
		{
			hull = a.returnConvexHull(list);
			Draw();
		}
		else if (mode == 1)
		{
			a.InitSteppable(list);
			Draw();
		}
		else if (mode == 2)
		{
			hull = a.returnConvexHull_Multithreaded(list);
			Draw();
		}
		else if (mode == 3)
		{

			a.returnConvexHull(list);
			a.returnConvexHull_Multithreaded(list);
		}

	std::cout << std::endl;
}

void Draw()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(1024, 1024), "Andrews");



	// run the main loop
	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (a.HasNextStep())
		{
			hull = a.Step();
		}

		// draw it
		window.clear();

		for (auto coord : list)
		{
			sf::CircleShape point;
			point.setPosition(sf::Vector2f((coord.x + 10) * sizeMultiplier, (coord.y + 10) * sizeMultiplier));
			point.setRadius(radius);
			window.draw(point);
		}

		for (int i = 0; i < hull.size(); i++)
		{
			if (i < hull.size() - 1)
			{
				sf::Vertex line[2];
				line[0].position = sf::Vector2f(((hull[i].x + 10) * sizeMultiplier) + radius, ((hull[i].y + 10) * sizeMultiplier) + radius);
				line[0].color = sf::Color::Red;
				line[1].position = sf::Vector2f(((hull[i + 1].x + 10) * sizeMultiplier) + radius, ((hull[i + 1].y + 10) * sizeMultiplier) + radius);
				line[1].color = sf::Color::Red;

				window.draw(line, 2, sf::Lines);
			}
			else if (!a.HasNextStep())
			{
				sf::Vertex line[2];
				line[0].position = sf::Vector2f(((hull[i].x + 10) * sizeMultiplier) + radius, ((hull[i].y + 10) * sizeMultiplier) + radius);
				line[0].color = sf::Color::Red;
				line[1].position = sf::Vector2f(((hull[0].x + 10) * sizeMultiplier) + radius, ((hull[0].y + 10) * sizeMultiplier) + radius);
				line[1].color = sf::Color::Red;

				window.draw(line, 2, sf::Lines);
			}
		}

		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
