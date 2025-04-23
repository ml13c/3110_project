#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <queue>
#include <stack>
#include <climits> // needed for INT_MAX and running on Cell Machines
using namespace std;

// Function to build the flight graph from the file
map<string, vector<string>> buildGraph(const string& filename) {
    map<string, vector<string>> graph;
    ifstream file(filename);
    string line, currentOrigin;
    bool readingDestinations = false;

    while (getline(file, line)) {
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        line.erase(remove(line.begin(), line.end(), '\n'), line.end());
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) continue;

        if (line.rfind("From:", 0) == 0) {
            currentOrigin = line.substr(5);
            currentOrigin.erase(0, currentOrigin.find_first_not_of(" \t"));
            currentOrigin.erase(currentOrigin.find_last_not_of(" \t") + 1);
            graph[currentOrigin]; // Ensure the origin is in the graph
            readingDestinations = false;
        } else if (line.rfind("To", 0) == 0 && line.find(':') != string::npos) {
            string dest = line.substr(line.find(':') + 1);
            dest.erase(0, dest.find_first_not_of(" \t"));
            dest.erase(dest.find_last_not_of(" \t") + 1);
            if (!dest.empty()) {
                graph[currentOrigin].push_back(dest);
            }
            readingDestinations = true;
        } else if (readingDestinations) {
            string dest = line;
            dest.erase(0, dest.find_first_not_of(" \t"));
            dest.erase(dest.find_last_not_of(" \t") + 1);
            if (!dest.empty()) {
                graph[currentOrigin].push_back(dest);
            }
        }
    }

    return graph;
}

// Function to print the flight graph in the desired format
void printGraph(const map<string, vector<string>>& graph) {
    for (const auto& [origin, destinations] : graph) {
        cout << "From:  " << origin << "\nTo  :  ";
        bool first = true;
        for (const string& dest : destinations) {
            if (!first) cout << "\n       ";
            cout << dest;
            first = false;
        }
        cout << "\n";
    }
}

//q1
bool findShortestRoute(const map<string, vector<string>>& graph, const string& start, const string& goal, int maxConnections) {

    map<string, string> parent;
    map<string, int> distance;
    queue<string> q;

    q.push(start);
    distance[start] = 0;

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        if ((current == goal && distance[current]) <= maxConnections) {
            // Reconstruct and print the path
            stack<string> path;
            string temp = goal;
            while (temp != start) {
                path.push(temp);
                temp = parent[temp];
            }
            path.push(start);

            cout << "Path from " << start << " to " << goal << " with "
                 << distance[goal] << " connections:\n";
            while (!path.empty()) {
                cout << path.top();
                path.pop();
                if (!path.empty()) cout << " -> ";
            }
            cout << endl;
            return true;
        }

        if (graph.find(current) != graph.end()) {
            for (const auto& neighbor : graph.at(current)) {
                if (distance.find(neighbor) == distance.end()) {
                    parent[neighbor] = current;
                    distance[neighbor] = distance[current] + 1;
                    if (distance[neighbor] <= maxConnections) {
                        q.push(neighbor);
                    }
                }
            }
        }
    }

    cout << "No route from " << start << " to " << goal
         << " within " << maxConnections << " connections.\n";
    return false;
}
//q2
bool findRouteThrough(const map<string, vector<string>>& graph, const string& start, const string& goal, const string& mustPass1, const string& mustPass2) {
    queue<tuple<string, vector<string>, bool, bool>> q;
    q.push({start, {start}, start == mustPass1, start == mustPass2});
    set<tuple<string, bool, bool>> visited; // To avoid revisiting same

    while (!q.empty()) {
        auto [current, path, seenB, seenC] = q.front();
        q.pop();

        if (current == goal && seenB && seenC) {
            cout << "Shortest path from " << start << " to " << goal
                 << " through " << mustPass1 << " and " << mustPass2
                 << ":\n";
            for (size_t i = 0; i < path.size(); ++i) {
                cout << path[i];
                if (i < path.size() - 1) cout << " -> ";
            }
            cout << "\nConnections: " << path.size() - 1 << endl;
            return true;
        }

        if (graph.find(current) != graph.end()) {
            for (const string& neighbor : graph.at(current)) {
                // Avoid cycles by checking full state
                bool newSeenB = seenB || (neighbor == mustPass1);
                bool newSeenC = seenC || (neighbor == mustPass2);
                auto stateKey = make_tuple(neighbor, newSeenB, newSeenC);

                if (visited.find(stateKey) == visited.end()) {
                    visited.insert(stateKey);
                    vector<string> newPath = path;
                    newPath.push_back(neighbor);
                    q.push({neighbor, newPath, newSeenB, newSeenC});
                }
            }
        }
    }

    cout << "No valid path from " << start << " to " << goal
         << " through both " << mustPass1 << " and " << mustPass2 << ".\n";
    return false;
}

