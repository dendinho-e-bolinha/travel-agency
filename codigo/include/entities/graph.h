#pragma once

#include <vector>
#include <list>
#include <set>
#include "dataset.h"

struct Node {
    std::vector<unsigned long> adj;

    bool visited;
    unsigned int parent;

    unsigned long distance;
    unsigned long capacity;

    unsigned long earliest_start;
    unsigned long in_degree;
};

/** @brief Manages a Edge */
class Edge {

    /** @brief The edge's origin */
    unsigned long origin;

    /** @brief The Edge' destination */
    unsigned long destination;

    /** @brief The Edge' capacity */
    unsigned long capacity;

    /** @brief The trip's duration */
    unsigned long duration;

    /** @brief Whether this edge is active or not */
    bool active = true;

    public:

        /**
         * @brief Construct a new Edge object
         * 
         * @param origin The edge's origin
         * @param destination The Edge' destination
         * @param capacity The Edge' capacity
         * @param duration The trip's duration
         */
        Edge(unsigned long origin, unsigned long destination, unsigned long capacity, unsigned long duration);

        /**
         * @brief Edge copy constructor
         */
        Edge(const Edge &edge);

        /**
         * @brief Creates a Edge object from a vector of entries
         * 
         * @param entry 
         * @return Edge object
         */
        static Edge from_entry(const FileEntry &entry);

        /**
         * @brief Creates all Edge from a file
         * 
         * @param file 
         * @return vector<Edge> 
         */
        static std::vector<Edge> from_file(const File &file);

        /** Getters */
        unsigned long get_origin() const;
        unsigned long get_destination() const;
        unsigned long get_capacity() const;
        unsigned long get_duration() const;
        bool is_active() const;

        /** Setters */
        void set_active(bool active);
};

class Graph {
    
    unsigned long n;
    std::vector<Node> nodes;
    std::vector<Edge> edges;

    public:

        /**
         * @brief Construct a new Graph object
         * 
         * @param n The number of nodes in this graph
         */
        Graph(int n);


        /**
         * @brief Copy constructor for a graph
         * 
         * @param g 
         */
        Graph(const Graph &g);

        /**
         * @brief Add a edge to this graph
         * 
         * @param edge The edge to add to this graph
         */
        void add_edge(const Edge &edge);

        /**
         * @brief Set the active edges for this graph
         * 
         * @param activeEdges a set of edges that should be kept active
         */
        void set_active_edges(const std::set<std::pair<unsigned long, unsigned long>> &activeEdges);

        /**
         * @brief Alters the graph to contain information about the maximum capacity path
         * 
         * @param start The starting point
         */
        void max_capacity_dijkstra(unsigned long start);

        /**
         * @brief Alters the graph to contain information about the minimum distance path
         * 
         * @param start The starting point
         */
        void min_distance_dijkstra(unsigned long start);

        /**
         * @brief Alters the graph to contain information about the biggest duration path
         * 
         * @param start The starting point
         */
        void biggest_duration(unsigned long start);

        /**
         * @brief Calculates the optimal path between two points
         * 
         * @param start The starting point
         * @param end The ending point
         * @return A list containing all the nodes in the path
         */
        std::list<unsigned long> get_path(unsigned long start, unsigned long end);

        /**
         * @brief Calculates the path that holds the most people in a group
         * 
         * @param start The starting point
         * @param end The ending point
         * @return A list containing all the nodes in the optimal path
         */
        std::list<unsigned long> get_max_capacity_path(unsigned long start, unsigned long end);

        /**
         * @brief Calculates the path that requires least bus transfers
         * 
         * @param start The starting point
         * @param end The ending point
         * @return A list containing all the nodes in the optimal path
         */
        std::list<unsigned long> get_min_distance_path(unsigned long start, unsigned long end);


        /**
         * @brief Get the earliest time at which a group of people would meetup again at a given destination
         * 
         * @param start The starting point
         * @param end The ending point
         * @return The time at which the group would meetup again
         */
        unsigned long get_earliest_meetup(const std::set<std::pair<unsigned long, unsigned long>> &activeEdges, unsigned long start, unsigned long end);

        /**
         * @brief Get the places and durations of where people will have to wait for others in order to proceed with their trip
         * 
         * @param start The starting point
         * @param end The ending point
         * @return A list of indexes of where the person will have to wait and the respective durations
         */
        std::list<std::pair<unsigned long, unsigned int>> get_waiting_periods(const std::set<std::pair<unsigned long, unsigned long>> &activeEdges, unsigned long start, unsigned long end);

};
