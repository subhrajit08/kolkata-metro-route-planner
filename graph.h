#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <utility>

using namespace std;

// Represents the metro network as a weighted, undirected graph
// using an adjacency list. Each station is assigned an integer ID
// the first time it is seen while loading the data file.
class Graph {
public:
    Graph();

    // Loads edges from a CSV-style file: StationA,StationB,TimeInMinutes
    // Lines starting with '#' or blank lines are ignored.
    // Returns false if the file could not be opened.
    bool loadFromFile(const string& filename);

    // Registers a station name and returns its ID (creates a new ID
    // if the station hasn't been seen before).
    int addStation(const string& name);

    // Adds a bidirectional edge between two stations with the given
    // travel time in minutes.
    void addEdge(const string& from, const string& to, int weightMinutes);

    // Lookup helpers. getStationId returns -1 if not found.
    int getStationId(const string& name) const;
    string getStationName(int id) const;
    bool isValidId(int id) const;

    int stationCount() const;
    void listStations() const;

    // BFS: shortest path measured in number of stops (unweighted).
    // Returns the path as a list of station IDs (empty if unreachable).
    // totalStops is set to the number of edges traversed.
    vector<int> bfsShortestPath(int srcId, int destId, int& totalStops) const;

    // Dijkstra: shortest path measured in total travel time (minutes).
    // Returns the path as a list of station IDs (empty if unreachable).
    // totalTime is set to the cumulative travel time in minutes.
    vector<int> dijkstraShortestPath(int srcId, int destId, int& totalTime) const;

private:
    unordered_map<string, int> nameToId;
    vector<string> idToName;
    vector<list<pair<int, int>>> adjList; // neighborId, weight(minutes)

    // Reconstructs a path from a parent[] array produced by BFS/Dijkstra.
    vector<int> reconstructPath(const vector<int>& parent, int srcId, int destId) const;

    // Case-insensitive trim/normalize helper used during file parsing/lookup.
    static string trim(const string& s);
};

#endif // GRAPH_H