/*
Question 3: Find all possible cities that can be visited from a given city while
returning to the same city only once.
*/
map<string, int> cityIndex;
vector<vector<int>> buildAdjMatrix(const map<string, vector<string>>& graph, int& x) {
    int idx = 0;
    for (const auto& [city, _] : graph) {
        cityIndex[city] = idx++;
    }
    x = cityIndex.size();
    vector<vector<int>> adjMatrix(x, vector<int>(x, 0));

    for (const auto& [origin, destinations] : graph) {
        int u = cityIndex[origin];
        for (const string& dest : destinations) {
            int v = cityIndex[dest];
            adjMatrix[u][v] = 1;
        }
    }

    return adjMatrix;
}

// Utility function to check if a vertex can be added to the Hamiltonian path
bool isSafe(int v, const vector<vector<int>>& graph, vector<int>& path, int pos) {
    if (graph[path[pos - 1]][v] == 0) return false;
    for (int i = 0; i < pos; i++) if (path[i] == v) return false;
    return true;
}

// Recursive function to solve the Hamiltonian path problem
bool hamCycleUtil(const vector<vector<int>>& graph, vector<int>& path, int pos) {
    if (pos == path.size()) {
        if (graph[path[pos - 1]][path[0]] == 1) return true;
        else return false;
    }

    for (int v = 1; v < graph.size(); v++) {
        if (isSafe(v, graph, path, pos)) {
            path[pos] = v;
            if (hamCycleUtil(graph, path, pos + 1)) return true;
            path[pos] = -1;
        }
    }

    return false;
}

// Function to find a Hamiltonian cycle and print the solution
void findCycle(const vector<vector<int>>& graph) {
    vector<int> path(graph.size(), -1);
    path[0] = 0;

    if (!hamCycleUtil(graph, path, 1)) {
        cout << "No Hamiltonian cycle exists." << endl;
        return;
    }

    cout << "Hamiltonian cycle found: ";
    for (int cityIdx : path) {
        for (const auto& [city, idx] : cityIndex) {
            if (idx == cityIdx) {
                cout << city << " ";
                break;
            }
        }
    }
    cout << endl;
}

