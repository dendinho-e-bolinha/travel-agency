#pragma once

#include "interact.h"
#include "entities/graph.h"

#include <vector>
#include <string>

class UI {
    std::string filename;
    bool groups_separation = false;
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


    Menu get_separation_menu();

    Menu get_non_separation_menu();

    std::pair<unsigned long, unsigned long> get_start_and_ending_points();

    public:
        /**
         * @brief Displays the menu
         */
        void start();

        UI();
};