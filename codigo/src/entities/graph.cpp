#include <iostream>
#include <queue>
#include <set>
#include <limits>

#include "entities/graph.h"
#include "max_heap.h"
#include "min_heap.h"

using namespace std;

Edge::Edge(unsigned long origin, unsigned long destination, unsigned long capacity, unsigned long duration): flow(0) {
    this->origin = origin;
    this->destination = destination;
    this->capacity = capacity;
    this->duration = duration;
}

Edge::Edge(const Edge &edge) : Edge(edge.get_origin(), edge.get_destination(), edge.get_capacity(), edge.get_duration()) {};


Edge Edge::from_entry(const FileEntry &entry) {
    unsigned long origin = entry.at(0);
    unsigned long destination = entry.at(1);
    unsigned long capacity = entry.at(2);
    unsigned long duration = entry.at(3);

    return Edge{origin, destination, capacity, duration};
}

vector<Edge> Edge::from_file(const File &file) {
    vector<Edge> edges;

    for (const FileEntry &entry : file.get_entries()) {
        edges.push_back(from_entry(entry));
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

bool Edge::is_active() const {
    return active;
}

void Edge::set_active(bool active) {
    this->active = active;
}

Graph::Graph(int n) : nodes(n + 1), n(n) {}

Graph::Graph(const Graph &g) : nodes(g.nodes), edges(g.edges), n(g.n) {}

void Graph::add_edge(const Edge &edge) {
    if (edge.get_origin() == 0 || edge.get_destination() == 0) {
        throw "Edge has origin or destination at 0";
    }

    unsigned long index = edges.size();
    edges.push_back(edge);

    nodes.at(edge.get_origin()).outgoing.push_back(index);
    nodes.at(edge.get_destination()).incoming.push_back(index);

    edges.at(index).set_active(true);
}

void Graph::max_capacity_dijkstra(unsigned long start) {
    for (unsigned long i = 1; i <= n; i++) {
        nodes.at(i).visited = false;
        nodes.at(i).parent = 0;
        nodes.at(i).capacity = 0;
    }

    nodes.at(start).capacity = 999999;

    MaxHeap<int, unsigned long> maxh(n, -1);
    while (maxh.get_size() > 0) {
        unsigned int node = maxh.remove_max();
        nodes.at(node).visited = true;

        for (const unsigned long &e : nodes[node].outgoing) {
            const Edge &edge =  edges[e];

            unsigned long dest = edge.get_destination();
            unsigned long minCap = min(nodes[node].capacity, edge.get_capacity());
            if (minCap > nodes[dest].capacity) {
                nodes[dest].capacity = minCap;
                nodes[dest].parent = node;
                
                maxh.insert(dest, nodes[dest].capacity);
                maxh.increase_key(dest, nodes[dest].capacity);
            }
        }
    }
}

void Graph::min_distance_dijkstra(unsigned long start) {
    for (unsigned long i = 1; i <= n; i++) {
        nodes.at(i).visited = false;
        nodes.at(i).parent = 0;
        nodes.at(i).distance = 999999;
    }

    nodes.at(start).distance = 0;

    MinHeap<int, unsigned long> minh(n, -1);
    while (minh.get_size() > 0) {
        unsigned int node = minh.remove_min();
        nodes.at(node).visited = true;

        for (const unsigned long &e : nodes[node].outgoing) {
            const Edge &edge =  edges[e];

            unsigned long dest = edge.get_destination();
            unsigned long minDis = max(nodes[node].distance, edge.get_capacity());
            if (minDis > nodes[dest].distance) {
                nodes[dest].distance = minDis;
                nodes[dest].parent = node;
                
                minh.insert(dest, nodes[dest].distance);
                minh.decrease_key(dest, nodes[dest].distance);
            }
        }
    }
}

void Graph::max_flow_increase_dijkstra(unsigned long start) {
    for (unsigned long i = 1; i <= n; i++) {
        nodes.at(i).visited = false;
        nodes.at(i).parent = 0;
        nodes.at(i).flow_increase = 0;
    }

    nodes.at(start).distance = 999999;

    MaxHeap<int, unsigned long> maxh(n, -1);
    while (maxh.get_size() > 0) {
        unsigned int node = maxh.remove_max();
        nodes.at(node).visited = true;

        for (const unsigned long &e : nodes[node].outgoing) {
            const Edge &edge = edges[e];

            unsigned long dest = edge.get_destination();
            unsigned long maxFluxIncrease = min(nodes[node].flow_increase, edge.get_capacity() - edge.get_flow());
            if (maxFluxIncrease > nodes[dest].flow_increase) {
                nodes[dest].flow_increase = maxFluxIncrease;
                nodes[dest].parent = e;
                
                maxh.insert(dest, nodes[dest].flow_increase);
                maxh.increase_key(dest, nodes[dest].flow_increase);
            }
        }

        for (const unsigned long &e : nodes[node].incoming) {
            const Edge &edge = edges[e];

            unsigned long origin = edge.get_origin();
            unsigned long maxFluxIncrease = min(nodes[node].flow_increase, edge.get_flow());
            if (maxFluxIncrease > nodes[origin].flow_increase) {
                nodes[origin].flow_increase = maxFluxIncrease;
                nodes[origin].parent = e;
                
                maxh.insert(origin, nodes[origin].flow_increase);
                maxh.increase_key(origin, nodes[origin].flow_increase);
            }
        }
    }
}


list<unsigned long> Graph::get_path(unsigned long start, unsigned long end) {
    list<unsigned long> path;

    unsigned long currNode = end;
    while (nodes[currNode].parent != 0) {
        path.push_front(currNode);
        currNode = nodes[currNode].parent;
    }

    path.push_front(currNode);

    return path;
}

list<unsigned long> Graph::get_max_capacity_path(unsigned long start, unsigned long end) {
    max_capacity_dijkstra(start);
    return get_path(start, end);
}

list<unsigned long> Graph::get_min_distance_path(unsigned long start, unsigned long end) {
    min_distance_dijkstra(start);
    return get_path(start, end);
}

void Graph::biggest_duration(unsigned long start) {
    for (unsigned long i = 1; i <= n; i++) {
        Node &node = nodes[i];

        node.parent = 0;
        node.visited = false;

        node.earliest_start = 0;
        node.in_degree = 0;
    }

    for (Edge &edge : edges) {
        if (edge.is_active()) {
            Node &node = nodes[edge.get_destination()];
            node.in_degree += 1;
        }
    }

    queue<unsigned long> next;
    next.push(start);

    while (!next.empty()) {
        unsigned long index = next.front(); next.pop();
        Node &node = nodes[index];

        node.visited = true;

        for (unsigned long e : node.outgoing) {
            const Edge &edge = edges[e];
            if (!edge.is_active()) {
                continue;
            }

            Node &neighbor = nodes[edge.get_destination()];
            unsigned long end = node.earliest_start + edge.get_duration();

            if (neighbor.earliest_start < end) {
                neighbor.earliest_start = end;
                neighbor.parent = index;
            }

            neighbor.in_degree -= 1;
            if (neighbor.in_degree == 0) {
                next.push(edge.get_destination());
            }
        }
    }
}

void Graph::set_active_edges(const set<pair<unsigned long, unsigned long>> &activeEdges) {
    for (Edge &edge : edges) {
        pair<unsigned long, unsigned long> e = { edge.get_origin(), edge.get_destination() };
        bool active = activeEdges.find(e) != activeEdges.end();

        edge.set_active(active);
    }
}

void Graph::ford_fulkerson(unsigned long start, unsigned long end, unsigned long flow_increase) {
    while (flow_increase > 0) {
        max_flow_increase_dijkstra(start);
        if (!nodes.at(end).visited) {
            break;
        }

        unsigned long increment = nodes.at(end).flow_increase;

        unsigned long curr = end;
        while (curr != start) {
            Node &curr_node = nodes.at(curr);
            Edge &edge = edges.at(curr_node.parent);
            
            if (curr == edge.get_destination()) {
                edge.set_flow(edge.get_flow() + increment);
                curr = edge.get_origin();
            } else {
                edge.set_flow(edge.get_flow() - increment);
                curr = edge.get_destination();
            }
        }

        flow_increase -= min(flow_increase, increment); // prevent overflow
    }
}

list<tuple<unsigned long, unsigned long, unsigned long>> Graph::get_flow_path(unsigned long start, unsigned long end) {
    list<tuple<unsigned long, unsigned long, unsigned long>> path;
    if (!nodes.at(end).visited) {
        return path;
    }

    for (Edge &edge : edges) {
        if (edge.get_flow() > 0) {
            path.push_back({ edge.get_origin(), edge.get_destination(), edge.get_flow() });
        }
    }

    return path;
}

list<tuple<unsigned long, unsigned long, unsigned long>> Graph::get_path_for_group_of_size(unsigned long start, unsigned long end, unsigned long size) {
    for (Edge &edge : edges) {
        edge.set_flow(0);
    }

    ford_fulkerson(start, end, size);
    return get_flow_path(start, end);
}

list<tuple<unsigned long, unsigned long, unsigned long>> Graph::get_path_with_increment(list<tuple<unsigned long, unsigned long, unsigned long>> previous_route, unsigned long increment) {
    for (Edge &edge : edges) {
        edge.set_flow(0);
    }

    vector<unsigned long> in(n + 1), out(n + 1);

    for (tuple<unsigned long, unsigned long, unsigned long> &tup : previous_route) {
        auto [ origin, dest, flow ] = tup;

        for (unsigned long e : nodes.at(origin).outgoing) {
            Edge &edge = edges.at(e);
            if (edge.get_destination() == dest) {
                in.at(edge.get_destination()) += 1;
                out.at(edge.get_origin()) += 1;

                edge.set_flow(flow);
            }
        }
    }

    unsigned long start = 0, end = 0;
    for (size_t i = 1; i <= n; i++) {
        if (in.at(i) == 0) {
            start = i;
        }

        if (out.at(i) == 0) {
            end = i;
        }
    }

    if (start == 0 || end == 0) {
        return {};
    }

    ford_fulkerson(start, end, increment);
    return get_flow_path(start, end);
}

list<tuple<unsigned long, unsigned long, unsigned long>> get_path_for_group_of_max_size() {
        for (Edge &edge : edges) {
        edge.set_flow(0);
    }

    ford_fulkerson(start, end, numeric_limits<unsigned long>::max());
    return get_flow_path(start, end);
}


unsigned long Graph::get_earliest_meetup(const set<pair<unsigned long, unsigned long>> &activeEdges, unsigned long start, unsigned long end) {
    set_active_edges(activeEdges);
    biggest_duration(start);

    Node &destination = nodes[end];
    if (!destination.visited || destination.in_degree > 0) {
        return 0;
    } else {
        return destination.earliest_start;
    }
}

list<pair<unsigned long, unsigned int>> Graph::get_waiting_periods(const set<pair<unsigned long, unsigned long>> &activeEdges, unsigned long start, unsigned long end) {
    set_active_edges(activeEdges);
    biggest_duration(start);

    list<pair<unsigned long, unsigned int>> waiting_periods;

    for (unsigned long i = 1; i <= n; i++) {
        Node &node = nodes[i];

        unsigned long earliest_arrival = node.earliest_start;

        for (unsigned long e : node.incoming) {
            const Edge &edge = edges[e];

            if (!edge.is_active()) {
                continue;
            }

            const Node &previous = nodes[edge.get_origin()];
            unsigned long arrival = previous.earliest_start + edge.get_duration();
            if (arrival < earliest_arrival) {
                earliest_arrival = arrival;
            }
        }

        if (earliest_arrival < node.earliest_start) {
            unsigned long wait = node.earliest_start - earliest_arrival;
            waiting_periods.push_back({ i, wait });
        }
    }

    return waiting_periods;
}

