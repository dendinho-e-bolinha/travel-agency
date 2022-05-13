#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "entities/bus.h"

using FileEntry = std::vector<unsigned long>;

/** @brief Manages file operations */
class File {

    std::vector<unsigned long> columns;

    /**
     * @brief Number of nodes
     * 
     */
    unsigned long nodes;

    /**
     * @brief Number of vehicles
     * 
     */
    unsigned long vehicles;

    void parse_header();
    
    /** @brief Vector containing all entry data */
    std::vector<FileEntry> entries;

    /**
     * @brief Reads file header
     * 
     * @param file The file to be read from
     * @param delim The character that delimits entries
     * @return true if data could be retrieved
     * @return false if data could not be retrieved
     */
    bool read_header(std::ifstream &file, char delim);
    
    /**
     * @brief Reads file body
     * 
     * @param file The file to be read from
     * @param delim The character that delimits entries
     * @return true if data could be retrieved
     * @return false if data could not be retrieved
     */
    bool read_body(std::ifstream &file, char delim);

public:
    /**
     * @brief Construct a new File object
     * 
     * @param path The file's absolute path
     * @param delim The character that delimits the data 
     */
    File(std::string path, char delim = ' ');

    unsigned long get_nodes() const;
    unsigned long get_vehicles() const;

    /**
     * @brief Get the entries object
     * 
     * @return std::vector<FileEntry> 
     */
    std::vector<FileEntry> get_entries() const;

    std::vector<Bus> read_buses() const;
};

