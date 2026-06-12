#include <iostream>
#include <fstream>
#include <vector>
#include "LinearProbing.hpp"
#include "DoubleHashing.hpp"
#include "SeparateChaining.hpp"
#include "Tester.hpp"

int main() {
    std::vector<int> sizes = { 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000};

    std::ofstream results("wyniki.csv");
    results << "Metoda,Przypadek,Operacja,Ilosc_Elementow_W_Tablicy,Czas_Pojedynczej_Operacji_ns\n";

    for (int n : sizes) {
        int capAvgPesOpt = n * 2;

        std::cout << "=== Uruchamianie testow dla N = " << n << " (Pojedyncza operacja) ===\n";

        auto createLP = [capAvgPesOpt]() { return new LinearProbing<long long, long long>(capAvgPesOpt); };
        Tester::runOptimisticCase(createLP, "LinearProbing", n, results);
        Tester::runAverageCase(createLP, "LinearProbing", n, results);
        Tester::runPessimisticCase(createLP, "LinearProbing", n, results);

        auto createDH = [capAvgPesOpt]() { return new DoubleHashing<long long, long long>(capAvgPesOpt); };
        Tester::runOptimisticCase(createDH, "DoubleHashing", n, results);
        Tester::runAverageCase(createDH, "DoubleHashing", n, results);
        Tester::runPessimisticCase(createDH, "DoubleHashing", n, results);

        auto createSC = [capAvgPesOpt]() { return new SeparateChaining<long long, long long>(capAvgPesOpt); };
        Tester::runOptimisticCase(createSC, "SeparateChaining", n, results);
        Tester::runAverageCase(createSC, "SeparateChaining", n, results);
        Tester::runPessimisticCase(createSC, "SeparateChaining", n, results);

        std::cout << "----------------------------------------\n";
    }

    results.close();
    std::cout << "Zakonczono. Wyniki wyeksportowano do wyniki.csv\n";

    return 0;
}