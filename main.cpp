#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void readFile();

int main (){
  //readFile();
  bool running = true;
  int choice;
  while (running){
    cout << "1: Smallest route" << endl;
    cout << "2: Smallest route through other cities" << endl;
    cout << "3: Visit all cities and come back to the start" << endl;
    cout << "4: Find optimal meeting city between 3" << endl;
    cout << "5: Exit"<< endl;
    cout << "-->";
    cin >> choice;
    cout << endl;

    switch(choice){
      case 1:

      break;
      case 2:

      break;
      case 3:

      break;
      case 4:

      break;
      case 5:
        running = false;
      break;
      default:
        running = false;
    }
  }
  

  return 0;
}

void readFile(){
  //open flight.txt
  ifstream file; 
  file.open("flight.txt");
  if (!file.is_open()){
    cout << "Could not open flight.txt." << endl;
    exit(1); //end program
  }
  //read flight.txt
  string line;
  bool read = false;

  while (getline(file, line)) {
    //edit line for spaces
    size_t start = line.find_first_not_of(" \t");
    size_t end = line.find_last_not_of(" \t");
    if (start == string::npos || end == string::npos) continue;
    line = line.substr(start, end - start + 1);

    //skip "From:" and "To"
    if (line.find("From:") == 0) {
      string from = line.substr(6); // Skip "From: "
      from = from.substr(from.find_first_not_of(" \t"));
      cout << from << endl; //FROM CITY (this line will be used to input into data structure)
      read = false;
      continue;
    }
    if (line.find("To") == 0) {
      read = true;
      continue;
    }
    if (read && !line.empty()) {
      cout << line << endl; //TO CITIES(this line will be used to input into data structure)
    }
  }
  file.close(); //close flight.txt
}
