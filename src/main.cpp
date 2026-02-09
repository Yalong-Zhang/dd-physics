#include "timer.hpp"
#include "graph.hpp"
#include "sis.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "Usage: main <dataset_address> <idn_address> <core_address>" << std::endl;
        return 1;
    }

    Timer timer;
    timer.start();
    Graph graph(argv[1], argv[2], argv[3]);
    timer.end();
    std::cout << "Read graph time: " << timer.time() << " seconds" << std::endl;

    high_prob_lasting_beta_k(graph);

    return 0;
}