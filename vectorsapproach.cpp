#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <queue>
#include <stack>

using namespace std;

// Function to build the flight graph from the file
map<string, vector<string>> buildGraph(const string& filename) {
    map<string, vector<string>> graph;
    ifstream file(filename);
    string line, currentOrigin;
    bool readingDestinations = false;

    while (getline(file, line)) {
        // Trim whitespace
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
void printGraph(const map<string, set<string>>& graph) {
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
bool findShortestRoute(const map<string, vector<string>>& graph,
    const string& start,
    const string& goal,
    int maxConnections) {

    map<string, string> parent;
    map<string, int> distance;
    queue<string> q;

    q.push(start);
    distance[start] = 0;

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        if (current == goal && distance[current] <= maxConnections) {
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

bool findRouteThrough(const map<string, vector<string>>& graph,
    const string& start,
    const string& goal,
    const string& mustPass1,
    const string& mustPass2) {
    queue<tuple<string, vector<string>, bool, bool>> q;
    q.push({start, {start}, start == mustPass1, start == mustPass2});
    set<tuple<string, bool, bool>> visited; // To avoid revisiting same state

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

int main() {
    string filename = "flight.txt";
    int choice,  maxConnections;
    string fromCity, toCity, city1, city2;
    map<string, vector<string>> graph = buildGraph(filename);

    cout << "Select what you want to do:\n";
    cout << "1. Find the shortest route\n";
    cout << "2. Find route through two cities\n";
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
        case 5:
            cout << "Printing the full graph...\n";
            // printGraph(graph);
            break;
        default:
            cout << "Invalid option.\n";
            return 1;
    }
    return 0;
}
