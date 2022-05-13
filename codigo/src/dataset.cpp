#include <iostream>
#include <string>
#include <sstream>

#include "dataset.h"

using namespace std;

File::File(string path, char delim){
    ifstream file(path);

    if (!file.is_open())  {
        throw invalid_argument("Couldn't open given file");
    }

    if (!read_header(file, delim) || !read_body(file, delim)) {
        throw invalid_argument("Invalid format");
    }
}

bool File::read_header(ifstream &file, char delim) {
    string line;

    if (!getline(file, line)) {
        return false;
    }

    istringstream header(line);
    while(getline(header, line, delim))  {
        columns.push_back(stoi(line));
    }

    return true;
}

bool File::read_body(ifstream &file, char delim) {
    string line;
    
    while(getline(file, line))  {
        string data;
        vector<unsigned long> entry;
        
        istringstream body(line);
        while(getline(body, data, delim))  {
            entry.push_back(stoi(data));
        }

        entries.push_back(entry);
    }

    return true;
}

unsigned long File::get_nodes() const {
    return nodes;
}

unsigned long File::get_vehicles() const {
    return vehicles;
}

void File::parse_header() {
    this->nodes = columns.at(0);
    this->vehicles = columns.at(1);
}

vector<FileEntry> File::get_entries() const {
    return entries;    
}

vector<Bus> File::read_buses() const {
    vector<Bus> buses;

    for (FileEntry entry : this->get_entries()) {
        Bus delivery = Bus::from_entry(entry);
        buses.push_back(delivery);
    }

    return buses;
}