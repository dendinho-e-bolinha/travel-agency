#include "ui.h"
#include "entities/graph.h"
#include "dataset.h"

#include <iostream>
#include <string>
#include <utility>
#include <list>
#include <set>
#include <fstream>
#include <iomanip>
#include <regex>
#include <string>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

UI::UI() {
    bool is_running = true;

    filename = choose_file_menu();
    File file(filename);
    int n = file.get_header()[0];

    Graph graph{n}; 

    for (Edge &edge : Edge::from_file(file)) {
        graph.add_edge(edge);
    }

    this->graph = graph;
}

void UI::start() {
    Menu scenario_choice = get_scenario_menu();

    while (is_running) {

        MenuBlock special_block;    
        special_block.add_option("Exit", [this]() { is_running = false; });

        scenario_choice.set_special_block(special_block);
        scenario_choice.show();

        if (!is_running)
            break;

        wait_for_input();
    }
}

string UI::choose_file_menu() {
    ///home/gui1612/git/travel-agency

    string user_choice = read_value<string>("Please choose the dataset file: ", "Invalid filename [1, 2, ..., 10]", [&](const string filename) {
        ifstream infile(filename);
        if (!infile.is_open())
            throw validation_error("File not found (" + (filename) + ")"); 

        return true;
    });

    return user_choice;
}

std::pair<unsigned long, unsigned long> UI::get_start_and_ending_points() {
    std::pair<ulong, ulong> pair;

    pair.first = read_value<unsigned long>("Choose a starting node: ", "Invalid start", [&](const unsigned long &start) {
        return start >= 1 && start <= this->graph.size();
    });

    pair.second = read_value<unsigned long>("Choose a ending node: ", "Invalid end", [&](const unsigned long &end) {
        return end >= 1 && end <= this->graph.size();
    });

    return pair;
}

Menu UI::get_separation_menu() {
    Menu scenario("Choose one of the following scenarios:");


    MenuBlock scenario_options;
    scenario_options.add_option("[Scenario 1] Determine a route to a group, given its dimension", [this]() {
        this->two_point_one_executed = true;
        std::pair<unsigned long, unsigned long> start_end_nodes = get_start_and_ending_points();

        list<unsigned long> lst = graph.get_max_capacity_path(start_end_nodes.first, start_end_nodes.second);

        for (auto it = lst.begin(); it != lst.end(); ++it) {
            if (it == --lst.end())
                cout << *it << endl;
            else
                cout << *it << " -> ";
        }
    });

    if (two_point_one_executed) {
        scenario_options.add_option("[Cenário 2] Correct a route, if needed, so that the size of the group can increase from a number of units given", [this]() {
            std::pair<unsigned long, unsigned long> start_end_nodes = get_start_and_ending_points();

            unsigned long increment = read_value<unsigned long>("Choose the increment value: ", "Invalid increment", [&](const unsigned long &start) {
                return start > 0;
            });

            list<tuple<unsigned long, unsigned long, unsigned long>> lst = graph.get_path_with_increment(start_end_nodes.first, start_end_nodes.second, increment);

            for (auto it = lst.begin(); it != lst.end(); ++it) {
                auto [origin, destination, flow] = *it;
                cout << "[ " << origin << " -> " << destination << " / " << flow << " ]" << endl;
            }
            
    });}

    scenario_options.add_option("[Scenario 3] Determine the maximum dimension of a group and a path", [this]() {
        this->two_point_three_executed = true;

        std::pair<unsigned long, unsigned long> start_end_nodes = get_start_and_ending_points();
        list<tuple<unsigned long, unsigned long, unsigned long>> lst = graph.get_path_for_group_of_max_size(start_end_nodes.first, start_end_nodes.second);

        unsigned long max_capacity = 0;

        for (auto it = lst.begin(); it != lst.end(); ++it) {
            auto [origin, destination, flow] = *it;
            if (origin == start_end_nodes.first)
                max_capacity += flow;

            cout << "[ " << origin << " -> " << destination << " / " << flow << " ]" << endl;
        }

        cout << "\nMax dimension of the group: " << max_capacity << endl;
    });

    if (two_point_three_executed) {
            scenario_options.add_option("[Scenario 4] Determine when the group would meet again at the destination, starting from a path that constitutes an acyclic graph", [this]() {
            std::pair<unsigned long, unsigned long> start_end_nodes = get_start_and_ending_points();
            unsigned long earliest_meetup = graph.get_earliest_meetup(start_end_nodes.first, start_end_nodes.second);


            cout << "Earliest time at which a group of people would meetup again: " << earliest_meetup << endl;
        });

        scenario_options.add_option("[Scenario 5] Determine the maximum waiting time and the places where there would be elements that wait for that time, assuming that the elements that leave the same location depart from that location at the same time (and as soon as possible), ", [this]() {
            std::pair<unsigned long, unsigned long> start_end_nodes = get_start_and_ending_points();
            std::list<std::pair<unsigned long, unsigned int>> lst = graph.get_waiting_periods(start_end_nodes.first, start_end_nodes.second);

            for (auto it = lst.begin(); it != lst.end(); ++it) {
                auto [place, duration] = *it;
                cout << "[ place: " << place << " ] (duration: " << duration << ")" << endl;
            }

    });}

    scenario.add_block(scenario_options);
    return scenario;
}

