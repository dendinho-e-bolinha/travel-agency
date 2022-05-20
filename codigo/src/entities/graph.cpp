#include <iostream>
#include <queue>

#include "entities/graph.h"
#include "max_heap.h"
#include "min_heap.h"

using namespace std;

Edge::Edge(unsigned long destination, unsigned long capacity, unsigned long duration) {
    this->destination = destination;
    this->capacity = capacity;
    this->duration = duration;
}

Edge::Edge(const Edge &Edge) : Edge(Edge.get_destination(), Edge.get_capacity(), Edge.get_duration()) {};


Edge Edge::from_entry(vector<unsigned long> entry) {
    unsigned long destination = entry.at(1);
    unsigned long capacity = entry.at(2);
    unsigned long duration = entry.at(3);

    return Edge{destination, capacity, duration};
}

vector<Edge> Edge::from_file(File file) {
    vector<Edge> edges;

    for (unsigned long i = 0; i < file.get_entries().size(); i++) {
        edges.push_back(from_entry(file.get_entries().at(i)));
    }

    return edges;
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

void Graph::capacity_dijkstra(unsigned long &start) {
    for (unsigned long i = 0; i < nodes.size(); i++) {
        nodes.at(i).visited = false;
        nodes.at(i).parent = 0;
        nodes.at(i).capacity = 0;
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
                
                maxh.insert(dest, nodes[dest].capacity);
                maxh.increase_key(dest, nodes[dest].capacity);
            }
        }
    }
}

void Graph::min_distance_dijkstra(unsigned long &start) {
        for (unsigned long i = 0; i < nodes.size(); i++) {
        nodes.at(i).visited = false;
        nodes.at(i).parent = 0;
        nodes.at(i).distance = 999999;
    }

    nodes.at(start).distance = 0;

    MinHeap<int, unsigned long> minh(nodes.size(), -1);
    while (minh.get_size() > 0) {
        unsigned int node = minh.remove_min();
        nodes.at(node).visited = true;

        for (const Edge &e : nodes[node].adj) {
            unsigned long dest = e.get_destination();
            unsigned long minDis = max(nodes[node].distance, e.get_capacity());
            if (minDis > nodes[dest].distance) {
                nodes[dest].distance = minDis;
                nodes[dest].parent = node;
                
                minh.insert(dest, nodes[dest].distance);
                minh.decrease_key(dest, nodes[dest].distance);
            }
        }
    }
}

list<unsigned long> Graph::get_path(unsigned long &start, unsigned long &end) {
    list<unsigned long> path;

    unsigned long currNode = end;
    while (nodes[currNode].parent != 0) {
        path.push_front(currNode);
        currNode = nodes[currNode].parent;
    }

    path.push_front(currNode);

    return path;
}

list<unsigned long> Graph::get_max_capacity_path(unsigned long &start, unsigned long &end) {
    capacity_dijkstra(start);
    get_path(start, end);
}



list<unsigned long> Graph::get_min_distance_path(unsigned long &start, unsigned long &end) {
    min_distance_dijkstra(start);
    get_path(start, end);
}
