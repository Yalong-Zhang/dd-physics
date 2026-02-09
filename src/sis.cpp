#include "sis.hpp"
#include "utility.hpp"
#include "graph.hpp"
#include <iostream>
#include <algorithm>

void SIS_solver::step() {
    std::vector<int> old_I = I_record.back();
    for (int u : old_I) state[u] = SUSPECT;
    for (int u : old_I) {
        for (int i = 0; i < graph.undeg[u]; i++) {
            Edge& edge = graph.edges[graph.adj[u][i]];
            int v = edge.u ^ edge.v ^ u;
            if (try_random(infect_prob)) state[v] = INFECTED;
        }
    }
    for (int u : old_I) {
        if (try_random(recover_prob)) {
            state[u] = SUSPECT;           
        }
    }
    std::vector<int> now_I;
    for (int u = 0; u < graph.V; u++) {
        if (state[u] == INFECTED) now_I.push_back(u);
    }
    I_record.push_back(now_I);
}

void SIS_solver::run(std::vector<int> init_I, int T, double beta, double gamma) {
    infect_prob = beta, recover_prob = gamma;
    for (int u = 0; u < graph.V; u++) state[u] = SUSPECT;
    for (int u : init_I) state[u] = INFECTED;
    I_record.push_back(init_I);
    size_t init_I_size = init_I.size();
    for (int t = 1; t <= T; t++) {
        step();
        // std::cout << "Time step: " << t << ", Infect nodes: " << I_record.back().size() << std::endl;
        if (I_record.back().size() >= 10 * init_I_size) {
            // std::cout << "Infect nodes exceed 100 times of initial infected nodes, stop simulation." << std::endl;
            break;
        }
    }
}

void SIS_solver::output() {
    if (true) {
        std::cout << "Infect nodes at each time step:" << std::endl;
        for (size_t t = 0; t < I_record.size(); t++) {
            std::cout << "[" << t << ": \033[31m" << I_record[t].size() << "\033[0m], ";
        }
        std::cout << std::endl;
    }
}

void sample_idn(Graph& graph, int idn, int sample_size, std::vector<int>& sample) {
    std::vector<int> candidates;
    for (int i = 0; i < graph.V; i++) {
        if (graph.idn[i] == idn) candidates.push_back(i);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(candidates.begin(), candidates.end(), gen);
    for (int i = 0; i < sample_size && i < (int)candidates.size(); i++) {
        sample.push_back(candidates[i]);
    }
}
void sample_core(Graph& graph, int k, int sample_size, std::vector<int>& sample) {
    std::vector<int> candidates;
    for (int i = 0; i < graph.V; i++) {
        if (graph.core[i] == k) candidates.push_back(i);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(candidates.begin(), candidates.end(), gen);
    for (int i = 0; i < sample_size && i < (int)candidates.size(); i++) {
        sample.push_back(candidates[i]);
    }
}

void sample_Dk(Graph& graph, int k, int sample_size, std::vector<int>& sample) {
    std::vector<int> candidates;
    for (int i = 0; i < graph.V; i++) {
        if (graph.idn[i] >= k) candidates.push_back(i);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(candidates.begin(), candidates.end(), gen);
    for (int i = 0; i < sample_size && i < (int)candidates.size(); i++) {
        sample.push_back(candidates[i]);
    }
}

void high_prob_lasting_beta_k(Graph& graph) {
    int num_runs = 300;
    int end_time = 100;
    double high_prob = 0.8;
    int init_I_size = 100;
    int max_idn = *std::max_element(graph.idn, graph.idn + graph.V);
    int max_core = *std::max_element(graph.core, graph.core + graph.V);
    double beta_epsilon = 0.001;
    double gamma = 0.5;

    std::vector<std::pair<int, double> > idn_beta_pairs;
    for (int now_idn = 1; now_idn <= max_idn; now_idn++) {
        double beta_lower = 0.0;
        double beta_upper = 1.0;
        while (beta_upper - beta_lower > beta_epsilon) {
            double beta_mid = (beta_lower + beta_upper) / 2.0;
            int success_count = 0;
            for (int run = 0; run < num_runs; run++) {
                SIS_solver solver(graph);
                std::vector<int> init_I;
                sample_idn(graph, now_idn, init_I_size, init_I);
                solver.run(init_I, end_time, beta_mid, gamma);
                if (!solver.I_record.back().empty()) success_count++;
            }
            double prob = (double)success_count / num_runs;
            std::cout << "IDN: " << now_idn << " \tBeta: " << beta_mid << " \tLasting probability: " << prob << std::endl;
            if (prob >= high_prob) {
                beta_upper = beta_mid;
            } else {
                beta_lower = beta_mid;
            }
        }
        std::cout << "IDN: " << now_idn << " \tHigh probability lasting beta: " << beta_lower << std::endl;
        idn_beta_pairs.emplace_back(now_idn, beta_lower);
    }

    std::vector<std::pair<int, double> > core_beta_pairs;
    for (int now_core = 1; now_core <= max_core; now_core++) {
        double beta_lower = 0.0;
        double beta_upper = 1.0;
        while (beta_upper - beta_lower > beta_epsilon) {
            double beta_mid = (beta_lower + beta_upper) / 2.0;
            int success_count = 0;
            for (int run = 0; run < num_runs; run++) {
                SIS_solver solver(graph);
                std::vector<int> init_I;
                sample_core(graph, now_core, init_I_size, init_I);
                solver.run(init_I, end_time, beta_mid, gamma);
                if (!solver.I_record.back().empty()) success_count++;
            }
            double prob = (double)success_count / num_runs;
            std::cout << "Core: " << now_core << " \tBeta: " << beta_mid << " \tLasting probability: " << prob << std::endl;
            if (prob >= high_prob) {
                beta_upper = beta_mid;
            } else {
                beta_lower = beta_mid;
            }
        }
        std::cout << "Core: " << now_core << " \tHigh probability lasting beta: \033[31m" << beta_lower << "\033[0m" << std::endl;
        core_beta_pairs.emplace_back(now_core, beta_lower);
    }
}