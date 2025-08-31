#include "../include/FuncoesAuxiliares.hpp"
#include <fstream>
#include <iostream>


double calcularCusto(const std::vector<int>& rota, const TSPInstance& instance) {
    double custo = 0.0;
    for (size_t i = 0; i < rota.size() - 1; ++i)
        custo += instance.getDistance(rota[i], rota[i + 1]);
    custo += instance.getDistance(rota.back(), rota[0]);
    return custo;
}

std::vector<int> gerarRotaInicial(const TSPInstance& instance) {
    std::vector<int> rota(instance.getDimension());
    for (size_t i = 0; i < rota.size(); ++i) rota[i] = i;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(rota.begin(), rota.end(), g);
    return rota;
}


std::vector<int> gerarVizinhaSwap(const std::vector<int>& rota) {
    std::vector<int> nova = rota;
    size_t i = rand() % rota.size();
    size_t j = rand() % rota.size();
    while (i == j) j = rand() % rota.size();
    std::swap(nova[i], nova[j]);
    return nova;
}

std::vector<int> gerarVizinhaInsertion(const std::vector<int>& rota) {
    std::vector<int> nova = rota;
    size_t i = rand() % rota.size();
    size_t j = rand() % rota.size();
    while (i == j) j = rand() % rota.size();

    int cidade = nova[i];
    nova.erase(nova.begin() + i);
    nova.insert(nova.begin() + j, cidade);
    return nova;
}



double calcularDeltaInsertion(const std::vector<int>& rota, int pos_remover, int pos_inserir, const TSPInstance& instance) {
    int n = rota.size();
    if (pos_remover == pos_inserir) return 0;

    auto mod = [n](int x) { return (x + n) % n; };

    int cidade = rota[pos_remover];
    int antes_remover = rota[mod(pos_remover - 1)];
    int depois_remover = rota[mod(pos_remover + 1)];

    double custo_antes = instance.getDistance(antes_remover, cidade) +
                         instance.getDistance(cidade, depois_remover);
    double custo_depois = instance.getDistance(antes_remover, depois_remover);

    std::vector<int> nova_rota = rota;
    nova_rota.erase(nova_rota.begin() + pos_remover);
    nova_rota.insert(nova_rota.begin() + pos_inserir, cidade);

    int antes_inserir = nova_rota[mod(pos_inserir - 1)];
    int depois_inserir = nova_rota[mod(pos_inserir + 1)];

    double custo_antigo = instance.getDistance(antes_inserir, depois_inserir);
    double custo_novo = instance.getDistance(antes_inserir, cidade) +
                        instance.getDistance(cidade, depois_inserir);

    return (custo_depois - custo_antes) + (custo_novo - custo_antigo);
}

std::pair<std::vector<int>, double> gerarVizinhaInsertionComDelta(const std::vector<int>& rota, double custoAtual, const TSPInstance& instance) {
    int n = rota.size();
    if (n <= 1) return {rota, 0.0};

    int pos_remover = rand() % n;
    int pos_inserir = rand() % (n - 1);
    while (pos_inserir == pos_remover) {
        pos_inserir = rand() % (n - 1);
    }

    double delta = calcularDeltaInsertion(rota, pos_remover, pos_inserir, instance);

    std::vector<int> nova_rota = rota;
    int cidade = nova_rota[pos_remover];
    nova_rota.erase(nova_rota.begin() + pos_remover);
    nova_rota.insert(nova_rota.begin() + pos_inserir, cidade);

    return {nova_rota, delta};
}

double calcularDeltaSwap(const std::vector<int>& rota, int i, int j, const TSPInstance& instance) {
    int n = rota.size();
    if (i == j) return 0.0;
    if (i > j) std::swap(i, j);

    auto mod = [n](int x) { return (x + n) % n; };

    int a = rota[i];
    int b = rota[j];

    int a_ant = rota[mod(i - 1)];
    int a_prox = rota[mod(i + 1)];
    int b_ant = rota[mod(j - 1)];
    int b_prox = rota[mod(j + 1)];

    double custoAntes = 0.0;
    double custoDepois = 0.0;

    if (i + 1 == j) {
        custoAntes += instance.getDistance(a_ant, a) + instance.getDistance(b, b_prox);
        custoAntes += instance.getDistance(a, b);

        custoDepois += instance.getDistance(a_ant, b) + instance.getDistance(a, b_prox);
        custoDepois += instance.getDistance(b, a);
    } else {
        custoAntes += instance.getDistance(a_ant, a) + instance.getDistance(a, a_prox);
        custoAntes += instance.getDistance(b_ant, b) + instance.getDistance(b, b_prox);

        custoDepois += instance.getDistance(a_ant, b) + instance.getDistance(b, a_prox);
        custoDepois += instance.getDistance(b_ant, a) + instance.getDistance(a, b_prox);
    }

    return custoDepois - custoAntes;
}

std::pair<std::vector<int>, double> gerarVizinhaSwapComDelta(const std::vector<int>& rota, double custoAtual, const TSPInstance& instance) {
    int n = rota.size();
    if (n <= 1) return {rota, 0.0};

    int i = rand() % n;
    int j = rand() % n;
    while (i == j) j = rand() % n;

    double delta = calcularDeltaSwap(rota, i, j, instance);

    std::vector<int> nova_rota = rota;
    std::swap(nova_rota[i], nova_rota[j]);

    return {nova_rota, delta};
}


double calcularCustoTotal(const TSPInstance& instance, const std::vector<int>& rota) {
    double custo = 0.0;
    for (size_t i = 0; i < rota.size() - 1; ++i) {
        custo += instance.getDistance(rota[i], rota[i + 1]);
    }
    custo += instance.getDistance(rota.back(), rota[0]); // volta ao início
    return custo;
}
