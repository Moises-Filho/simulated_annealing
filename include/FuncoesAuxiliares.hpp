#ifndef FUNCOES_AUXILIARES_HPP
#define FUNCOES_AUXILIARES_HPP

#include <vector>
#include "TSPInstance.hpp"
#include <algorithm>
#include <cstdlib>
#include <random>

struct GraphData {
    int iteration;
    double temperature;
    double cost;
    double best_cost;
};

double calcularCusto(const std::vector<int>& rota, const TSPInstance& instance);
std::vector<int> gerarRotaInicial(const TSPInstance& instance);
std::vector<int> gerarVizinhaSwap(const std::vector<int>& rota);
std::vector<int> gerarVizinhaInsertion(const std::vector<int>& rota);

double calcularDeltaInsertion(const std::vector<int>& rota, int pos_remover, int pos_inserir, const TSPInstance& instance);
std::pair<std::vector<int>, double> gerarVizinhaInsertionComDelta(const std::vector<int>& rota, double custoAtual, const TSPInstance& instance);

double calcularDeltaSwap(const std::vector<int>& rota, int i, int j, const TSPInstance& instance);
std::pair<std::vector<int>, double> gerarVizinhaSwapComDelta(const std::vector<int>& rota, double custoAtual, const TSPInstance& instance);
double calcularCustoTotal(const TSPInstance& instance, const std::vector<int>& rota);

#endif
