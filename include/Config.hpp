#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <string>

namespace Config {

    // Escolha do algoritmo: "SA" ou "SAReaquecimento"
    const std::string algoritmo = "SA";

    // Caminho da instância TSP
    const std::string instanciaFile = "../data/TSPlib/berlin52.tsp";

    // Tipo de vizinhança: "Insertion" ou "Swap"
    const std::string vizinhanca = "Insertion";

    // Parâmetros do SA normal
    const double sa_tempInicial      = 1000.0;
    const double sa_taxaResfriamento = 0.995;
    const int    sa_iterPorTemp      = 1000;

    // Parâmetros do SA com reaquecimento
    const std::vector<double> startTemp   = {1000.0, 500.0, 1000.0, 250.0};
    const std::vector<double> endTemp     = {100.0, 50.0, 25.0, 1.0};
    const std::vector<double> coolingRate = {0.995, 0.99, 0.98, 0.9};
    const std::vector<int>    maxIters    = {1000, 1000, 1000, 1000};

}

#endif
