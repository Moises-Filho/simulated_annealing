#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
#include <random>
#include <fstream>
#include <sstream>
#include <string>
#include "TSPInstance.h"


double calcularCustoTotal(const TSPInstance& instance, const std::vector<int>& rota) {
    double custo = 0.0;
    for (size_t i = 0; i < rota.size() - 1; ++i) {
        custo += instance.getDistance(rota[i], rota[i + 1]);
    }
    // Voltar para a cidade inicial
    custo += instance.getDistance(rota.back(), rota[0]);
    return custo;
}

// Função para gerar uma rota inicial aleatória
std::vector<int> gerarRotaInicial(size_t numCidades) {
    std::vector<int> rota(numCidades);
    for (size_t i = 0; i < numCidades; ++i) {
        rota[i] = i;
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(rota.begin(), rota.end(), g);
    return rota;
}

// Função para gerar uma rota vizinha (troca duas cidades aleatórias)
std::vector<int> gerarRotaVizinha(const std::vector<int>& rota) {
    std::vector<int> novaRota = rota;
    size_t i = rand() % novaRota.size();
    size_t j = rand() % novaRota.size();
    std::swap(novaRota[i], novaRota[j]);
    return novaRota;
}

// Função Simulated Annealing
std::vector<int> simulatedAnnealing(const TSPInstance& instance, double temperaturaInicial, 
    double taxaResfriamento, int iteracoesPorTemperatura) {

    std::vector<int> melhorRota = gerarRotaInicial(instance.getDimension());
    double melhorCusto = calcularCustoTotal(instance, melhorRota);

    std::vector<int> rotaAtual = melhorRota;
    double custoAtual = melhorCusto;

    double temperatura = temperaturaInicial;

    while (temperatura > 1.0) {
        for (int i = 0; i < iteracoesPorTemperatura; ++i) {
            std::vector<int> novaRota = gerarRotaVizinha(rotaAtual);
            double novoCusto = calcularCustoTotal(instance, novaRota);

            double deltaCusto = novoCusto - custoAtual;

            if (deltaCusto < 0 || (std::exp(-deltaCusto / temperatura) > ((double) rand() / RAND_MAX))) {
                rotaAtual = novaRota;
                custoAtual = novoCusto;

                if (custoAtual < melhorCusto) {
                    melhorRota = rotaAtual;
                    melhorCusto = custoAtual;
                }
            }
        }
        temperatura *= taxaResfriamento;
    }

    return melhorRota;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));


    TSPInstance instance;
    try {
        // instance.loadFromFile("TSPlib/berlin52.tsp");
        // instance.loadFromFile("TSPlib/brazil58.tsp");
        // instance.loadFromFile("TSPlib/a280.tsp");
        instance.loadFromFile("TSPlib/gr229.tsp");


        // Verificar se é um problema simétrico
        if (instance.getType() != TSPInstance::ProblemType::TSP) {
            std::cerr << "Erro: O problema não é um TSP simétrico." << std::endl;
            return 1;
        }
        
        std::cout << "Instância carregada com sucesso: " << instance.getName() << std::endl;
        std::cout << "Número de cidades: " << instance.getDimension() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erro ao carregar arquivo: " << e.what() << std::endl;
        return 1;
    }

    // Parâmetros do Simulated Annealing
    double temperaturaInicial = 10000.0;
    double taxaResfriamento = 0.995;
    int iteracoesPorTemperatura = 10000;

    // Executar o Simulated Annealing
    std::vector<int> melhorRota = simulatedAnnealing(instance, temperaturaInicial, taxaResfriamento, iteracoesPorTemperatura);

    // Exibir a melhor rota encontrada
    std::cout << "Melhor rota encontrada: ";
    for (size_t i = 0; i < melhorRota.size(); ++i) {
        std::cout << melhorRota[i] + 1 << " ";
    }
    std::cout << melhorRota[0] + 1 << std::endl;
    std::cout << "\nCusto total: " << calcularCustoTotal(instance, melhorRota) << std::endl;

    return 0;
}