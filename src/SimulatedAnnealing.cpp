#include "../include/SimulatedAnnealing.hpp"

SimulatedAnnealing::SimulatedAnnealing(const TSPInstance& instance, double tempInicial,
                                       double taxaResfriamento, int iterPorTemp)
    : instance(instance),
      temperaturaInicial(tempInicial),
      taxaResfriamento(taxaResfriamento),
      iteracoesPorTemperatura(iterPorTemp)
{}

SimulatedAnnealing::~SimulatedAnnealing() = default;
