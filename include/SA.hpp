#ifndef SA_HPP
#define SA_HPP
#include <functional> 
#include "SimulatedAnnealing.hpp"

class SA : public SimulatedAnnealing {
public:
    using VizinhancaFunc = std::function<std::pair<std::vector<int>, double>(
        const std::vector<int>&, double, const TSPInstance&)>;

private:
    VizinhancaFunc gerarVizinha;

public:
    SA(const TSPInstance& instance, double tempInicial, double taxaResfriamento,
       int iterPorTemp, VizinhancaFunc vizinhanca)
       : SimulatedAnnealing(instance, tempInicial, taxaResfriamento, iterPorTemp),
         gerarVizinha(vizinhanca) {}

    std::vector<int> executar() override;
};

#endif
