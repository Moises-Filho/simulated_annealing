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
#include <chrono>
#include "TSPInstance.h"

// Estrutura para armazenar dados para os gráficos
struct GraphData {
    std::vector<int> iterations;
    std::vector<double> temperatures;
    std::vector<double> best_costs;
    std::vector<double> current_costs;
};

double calcularCustoTotal(const TSPInstance& instance, const std::vector<int>& rota) {
    double custo = 0.0;
    for (size_t i = 0; i < rota.size() - 1; ++i) {
        custo += instance.getDistance(rota[i], rota[i + 1]);
    }
    custo += instance.getDistance(rota.back(), rota[0]);
    return custo;
}

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

std::vector<int> gerarRotaVizinha(const std::vector<int>& rota) {
    std::vector<int> novaRota = rota;
    size_t i = rand() % novaRota.size();
    size_t j = rand() % novaRota.size();
    while (i == j) {  // Garante que trocará cidades diferentes
        j = rand() % novaRota.size();
    }
    std::swap(novaRota[i], novaRota[j]);
    return novaRota;
}

std::vector<int> simulatedAnnealingWithReheating(const TSPInstance& instance, 
    double temperaturaInicial, double taxaResfriamento, 
    int iteracoesPorTemperatura, GraphData& graph_data,
    int maxReheating = 3, int stagnancyLimit = 10, 
    double reheatingFactor = 0.5) {

    // Inicializa estrutura de dados para gráficos
    graph_data.iterations.clear();
    graph_data.temperatures.clear();
    graph_data.best_costs.clear();
    graph_data.current_costs.clear();

    std::vector<int> melhorRota = gerarRotaInicial(instance.getDimension());
    double melhorCusto = calcularCustoTotal(instance, melhorRota);

    std::vector<int> rotaAtual = melhorRota;
    double custoAtual = melhorCusto;

    double temperatura = temperaturaInicial;
    int reheatingCount = 0;
    int stagnancyCounter = 0;
    double lastBestCost = melhorCusto;
    int totalIterations = 0;

    // Coleta o ponto inicial
    graph_data.iterations.push_back(totalIterations);
    graph_data.temperatures.push_back(temperatura);
    graph_data.best_costs.push_back(melhorCusto);
    graph_data.current_costs.push_back(custoAtual);

    while (temperatura > 1.0 && reheatingCount <= maxReheating) {
        bool improved = false;
        
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
                    improved = true;
                }
            }
            totalIterations++;

            // Coleta dados a cada 100 iterações (ajuste conforme necessário)
            if (totalIterations % 100 == 0) {
                graph_data.iterations.push_back(totalIterations);
                graph_data.temperatures.push_back(temperatura);
                graph_data.best_costs.push_back(melhorCusto);
                graph_data.current_costs.push_back(custoAtual);
            }
        }

        if (improved) {
            stagnancyCounter = 0;
            lastBestCost = melhorCusto;
        } else {
            stagnancyCounter++;
        }

        if (stagnancyCounter >= stagnancyLimit && reheatingCount < maxReheating) {
            temperatura = std::max(temperaturaInicial * reheatingFactor, temperatura * 2.0);
            reheatingCount++;
            stagnancyCounter = 0;
            std::cout << "Reaquecimento " << reheatingCount << "/" << maxReheating 
                      << " - Temperatura ajustada para: " << temperatura 
                      << " | Melhor custo atual: " << melhorCusto << std::endl;
            
            // Coleta dados após reaquecimento
            graph_data.iterations.push_back(totalIterations);
            graph_data.temperatures.push_back(temperatura);
            graph_data.best_costs.push_back(melhorCusto);
            graph_data.current_costs.push_back(custoAtual);
        } else {
            temperatura *= taxaResfriamento;
        }
    }

    // Coleta o ponto final
    graph_data.iterations.push_back(totalIterations);
    graph_data.temperatures.push_back(temperatura);
    graph_data.best_costs.push_back(melhorCusto);
    graph_data.current_costs.push_back(custoAtual);

    std::cout << "Total de iteracoes: " << totalIterations << std::endl;
    std::cout << "Reaquecimentos realizados: " << reheatingCount << std::endl;
    return melhorRota;
}

void saveGraphData(const GraphData& data, const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Erro ao abrir arquivo para salvar dados." << std::endl;
        return;
    }

    outfile << "Iteration,Temperature,BestCost,CurrentCost\n";
    for (size_t i = 0; i < data.iterations.size(); ++i) {
        outfile << data.iterations[i] << ","
                << data.temperatures[i] << ","
                << data.best_costs[i] << ","
                << data.current_costs[i] << "\n";
    }
    outfile.close();
    std::cout << "Dados para gráficos salvos em " << filename << std::endl;
}


int main() {
    srand(static_cast<unsigned int>(time(0)));

    TSPInstance instance;
    try {
        instance.loadFromFile("TSPlib/berlin52.tsp");
        if (instance.getType() != TSPInstance::ProblemType::TSP) {
            std::cerr << "Erro: O problema não é um TSP simétrico." << std::endl;
            return 1;
        }
        
        std::cout << "Instância carregada: " << instance.getName() 
                  << " | Cidades: " << instance.getDimension() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erro ao carregar arquivo: " << e.what() << std::endl;
        return 1;
    }

    // Parâmetros do SA
    double temperaturaInicial = 10000.0;
    double taxaResfriamento = 0.995;
    int iteracoesPorTemperatura = 10000;
    int maxReheating = 10;
    int stagnancyLimit = 20;
    double reheatingFactor = 0.4;

    // Estrutura para armazenar dados dos gráficos
    GraphData graph_data;

    auto inicio = std::chrono::high_resolution_clock::now();

    std::vector<int> melhorRota = simulatedAnnealingWithReheating(
        instance, temperaturaInicial, taxaResfriamento, 
        iteracoesPorTemperatura, graph_data,
        maxReheating, stagnancyLimit, reheatingFactor);

    auto fim = std::chrono::high_resolution_clock::now();
    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);

    // Salvar dados e gerar script de plotagem
    saveGraphData(graph_data, "sa_data.csv");
 

    std::cout << "\nMelhor rota encontrada (" << instance.getDimension() << " cidades):\n";
    for (size_t i = 0; i < std::min(melhorRota.size(), size_t(10)); ++i) {
        std::cout << melhorRota[i] + 1 << " ";
    }
    if (melhorRota.size() > 10) std::cout << "...";
    std::cout << melhorRota[0] + 1 << std::endl;
    
    std::cout << "\nCusto total: " << calcularCustoTotal(instance, melhorRota) << std::endl;
    std::cout << "Tempo de execução: " << duracao.count() / 1000.0 << " segundos" << std::endl;

    std::cout << "\nPara gerar os gráficos, execute: python plot_sa.py" << std::endl;

    return 0;
}