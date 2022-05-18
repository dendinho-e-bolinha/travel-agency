#include "entities/graph.h"

using namespace std;

Edge::Edge(unsigned long origin, unsigned long destination, unsigned long capacity, unsigned long duration) {
    this->origin = origin;
    this->destination = destination;
    this->capacity = capacity;
    this->duration = duration;
}

Edge::Edge(const Edge &Edge) : Edge(Edge.get_origin(), Edge.get_destination(), Edge.get_capacity(), Edge.get_duration()) {};


Edge Edge::from_entry(vector<unsigned long> entry) {
    unsigned long origin = entry.at(0);
    unsigned long destination = entry.at(1);
    unsigned long capacity = entry.at(2);
    unsigned long duration = entry.at(3);

    return Edge{origin, destination, capacity, duration};
}

vector<Edge> Edge::from_file(File file) {
    vector<Edge> edges;

    for (unsigned long i = 0; i < file.get_entries().size(); i++) {
        edges.push_back(from_entry(file.get_entries().at(i)));
    }

    return edges;
}

unsigned long Edge::get_origin() const {
    return origin;
}

unsigned long Edge::get_destination() const {
    return destination;
}

unsigned long Edge::get_capacity() const {
    return capacity;
}

unsigned long Edge::get_duration() const {
    return duration;
}
