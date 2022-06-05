#include "entities/graph.h"
#include "dataset.h"
#include "ui.h"

using namespace std;

int main() {
    try {
        UI ui;
        ui.start();
    } catch (exception ignored) {
        return 1;
    }
 
    return 0;
}