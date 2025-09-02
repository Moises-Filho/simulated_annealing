#include "../include/SAReaquecimento.hpp"
#include "../include/FuncoesAuxiliares.hpp"
#include <iostream>
#include <functional>

SAReaquecimento::SAReaquecimento(
    const TSPInstance& instance,
    const std::vector<double>& startTemp,
    const std::vector<double>& endTemp,
    const std::vector<double>& coolingRate,
    const std::vector<int>& maxIters,
    VizinhancaFunc vizinhanca
) : SimulatedAnnealing(instance, startTemp[0], coolingRate[0], maxIters[0]),
    startTemp(startTemp),
    endTemp(endTemp),
    coolingRate(coolingRate),
    maxIters(maxIters),
    gerarVizinha(vizinhanca) {}
    
std::vector<int> SAReaquecimento::executar() {
    int maxReheating = startTemp.size();

    std::vector<int> melhorRota = gerarRotaInicial(instance);
    double melhorCusto = calcularCusto(melhorRota, instance);

    std::vector<int> rotaAtual = melhorRota;
    double custoAtual = melhorCusto;

    int ctIteracao = 0;
    graph_data.clear();


    for (int fase = 0; fase < maxReheating; ++fase) {
        double temperatura = startTemp[fase];

        while (temperatura > endTemp[fase]) {
            ctIteracao++;

            std::vector<int> melhorRotaFase;
            double melhorCustoFase = -1;

            for (int i = 0; i < maxIters[fase]; ++i) {
                // Use vizinha com delta
                auto [novaRota, delta] = gerarVizinha(rotaAtual, custoAtual, instance);
                double novoCusto = custoAtual + delta;

                if (melhorCustoFase == -1 || delta < 0 || (std::exp(-delta / temperatura) > ((double) rand() / RAND_MAX))) {
                    rotaAtual = novaRota;
                    custoAtual = novoCusto;

                    if (melhorCustoFase == -1 || novoCusto < melhorCustoFase) {
                        melhorRotaFase = rotaAtual;
                        melhorCustoFase = custoAtual;
                    }
                }
            }

            if (melhorCustoFase < melhorCusto) {
                melhorRota = melhorRotaFase;
                melhorCusto = melhorCustoFase;
            }
            temperatura *= coolingRate[fase];
            graph_data.push_back({ctIteracao, temperatura, melhorCustoFase, melhorCusto});
        }

        std::cout << "Reaquecimento aplicado. Nova temperatura: " << startTemp[fase] << std::endl;
    }

    std::cout << "Melhor custo encontrado: " << melhorCusto << std::endl;
    return melhorRota;
}

