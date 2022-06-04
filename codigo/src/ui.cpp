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
    const string DATASET_PATH = "/home/gui1612/git/travel-agency/input/";
    string fn = "";

    string user_choice = read_value<string>("Please choose the dataset file: ", "Invalid filename [1, 2, ..., 10]", [&](const string filename) {
        stringstream ss(filename);
        int num;
        if (ss >> num) {
            if (num < 1 || num > 10)
                return false;
        } else {
            return false;
        }

        ss.clear();
        ss << DATASET_PATH << "in" << setfill('0') << setw(2) << num << setfill(' ') << "_b.txt";

        fn = ss.str();
        ifstream infile(fn);
        if (!infile.is_open()) {
            throw validation_error("File not found (" + (fn) + ")");
        }

        return true;
    });

    return fn;
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
    Menu scenario("Escolhe um dos seguintes cenários");


    MenuBlock scenario_options;
    scenario_options.add_option("[Cenário 1] Determinar um encaminhamento para um grupo, dada a sua dimensão", [this]() {
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
        scenario_options.add_option("[Cenário 2] Corrigir um encaminhamento, se necessário, para que a dimensão do grupo possa aumentar de um número de unidades dado", [this]() {
            std::pair<unsigned long, unsigned long> start_end_nodes = get_start_and_ending_points();

            unsigned long increment = read_value<unsigned long>("Choose the increment value: ", "Invalid start", [&](const unsigned long &start) {
                return start > 0;
            });

            list<tuple<unsigned long, unsigned long, unsigned long>> lst = graph.get_path_with_increment(start_end_nodes.first, start_end_nodes.second, increment);

            for (auto it = lst.begin(); it != lst.end(); ++it) {
                auto [origin, destination, flow] = *it;
                cout << "[ " << origin << " -> " << destination << " / " << flow << " ]" << endl;
        }
            
    });}

    scenario_options.add_option("[Cenário 3] Determinar a dimensão máxima do grupo e um encaminhamento.", [this]() {
        this->two_point_three_executed = true;

        std::pair<unsigned long, unsigned long> start_end_nodes = get_start_and_ending_points();
        list<unsigned long> lst = graph.get_max_capacity_path(start_end_nodes.first, start_end_nodes.second);

        for (auto it = lst.begin(); it != lst.end(); ++it) {
            if (it == --lst.end())
                cout << *it << endl;
            else
                cout << *it << " -> ";
        }
    });

    if (two_point_three_executed) {
        scenario_options.add_option("[Cenário 4] Partindo de um encaminhamento que constitui um grafo acíclico, determinar quando é que o grupo se reuniria novamente no destino, no mínimo.", [this]() {
            std::pair<unsigned long, unsigned long> start_end_nodes = get_start_and_ending_points();
            unsigned long earliest_meetup = graph.get_earliest_meetup(start_end_nodes.first, start_end_nodes.second);


            cout << "Earliest time at which a group of people would meetup again: " << earliest_meetup << endl;
        });

        scenario_options.add_option("[Cenário 5] Admitindo que os elementos que saem de um mesmo local partem desse local à mesma hora (e o mais cedo possível), indicar o tempo máximo de espera e os locais em que haveria elementos que esperam esse tempo.", [this]() {
            std::pair<unsigned long, unsigned long> start_end_nodes = get_start_and_ending_points();
            std::list<std::pair<unsigned long, unsigned int>> earliest_meetup = graph.get_waiting_periods(start_end_nodes.first, start_end_nodes.second);

            for (auto it = lst.begin(); it != lst.end(); ++it) {
                auto [place, duration] = *it;
                cout << "[ place: " << place << " ] (duration: " << duration << ")" << endl;
            }

    });}

    scenario.add_block(scenario_options);
    return scenario;
}

Menu UI::get_non_separation_menu() {
    Menu scenario("Escolhe um dos seguintes cenários:");

    MenuBlock scenario_options;

    scenario_options.add_option("[Cenário 1] Maximizar a dimensão do grupo e indicar um qualquer encaminhamento", [this]() {
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

    scenario_options.add_option("[Cenário 2] Maximizar a dimensão do grupo e minimizar o número de transbordos", [this]() {
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
        
        for (auto it = lst2.begin(); it != lst2.end(); ++it) {
            
            if (it == --lst2.end())
                cout << *it;
            else
                cout << *it << " -> ";
        }
        cout << "\n";
    });

    scenario.add_block(scenario_options);
    return scenario;

}

Menu UI::get_scenario_menu() {

    Menu scenario("Escolhe um dos seguintes cenários");

    MenuBlock scenario_options;
    scenario_options.add_option("[Cenário 1] Grupos que nao se separam", [this]() {
        this->groups_separation = false;

        Menu scenario_choice = get_non_separation_menu();

        bool is_running = true;
        

        MenuBlock special_block;    
        special_block.add_option("Exit", [this]() { this->is_running = false; });


        scenario_choice.set_special_block(special_block);
        scenario_choice.show();
    });

    scenario_options.add_option("[Cenário 2] Grupos que podem separar-se", [this]() {
            this->groups_separation = true;

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