/***********************************************************
  Synopsis: generate a graph that represent the flight information
***********************************************************/
#include <iostream>
#include <vector>
#include <ctime> 
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

class Node{
   public:
     int nodeID;
     vector <int> neighbors;
};


string city[150];

int myRand(int lowest, int highest){
    
    int random_integer; 
    float range=(highest-lowest)+1; 
    random_integer = int(lowest+ (range*rand()/(RAND_MAX + 1.0))); 
    return random_integer;
};

int main(int argc, char *argv[]){
   int i,j, num_neighbors,num_nodes;
   vector <Node> graph;
   char line[256];


   if(argc<2)
   {
       cout<<"Usage: "<<argv[0]<<" number_of_nodes "<<endl;
       return 0;
   }

   ifstream cityFile;
  
   cityFile.open("city.name", ios::in);


   for (i=0; i<140; i++){
      cityFile.getline(line,256);
      city[i] = line;
     // cout << "read city : " << line << endl;
   }
   
   cityFile.close();

   num_nodes = atoi(argv[1]);
   //cout << "There are " << num_nodes << " nodes in the graph." << endl;
 
   srand((unsigned) time(0)); 

   /* generate flight to cities for each city */
   for (i=0; i< num_nodes; i++){
      Node node;
      node.nodeID = i;
     
      num_neighbors = myRand(0,10); 

      for (j = 0; j < num_neighbors; j++){
          int neighbor= myRand(0, num_nodes-1);
          int flag = 0;
          if (neighbor == i) {flag =1;}
          else {
              for (int k=0; k < node.neighbors.size(); k++){
                 if (node.neighbors[k] == neighbor)
                    flag = 1;
              }
          }
          if (!flag)
             node.neighbors.push_back(neighbor);
      }
      graph.push_back(node); 
   }

   for (i=0; i< num_nodes; i++){
     if (!graph[i].neighbors.size()) continue;
     cout << "From:  " << city[graph[i].nodeID] << endl << "To  : ";
     for (j =0; j < graph[i].neighbors.size(); j++){
         if (j)
             cout <<  "       " << city[graph[i].neighbors[j]] << endl; 
           else cout <<  " " << city[graph[i].neighbors[j]] << endl;
     }
   }
}
