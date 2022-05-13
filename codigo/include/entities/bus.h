#pragma once

#include <vector>

/** @brief Manages a Bus */
class Bus {

    /** @brief The bus origin */
    unsigned long origin;

    /** @brief The bus' destination */
    unsigned long destination;

    /** @brief The bus' capacity */
    unsigned long capacity;

    /** @brief The trip's duration */
    unsigned long duration;

    public:

        /**
         * @brief Construct a new Bus object
         * 
         * @param origin The bus' origin
         * @param destination The bus' destination
         * @param capacity The bus' capacity
         * @param duration The trip's duration
         */
        Bus(unsigned long origin, unsigned long destination, unsigned long capacity, unsigned long duration);

        /**
         * @brief Bus copy constructor
         */
        Bus(const Bus &bus);

        /**
         * @brief Creates a Bus object from a vector of entries
         * 
         * @param entry 
         * @return Bus object
         */
        static Bus from_entry(std::vector<unsigned long> entry);

        /** Getters */
        unsigned long get_origin() const;
        unsigned long get_destination() const;
        unsigned long get_capacity() const;
        unsigned long get_duration() const;
};