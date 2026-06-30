# Kolkata Metro Route Planner

A C++ command-line application that simulates a metro network inspired by
the **Kolkata Metro** (50 stations across the Blue, Green, and Purple lines
plus a north extension branch) and finds the shortest route between any two
stations using **BFS** or **Dijkstra's algorithm**.

---

## Features

- Graph represented via an **adjacency list**
- **BFS** for unweighted shortest path (minimum number of stops)
- **Dijkstra's algorithm** for weighted shortest path (minimum travel time)
- Station input by **name or ID**
- Output includes:
  - Full route with station names
  - Total stops (BFS) or total travel time in minutes (Dijkstra)
  - Computation time in milliseconds
- Network data loaded from an external file: `stations.txt`
- **Modular structure**: `graph.h` / `graph.cpp` for core graph logic,
  `main.cpp` for the CLI

---

## File Structure

```
metro-route-planner/
├── graph.h          # Graph class declaration
├── graph.cpp         # BFS, Dijkstra, file loading, station lookups
├── main.cpp          # CLI menu and user interaction
├── stations.txt       # Network data (edges: StationA,StationB,TimeInMinutes)
└── README.md
```

---

## Building

Requires a C++17-capable compiler.

```bash
g++ -std=c++17 -O2 -Wall -o metro main.cpp graph.cpp
```

## Running

```bash
./metro
```

Make sure `stations.txt` is in the same directory you run the binary from
(it's loaded by relative path at startup).

---

## Network Data Format

`stations.txt` uses a simple CSV-style edge list:

```
StationA,StationB,TimeInMinutes
```

Lines starting with `#` are treated as comments. Each line creates a
bidirectional edge with the given travel time in minutes. Station IDs are
assigned automatically, in the order each name is first encountered.

---

## Example Session

```
1. Find route (BFS - fewest stops)
2. Find route (Dijkstra - fastest time)
3. List all stations
4. Exit
Choose an option: 2
Enter source station (name or ID): Joka
Enter destination station (name or ID): Salt Lake Sector V

--- Dijkstra Route (fastest time) ---
Joka -> Thakurpukur -> Sakherbazar -> Behala Chowrasta -> Behala Bazar ->
Taratala -> Majerhat -> Jatin Das Park -> Netaji Bhavan -> Rabindra Sadan ->
Maidan -> Park Street -> Esplanade -> Sealdah -> Phoolbagan -> Salt Lake Stadium ->
Bengal Chemical -> City Centre -> Central Park -> Karunamoyee -> Salt Lake Sector V
Total travel time: 42 minutes
Computation time: 0.0037 ms
```

---

## Possible Extensions

- Add interchange/line metadata and report line changes in the route
- Support fare calculation based on distance/stops
- Add a simple graphical (or web) front end on top of the same `Graph` class
- Allow loading alternate network files via a command-line argument
