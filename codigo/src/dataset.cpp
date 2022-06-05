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

    istringstream stream(line);
    while(getline(stream, line, delim))  {
        header.push_back(stoi(line));
    }

    return true;
}

bool File::read_body(ifstream &file, char delim) {

    for (unsigned long i = 0; i < this->get_header().size(); i++) {
        string data;
        FileEntry entry;

        while(getline(file, data, delim)) {
            entry.push_back(stoi(data));
        }

        this->entries.push_back(entry);
    }

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

vector<unsigned long> File::get_header() const {
    return header;
}

vector<FileEntry> File::get_entries() const {
    return entries;    
}

vector<Edge> File::read_edges() const {
    vector<Edge> edges;

    for (FileEntry entry : this->get_entries()) {
        Edge delivery = Edge::from_entry(entry);
        edges.push_back(delivery);
    }

    return edges;
}
