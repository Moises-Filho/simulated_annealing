#ifndef SIMULATED_ANNEALING_HPP
#define SIMULATED_ANNEALING_HPP

#include "TSPInstance.hpp"
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

class SimulatedAnnealing {
protected:
    const TSPInstance& instance;
    double temperaturaInicial;
    double taxaResfriamento;
    int iteracoesPorTemperatura;

    // Dados para gráfico: pares (iteração, custo)
    std::vector<std::pair<int, double>> historicoCusto;

public:
    SimulatedAnnealing(const TSPInstance& inst, double tempInicial, double taxaResf, int iteracoes)
        : instance(inst), temperaturaInicial(tempInicial), taxaResfriamento(taxaResf),
          iteracoesPorTemperatura(iteracoes) {}

    virtual ~SimulatedAnnealing() = default;

    double calcularCusto(const std::vector<int>& rota) const {
        double custo = 0.0;
        for (size_t i = 0; i < rota.size() - 1; ++i)
            custo += instance.getDistance(rota[i], rota[i + 1]);
        custo += instance.getDistance(rota.back(), rota[0]);
        return custo;
    }
    std::vector<int> gerarRotaInicial() const {
        std::vector<int> rota(instance.getDimension());
        for (size_t i = 0; i < rota.size(); ++i) rota[i] = i;
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(rota.begin(), rota.end(), g);
        return rota;
    }   
    // metodo do Insertion Move
    std::vector<int> gerarVizinhaInsertion(const std::vector<int>& rota) const {
        std::vector<int> nova = rota;
        size_t n = nova.size();
        
        if (n <= 1) return nova;
        
        // Seleciona uma posicao aleatoria para remover
        size_t pos_remover = rand() % n;
        int cidade = nova[pos_remover];
        
        // Remove a cidade
        nova.erase(nova.begin() + pos_remover);
        
        // Seleciona uma nova posicao para inserir (tem que ser diferente da original)
        size_t pos_inserir = rand() % (n - 1);
        while (pos_inserir == pos_remover) {
            pos_inserir = rand() % (n - 1);
        }
        
        // Insere a cidade na nova posição
        nova.insert(nova.begin() + pos_inserir, cidade);
        
        return nova;
    }

    // metodo do swap
    std::vector<int> gerarVizinhaSwap(const std::vector<int>& rota) const {
        std::vector<int> nova = rota;
        size_t i = rand() % rota.size();
        size_t j = rand() % rota.size();
        while (i == j) j = rand() % rota.size(); // evita troca com ela mesma
        std::swap(nova[i], nova[j]);
        return nova;
    }

    // Salvar custo por iteração para gráfico
    void registrarHistorico(int iteracao, double custo) {
        historicoCusto.emplace_back(iteracao, custo);
    }

    const std::vector<std::pair<int, double>>& getHistoricoCusto() const {
        return historicoCusto;
    }

    virtual std::vector<int> executar() = 0;
};

#endif