/* Question 4: Find the best city to meet at for three friends.
Search for all possible paths from the start city. This will be used to check if other paths from other 2 cities intersect 
with one another in nodes. The goal is to find a node that is common to all three cities. 
We want to find an end node for all paths to meet at. The goal is to keep it to a
minimum amount of travel between connections/nodes if there is one between the 3 total cities.
*/
void bfs(const map<string, vector<string>>& graph, const string& start, map<string, int>& distance, map<string, string>& parent) {
    queue<string> q;
    distance[start] = 0;
    q.push(start);

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        if (graph.find(current) == graph.end()) continue;

        const vector<string>& neighbors = graph.at(current);
        for (size_t i = 0; i < neighbors.size(); ++i) {
            const string& neighbor = neighbors[i];
            if (distance.find(neighbor) == distance.end()) {
                distance[neighbor] = distance[current] + 1;
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }
}

//function to help print path from a start city to destination
void printPath(const string& start, const string& destination, const map<string, string>& parent) {
    stack<string> path;
    string current = destination;

    while (current != start) {
        path.push(current);
        current = parent.at(current);
    }
    path.push(start);

    while (!path.empty()) {
        cout << path.top();
        path.pop();
        if (!path.empty()) cout << " -> ";
    }
    cout << "\n";
}

// function to find the best city to meet at for three friends
void findBestCity(const map<string, vector<string>>& graph, const string& cityA, const string& cityB, const string& cityC) {
    map<string, int> distA, distB, distC;
    map<string, string> parentA, parentB, parentC;

    bfs(graph, cityA, distA, parentA);
    bfs(graph, cityB, distB, parentB);
    bfs(graph, cityC, distC, parentC);

    string bestCity = "";
    int minTotalConnections = INT_MAX;

    map<string, vector<string>>::const_iterator it;
    for (it = graph.begin(); it != graph.end(); ++it) {
        const string& city = it->first;

        if (city == cityA || city == cityB || city == cityC) {
            continue;
        }

        if (distA.count(city) > 0 && distB.count(city) > 0 && distC.count(city) > 0) {
            int total = distA[city] + distB[city] + distC[city];
            if (total < minTotalConnections) {
                minTotalConnections = total;
                bestCity = city;
            }
        }
    }

    if (bestCity == "") {
        cout << "No common meeting city found for " << cityA << ", " << cityB << ", and " << cityC << ".\n";
        return;
    }

    cout << "You three should meet at: " << bestCity << "\n\n";

    cout << "Route for " << cityA << ":\n";
    printPath(cityA, bestCity, parentA);
    cout << "Connections: " << distA[bestCity] << "\n\n";

    cout << "Route for " << cityB << ":\n";
    printPath(cityB, bestCity, parentB);
    cout << "Connections: " << distB[bestCity] << "\n\n";

    cout << "Route for " << cityC << ":\n";
    printPath(cityC, bestCity, parentC);
    cout << "Connections: " << distC[bestCity] << "\n\n";

    cout << "Total connections required: " << minTotalConnections << "\n";
}
int main() {
    string filename = "flight.txt";
    int choice,  maxConnections, x;
    string fromCity, toCity, city1, city2;
    map<string, vector<string>> graph = buildGraph(filename);
    vector<vector<int>> adjMatrix = buildAdjMatrix(graph, x);

    cout << "Select what you want to do:\n";
    cout << "1. Find the shortest route\n";
    cout << "2. Find route through two cities\n";
    cout << "3. Visit all possible cities from Origin City\n";
    cout << "4. 3 cities friends, minumum connections between to meet at same city\n";
    cout << "5. Print the full graph\n";
    cin >> choice;
    cin.ignore();
    switch(choice)
    {
        case 1:
            cout << "Input two cities to find path\n";
            cout << "From:";
            getline(cin, fromCity);
            cout << "To:";
            getline(cin, toCity);
            cout << "Maximum number of connections?\n";
            cin >> maxConnections;
            cout << "Finding the shortest route...\n";
            findShortestRoute(graph, fromCity, toCity, maxConnections);
            break;
        case 2:
            cout << "Input two cities to find path\n";
            cout << "From:";
            getline(cin, fromCity);
            cout << "To:";
            getline(cin, toCity);
            cout << "Through which cities(1)?\n";
            getline(cin, city1);
            cout << "Through which cities(2)?\n";
            getline(cin, city2);
            cout << "Finding route(if possible)...\n";
            findRouteThrough(graph, fromCity, toCity, city1, city2);
            break;
        case 3:
            cout << "Select City you want to start off and end up back in\n";
            getline(cin, fromCity);
            cout << "Finding all possible cities...\n";
            findCycle(adjMatrix);
            break;
        case 4:
            cout << "Input three cities to find min path for each\n";
            cout << "Your City: ";
            getline(cin, fromCity);
            cout << "Friend 1 City: ";
            getline(cin, city1);
            cout << "Friend 2 City: ";
            getline(cin, city2);
            cout << "Finding besty city and route(...\n";
            findBestCity(graph, fromCity, city1, city2);
            break;
        case 5:
            cout << "Printing the full graph...\n";
            printGraph(graph);
            cout << graph.size() << " cities in total.\n";
            cout << "Done.\n";
            break;
        default:
            cout << "Invalid option.\n";
            return 1;
    }
    return 0;
}
