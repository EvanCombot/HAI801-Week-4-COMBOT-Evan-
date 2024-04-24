#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Définition d'une stratégie pour les échecs (séquence de mouvements)
typedef std::vector<int> ChessStrategy;

// Fonction d'évaluation d'une stratégie (simule une partie d'échecs)
int evaluate(const ChessStrategy& strategy) {
    std::vector<std::vector<char>> chessboard(8, std::vector<char>(8, '.'));

    // Placement des pièces en fonction de la stratégie
    for (int i = 0; i < strategy.size(); ++i) {
        int row = strategy[i] / 8;
        int col = strategy[i] % 8;
        chessboard[row][col] = 'Q';
    }

    // Simulation d'une partie d'échecs (évaluation basique)
    int evaluation = 0;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (chessboard[row][col] == 'Q') {
                // Évaluation : pour chaque reine, j'ajoute 1 point pour chaque case menacée
                for (int i = 0; i < 8; ++i) {
                    if (i != row) evaluation++;
                    if (i != col) evaluation++;
                    if (row + i < 8 && col + i < 8) evaluation++;
                    if (row + i < 8 && col - i >= 0) evaluation++;
                }
            }
        }
    }

    return evaluation;
}

// Générer une population initiale de stratégies aléatoires pour les échecs
std::vector<ChessStrategy> generateInitialPopulation(int populationSize, int strategyLength) {
    std::vector<ChessStrategy> population;
    for (int i = 0; i < populationSize; ++i) {
        ChessStrategy strategy;
        for (int j = 0; j < strategyLength; ++j) {
            strategy.push_back(rand() % 64);
        }
        population.push_back(strategy);
    }
    return population;
}

// Sélectionner les meilleures stratégies de la population actuelle
std::vector<ChessStrategy> selection(const std::vector<ChessStrategy>& population, int numBest) {
    std::vector<ChessStrategy> selected;
    std::vector<std::pair<int, int>> evaluations;
    for (int i = 0; i < population.size(); ++i) {
        evaluations.push_back({evaluate(population[i]), i});
    }
    std::sort(evaluations.begin(), evaluations.end(), std::greater<>());
    for (int i = 0; i < numBest; ++i) {
        selected.push_back(population[evaluations[i].second]);
    }
    return selected;
}

// Appliquer une mutation à une stratégie pour les échecs
void mutate(ChessStrategy& strategy) {
    int indexToMutate = rand() % strategy.size();
    strategy[indexToMutate] = rand() % 64; // Changer le mouvement en un index de case aléatoire (de 0 à 63)
}

// Croiser deux stratégies pour créer une nouvelle stratégie pour les échecs
ChessStrategy crossover(const ChessStrategy& strategy1, const ChessStrategy& strategy2) {
    int crossoverPoint = rand() % (strategy1.size() - 1) + 1; // Point de croisement aléatoire
    ChessStrategy newStrategy;
    newStrategy.insert(newStrategy.end(), strategy1.begin(), strategy1.begin() + crossoverPoint);
    newStrategy.insert(newStrategy.end(), strategy2.begin() + crossoverPoint, strategy2.end());
    return newStrategy;
}

// Algorithme génétique principal pour les échecs
void geneticAlgorithm(int populationSize, int numGenerations, int numBest) {
    std::vector<ChessStrategy> population = generateInitialPopulation(populationSize, numBest); // Taille de la stratégie arbitraire (10 mouvements)
    for (int generation = 0; generation < numGenerations; ++generation) {
        std::vector<ChessStrategy> selected = selection(population, numBest);
        std::vector<ChessStrategy> newPopulation;
        while (newPopulation.size() < populationSize) {
            ChessStrategy child = crossover(selected[rand() % numBest], selected[rand() % numBest]);
            if (rand() % 100 < 10) { // Probabilité de mutation : 10%
                mutate(child);
            }
            newPopulation.push_back(child);
        }
        population = newPopulation;
    }
    ChessStrategy bestStrategy = selection(population, 1)[0];
    std::cout << "Best strategy found:" << std::endl;
    for (int move : bestStrategy) {
        std::cout << move << " ";
    }
    std::cout << std::endl;
    std::cout << "Evaluation: " << evaluate(bestStrategy) << std::endl;
}

int main() {
    srand(time(nullptr)); 
    geneticAlgorithm(1000, 1000, 10); // Exemple : 1000 individus, 1000 générations, 10 meilleures stratégies sélectionnées
    return 0;
}
