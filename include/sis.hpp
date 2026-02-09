#pragma once
#include "graph.hpp"
#include <vector>
#include <set>
#include <cmath>

struct SIS_solver {
    Graph& graph;

    double beta, gamma;
    double infect_prob, recover_prob;
    int* state; enum {SUSPECT, INFECTED} states;
    std::vector<std::vector<int> > I_record;

    SIS_solver(Graph& g) : graph(g) { I_record.clear(); state = new int[graph.V]; }
    ~SIS_solver() { delete[] state; }

    void step();
    void run(std::vector<int> init_I, int T, double beta, double gamma);
    void output();
};

void sample_idn(Graph& graph, int idn, int sample_size, std::vector<int>& sample);
void sample_Dk(Graph& graph, int k, int sample_size, std::vector<int>& sample);
void sample_core(Graph& graph, int k, int sample_size, std::vector<int>& sample);

void high_prob_lasting_beta_k(Graph& graph);