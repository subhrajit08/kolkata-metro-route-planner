#include "graph.h"
#include <iostream>
#include <chrono>
#include <limits>
#include <algorithm>
#include <cctype>

using namespace std;

void printPath(const Graph& g, const vector<int>& path) {
    for (size_t i = 0; i < path.size(); i++) {
        cout << g.getStationName(path[i]);
        if (i + 1 < path.size()) cout << "  ->  ";
    }
    cout << "\n";
}

// Accepts either a station name or a numeric ID and resolves it to an ID.
int resolveStation(const Graph& g, const string& input) {
    // Try parsing as a number first
    bool isNumeric = !input.empty() &&
        all_of(input.begin(), input.end(), [](unsigned char c) { return isdigit(c); });

    if (isNumeric) {
        int id = stoi(input);
        if (g.isValidId(id)) return id;
        return -1;
    }
    return g.getStationId(input);
}

void printMenu() {
    cout << "\n========================================\n";
    cout << "   KOLKATA METRO ROUTE PLANNER\n";
    cout << "========================================\n";
    cout << "1. Find route (BFS - fewest stops)\n";
    cout << "2. Find route (Dijkstra - fastest time)\n";
    cout << "3. List all stations\n";
    cout << "4. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    Graph metro;
    cout << "Loading metro network from stations.txt...\n";
    if (!metro.loadFromFile("stations.txt")) {
        cerr << "Failed to load station data. Exiting.\n";
        return 1;
    }
    cout << "Loaded " << metro.stationCount() << " stations.\n";

    while (true) {
        printMenu();
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 4) {
            cout << "Thanks for riding the Kolkata Metro. Goodbye!\n";
            break;
        }

        if (choice == 3) {
            cout << "\nAvailable stations (name or ID can be used):\n";
            metro.listStations();
            continue;
        }

        if (choice != 1 && choice != 2) {
            cout << "Invalid option. Please choose 1-4.\n";
            continue;
        }

        string srcInput, destInput;
        cout << "Enter source station (name or ID): ";
        getline(cin, srcInput);
        cout << "Enter destination station (name or ID): ";
        getline(cin, destInput);

        int srcId = resolveStation(metro, srcInput);
        int destId = resolveStation(metro, destInput);

        if (srcId == -1 || destId == -1) {
            cout << "One or both station names/IDs were not recognized. "
                         "Use option 3 to see the full list.\n";
            continue;
        }

        if (srcId == destId) {
            cout << "Source and destination are the same station.\n";
            continue;
        }

        auto start = chrono::high_resolution_clock::now();

        vector<int> path;
        if (choice == 1) {
            int totalStops = 0;
            path = metro.bfsShortestPath(srcId, destId, totalStops);
            auto end = chrono::high_resolution_clock::now();
            double ms = chrono::duration<double, milli>(end - start).count();

            if (path.empty()) {
                cout << "No route found between these stations.\n";
            } else {
                cout << "\n--- BFS Route (fewest stops) ---\n";
                printPath(metro, path);
                cout << "Total stops: " << totalStops << "\n";
                cout << "Computation time: " << ms << " ms\n";
            }
        } else {
            int totalTime = 0;
            path = metro.dijkstraShortestPath(srcId, destId, totalTime);
            auto end = chrono::high_resolution_clock::now();
            double ms = chrono::duration<double, milli>(end - start).count();

            if (path.empty()) {
                cout << "No route found between these stations.\n";
            } else {
                cout << "\n--- Dijkstra Route (fastest time) ---\n";
                printPath(metro, path);
                cout << "Total travel time: " << totalTime << " minutes\n";
                cout << "Computation time: " << ms << " ms\n";
            }
        }
    }

    return 0;
}
