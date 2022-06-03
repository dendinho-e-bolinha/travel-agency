#include <iostream>
#include <fstream>
#include "entities/graph.h"
#include "dataset.h"

using namespace std;

int main() {
    File file("/home/lima/university/FEUP-DA/projects/travel-agency/input/in01_b.txt");
    
    int n = file.get_header()[0];
    Graph graph(n);

    for (Edge &edge : Edge::from_file(file)) {
        graph.add_edge(edge);
    }

    // graph.get_path_for_group_of_max_size(8, 12);
    
    // ofstream stream("flow.csv");
    // stream << "Source,Target,Label,Capacity,Flow,Duration,Step,Increment\n";
    // for (Edge &edge : graph.edges) {
    //     stream << edge.get_origin() <<','<< edge.get_destination() <<',' << edge.get_flow()<<'/'<< edge.get_capacity() << ',' <<edge.get_capacity() <<',' << edge.get_flow() <<','<< edge.get_duration() << ",0,0" << endl;
    // }

    set<pair<unsigned long, unsigned long>> active;
    active.insert({ 46, 12 });
    active.insert({ 8, 46 });
    active.insert({ 13, 12 });
    active.insert({ 45, 13 });
    active.insert({ 28, 45 });
    active.insert({ 25, 28 });
    active.insert({ 10, 28});
    active.insert({ 4, 10 });
    active.insert({ 27, 4 });
    active.insert({ 8, 27 });
    active.insert({ 8, 25 });

    // cout << graph.get_earliest_meetup(active, 8, 12) << endl;
    auto c = graph.get_waiting_periods(active, 8, 12);
    auto it = c.begin();

    ofstream stream("duration.csv");
    stream << "Source,Target,Duration,Active,Dominant,Arrival" << endl;

    for (Edge &edge : graph.edges) {
        stream << edge.get_origin() << ',' << edge.get_destination() << ',' << edge.get_duration() << ',' << edge.is_active() << ',' << (((graph.nodes[edge.get_destination()].parent == edge.get_origin()) << 1) | edge.is_active()) << ',' << graph.nodes[edge.get_origin()].earliest_start + edge.get_duration() << endl;
    }


    ofstream stream2("duration_nodes.csv");
    stream2 << "Id,Start,Parent,Wait" << endl;
    for (int i = 1; i <= graph.n; i++) {
        while (it->first < i && it != c.end()) it++;
        Node &node = graph.nodes.at(i);
        stream2 << i << ',' << node.earliest_start << ',' << node.parent << ',' << (it->first == i ? it->second : 0) << endl;
    }

    
    return 0;
}