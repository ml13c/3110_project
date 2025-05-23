#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <queue>
#include <stack>
#include <climits> // NEEDED for INT_MAX and running on Linux/Cell Machines
/*
    CSCE 3110: Algorithms and Data Structures (Project Flight Graph)
    This program reads a flight graph from a file, allowing users to find the shortest route between cities, 
    find routes through specific cities, find the longest possible path to explore cities while returning to origin,
    and determine the best city to meet in for 3 friends while maintaining a minumum amount of travel between connections.
    For more clear understanding, please refer to the comments in the code.
    @ author: Marcus, Haley, and Mario 
*/
using namespace std;

/*
This function build the graph from the file. It reads the file line by line, extracting the origin and destination cities
into a map in the format of <origin, vector<destinations>>. It's important that the file is formatted correctly like flight.txt as that is how the graph is built.
This graph is used for all the solution functions in this program.
*/
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

/*
This function prints the graph back out in the format it was read in through the buildGraph function.
This is only used for debugging purposes to check if the graph was built correctly and option 5 in the switch statement UI.
*/
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

/*SOLUTION TO QUESTION 1
This function finds the shortest route between two cities in the flight graph.
It uses a breadthfirst search algorithm to explore the graph while keeping in mind the distance from the start city to each city visited.
*/
bool findShortestRoute(const map<string, vector<string>>& graph, const string& start, const string& goal, int maxConnections) {

    map<string, string> parent;// to reconstruct the path and store origin of each city visited
    map<string, int> distance;// counter for amount of connections done
    queue<string> q; //for bfs to find shortest path as answer

    q.push(start);
    distance[start] = 0;
    // traverses the graph using bfs
    while (!q.empty()) {
        string current = q.front();
        q.pop();
        // if the requirements are left and the goal is reached, we can stop searching and print out the path
        if ((current == goal && distance[current]) <= maxConnections) {
            stack<string> path;//used to print out path in correct order
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

        if (graph.find(current) != graph.end()) { // checking if the current city is the end or has no neighbors
            for (const auto& neighbor : graph.at(current)) { // any reference to the current node is its neighbor so we check the 'to' cities(paths)
                if (distance.find(neighbor) == distance.end()) {
                    parent[neighbor] = current; // when visitng a new city, we set the parent to the current city to keep track of the path
                    distance[neighbor] = distance[current] + 1; // increment the distance from the start city to the current city as we move further from start
                    if (distance[neighbor] <= maxConnections) { // only searches for neighbors if the distance is less than or equal to the max connections
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
/*SOLUTION TO QUESTION 2
This function finds a route from the start city to the goal city while passing through two specified cities.
It uses bfs to explore the graph, keeping track of whether the required cities have yet to be visited. Takes in 5 parameters:
the generated graph to iterate through, and the 4 cities from the citys to check for a route through.
*/
bool findRouteThrough(const map<string, vector<string>>& graph, const string& start, const string& goal, const string& mustPass1, const string& mustPass2) {
    queue<tuple<string, vector<string>, bool, bool>> q;// queue to store the current city, path taken, and if the must pass cities have been seen
    q.push({start, {start}, start == mustPass1, start == mustPass2});// initial starting points
    set<tuple<string, bool, bool>> visited; // used tuple since we need to check for 3 variables (city, must pass 1, must pass 2)
    // ^ this allows for us to pass through the same city to find the shortest path.

    while (!q.empty()) {
        auto [current, path, seenB, seenC] = q.front();// get the current city, path taken, and if the must pass cities have been seen
        q.pop();

        if (current == goal && seenB && seenC) {// stops searching if the goal is reached and both must pass cities have been seen
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

        if (graph.find(current) != graph.end()) {// bfs logic like from question 1
            for (const string& neighbor : graph.at(current)) {
                // Avoid cycles by checking full state
                bool newSeenB = seenB || (neighbor == mustPass1);// flags to check if the must pass cities have been passed
                bool newSeenC = seenC || (neighbor == mustPass2);
                auto stateKey = make_tuple(neighbor, newSeenB, newSeenC);// allows for multiple states of node, (revisiting the same node with different states)

                if (visited.find(stateKey) == visited.end()) {// if the state node has not been visited yet, we can add it to the queue
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

/*SOLUTION TO QUESTION 3
This function finds all possible cities that can be visited from a given city while returning to the same city only once.
It used depth first search to explore the graph, keeping track of visited cities and the current path.
It also checks if the current city is the same as the start city to determine if a cycle has been found. In order check if a vertex can be added to the path,
it checks if the vertex is already in the path and if there is an edge between the last city in the path and the current city.
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

// Utility function to check if a vertex can be added to the Hamiltonian path, needed for the recursive function
bool isSafe(int v, const vector<vector<int>>& graph, vector<int>& path, int pos) {
    if (graph[path[pos - 1]][v] == 0) return false;// check if there is an edge between the last city in the path and the current city
    for (int i = 0; i < pos; i++) if (path[i] == v) return false;
    return true;
}

// Recursive function to solve the problem
bool hamCycleUtil(const vector<vector<int>>& graph, vector<int>& path, int pos) {
    if (pos == path.size()) {
        return graph[path[pos - 1]][path[0]] == 1;
    }

    for (int v = 0; v < graph.size(); v++) {
        if (v == path[0]) continue;
        if (isSafe(v, graph, path, pos)) {
            path[pos] = v;
            if (hamCycleUtil(graph, path, pos + 1)) return true;
            path[pos] = -1;
        }
    }

    return false;
}

// function to find a route and print the solution
void findCycle(const vector<vector<int>>& graph, const string start) {
    vector<int> path(graph.size(), -1);
    int startIdx = cityIndex.at(start);
    path[0] = startIdx;

    if (!hamCycleUtil(graph, path, 1)) {
        cout << "No route exists." << endl;
        return;
    }

    for (int cityIdx : path) {
        for (const auto& [city, idx] : cityIndex) {
            if (idx == cityIdx) {
                cout << city << "-> ";
                break;
            }
        }
    }
    cout << endl;
    cout << "Smallest number of connections: " << path.size();
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
void printPath(const string& start, const string& destination, const map<string, string>& parent) {// used to prevent repetitive code
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
// search through each city using bfs to find the shortest path from each city to all other cities
    bfs(graph, cityA, distA, parentA);
    bfs(graph, cityB, distB, parentB);
    bfs(graph, cityC, distC, parentC);

    string bestCity = "";
    int minTotalConnections = INT_MAX;

    map<string, vector<string>>::const_iterator it;
    for (it = graph.begin(); it != graph.end(); ++it) { // iterate through the graph to find the best city to meet at
        const string& city = it->first;

        if (city == cityA || city == cityB || city == cityC) {// skip the cities of the friends since they cant meet up at their own cities
            continue;
        }

        if (distA.count(city) > 0 && distB.count(city) > 0 && distC.count(city) > 0) {// check if the city is reachable from all three friends
            int total = distA[city] + distB[city] + distC[city];// total connections needed to reach the city from all three friends
            if (total < minTotalConnections) {// if the total connections is less than the current minimum, update the best city and minimum connections
                minTotalConnections = total;// this is done repetively for all cities in the graph until the best one is found
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
    string filename = "flight.txt";//file argument to read and construct graph from
    int choice,  maxConnections, x;
    string fromCity, toCity, city1, city2;
    map<string, vector<string>> graph = buildGraph(filename);
    vector<vector<int>> adjMatrix = buildAdjMatrix(graph, x);//adjacency matrix used for question 3
//Basic UI for the program using a switch statement to select which problem to solve.
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
            findCycle(adjMatrix, fromCity);
            break;
        case 4:
            cout << "Input three cities to find min path for each\n";
            cout << "Your City: ";
            getline(cin, fromCity);
            cout << "Friend 1 City: ";
            getline(cin, city1);
            cout << "Friend 2 City: ";
            getline(cin, city2);
            cout << "Finding besty city and route for 3 friends...\n";
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
