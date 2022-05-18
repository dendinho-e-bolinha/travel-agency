#include <iostream>
#include <queue>

#include "entities/graph.h"
#include "max_heap.h"

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


Graph::Graph(vector<Node> nodes) {
    this->nodes = nodes;
}

Graph::Graph(Graph &g) {
    this->nodes = g.nodes;
}

void Graph::dijkstra(unsigned long &start) {
    for (unsigned long i = 0; i < nodes.size(); i++) {
        nodes.at(i).visited = false;
        nodes.at(i).parent = 0;
        nodes.at(i).capacity = 0;
        nodes.at(i).transbordos = 0;
    }

    nodes.at(start).capacity = 999999;

    MaxHeap<int, unsigned long> maxh(nodes.size(), -1);
    while (maxh.get_size() > 0) {
        unsigned int node = maxh.remove_max();
        nodes.at(node).visited = true;

        for (const Edge &e : nodes[node].adj) {
            unsigned long dest = e.get_destination();
            unsigned long minCap = min(nodes[node].capacity, e.get_capacity());
            if (minCap > nodes[dest].capacity) {
                nodes[dest].capacity = minCap;
                nodes[dest].parent = node;
                
                maxh.increase_key(dest, nodes[dest].capacity);
                maxh.increase_key(dest, nodes[dest].capacity);
            }
        }
    }
}

list<unsigned long> Graph::get_max_capacity_path(unsigned long &start, unsigned long &end) {
    dijkstra(start);

    list<unsigned long> path;

    unsigned int currNode = end;
    while (nodes[currNode].parent != 0) {
        path.push_front(currNode);
        currNode = nodes[currNode].parent;
    }

    path.push_front(currNode);

    return path;
}
