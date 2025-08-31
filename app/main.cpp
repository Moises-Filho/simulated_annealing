#include "../include/FuncoesMain.hpp"
#include "../include/TSPInstance.hpp"
#include "../include/FuncoesAuxiliares.hpp"

int main() {
    try {
        TSPInstance instancia;
        instancia.loadFromFile(Config::instanciaFile);

        SA::VizinhancaFunc vizFunc = escolherVizinhanca();
        auto [melhorRota, graphData] = executarAlgoritmo(instancia, vizFunc);
        exibirResultados(instancia, melhorRota);

        salvarCSV("../output/resultado.csv", graphData);

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
