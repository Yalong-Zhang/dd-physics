#include "graph.hpp"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <unordered_map>

Graph::Graph(char* dataset_address, char* idn_address, char* core_address) {
    FILE* file = fopen(dataset_address, "r");
    if (!file) {
        std::cout << "Failed to open file: " << dataset_address << std::endl;
        exit(1);
    }

    auto read_number = [](FILE* in) -> int {
        int x = 0;
        int ch = 0;
        while (ch < '0' || ch > '9') ch = fgetc(in);
        while (ch >= '0' && ch <= '9') {
            x = x * 10 + (ch - '0');
            ch = fgetc(in);
        }
        return x;
    };

    E = read_number(file), V = read_number(file); V++;
    edges = (Edge*)malloc(E * sizeof(Edge));
	undeg = (int*)malloc(V * sizeof(int));
	memset(undeg, 0, V * sizeof(int));
	adj = (int**)malloc(V * sizeof(int*));
	idn = (int*)malloc(V * sizeof(int));
    core = (int*)malloc(V * sizeof(int));

    for (int i = 0; i < E; i++) {
        edges[i].u = read_number(file), edges[i].v = read_number(file);
        undeg[edges[i].u]++, undeg[edges[i].v]++;
    }
	for (int x = 0; x < V; x++) adj[x] = (int*)malloc(undeg[x] * sizeof(int));

    memset(undeg, 0, V * sizeof(int));
	for (int i = 0; i < E; i++) {
		int u = edges[i].u, v = edges[i].v;
		adj[u][undeg[u]++] = adj[v][undeg[v]++] = i;
	}

    fclose(file);

    FILE* idn_file = fopen(idn_address, "r");
    if (!idn_file) {
        std::cout << "Failed to open file: " << idn_address << std::endl;
        exit(1);
    }
    for (int i = 0; i < V; i++) {
        int u = read_number(idn_file), idn_value = read_number(idn_file);
        idn[u] = idn_value;
    }
    fclose(idn_file);

    FILE* core_file = fopen(core_address, "r");
    if (!core_file) {
        std::cout << "Failed to open file: " << core_address << std::endl;
        exit(1);
    }
    for (int i = 0; i < V; i++) {
        int u = read_number(core_file), core_value = read_number(core_file);
        core[u] = core_value;
    }
    fclose(core_file);
}

Graph::~Graph() {
    free(edges);
    free(undeg);
    for (int x = 0; x < V; x++) free(adj[x]);
    free(adj);
    free(idn);
    free(core);
}

void Graph::output() {
    if (true) {
        std::unordered_map<int, int> idn_count;
        for (int i = 0; i < V; i++) {
            idn_count[idn[i]]++;
        }
        for (const auto& pair : idn_count) {
            std::cout << "IDN: " << pair.first << ", Count: " << pair.second << std::endl;
        }
    }
}