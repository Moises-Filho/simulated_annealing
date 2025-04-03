#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include <numeric>

class TSPInstance {
public:
    struct City {
        int id;
        double x;
        double y;
        double z;
    };

    enum class ProblemType { TSP, ATSP, HCP, SOP, CVRP, UNKNOWN };
    enum class EdgeWeightType { EXPLICIT, EUC_2D, EUC_3D, MAX_2D, MAX_3D, MAN_2D, MAN_3D, CEIL_2D, GEO, ATT, XRAY1, XRAY2, SPECIAL, UNKNOWN };
    enum class EdgeWeightFormat { FUNCTION, FULL_MATRIX, UPPER_ROW, LOWER_ROW, UPPER_DIAG_ROW, LOWER_DIAG_ROW, UPPER_COL, LOWER_COL, UPPER_DIAG_COL, LOWER_DIAG_COL, UNKNOWN };

private:
    ProblemType type;
    std::string name;
    std::string comment;
    int dimension;
    EdgeWeightType edge_weight_type;
    EdgeWeightFormat edge_weight_format;
    std::vector<City> cities;
    std::vector<std::vector<int>> edge_weights;
    std::vector<std::vector<int>> fixed_edges;
    int capacity;
    std::vector<int> demands;
    std::vector<int> depots;

    std::unordered_map<std::string, ProblemType> problemTypeMap = {
        {"TSP", ProblemType::TSP}, {"ATSP", ProblemType::ATSP},
        {"HCP", ProblemType::HCP}, {"SOP", ProblemType::SOP},
        {"CVRP", ProblemType::CVRP}
    };

    std::unordered_map<std::string, EdgeWeightType> edgeWeightTypeMap = {
        {"EXPLICIT", EdgeWeightType::EXPLICIT},
        {"EUC_2D", EdgeWeightType::EUC_2D}, {"EUC_3D", EdgeWeightType::EUC_3D},
        {"MAX_2D", EdgeWeightType::MAX_2D}, {"MAX_3D", EdgeWeightType::MAX_3D},
        {"MAN_2D", EdgeWeightType::MAN_2D}, {"MAN_3D", EdgeWeightType::MAN_3D},
        {"CEIL_2D", EdgeWeightType::CEIL_2D}, {"GEO", EdgeWeightType::GEO},
        {"ATT", EdgeWeightType::ATT}, {"XRAY1", EdgeWeightType::XRAY1},
        {"XRAY2", EdgeWeightType::XRAY2}, {"SPECIAL", EdgeWeightType::SPECIAL}
    };

    std::unordered_map<std::string, EdgeWeightFormat> edgeWeightFormatMap = {
        {"FUNCTION", EdgeWeightFormat::FUNCTION},
        {"FULL_MATRIX", EdgeWeightFormat::FULL_MATRIX},
        {"UPPER_ROW", EdgeWeightFormat::UPPER_ROW},
        {"LOWER_ROW", EdgeWeightFormat::LOWER_ROW},
        {"UPPER_DIAG_ROW", EdgeWeightFormat::UPPER_DIAG_ROW},
        {"LOWER_DIAG_ROW", EdgeWeightFormat::LOWER_DIAG_ROW},
        {"UPPER_COL", EdgeWeightFormat::UPPER_COL},
        {"LOWER_COL", EdgeWeightFormat::LOWER_COL},
        {"UPPER_DIAG_COL", EdgeWeightFormat::UPPER_DIAG_COL},
        {"LOWER_DIAG_COL", EdgeWeightFormat::LOWER_DIAG_COL}
    };

    double euclideanDistance2D(const City& a, const City& b) const {
        double dx = a.x - b.x;
        double dy = a.y - b.y;
        return std::sqrt(dx*dx + dy*dy);
    }

