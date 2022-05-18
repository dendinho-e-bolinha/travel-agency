#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "graph.h"

using FileEntry = std::vector<unsigned long>;

/** @brief Manages file operations */
class File {

    std::vector<unsigned long> header;  // meter file entry aqui? ou não fica explicito?
    
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

    /**
     * @brief Get the header object
     * 
     * @return std::vector<unsigned long> 
     */
    std::vector<unsigned long> get_header() const;

    /**
     * @brief Get the entries object
     * 
     * @return std::vector<FileEntry> 
     */
    std::vector<FileEntry> get_entries() const;

    std::vector<Edge> read_edges() const;
};