Menu UI::get_non_separation_menu() {
    Menu scenario("Choose one of the following scenarios:");

    MenuBlock scenario_options;

    scenario_options.add_option("[Scenario 1] Maximize the size of the group and indicate any routing", [this]() {
        std::pair<unsigned long, unsigned long> start_end_nodes = get_start_and_ending_points();

        list<unsigned long> lst = graph.get_max_capacity_path(start_end_nodes.first, start_end_nodes.second);

        for (auto it = lst.begin(); it != lst.end(); ++it) {
            if (it == --lst.end())
                cout << *it;
            else
                cout << *it << " -> ";
        }
        cout << "\n";
    });

    scenario_options.add_option("[Scenario 2] Find pareto-optimal paths", [this]() {
        std::pair<unsigned long, unsigned long> start_end_nodes = get_start_and_ending_points();
       
        list<unsigned long> lst = this->graph.get_pareto_optimal_path(start_end_nodes.first, start_end_nodes.second, true);

        for (auto it = lst.begin(); it != lst.end(); ++it) {
            if (it == --lst.end())
                cout << *it;
            else
                cout << *it << " -> ";
        }
        cout << "\n-----\n";

        list<unsigned long> lst2 = graph.get_pareto_optimal_path(start_end_nodes.first, start_end_nodes.second, false);
        
        if (lst != lst2) {
            for (auto it = lst2.begin(); it != lst2.end(); ++it) {
                
                if (it == --lst2.end())
                    cout << *it;
                else
                    cout << *it << " -> ";
            }
            cout << "\n";
        }
    });

    scenario.add_block(scenario_options);
    return scenario;

}

Menu UI::get_scenario_menu() {

    Menu scenario("Choose one of the following scenarios:");

    MenuBlock scenario_options;
    scenario_options.add_option("[Scenario 1] Groups that don't separate", [this]() {
        Menu scenario_choice = get_non_separation_menu();

        bool is_running = true;
        

        MenuBlock special_block;    
        special_block.add_option("Exit", [this]() { this->is_running = false; });


        scenario_choice.set_special_block(special_block);
        scenario_choice.show();
    });

    scenario_options.add_option("[Scenario 2] Groups that separate", [this]() {
            Menu scenario_choice = get_separation_menu();

            bool is_running = true;
            

            MenuBlock special_block;    
            special_block.add_option("Exit", [this]() { this->is_running = false; });


            scenario_choice.set_special_block(special_block);
            scenario_choice.show();
    });

    scenario_options.add_option("[Options] Change the dataset file", [this](){
        filename = choose_file_menu();
        File file(filename);
        int n = file.get_header()[0];

        Graph graph{n}; 

        for (Edge &edge : Edge::from_file(file)) {
            graph.add_edge(edge);
        }

        this->graph = graph;
    });

    scenario.add_block(scenario_options);
    return scenario;
}