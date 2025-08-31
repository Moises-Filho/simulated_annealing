#ifndef SIMULATED_ANNEALING_HPP
#define SIMULATED_ANNEALING_HPP

#include "TSPInstance.hpp"
#include "FuncoesAuxiliares.hpp"
#include <vector>

class SimulatedAnnealing {
protected:
    const TSPInstance& instance;
    double temperaturaInicial;
    double taxaResfriamento;
    int iteracoesPorTemperatura;

    std::vector<GraphData> graph_data;

public:
    SimulatedAnnealing(const TSPInstance& instance, double tempInicial,
                       double taxaResfriamento, int iterPorTemp);

    virtual std::vector<int> executar() = 0; 
    
    const std::vector<GraphData>& getGraphData() const { return graph_data; }

    virtual ~SimulatedAnnealing();
};

#endif
