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
    graph.get_path_for_group_of_max_size(8, 12);
    
    ofstream stream("result.csv");
    stream << "Source,Target,Label,Capacity,Flow,Duration,Step,Increment\n";
    for (Edge &edge : graph.edges) {
        stream << edge.get_origin() <<','<< edge.get_destination() <<',' << edge.get_flow()<<'/'<< edge.get_capacity() << ',' <<edge.get_capacity() <<',' << edge.get_flow() <<','<< edge.get_duration() << ",0,0" << endl;
    }

    
    return 0;
}