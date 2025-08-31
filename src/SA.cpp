#include "../include/SA.hpp"
#include "../include/FuncoesAuxiliares.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

std::vector<int> SA::executar() {
    std::vector<int> melhorRota = gerarRotaInicial(instance);
    double melhorCusto = calcularCusto(melhorRota, instance);

    std::vector<int> rotaAtual = melhorRota;
    double custoAtual = melhorCusto;

    double temperatura = temperaturaInicial;
    int ctIteracao = 0;

    graph_data.clear();

    while (temperatura > 1.0) { // critério de parada
        for (int i = 0; i < iteracoesPorTemperatura; ++i) {
            auto [novaRota, delta] = gerarVizinha(rotaAtual, custoAtual, instance);
            double novoCusto = custoAtual + delta;

            if (delta < 0 || (std::exp(-delta / temperatura) > ((double) rand() / RAND_MAX))) {
                rotaAtual = novaRota;
                custoAtual = novoCusto;
            }

            if (custoAtual < melhorCusto) {
                melhorRota = rotaAtual;
                melhorCusto = custoAtual;
            }

            ctIteracao++;
            graph_data.push_back({ctIteracao, temperatura, custoAtual, melhorCusto});
        }

        temperatura *= taxaResfriamento; // resfriamento
    }

    std::cout << "Melhor custo encontrado: " << melhorCusto << std::endl;
    return melhorRota;
}
