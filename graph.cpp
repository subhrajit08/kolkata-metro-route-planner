#include "graph.h"
#include <fstream>
#include <sstream>
#include <queue>
#include <climits>
#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;

Graph::Graph() {}

string Graph::trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

int Graph::addStation(const string& name) {
    string clean = trim(name);
    auto it = nameToId.find(clean);
    if (it != nameToId.end()) {
        return it->second;
    }
    int newId = static_cast<int>(idToName.size());
    nameToId[clean] = newId;
    idToName.push_back(clean);
    adjList.emplace_back();
    return newId;
}

void Graph::addEdge(const string& from, const string& to, int weightMinutes) {
    int a = addStation(from);
    int b = addStation(to);
    adjList[a].emplace_back(b, weightMinutes);
    adjList[b].emplace_back(a, weightMinutes); // undirected: trains run both ways
}

bool Graph::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open data file \"" << filename << "\"\n";
        return false;
    }

    string line;
    int lineNum = 0;
    while (getline(file, line)) {
        lineNum++;
        string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#') continue; // skip comments/blank lines

        stringstream ss(trimmed);
        string stationA, stationB, weightStr;
        if (!getline(ss, stationA, ',')) continue;
        if (!getline(ss, stationB, ',')) continue;
        if (!getline(ss, weightStr, ',')) continue;

        try {
            int weight = stoi(trim(weightStr));
            addEdge(stationA, stationB, weight);
        } catch (const exception&) {
            cerr << "Warning: skipping malformed line " << lineNum << " in " << filename << "\n";
        }
    }
    file.close();
    return true;
}

int Graph::getStationId(const string& name) const {
    string clean = trim(name);
    // Case-insensitive match
    string lowerClean = clean;
    transform(lowerClean.begin(), lowerClean.end(), lowerClean.begin(), ::tolower);

    for (const auto& pair : nameToId) {
        string lowerName = pair.first;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        if (lowerName == lowerClean) return pair.second;
    }
    return -1;
}

string Graph::getStationName(int id) const {
    if (id < 0 || id >= static_cast<int>(idToName.size())) return "";
    return idToName[id];
}

bool Graph::isValidId(int id) const {
    return id >= 0 && id < static_cast<int>(idToName.size());
}

int Graph::stationCount() const {
    return static_cast<int>(idToName.size());
}

void Graph::listStations() const {
    for (int i = 0; i < static_cast<int>(idToName.size()); i++) {
        cout << "  [" << i << "] " << idToName[i] << "\n";
    }
}

vector<int> Graph::reconstructPath(const vector<int>& parent, int srcId, int destId) const {
    vector<int> path;
    if (parent[destId] == -2) return path; // unreachable marker

    int current = destId;
    while (current != -1) {
        path.push_back(current);
        if (current == srcId) break;
        current = parent[current];
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<int> Graph::bfsShortestPath(int srcId, int destId, int& totalStops) const {
    totalStops = 0;
    int n = stationCount();
    vector<bool> visited(n, false);
    vector<int> parent(n, -2); // -2 = unreached, -1 = root

    queue<int> q;
    q.push(srcId);
    visited[srcId] = true;
    parent[srcId] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (u == destId) break;

        for (const auto& edge : adjList[u]) {
            int v = edge.first;
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    vector<int> path = reconstructPath(parent, srcId, destId);
    if (!path.empty()) totalStops = static_cast<int>(path.size()) - 1;
    return path;
}

vector<int> Graph::dijkstraShortestPath(int srcId, int destId, int& totalTime) const {
    totalTime = 0;
    int n = stationCount();
    vector<int> dist(n, INT_MAX);
    vector<int> parent(n, -2);

    // min-heap of (distance, stationId)
    priority_queue<pair<int, int>,
                     vector<pair<int, int>>,
                     greater<pair<int, int>>> pq;

    dist[srcId] = 0;
    parent[srcId] = -1;
    pq.push({0, srcId});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue; // stale entry
        if (u == destId) break;

        for (const auto& edge : adjList[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    vector<int> path = reconstructPath(parent, srcId, destId);
    if (!path.empty() && dist[destId] != INT_MAX) totalTime = dist[destId];
    return path;
}
