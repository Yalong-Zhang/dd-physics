#pragma once

struct Edge {
    int u, v;
};

struct Graph {
    int V, E;
    Edge* edges;
    int* undeg, *idn, *core;
    int** adj;

    Graph() {};
    Graph(char* dataset_address, char* idn_address, char* core_address);
    ~Graph();
    void output();
};