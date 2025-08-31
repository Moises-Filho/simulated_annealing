#ifndef FUNCOESMAIN_HPP
#define FUNCOESMAIN_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include "../include/TSPInstance.hpp"
#include "../include/SA.hpp"
#include "../include/SAReaquecimento.hpp"
#include "../include/FuncoesAuxiliares.hpp"
#include "../include/Config.hpp"

// Seleciona a função de vizinhança
SA::VizinhancaFunc escolherVizinhanca();

// Executa o algoritmo e retorna melhor rota + dados do gráfico
std::pair<std::vector<int>, std::vector<GraphData>> executarAlgoritmo(
    const TSPInstance& instancia, SA::VizinhancaFunc vizFunc);

// Exibe resultados: custo, gap, instância
void exibirResultados(const TSPInstance& instancia, const std::vector<int>& melhorRota);

// Salva os dados de execução em CSV
void salvarCSV(const std::string& filename, const std::vector<GraphData>& dados);

// Calcula o gap percentual
double calcularGap(double custoObtido, double custoOtimo);

// Retorna mapa com os melhores resultados conhecidos
const std::unordered_map<std::string, double>& getMelhoresResultados();

#endif
