#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <set>
#include <stack>

using namespace std;

unordered_map<string, vector<string>> buildGraph(const string& filename) {
    unordered_map<string, vector<string>> graph;
    ifstream file(filename);
    string line, fromCity;
    
    while (getline(file, line)) {
        // Trim whitespace
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        if (line.rfind("From:", 0) == 0) {
            fromCity = line.substr(6);  // Skip "From: "
            fromCity.erase(0, fromCity.find_first_not_of(" "));
        } else if (line.rfind("To", 0) == 0 || line.find(':') != string::npos) {
            continue; // skip the "To  :" line
        } else if (!line.empty()) {
            string toCity = line;
            toCity.erase(0, toCity.find_first_not_of(" "));
            graph[fromCity].push_back(toCity);
        }
    }

    return graph;
}

void printGraph(const unordered_map<string, vector<string>>& graph) {
    for (const auto& pair : graph) {
        cout << pair.first << " -> ";
        for (const auto& dest : pair.second) {
            cout << dest << ", ";
        }
        cout << endl;
    }
}

bool findShortestRoute(const unordered_map<string, vector<string>>& graph,
    const string& start,
    const string& goal,
    int maxConnections) {
unordered_map<string, string> parent;
unordered_map<string, int> distance;
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

int main() {
    string filename = "flight.txt";
    auto graph = buildGraph(filename);
    
    // Optional: print the full graph
    // printGraph(graph);

    // Example test: Can you get from Moscow to Singapore in <3 connections?
    string startCity = "Moscow, Russia";
    string goalCity = "Milan, Italy";
    int maxConnections = 3;

    cout << "\nChecking if you can fly from " << startCity 
         << " to " << goalCity << " with fewer than " 
         << maxConnections << " connections...\n";

    if (!findShortestRoute(graph, startCity, goalCity, maxConnections)) {
        cout << "No route found within " << maxConnections << " connections.\n";
    }

    return 0;
}
