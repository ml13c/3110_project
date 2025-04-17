#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main (){
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
          cout << from << endl; //FROM CITY
          read = false;
          continue;
        continue;
    }
    if (line.find("To") == 0) {
      read = true;
      continue;
    }

    if (read && !line.empty()) {
      cout << line << endl; //TO CITIES
    }
  }
  file.close(); //close flight.txt

  return 0;
}
