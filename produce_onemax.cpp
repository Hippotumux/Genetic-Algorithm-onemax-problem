#include <iostream>
#include <algorithm>
#include <random>
#include <time.h>
#include <bitset>

// 定義常數
const int ONE_MAX_LENGTH = 100; // 基因長度
const double P_CROSSOVER = 0.9; // 交叉機率 (交配)
const double P_MUTATION = 0.1; // 變異機率
const int MAX_GENERATION = 500; // 停止世代 (幾個世代後要結束)
const int POPULATION_SIZE = 200; // 群體大小

typedef std::bitset<ONE_MAX_LENGTH> Individual;
typedef std::pair<double,Individual> pair_double_Individual;

// mt19937 隨機數定義
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> rand_prop(0, 1); // 0~1 的隨機浮點數，用於隨機交叉or變異選擇
std::uniform_real_distribution<> rand_length(0, ONE_MAX_LENGTH); // 用於隨機選擇 變異位置/交叉位置

// 適應度函數
int fitness(const Individual &individual) {
    return individual.count();
}

// 初始化群組
std::vector<pair_double_Individual> initailizePopulation() {
    std::vector<pair_double_Individual> population(POPULATION_SIZE);
    for (int i = 0 ; i < POPULATION_SIZE ; i++) {
        Individual individual;
        for (int j = 0 ; j < ONE_MAX_LENGTH ; j++) {
            individual[j] = rand()%2;
        }
        population[i].second = individual;
        population[i].first = 0;
    }
    return population;
}

// 選擇 
std::vector<pair_double_Individual> select(const std::vector<pair_double_Individual> &population) {
    std::vector<pair_double_Individual> select_population(POPULATION_SIZE);
    for (int i = 0 ; i < POPULATION_SIZE ; i++) {
        double random_value = rand_prop(gen);

        // 會保留 20% 最好的基因
        if (random_value > 0.8) {
            select_population[i] = population[0];
            continue;
        }

        // 其餘根據俄羅斯轉盤選擇子代
        double choose_probability = 0.0;
        int choose_index = 0;
        while (true) {
            choose_probability += population[choose_index].first;
            if (random_value <= choose_probability) {
                select_population[i] = population[choose_index];
                break;
            } else if (choose_index == POPULATION_SIZE - 1) {
                select_population[i] = population[choose_index];
                break; 
            }
            choose_index ++;
        }
    }
    return select_population;
}

// 交叉 (這邊先用單點交叉，順序是最前面跟最後面做變異)
void crossover(std::vector<pair_double_Individual> &population) {
    const int cross_index = rand_length(gen);
    for (int i = 0, j = POPULATION_SIZE - 1 ; i < POPULATION_SIZE / 2 ; i++, j--) {
        const Individual ind_copy = population[i].second;

        // 交叉感染，使用位元運算轉換
        population[i].second = ( ((population[i].second >> cross_index) << cross_index) | 
                                ((population[j].second >> cross_index) << cross_index) ^ population[j].second ); 
        
        population[j].second = ( ((population[j].second >> cross_index) << cross_index) | 
                                ((ind_copy >> cross_index) << cross_index) ^ ind_copy ); 
    }
    return ;
}

// 變異(單點變異，並且只有一半的機率變異)
void mutation(std::vector<pair_double_Individual> &population) {
    for (auto &ind : population) {
        const int situation = rand_length(gen);
        if (rand() % 2 == 0)
            ind.second[situation] = !ind.second[situation];
    }
    return;
}

int main(void) {
    srand(time(NULL));
    std::cout << "initailizePopulation\n";

    // 給一個群體 機率 和 bit表示
    std::vector<pair_double_Individual> population = initailizePopulation();
    std::cout << "init success\n";

    for (int i = 0 ; i < MAX_GENERATION ; i++) {
        if (i % 10 == 0)
            std::cout << "round " << i << "times\n"; 

        // 變異 or 交叉
        double rand_value = rand_prop(gen);
        if (rand_value < P_CROSSOVER) 
            crossover(population);
        else 
            mutation(population);   


        // 計算每個點的權重(機率)
        int fit_sum = 0;
        int fit_index = 0;
        for (auto &ind : population) {
            int fit = fitness(ind.second);
            fit_sum += fit;
            ind.first = fit;
        }

        for (auto &ind : population) 
            ind.first = ind.first/fit_sum;
        

        // 根據權重排序(大到小)
        std::sort(population.begin(), population.end(), [](pair_double_Individual a, pair_double_Individual b){
            return a.first > b.first;
        });

        // 根據每個點的權重挑選下一個群體
        population = select(population); 
    }

    std::cout << "The result is\n";
    for (auto it : population) {
        std::cout << it.second << '\n';
    }
    return 0;
}