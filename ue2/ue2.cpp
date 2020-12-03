// ue2.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

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
float sizeMultiplier = 70;
int randomCnt = 10;
float randomMin=0, randomMax=14;
int mode = 0;

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
    //auto stopQs = std::chrono::high_resolution_clock::now();
    //std::cout << "Time for Quicksort: " << std::chrono::duration_cast<std::chrono::microseconds>(stopQs - startQs).count() << std::endl << std::endl;
    

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

        int x, y;

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

    Andrews a;
    
    hull = a.returnConvexHull(list);

    Draw();
    /*for (int i = 0; i < hull.size(); i++)
    {
        std::cout << hull[i].x << " " << hull[i].y << std::endl;
    }*/
    
    std::cout << std::endl;

    /*a.InitSteppable(list);
    while (a.HasNextStep())
    {
        hull = a.Step();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    for (int i = 0; i < hull.size(); i++)
    {
        std::cout << hull[i].x << " " << hull[i].y << std::endl;
    }*/
}

void Draw()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "Andrews");
    Andrews a;
    a.InitSteppable(list);

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
            point.setPosition(sf::Vector2f(coord.x * sizeMultiplier, coord.y * sizeMultiplier));
            point.setRadius(radius);
            window.draw(point);
        }

        for (int i = 0; i < hull.size(); i++)
        {
            if (i < hull.size() - 1)
            {
                sf::Vertex line[2];
                line[0].position = sf::Vector2f((hull[i].x * sizeMultiplier) + radius, (hull[i].y * sizeMultiplier) + radius);
                line[0].color = sf::Color::Red;
                line[1].position = sf::Vector2f((hull[i + 1].x * sizeMultiplier) + radius, (hull[i + 1].y * sizeMultiplier) + radius);
                line[1].color = sf::Color::Red;

                window.draw(line, 2, sf::Lines);
            }
            else if (!a.HasNextStep())
            {
                sf::Vertex line[2];
                line[0].position = sf::Vector2f((hull[i].x * sizeMultiplier) + radius, (hull[i].y * sizeMultiplier) + radius);
                line[0].color = sf::Color::Red;
                line[1].position = sf::Vector2f((hull[0].x * sizeMultiplier) + radius, (hull[0].y * sizeMultiplier) + radius);
                line[1].color = sf::Color::Red;

                window.draw(line, 2, sf::Lines);
            }
        }

        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