    double manhattanDistance2D(const City& a, const City& b) const {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

    double geographicalDistance(const City& a, const City& b) const {
        const double PI = 3.141592;
        const double RRR = 6378.388;

        auto toRadians = [PI](double coord) {
            double deg = std::round(coord);
            double min = coord - deg;
            return PI * (deg + 5.0 * min / 3.0) / 180.0;
        };

        double lat1 = toRadians(a.x);
        double lon1 = toRadians(a.y);
        double lat2 = toRadians(b.x);
        double lon2 = toRadians(b.y);

        double q1 = std::cos(lon1 - lon2);
        double q2 = std::cos(lat1 - lat2);
        double q3 = std::cos(lat1 + lat2);

        return RRR * std::acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0;
    }

    double attDistance(const City& a, const City& b) const {
        double dx = a.x - b.x;
        double dy = a.y - b.y;
        double rij = std::sqrt((dx*dx + dy*dy) / 10.0);
        double tij = std::round(rij);
        return tij < rij ? tij + 1 : tij;
    }

    void expandMatrix() {
        if (edge_weights.empty() || edge_weights[0].empty()) return;

        std::vector<std::vector<int>> full_matrix(dimension, std::vector<int>(dimension, 0));
        const auto& weights = edge_weights[0];
        size_t index = 0;

        switch (edge_weight_format) {
            case EdgeWeightFormat::LOWER_ROW:
                for (int i = 1; i < dimension; ++i) {
                    for (int j = 0; j < i; ++j) {
                        full_matrix[i][j] = weights[index++];
                        full_matrix[j][i] = full_matrix[i][j];
                    }
                }
                break;
                
            case EdgeWeightFormat::UPPER_ROW:
                for (int i = 0; i < dimension; ++i) {
                    for (int j = i + 1; j < dimension; ++j) {
                        full_matrix[i][j] = weights[index++];
                        full_matrix[j][i] = full_matrix[i][j];
                    }
                }
                break;
                
            case EdgeWeightFormat::LOWER_DIAG_ROW:
                for (int i = 0; i < dimension; ++i) {
                    for (int j = 0; j <= i; ++j) {
                        full_matrix[i][j] = weights[index++];
                        full_matrix[j][i] = full_matrix[i][j];
                    }
                }
                break;
                
            case EdgeWeightFormat::UPPER_DIAG_ROW:
                for (int i = 0; i < dimension; ++i) {
                    for (int j = i; j < dimension; ++j) {
                        full_matrix[i][j] = weights[index++];
                        full_matrix[j][i] = full_matrix[i][j];
                    }
                }
                break;
                
            default:
                return;
        }

        edge_weights = full_matrix;
        edge_weight_format = EdgeWeightFormat::FULL_MATRIX;
    }

public:
    TSPInstance() : type(ProblemType::UNKNOWN), dimension(0), 
                   edge_weight_type(EdgeWeightType::UNKNOWN),
                   edge_weight_format(EdgeWeightFormat::UNKNOWN), capacity(0) {}

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Não foi possível abrir o arquivo: " + filename);
        }

        std::string line;
        bool in_spec_section = true;
        bool in_node_coord_section = false;
        bool in_edge_weight_section = false;
        bool in_fixed_edges_section = false;
        bool in_depot_section = false;
        bool in_demand_section = false;

        while (std::getline(file, line)) {
            // Processar linha
            line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(), line.end());

            if (line.empty()) continue;

            std::string upper_line = line;
            std::transform(upper_line.begin(), upper_line.end(), upper_line.begin(), ::toupper);

            if (in_spec_section) {
                if (upper_line.find("NODE_COORD_SECTION") != std::string::npos) {
                    in_spec_section = false;
                    if (edge_weight_type != EdgeWeightType::EXPLICIT) {
                        in_node_coord_section = true;
                    }
                    continue;
                } else if (upper_line.find("EDGE_WEIGHT_SECTION") != std::string::npos) {
                    in_spec_section = false;
                    in_edge_weight_section = true;
                    edge_weights.push_back({});
                    continue;
                } else if (upper_line.find("EOF") != std::string::npos) {
                    break;
                }

                size_t colon_pos = line.find(':');
                if (colon_pos == std::string::npos) continue;

                std::string keyword = line.substr(0, colon_pos);
                std::string value = line.substr(colon_pos + 1);
                
                keyword.erase(std::remove_if(keyword.begin(), keyword.end(), ::isspace), keyword.end());
                value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());

