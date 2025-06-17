#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <chrono>
#include "TSPInstance.h"

using namespace std;

double calcularCustoTotal(const TSPInstance& instance, const vector<int>& rota) {
    double custo = 0.0;
    for (size_t i = 0; i < rota.size() - 1; ++i) {
        custo += instance.getDistance(rota[i], rota[i + 1]);
    }
    custo += instance.getDistance(rota.back(), rota[0]);
    return custo;
}

vector<int> gerarRotaInicial(size_t numCidades) {
    vector<int> rota(numCidades);
    for (size_t i = 0; i < numCidades; ++i) {
        rota[i] = i;
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(rota.begin(), rota.end(), g);
    return rota;
}

vector<int> gerarRotaVizinha(const vector<int>& rota) {
    vector<int> novaRota = rota;
    size_t i = rand() % novaRota.size();
    size_t j = rand() % novaRota.size();
    while (i == j) {
        j = rand() % novaRota.size();
    }
    swap(novaRota[i], novaRota[j]);
    return novaRota;
}

vector<int> simulatedAnnealingWithFixedReheating(const TSPInstance& instance) {
    // Parâmetros fixos do algoritmo
    const vector<double> reheatingTemps = {1000.0, 500.0, 333.0}; // T1, T2, T3
    const double coolingRate = 0.995; // Taxa de resfriamento normal
    const double reheatingRate = 0.005; // 0.5% de taxa de reaquecimento
    const int maxIterationsPerTemp = 1000; // Máximo de iterações por temperatura
    const int maxReheating = reheatingTemps.size(); // Número de reaquecimentos

    // Inicialização
    vector<int> melhorRota = gerarRotaInicial(instance.getDimension());
    double melhorCusto = calcularCustoTotal(instance, melhorRota);
    vector<int> rotaAtual = melhorRota;
    double custoAtual = melhorCusto;

    int totalIterations = 0;
    int reheatingCount = 0;
    double temperatura = reheatingTemps[0]; // Começa em T1

    cout << "Iniciando Simulated Annealing com reaquecimento controlado" << endl;
    cout << "Temperaturas de reaquecimento: ";
    for (double temp : reheatingTemps) cout << temp << " ";
    cout << endl;

   
    while (reheatingCount < maxReheating) {
        cout << "\nFase " << reheatingCount + 1 << " - Temperatura: " << temperatura << endl;

        int iteracoesSemMelhora = 0;
        bool improved = false;

        for (int i = 0; i < maxIterationsPerTemp; ++i) {
            vector<int> novaRota = gerarRotaVizinha(rotaAtual);
            double novoCusto = calcularCustoTotal(instance, novaRota);
            double deltaCusto = novoCusto - custoAtual;

            // Critério de aceitação
            if (deltaCusto < 0 || exp(-deltaCusto / temperatura) > ((double)rand() / RAND_MAX)) {
                rotaAtual = novaRota;
                custoAtual = novoCusto;

                if (custoAtual < melhorCusto) {
                    melhorRota = rotaAtual;
                    melhorCusto = custoAtual;
                    improved = true;
                    iteracoesSemMelhora = 0;
                    cout << "Melhora encontrada: " << melhorCusto 
                         << " (Iteração: " << totalIterations << ")" << endl;
                }
            }

            iteracoesSemMelhora++;
            totalIterations++;

            // Resfriamento durante cada fase
            temperatura *= coolingRate;
        }

        reheatingCount++;
        if (reheatingCount < maxReheating) {
            // Aplica reaquecimento para a próxima temperatura específica
            temperatura = reheatingTemps[reheatingCount];
            // Aplica taxa de reaquecimento adicional
            temperatura *= (1.0 + reheatingRate);
            cout << "Reaquecimento aplicado. Nova temperatura: " << temperatura << endl;
        }
    }

    cout << "\nExecução finalizada." << endl;
    cout << "Total de iterações: " << totalIterations << endl;
    cout << "Reaquecimentos realizados: " << reheatingCount << endl;
    cout << "Melhor custo encontrado: " << melhorCusto << endl;

    return melhorRota;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    TSPInstance instance;
    try {
        instance.loadFromFile("TSPlib/berlin52.tsp");
        if (instance.getType() != TSPInstance::ProblemType::TSP) {
            cerr << "Erro: O problema não é um TSP simétrico." << endl;
            return 1;
        }
        
        cout << "Instância carregada: " << instance.getName() 
             << " | Cidades: " << instance.getDimension() << endl;
    } catch (const exception& e) {
        cerr << "Erro ao carregar arquivo: " << e.what() << endl;
        return 1;
    }

    auto inicio = chrono::high_resolution_clock::now();

    vector<int> melhorRota = simulatedAnnealingWithFixedReheating(instance);

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);

    cout << "\nMelhor rota encontrada (" << instance.getDimension() << " cidades):\n";
    for (size_t i = 0; i < min(melhorRota.size(), size_t(10)); ++i) {
        cout << melhorRota[i] + 1 << " ";
    }
    if (melhorRota.size() > 10) cout << "...";
    cout << melhorRota[0] + 1 << endl;
    
    cout << "\nCusto total: " << calcularCustoTotal(instance, melhorRota) << endl;
    cout << "Tempo de execução: " << duracao.count() / 1000.0 << " segundos" << endl;

    return 0;
}