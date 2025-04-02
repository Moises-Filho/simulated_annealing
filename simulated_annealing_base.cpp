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

// Estrutura para representar uma cidade com coordenadas (x, y)
struct Cidade {
    int id;
    double x;
    double y;
};

// Função para calcular a distância  entre duas cidades
double calcularDistancia(const Cidade& cidade1, const Cidade& cidade2) {
    double dx = cidade1.x - cidade2.x;
    double dy = cidade1.y - cidade2.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Função para carregar as cidades de um arquivo TSPLIB
std::vector<Cidade> carregarCidadesDoArquivo(const std::string& caminhoArquivo) {
    std::vector<Cidade> cidades;
    std::ifstream arquivo(caminhoArquivo);

    if (!arquivo.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << caminhoArquivo << std::endl;
        return cidades;
    }

    std::string linha;
    bool naSecaoCoordenadas = false;

    while (std::getline(arquivo, linha)) {
        if (linha.find("NODE_COORD_SECTION") != std::string::npos) {
            naSecaoCoordenadas = true;
            continue;
        }

        if (linha.find("EOF") != std::string::npos) {
            break;
        }

        if (naSecaoCoordenadas) {
            std::istringstream iss(linha);
            Cidade cidade;
            iss >> cidade.id >> cidade.x >> cidade.y;
            cidades.push_back(cidade);
        }
    }

    arquivo.close();
    return cidades;
}


// Função para calcular o custo total de um caminho (rota)
double calcularCustoTotal(const std::vector<Cidade>& cidades, const std::vector<int>& rota) {
    double custo = 0.0;
    for (size_t i = 0; i < rota.size() - 1; ++i) {
        int cidadeAtual = rota[i] - 1; // Ajusta para índice 0-based
        int proximaCidade = rota[i + 1] - 1;
        custo += calcularDistancia(cidades[cidadeAtual], cidades[proximaCidade]);
    }
    // Voltar para a cidade inicial
    custo += calcularDistancia(cidades[rota.back() - 1], cidades[rota[0] - 1]);
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
std::vector<int> simulatedAnnealing(const std::vector<Cidade>& cidades, double temperaturaInicial, double taxaResfriamento, int iteracoesPorTemperatura) {
    std::vector<int> melhorRota = gerarRotaInicial(cidades.size());
    double melhorCusto = calcularCustoTotal(cidades, melhorRota);

    std::vector<int> rotaAtual = melhorRota;
    double custoAtual = melhorCusto;

    double temperatura = temperaturaInicial;

    while (temperatura > 1.0) {
        for (int i = 0; i < iteracoesPorTemperatura; ++i) {
            std::vector<int> novaRota = gerarRotaVizinha(rotaAtual);
            double novoCusto = calcularCustoTotal(cidades, novaRota);

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

    // Carregar as cidades de um arquivo TSPLIB
    // std::string caminhoArquivo = "TSPlib/ali535.tsp";
    std::string caminhoArquivo = "TSPlib/berlin52.tsp";

    std::vector<Cidade> cidades = carregarCidadesDoArquivo(caminhoArquivo);

    if (cidades.empty()) {
        std::cerr << "Erro: Nenhuma cidade foi carregada. Verifique o arquivo TSPLIB." << std::endl;
        return 1;
    }

    std::cout << "Cidades carregadas com sucesso." << std::endl;

    // Parâmetros do Simulated Annealing
    double temperaturaInicial = 10000.0;
    double taxaResfriamento = 0.995;
    int iteracoesPorTemperatura = 10000;

    // Executar o Simulated Annealing
    std::vector<int> melhorRota = simulatedAnnealing(cidades, temperaturaInicial, taxaResfriamento, iteracoesPorTemperatura);

    // Exibir a melhor rota encontrada
    std::cout << "Melhor rota encontrada: ";
    for (int cidade : melhorRota) {
        std::cout << cidade + 1 << " "; // Ajustar o ID para começar em 1
    }
    std::cout << "\nCusto total: " << calcularCustoTotal(cidades, melhorRota) << std::endl;

    return 0;
}