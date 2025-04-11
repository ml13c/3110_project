#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main (){
  string city;
  string country;
  
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
        string fromLine = line.substr(7);
        stringstream ss(fromLine);
        getline(ss, city, ',');
        getline(ss, country);

        //trim
        city = city.substr(0, city.find_last_not_of(" \t") + 1);
        country = country.substr(country.find_first_not_of(" \t"));

        cout << city << " " << country << endl; // THIS PRINTS OUT THE "FROM" CITY AND COUNTRY
        continue;
    }
    if (line.find("To") == 0) {
        read = true;
        continue;
    }

    if (read) {
        stringstream ss(line);
        getline(ss, city, ',');
        getline(ss, country);

        city = city.substr(0, city.find_last_not_of(" \t") + 1);
        country = country.substr(country.find_first_not_of(" \t"));

        cout << city << " " << country << endl; // THIS WILL PRINT OUT THE "TO" CITIES AND COUNTRIES
    }
  }
  file.close(); //close flight.txt

  /*
  Lines 43 and 59 will be replaced to enter all of the data into a data structure.
  Ideally, this will all just be in a function, but I am still unsure about the data structure.
  */  
  return 0;
}
