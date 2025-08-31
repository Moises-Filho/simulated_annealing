#ifndef SA_REAQUECIMENTO_HPP
#define SA_REAQUECIMENTO_HPP

#include "SimulatedAnnealing.hpp"
#include <functional> 

class SAReaquecimento : public SimulatedAnnealing {
    
public:
    using VizinhancaFunc = std::function<std::pair<std::vector<int>, double>(
        const std::vector<int>&, double, const TSPInstance&)>;

private:
    std::vector<double> startTemp, endTemp, coolingRate;
    std::vector<int> maxIters;
    VizinhancaFunc gerarVizinha;

public:
SAReaquecimento(const TSPInstance& instance,
                const std::vector<double>& startTemp,
                const std::vector<double>& endTemp,
                const std::vector<double>& coolingRate,
                const std::vector<int>& maxIters,
                VizinhancaFunc vizinhanca);

    const std::vector<GraphData>& getGraphData() const { return graph_data; }
    std::vector<int> executar() override;
};

#endif
