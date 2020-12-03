// ue2.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <chrono>
#include <vector>

#include "Point.h"
#include "Andrews.h"

int main()
{
    //auto stopQs = std::chrono::high_resolution_clock::now();
    //std::cout << "Time for Quicksort: " << std::chrono::duration_cast<std::chrono::microseconds>(stopQs - startQs).count() << std::endl << std::endl;
    
    std::vector<Point> list = { 
                        Point(2.0f,4.0f), 
                        Point(8.0f,8.0f), 
                        Point(12.0f,2.0f), 
                        Point(5.0f,1.0f), 
                        Point(4.0f,5.0f), 
                        Point(6.0f,3.5f), 
                        Point(10.0f,3.0f) 
                    };

    Andrews a;
    std::vector<Point> hull = a.returnConvexHull(list);
    for (int i = 0; i < hull.size(); i++)
    {
        std::cout << hull[i].x << " " << hull[i].y << std::endl;
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
