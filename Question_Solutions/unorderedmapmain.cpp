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
    string line;
    string currentOrigin;
    bool readingDestinations = false;

    while (getline(file, line)) {
        // Clean up line endings and trim leading/trailing spaces
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        line.erase(remove(line.begin(), line.end(), '\n'), line.end());
        line.erase(0, line.find_first_not_of(" \t"));

        if (line.empty()) continue;

        // Check for "From:"
        if (line.rfind("From:", 0) == 0) {
            currentOrigin = line.substr(5);
            currentOrigin.erase(0, currentOrigin.find_first_not_of(" \t"));
            readingDestinations = false;
        }
        // Check for "To:" and extract first destination (if any)
        else if (line.rfind("To", 0) == 0 && line.find(':') != string::npos) {
            size_t colonPos = line.find(':');
            string firstDest = line.substr(colonPos + 1);
            firstDest.erase(0, firstDest.find_first_not_of(" \t"));
            if (!firstDest.empty()) {
                graph[currentOrigin].push_back(firstDest);
            }
            readingDestinations = true;
        }
        // Continue reading destinations
        else if (readingDestinations) {
            string destination = line;
            destination.erase(0, destination.find_first_not_of(" \t"));
            if (!destination.empty()) {
                graph[currentOrigin].push_back(destination);
            }
        }
    }

    return graph;
}

void printGraph(const unordered_map<string, vector<string>>& graph) {
    for (const auto& pair : graph) {
        cout << pair.first << " -> [";
        for (size_t i = 0; i < pair.second.size(); ++i) {
            cout << pair.second[i];
            if (i < pair.second.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
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

bool findShortestRouteThrough(const unordered_map<string, vector<string>>& graph,
    const string& start,
    const string& goal,
    const string& mustPass1,
    const string& mustPass2) {
struct NodeState {
string city;
vector<string> path;
bool seenB;
bool seenC;
};

queue<NodeState> q;
q.push({start, {start}, start == mustPass1, start == mustPass2});
set<tuple<string, bool, bool>> visited; // To avoid revisiting same state

while (!q.empty()) {
NodeState current = q.front();
q.pop();

if (current.city == goal && current.seenB && current.seenC) {
cout << "Shortest path from " << start << " to " << goal
<< " through " << mustPass1 << " and " << mustPass2
<< ":\n";
for (size_t i = 0; i < current.path.size(); ++i) {
cout << current.path[i];
if (i < current.path.size() - 1) cout << " -> ";
}
cout << "\nConnections: " << current.path.size() - 1 << endl;
return true;
}

if (graph.find(current.city) != graph.end()) {
for (const string& neighbor : graph.at(current.city)) {
// Avoid cycles by checking full state
bool newSeenB = current.seenB || (neighbor == mustPass1);
bool newSeenC = current.seenC || (neighbor == mustPass2);
auto stateKey = make_tuple(neighbor, newSeenB, newSeenC);

if (visited.find(stateKey) == visited.end()) {
visited.insert(stateKey);
vector<string> newPath = current.path;
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
    auto graph = buildGraph(filename);
    
    // Optional: print the full graph
    // printGraph(graph);

    // Example test: Can you get from Moscow to Singapore in <3 connections?
    string startCity = "Moscow, Russia";
    string goalCity = "Hong Kong, SAR";
    int maxConnections = 3;
    string A = "Moscow, Russia";
    string B = "London, United Kingdom";
    string C = "Singapore, Singapore";
    string Z = "Sydney, Australia";
    findShortestRouteThrough(graph, A, Z, B, C);

    cout << "\nChecking if you can fly from " << startCity 
         << " to " << goalCity << " with fewer than " 
         << maxConnections << " connections...\n";

    if (!findShortestRoute(graph, startCity, goalCity, maxConnections)) {
        cout << "No route found within " << maxConnections << " connections.\n";
    }

    return 0;
}