                if (keyword == "NAME") name = value;
                else if (keyword == "TYPE") {
                    auto it = problemTypeMap.find(value);
                    if (it != problemTypeMap.end()) type = it->second;
                }
                else if (keyword == "DIMENSION") dimension = std::stoi(value);
                else if (keyword == "EDGE_WEIGHT_TYPE") {
                    auto it = edgeWeightTypeMap.find(value);
                    if (it != edgeWeightTypeMap.end()) edge_weight_type = it->second;
                }
                else if (keyword == "EDGE_WEIGHT_FORMAT") {
                    auto it = edgeWeightFormatMap.find(value);
                    if (it != edgeWeightFormatMap.end()) edge_weight_format = it->second;
                }
            } 
            else if (in_node_coord_section) {
                std::istringstream iss(line);
                City city;
                iss >> city.id >> city.x >> city.y;
                city.z = 0.0;
                cities.push_back(city);
                
                if (cities.size() == static_cast<size_t>(dimension)) {
                    in_node_coord_section = false;
                }
            } 
            else if (in_edge_weight_section) {
                std::istringstream iss(line);
                int weight;
                while (iss >> weight) {
                    edge_weights.back().push_back(weight);
                }

                // Verificar se completou a matriz
                size_t expected_size = 0;
                switch (edge_weight_format) {
                    case EdgeWeightFormat::FULL_MATRIX:
                        if (edge_weights.size() == static_cast<size_t>(dimension)) {
                            in_edge_weight_section = false;
                        }
                        break;
                    case EdgeWeightFormat::LOWER_ROW:
                    case EdgeWeightFormat::UPPER_ROW:
                        expected_size = dimension * (dimension - 1) / 2;
                        if (edge_weights[0].size() >= expected_size) {
                            in_edge_weight_section = false;
                            expandMatrix();
                        }
                        break;
                    case EdgeWeightFormat::LOWER_DIAG_ROW:
                    case EdgeWeightFormat::UPPER_DIAG_ROW:
                        expected_size = dimension * (dimension + 1) / 2;
                        if (edge_weights[0].size() >= expected_size) {
                            in_edge_weight_section = false;
                            expandMatrix();
                        }
                        break;
                    default:
                        if (edge_weights[0].size() >= static_cast<size_t>(dimension * dimension)) {
                            in_edge_weight_section = false;
                        }
                }
            }
        }

        // Se for matriz explícita sem coordenadas, cria cidades fictícias
        if (edge_weight_type == EdgeWeightType::EXPLICIT && cities.empty()) {
            for (int i = 1; i <= dimension; ++i) {
                cities.push_back({i, 0.0, 0.0, 0.0});
            }
        }

        file.close();
    }

    double getDistance(int i, int j) const {
        if (i == j) return 0.0;
        if (i < 0 || i >= dimension || j < 0 || j >= dimension) {
            throw std::out_of_range("Índices de cidade inválidos");
        }
        
        if (edge_weight_type == EdgeWeightType::EXPLICIT) {
            return edge_weights[i][j];
        }
        
        const City& a = cities[i];
        const City& b = cities[j];
        
        switch (edge_weight_type) {
            case EdgeWeightType::EUC_2D: return euclideanDistance2D(a, b);
            case EdgeWeightType::MAN_2D: return manhattanDistance2D(a, b);
            case EdgeWeightType::GEO: return geographicalDistance(a, b);
            case EdgeWeightType::ATT: return attDistance(a, b);
            default: throw std::runtime_error("Tipo de distância não implementado");
        }
    }

    // Getters
    ProblemType getType() const { return type; }
    const std::string& getName() const { return name; }
    const std::string& getComment() const { return comment; }
    int getDimension() const { return dimension; }
    EdgeWeightType getEdgeWeightType() const { return edge_weight_type; }
    EdgeWeightFormat getEdgeWeightFormat() const { return edge_weight_format; }
    const std::vector<City>& getCities() const { return cities; }
    const std::vector<std::vector<int>>& getEdgeWeights() const { return edge_weights; }
};

// Exemplo: 
// int main() {
//     TSPInstance instance;
//     try {
//         instance.loadFromFile("gr17.tsp");
        
//         std::cout << "Instância carregada: " << instance.getName() << std::endl;
//         std::cout << "Tipo: " << (instance.getType() == TSPInstance::ProblemType::TSP ? "TSP" : "Outro") << std::endl;
//         std::cout << "Dimensão: " << instance.getDimension() << std::endl;
//         std::cout << "Tipo de peso: " << (instance.getEdgeWeightType() == TSPInstance::EdgeWeightType::EXPLICIT ? "EXPLICIT" : "Outro") << std::endl;
        
//         if (instance.getDimension() > 1) {
//             std::cout << "Distância entre 0 e 1: " << instance.getDistance(0, 1) << std::endl;
//             std::cout << "Distância entre 1 e 0: " << instance.getDistance(1, 0) << std::endl;
//         }
//     } catch (const std::exception& e) {
//         std::cerr << "Erro: " << e.what() << std::endl;
//         return 1;
//     }
//     return 0;
// }