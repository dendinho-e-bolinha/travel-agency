#include "entities/bus.h"

using namespace std;

Bus::Bus(unsigned long origin, unsigned long destination, unsigned long capacity, unsigned long duration) {
    this->origin = origin;
    this->destination = destination;
    this->capacity = capacity;
    this->duration = duration;
}

Bus::Bus(const Bus &bus) : Bus(bus.get_origin(), bus.get_destination(), bus.get_capacity(), bus.get_duration()) {};


Bus Bus::from_entry(vector<unsigned long> entry) {
    unsigned long origin = entry.at(0);
    unsigned long destination = entry.at(1);
    unsigned long capacity = entry.at(2);
    unsigned long duration = entry.at(3);

    return Bus{origin, destination, capacity, duration};
}

unsigned long Bus::get_origin() const {
    return origin;
}

unsigned long Bus::get_destination() const {
    return destination;
}

unsigned long Bus::get_capacity() const {
    return capacity;
}

unsigned long Bus::get_duration() const {
    return duration;
}
