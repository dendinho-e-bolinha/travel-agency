#pragma once

#include <vector>
#include <list>
#include "dataset.h"

struct Node {
    unsigned int parent;

    std::list<Edge> adj;

    bool visited;
    unsigned long distance;
    unsigned long capacity;
};

/** @brief Manages a Edge */
class Edge {

    /** @brief The Edge' destination */
    unsigned long destination;

    /** @brief The Edge' capacity */
    unsigned long capacity;

    /** @brief The trip's duration */
    unsigned long duration;

    public:

        /**
         * @brief Construct a new Edge object
         * 
         * @param destination The Edge' destination
         * @param capacity The Edge' capacity
         * @param duration The trip's duration
         */
        Edge(unsigned long destination, unsigned long capacity, unsigned long duration);

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
        static Edge from_entry(std::vector<unsigned long> entry);

        /**
         * @brief Creates all Edge from a file
         * 
         * @param file 
         * @return vector<Edge> 
         */
        static std::vector<Edge> from_file(File file);

        /** Getters */
        unsigned long get_destination() const;
        unsigned long get_capacity() const;
        unsigned long get_duration() const;
};

class Graph {
    
    std::vector<Node> nodes;

    public:

        /**
         * @brief Construct a new Graph object
         * 
         * @param nodes The nodes belonging to the graph
         */
        Graph(std::vector<Node> nodes);


        /**
         * @brief Copy constructor for a graph
         * 
         * @param g 
         */
        Graph(Graph &g);

        /**
         * @brief Alters the graph to contain information about the maximum capacity path
         * 
         * @param start The starting point
         */
        void capacity_dijkstra(unsigned long &start);

        /**
         * @brief Alters the graph to contain information about the minimum distance path
         * 
         * @param start The starting path
         */
        void min_distance_dijkstra(unsigned long &start);

        /**
         * @brief Calculates the optimal path between two points
         * 
         * @param start The starting point
         * @param end The ending point
         * @return A list containing all the nodes in the path
         */
        std::list<unsigned long> get_path(unsigned long &start, unsigned long &end);

        /**
         * @brief Calculates the path that holds the most people in a group
         * 
         * @param start The starting point
         * @param end The ending point
         * @return A list containing all the nodes in the optimal path
         */
        std::list<unsigned long> get_max_capacity_path(unsigned long &start, unsigned long &end);

        /**
         * @brief Calculates the path that requires least bus transfers
         * 
         * @param start The starting point
         * @param end The ending point
         * @return A list containing all the nodes in the optimal path
         */
        std::list<unsigned long> get_min_distance_path(unsigned long &start, unsigned long &end);
};
