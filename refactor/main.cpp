#include <iostream>
#include <chrono>
#include "TSPInstance.hpp"
#include "SimulatedAnnealing.hpp"

// SA padrão (sem reaquecimento)
class SAComum : public SimulatedAnnealing {
public:
    using SimulatedAnnealing::SimulatedAnnealing;

    std::vector<int> executar() override {
        std::vector<int> melhor = gerarRotaInicial();
        double melhorCusto = calcularCusto(melhor);

        std::vector<int> atual = melhor;
        double custoAtual = melhorCusto;
        double temperatura = temperaturaInicial;

        int totalIteracoes = 0; // para grafico

        while (temperatura > 1.0) {
            for (int i = 0; i < iteracoesPorTemperatura; ++i) {
                auto vizinha = gerarVizinha(atual);
                double custoNovo = calcularCusto(vizinha);
                double delta = custoNovo - custoAtual;

                if (delta < 0 || exp(-delta / temperatura) > ((double) rand() / RAND_MAX)) {
                    atual = vizinha;
                    custoAtual = custoNovo;

                    if (custoAtual < melhorCusto) {
                        melhor = atual;
                        melhorCusto = custoAtual;
                    }
                }
                registrarHistorico(totalIteracoes,melhorCusto);
                ++totalIteracoes;
            }
            temperatura *= taxaResfriamento;
        }

        return melhor;
    }
};

// SA reheating (com reaquecimento)
class SAReaquecimentoFixo : public SimulatedAnnealing {
public:
    using SimulatedAnnealing::SimulatedAnnealing;

    std::vector<int> executar() override {
        const std::vector<double> tempsReaquecimento = {1000.0, 500.0, 333.0};
        const double taxaReaquecimentoExtra = 0.005;
        const int maxReaquecimentos = tempsReaquecimento.size();

        std::vector<int> melhorRota = gerarRotaInicial();
        double melhorCusto = calcularCusto(melhorRota);

        std::vector<int> atual = melhorRota;
        double custoAtual = melhorCusto;

        double temperatura = tempsReaquecimento[0];
        int totalIteracoes = 0;
        int fase = 0;

        while (fase < maxReaquecimentos) {
            for (int i = 0; i < iteracoesPorTemperatura; ++i) {
                auto vizinha = gerarVizinha(atual);
                double novoCusto = calcularCusto(vizinha);
                double delta = novoCusto - custoAtual;

                if (delta < 0 || std::exp(-delta / temperatura) > ((double)rand() / RAND_MAX)) {
                    atual = vizinha;
                    custoAtual = novoCusto;

                    if (custoAtual < melhorCusto) {
                        melhorRota = atual;
                        melhorCusto = custoAtual;
                        std::cout << "Melhora: " << melhorCusto
                                  << " (Iteração " << totalIteracoes << ")\n";
                    }
                }

                registrarHistorico(totalIteracoes, melhorCusto);
                ++totalIteracoes;
                temperatura *= taxaResfriamento;
            }

            ++fase;
            if (fase < maxReaquecimentos) {
                temperatura = tempsReaquecimento[fase] * (1.0 + taxaReaquecimentoExtra);
                std::cout << "Reaquecimento aplicado! Nova temperatura: " << temperatura << "\n";
            }
        }

        return melhorRota;
    }
};


int main() {
    srand(static_cast<unsigned int>(time(0)));

    TSPInstance instance;
    instance.loadFromFile("../TSPlib/berlin52.tsp");

    double tempInicial = 10000.0;
    double taxaResfriamento = 0.995;
    int iteracoesPorTemperatura = 10000;

    //descomentar caso necessario
    SAComum solver(instance, tempInicial, taxaResfriamento, iteracoesPorTemperatura);
    std::string nomeArquivo = "grafico_comum.csv";

    // SAReaquecimentoFixo solver(instance, tempInicial, taxaResfriamento, iteracoesPorTemperatura);
    // std::string nomeArquivo = "grafico_reaquecimento.csv";
    
    auto inicio = std::chrono::high_resolution_clock::now();
    auto melhorRota = solver.executar();
    auto fim = std::chrono::high_resolution_clock::now();

    std::cout << "Melhor rota: ";
    for (int cidade : melhorRota) std::cout << cidade + 1 << " ";
    std::cout << melhorRota[0] + 1 << "\n";

    std::cout << "Custo total: " << solver.calcularCusto(melhorRota) << "\n";
    std::cout << "Tempo: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count()
              << " ms\n";

    //Dados do grafico
    std::ofstream out("grafico_dados.csv");
    if (out.is_open()) {
        out << "Iteracao,Custo\n";
        for (const auto& [it, custo] : solver.getHistoricoCusto()) {
            out << it << "," << custo << "\n";
        }
        out.close();
        std::cout << "Arquivo grafico_dados.csv gerado com sucesso.\n";
    } else {
        std::cerr << "Erro ao criar arquivo grafico_dados.csv\n";
    }

    return 0;
}

