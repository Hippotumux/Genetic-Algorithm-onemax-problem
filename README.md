> 本專案目的為使用 GA 遺傳演算法來解決 onemax 問題

# <center> Genetic Algorithm (遺傳演算法)  </center>

我對於遺傳演算法產生興趣是源自於當時高中時在研究怎麼寫電腦五子棋，透過老師的一點啟發，我在沒看過 min-max 的時候把它實作出來，後來遇到瓶頸的時候學了 alpha-beta 剪枝法，也是在這時候發現有個算法叫遺傳演算法，當時的我只能夠明白它的原理，不過就是不知道他到底是怎麼實作的。 後來在我慢慢接觸一些機器學習，跟深度學習的一些東西後，加上最近我剛好又看到遺傳演算法，就嘗試著把它學起來。 

目前只有實作了最簡單的 OneMax，另外應該會找個題目來當我的專案在大二寫 (目前考慮的點之一)。  

## 本章內容

- 簡介
- 達爾文進化論
- 遺傳算法
- OneMax應用

備註：此篇的實作並不是使用 python 而是用 c++

## 簡介

基因演算法，或稱遺傳演算法（英語：Genetic Algorithm，GA）是計算數學中用於解決最佳化的搜尋演算法，是進化演算法的一種。進化演算法最初是借鑑了進化生物學中的一些現象而發展起來的，這些現象包括遺傳、突變、自然選擇以及雜交等等。

基因演算法通常實現方式為一種電腦模擬。對於一個最佳化問題，一定數量的候選解（稱為個體）可抽象表示為染色體，使種群向更好的解進化。傳統上，解用二進位表示（即0和1的串），但也可以用其他表示方法。進化從完全隨機個體的種群開始，之後一代一代發生。在每一代中評價整個種群的適應度，從當前種群中隨機地選擇多個個體（基於它們的適應度），通過自然選擇和突變產生新的生命種群，該種群在演算法的下一次迭代中成為當前種群。

__ by wiki

## 達爾文進化論

國高中上過生物課，其中就會講到達爾文進化論，適者生存不適者淘汰! 然而就是這一個概念得到了啟發。

> 把一堆小兵丟進決鬥場中，勝者就是我們想要的勇士!!

其中主要應用其三大重點:
- 遺傳： 基因來自父母的各一半。
- 變異： 個體間有一定機率發生突變
- 選擇： 也就是讓適合的個體可以有後代，不適合的被淘汰。 

在一個群體中，除了能夠讓強者留下(想要的結果)，同時也因為變異和遺傳帶來的多樣性，較不會被限縮住，所以這個概念很適合拿來模擬最佳化的問題。

## 遺傳演算法名詞介紹

在遺傳算法中，有以下幾個重要概念

- 基因 (Genotype)
- 個體 (Individual)
- 群體 (Population)
- 適應度函數 (Fitness function)
- 選擇 (Selection)
- 交叉 (Crossover)
- 變異 (Mutation)
- 終止條件 (Termination Criteria)

一群基因代表著個體的狀態，不同個體一起組成了一個群體，會透過適應度函數來選擇每一個後代，同時這期間可能會發生交叉(交配)或者變異。


### 基因 (Genotype)

> 基因是染色體中的最小單位，它通常表示一個特定的特徵或參數。基因可以是二進制、實數、整數等不同類型的編碼方式，具體取決於解決的問題。基因決定了染色體的性質，不同的基因組合可以產生不同的解決方案。

這種狀態常會使用二進位來表示，例如

