#pragma once

#include "interact.h"
#include "entities/graph.h"

#include <vector>
#include <string>

class UI {
    std::string filename;
    Graph graph;
    bool is_running = true;
    bool two_point_one_executed = false;
    bool two_point_three_executed = false;

    /**
     * @brief Lets the user pick the file where to get the data from
     * 
     * @return true if the file exists, false otherwise
     */
    std::string choose_file_menu();

    /**
     * @brief Gets the scenario the user chose
     * 
     * @return Menu 
     */
    Menu get_scenario_menu();

    /**
     * @brief Gets the menu for the scenarios where groups might separate
     * 
     * @return Menu 
     */
    Menu get_separation_menu();

    /**
     * @brief Gets the menu for the scenarios where groups do not separate
     * 
     * @return Menu 
     */
    Menu get_non_separation_menu();

    /**
     * @brief Asks the user for the start and ending nodes, and verificates if
     * they are valid
     * 
     * @return std::pair<unsigned long, unsigned long> 
     */
    std::pair<unsigned long, unsigned long> get_start_and_ending_points();

    public:
        /**
         * @brief Displays the menu
         */
        void start();

        UI();
};