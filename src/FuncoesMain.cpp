#include "../include/FuncoesMain.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

// ===== Vizinhanca e execução já estavam aqui =====
SA::VizinhancaFunc escolherVizinhanca() {
    if (Config::vizinhanca == "Insertion")
        return gerarVizinhaInsertionComDelta;
    else if (Config::vizinhanca == "Swap")
        return gerarVizinhaSwapComDelta;
    else
        throw std::runtime_error("Vizinhança inválida!");
}

std::pair<std::vector<int>, std::vector<GraphData>> executarAlgoritmo(
    const TSPInstance& instancia, SA::VizinhancaFunc vizFunc) 
{
    std::vector<int> melhorRota;
    std::vector<GraphData> graphData;

    auto inicio = std::chrono::high_resolution_clock::now();

    if (Config::algoritmo == "SA") {
        SA sa(instancia, Config::sa_tempInicial, Config::sa_taxaResfriamento,
              Config::sa_iterPorTemp, vizFunc);
        melhorRota = sa.executar();
        graphData = sa.getGraphData();
    } 
    else if (Config::algoritmo == "SAReaquecimento") {
        SAReaquecimento saR(instancia, Config::startTemp, Config::endTemp,
                            Config::coolingRate, Config::maxIters, vizFunc);
        melhorRota = saR.executar();
        graphData = saR.getGraphData();
    } 
    else {
        throw std::runtime_error("Algoritmo inválido em Config.hpp!");
    }

    auto fim = std::chrono::high_resolution_clock::now();
    double tempoExec = std::chrono::duration<double>(fim - inicio).count();
    std::cout << "Tempo de execução: " << tempoExec << " segundos\n";

    return {melhorRota, graphData};
}

// ===== Exibir resultados =====
void exibirResultados(const TSPInstance& instancia, const std::vector<int>& melhorRota) {
    double custoObtido = calcularCustoTotal(instancia, melhorRota);
    auto& melhoresResultados = getMelhoresResultados();
    std::filesystem::path path(Config::instanciaFile);
    std::string nomeInstancia = path.stem().string();

    std::cout << "\nInstância: " << nomeInstancia << "\n";
    std::cout << "Custo obtido: " << custoObtido << "\n";
    std::cout << "Melhor rota encontrada:\n";
    for (int cidade : melhorRota) std::cout << cidade << " ";
    std::cout << "\n";

    if (melhoresResultados.count(nomeInstancia)) {
        double custoOtimo = melhoresResultados.at(nomeInstancia);
        double gap = calcularGap(custoObtido, custoOtimo);
        std::cout << "Custo ótimo conhecido: " << custoOtimo << "\n";
        std::cout << "Gap em relação ao ótimo conhecido: " << gap << "%\n";
    } else {
        std::cout << "Ótimo da instância não disponível.\n";
    }
}

// ===== CSV =====
void salvarCSV(const std::string& filename, const std::vector<GraphData>& dados) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << filename << std::endl;
        return;
    }

    std::cout << "Salvando " << dados.size() << " linhas no CSV..." << std::endl;

    file << "Iteration,Temperature,Cost,BestCost\n";
    for (const auto& d : dados) {
        file << d.iteration << ","
             << d.temperature << ","
             << d.cost << ","
             << d.best_cost << "\n";
    }

    file.close();
    std::cout << "Arquivo salvo em: " << filename << std::endl;
}

// ===== Gap =====
double calcularGap(double custoObtido, double custoOtimo) {
    return ((custoObtido - custoOtimo) / custoOtimo) * 100.0;
}

// ===== Melhores resultados =====
const std::unordered_map<std::string, double>& getMelhoresResultados() {
    static std::unordered_map<std::string, double> melhoresResultados = {
        {"a280", 2579},
        {"ali535", 202339},
        {"att48", 10628},
        {"att532", 27686},
        {"bayg29", 1610},
        {"bays29", 2020},
        {"berlin52", 7542},
        {"bier127", 118282},
        {"brazil58", 25395},
        {"brd14051", 469385},
        {"brg180", 1950},
        {"burma14", 3323},
        {"ch130", 6110},
        {"ch150", 6528},
        {"d198", 15780},
        {"d493", 35002},
        {"d657", 48912},
        {"d1291", 50801},
        {"d1655", 62128},
        {"d2103", 80450},
        {"d15112", 1573084},
        {"d18512", 645238},
        {"dantzig42", 699},
        {"dsj1000_EUC_2D", 18659688},
        {"dsj1000_CEIL_2D", 18660188},
        {"eil51", 426},
        {"eil76", 538},
        {"eil101", 629},
        {"fl417", 11861},
        {"fl1400", 20127},
        {"fl1577", 22249},
        {"fl3795", 28772},
        {"fnl4461", 182566},
        {"fri26", 937},
        {"gil262", 2378},
        {"gr17", 2085},
        {"gr21", 2707},
        {"gr24", 1272},
        {"gr48", 5046},
        {"gr96", 55209},
        {"gr120", 6942},
        {"gr137", 69853},
        {"gr202", 40160},
        {"gr229", 134602},
        {"gr431", 171414},
        {"gr666", 294358},
        {"hk48", 11461},
        {"kroA100", 21282},
        {"kroB100", 22141},
        {"kroC100", 20749},
        {"kroD100", 21294},
        {"kroE100", 22068},
        {"kroA150", 26524},
        {"kroB150", 26130},
        {"kroA200", 29368},
        {"kroB200", 29437},
        {"lin105", 14379},
        {"lin318", 42029},
        {"linhp318", 41345},
        {"nrw1379", 56638},
        {"p654", 34643},
        {"pa561", 2763},
        {"pcb442", 50778},
        {"pcb1173", 56892},
        {"pcb3038", 137694},
        {"pla7397", 23260728},
        {"pla33810", 66048945},
        {"pla85900", 142382641},
        {"pr76", 108159},
        {"pr107", 44303},
        {"pr124", 59030},
        {"pr136", 96772},
        {"pr144", 58537},
        {"pr152", 73682},
        {"pr226", 80369},
        {"pr264", 49135},
        {"pr299", 48191},
        {"pr439", 107217},
        {"pr1002", 259045},
        {"pr2392", 378032},
        {"rat99", 1211},
        {"rat195", 2323},
        {"rat575", 6773},
        {"rat783", 8806},
        {"rd100", 7910},
        {"rd400", 15281},
        {"rl1304", 252948},
        {"rl1323", 270199},
        {"rl1889", 316536},
        {"rl5915", 565530},
        {"rl5934", 556045},
        {"rl11849", 923288},
        {"si175", 21407},
        {"si535", 48450},
        {"si1032", 92650},
        {"st70", 675},
        {"swiss42", 1273},
        {"ts225", 126643},
        {"tsp225", 3916},
        {"u159", 42080},
        {"u574", 36905},
        {"u724", 41910},
        {"u1060", 224094},
        {"u1432", 152970},
        {"u1817", 57201},
        {"u2152", 64253},
        {"u2319", 234256},
        {"ulysses16", 6859},
        {"ulysses22", 7013},
        {"usa13509", 19982859},
        {"vm1084", 239297},
        {"vm1748", 336556}
    };
    return melhoresResultados;
}