![](https://hackmd.io/_uploads/BkaVAwXCh.png)

### 個體（Individual）

> 在遺傳算法中，每個解決方案被表示為一個個體。這個個體包含了一組基因，這些基因描述了解決方案的特徵。一個個體通常用一個染色體來表示。

### 群體 (Population)

> 種群是包含多個個體的集合，代表了當前時刻的候選解決方案。在每一代中，種群中的個體都會被選擇、交叉和變異，以產生下一代的個體。

### 適應度函數 (Fitness function)

> 適應度函數用於評估每個個體的優劣程度，它量化了一個個體解決問題的能力。適應度函數的定義取決於具體的問題，通常越接近問題的最優解，適應度越高。

### 選擇 (Selection)

> 選擇操作根據個體的適應度來確定哪些個體將會被複制到下一代。通常，適應度高的個體被選中的概率更高，以提高更好解決方案的傳播率。

### 交叉 (Crossover)

> 交叉操作是模擬生物的基因組合過程，將兩個父代個體的染色體互相交換部分基因來生成子代個體。這有助於探索新的解決方案。

### 變異 (Mutation)

> 變異操作引入了一些隨機性，通過改變個體染色體中的一些基因來產生多樣性。變異有助於跳出局部最優解，以更廣泛地搜索解空間。

### 終止條件 (Termination Criteria)

> 遺傳算法需要一個停止條件來確定何時結束優化過程。這可以是達到一定的代數、適應度閾值或時間限制等。


## 遺傳算法流程

下面的圖展示了基本的流程

![](https://hackmd.io/_uploads/SyeZzO7Cn.png)


### 初始化群體

初始化群組，正常會使用隨機的方式來創建，也就是把每個個體都隨機生成，組成一個隨機的群體。

### 計算適應度 (Fitness function)

遺傳算法中，正常會使用分數的方式，也就是分數較高的就是較好基因，反之如果是希望越小越好，就是較小的為好基因。

適應度函數是一個用於計算個體適應度值的數學函數。個體作為輸入，並返回一個數值(分數)，表示個體對問題的適應度。適應度函數的設計取決於解決的問題，它應該能夠明確反映出個體在問題上的表現。

這邊在處理的基本上是獨立事件，所以可以使用平行處理來計算。

> ps 如何計算適應度就是問題所在，這邊要自己天馬行空的想像囉。 (當然如果有明確的定義可以直接使用，例如計算區間 \[a,b] 在 $f(x) = x^2 + 2x + 5$ 的極值，這種就可以用定義給分)

### 挑選方式

- 俄羅斯輪盤（Roulette Wheel Selection）： 相信大家都看過俄羅斯輪盤，拿著一個左輪手槍，放幾顆子旋轉彈夾，對自己開槍，沒死就可以拿到錢。 這邊的挑選方式就是根據所有個體的適應度分數，從零開始累加，如果目前的值 > 初始化的隨機值，就挑選該個體，重新在一次，直到挑滿。

- 競賽選擇（Tournament Selection）：競賽選擇是一種比較式選擇方法，它隨機選擇一個固定大小的子集（稱為錦標賽），然後從這個子集中選擇適應度最高的個體作為父代，但這樣後果可能就是局部收斂。

- 精英選擇（Elitism）：精英選擇是一種特殊的選擇方法，其中一些最優秀的個體（通常是適應度最高的一部分）被直接複製到下一代，以確保這些優秀的個體不會在進化過程中丟失。

選擇方法的選擇取決於特定問題和遺傳算法的設計。通常，輪盤選擇和競賽選擇是最常見的選擇方式，但在某些情況下，其他方法也可能更適合。選擇方法的目標是確保高適應度的個體有更大的機會被選中，以推動解空間的探索和優化過程。

### 交叉方式

- 單點交叉（Single-Point Crossover）：在兩個父代中選擇一個交叉點，然後將兩個父代分成兩部分，交換這兩個部分，以生成兩個子代。

- 多點交叉（Multi-Point Crossover）：類似於單點交叉，但可以選擇多個交叉點，進行多次交換，以生成子代。

- 均勻交叉（Uniform Crossover）：對每一個基因位置，根據一個機率來選擇父代的對應基因，然後生成子代。


目的是使遺傳演算法搜索解空間時具有多樣性!! 較不會陷入局部收斂。

> 這邊可能會對於交叉的方式理解有問題，這邊舉單點交叉：

![](https://hackmd.io/_uploads/S18PSiV02.png)


### 變異方式

- 單點變異（Single-Point Mutation）：在一個個體的基因型中隨機選擇一個基因位置，然後將該位置的基因值進行變異。對於二進制編碼，可以簡單地翻轉位元；對於實數編碼，可以對該基因值加入一個隨機的小變化。

- 多點變異（Multi-Point Mutation）：類似於單點變異，但可以選擇多個基因位置來進行變異。這增加了變異的多樣性。

- 均勻變異（Uniform Mutation）：對每一個基因位置，以一個小的概率（變異率）來決定是否對該位置進行變異。這種方式適用於二進制編碼。

![](https://hackmd.io/_uploads/SkTpIo4A3.png)

變異率是一個重要的參數，它控制著變異操作的強度，過高的變異率可能會導致過多的隨機性，訓練起來就不好。

有些人會覺得交叉比變異來的重要，也有些人認為變異比交叉重要，至於我，我覺得...交叉應該來的重要一點，以進化論來看，比較重要的當然會是交配，而不是突變。

### 終止條件

- 達到最大演化世代：設定一個最大迭代次數，當達到這個數字時，算法停止運行。這個方法是最簡單的終止條件之一。

- 達到目標解的質量：如果已經找到一個滿足問題要求的解，則可以停止運行。這通常用於最優化問題，當找到足夠好的解時，可以停止搜索。

- 收斂：監控算法的收斂情況，如果在一定次數的迭代內解沒有明顯改善，可以視為算法已經收斂，然後停止運行。


總而言之，遺傳算法是一種強大的優化算法，通過模擬進化過程中的遺傳、選擇和變異機制來搜索解決方案空間，以找到問題的最優解或者近似最優解。基因是遺傳算法中個體的組成部分，它們通過不斷的進化來適應和改善解決方案。遺傳算法可以應用於各種各樣的問題領域，包括機器學習、調度問題、函數優化等。

## OneMax 實作練習

OneMax 是一個經典的二進制最優化問題，它通常用於演示和測試優化演算法的性能。問題的目標非常簡單：找到一個二進制串，使其所有的位元都等於 1。換句話說，你需要將所有位元翻轉為 1，使得二進制串的每個位元都達到最大值。

而答案很明顯，目標是找到一個二進制串 x，使得 x 中的每個位元都等於 1，不過一開始程式完全不知道答案，所以很明顯這就是 GA 的應用題，可以說是 GA 中的 Hello World!!

接下來就來根據流程寫出程式碼囉!

### 變數設定

首先要先定義明確的變數，總共有 基因長度、交叉機率、變異機率、停止世代、群體大小

```cpp=
// 定義常數
const int ONE_MAX_LENGTH = 100; // 基因長度
const double P_CROSSOVER = 0.9; // 交叉機率 (交配)
const double P_MUTATION = 0.1; // 變異機率
const int MAX_GENERATION = 500; // 停止世代 (幾個世代後要結束)
const int POPULATION_SIZE = 200; // 群體大小

typedef std::bitset<ONE_MAX_LENGTH> Individual;
typedef std::pair<double,Individual> pair_double_Individual;
```

存放的方式我選擇用 bitset 前面的 double 則是每一個基因的權重。

### 隨機設定

因為 mt19937 會比單純用 rand() 來的隨機。

```cpp=
// mt19937 隨機數定義
std::random_device rd;
std::mt19937 gen(rd());

// 0~1 的隨機浮點數，用於隨機交叉or變異選擇
std::uniform_real_distribution<> rand_prop(0, 1); 
// 用於隨機選擇 變異位置/交叉位置
std::uniform_real_distribution<> rand_length(0, ONE_MAX_LENGTH); 
```

### 初始化設定

就簡單的根據個體大小，生成基因出來，那因為只有 0 1 所以用 rand 也沒差。

```cpp=
// 初始化群組
std::vector<pair_double_Individual> initailizePopulation() {
    std::vector<pair_double_Individual> population(POPULATION_SIZE);
    for (int i = 0 ; i < POPULATION_SIZE ; i++) {
        Individual individual;
        for (int j = 0 ; j < ONE_MAX_LENGTH ; j++) {
            individual[j] = rand() % 2;
        }
        population[i].second = individual;
        population[i].first = 0;
    }
    return population;
}
```

### 計算適應度 

題目是有越多 1 越好，不妨設分數就是有幾個 1 ，要得到機率就算完全部在除以總數即可

```cpp=
// 適應度函數
int fitness(const Individual &individual) {
    return individual.count();
}
```


```cpp=
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
```


### 選擇

我一開始是用俄羅斯輪盤來做選擇，但每次都會提早收斂 = =，基本上做不太出來，所以我後來一樣保留俄羅斯輪盤，但是有 20% 是菁英挑選，確保好的基因會留下來!

> 注意這邊選擇最好的基因前，我有先根據適應度來排序。

```cpp=
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
```

### 交叉

這邊選擇單點交叉，輕鬆處理，下面因為要交叉互換位元，可能會看不太懂？，簡單說假設現在有

01001010
10100111

想要在從右邊數來第4個位置
就先把他右移 4 在左移 4 ，會變成

01000000
10100000

把第二條基因，跟原本的第二條做 xor ，會剩下

00000111

此時把第一條處理的跟處理兩次的第二條做 or，得到

01001111 

就換過去了，同理得到另外一條，只是因為會改變，所以會先把它存在另外一個地方 (ind_copy)。


```cpp=
// 交叉 (這邊先用單點交叉，順序是最前面跟最後面做變異)
void crossover(std::vector<pair_double_Individual> &population) {
    const int cross_index = rand_length(gen);
    for (int i = 0, j = POPULATION_SIZE - 1 ; i < POPULATION_SIZE / 2 ; i++, j--) {
        // 交叉感染，使用位元運算轉換
        const Individual ind_copy = population[i].second;
        
        population[i].second = ( ((population[i].second >> cross_index) << cross_index) | 
                                ((population[j].second >> cross_index) << cross_index) ^ population[j].second );
        
        population[j].second = ( ((population[j].second >> cross_index) << cross_index) | 
                                ((ind_copy >> cross_index) << cross_index) ^ ind_copy ); 
    }
    return ;
}
```

### 變異

使用輕鬆的單點變異，同時是個體變
使用輕鬆的單點變異，同時是個體變異，裡面再設定基因變異機率。
也就是一開始會先決定這個個體要不要變異，之後根據每個基因決定它要不要變異。

```cpp=
// 變異(單點變異，並且只有一半的機率變異)
void mutation(std::vector<pair_double_Individual> &population) {
    for (auto &ind : population) {
        const int situation = rand_length(gen);
        if (rand() % 2 == 0)
            ind.second[situation] = !ind.second[situation];
    }
    return;
}
```

### 結束條件

使用最簡單的最大世代結束。

```cpp=
for (int i = 1 ; i <= MAX_GENERATION ; i++) {
    // 開始每一個 round!!
}
```

就這樣簡簡單單的實現了這個 OneMax 題目了~~

### 結果

可以看到大部分都是 1 ，會出現 0 的是因為有變異，啊我懶得把它挑出來。

![](https://hackmd.io/_uploads/H1aYZnNR3.png)


## 聯絡作者

我是河馬 ~~ 很高興認識大家，如果有任何疑問請使用以下方式來聯絡我

- [Blog](https://hackmd.io/@HIPP0/notebook)
- [Facebook](https://www.facebook.com/profile.php?id=100008989923059)
- [Discord] 河馬#9738
