#ifndef TESTER_HPP
#define TESTER_HPP

#include "HashTableBase.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <cstdlib>
#include <functional>
#include <random>

class Tester {
private:
    static const int REPETITIONS = 100; 

public:
    static void runOptimisticCase(std::function<HashTableBase<long long, long long>* ()> createTable, const std::string& name, int elementsCount, std::ofstream& file) {
        HashTableBase<long long, long long>* table = createTable();

        for (long long i = 1; i <= elementsCount; ++i) {
            table->insert(i, i);
        }

        double totalAddNs = 0.0;
        double totalRemNs = 0.0;

        for (int r = 1; r <= REPETITIONS; ++r) {
            long long singleKey = elementsCount + r;

            auto startAdd = std::chrono::high_resolution_clock::now();
            table->insert(singleKey, singleKey);
            auto endAdd = std::chrono::high_resolution_clock::now();
            totalAddNs += std::chrono::duration<double, std::nano>(endAdd - startAdd).count();

            auto startRem = std::chrono::high_resolution_clock::now();
            table->remove(singleKey);
            auto endRem = std::chrono::high_resolution_clock::now();
            totalRemNs += std::chrono::duration<double, std::nano>(endRem - startRem).count();
        }

        delete table;

        double avgAdd = totalAddNs / REPETITIONS;
        double avgRem = totalRemNs / REPETITIONS;

        file << name << ",Optymistyczny,Dodawanie," << elementsCount << "," << avgAdd << "\n";
        file << name << ",Optymistyczny,Usuwanie," << elementsCount << "," << avgRem << "\n";

        std::cout << name << " [Optymistyczny | N=" << elementsCount << "]\n"
            << "  -> Pojedyncze Dodanie: " << avgAdd << " ns\n"
            << "  -> Pojedyncze Usuwanie: " << avgRem << " ns\n";
    }

    static void runAverageCase(std::function<HashTableBase<long long, long long>* ()> createTable, const std::string& name, int elementsCount, std::ofstream& file) {
        std::mt19937_64 rng(42);
        std::uniform_int_distribution<long long> dist(1LL, 10000000000LL);

        HashTableBase<long long, long long>* table = createTable();

        for (int i = 0; i < elementsCount; ++i) {
            long long k = dist(rng);
            table->insert(k, k);
        }

        double totalAddNs = 0.0;
        double totalRemNs = 0.0;

        for (int r = 1; r <= REPETITIONS; ++r) {
            long long singleKey = dist(rng);

            auto startAdd = std::chrono::high_resolution_clock::now();
            table->insert(singleKey, singleKey);
            auto endAdd = std::chrono::high_resolution_clock::now();
            totalAddNs += std::chrono::duration<double, std::nano>(endAdd - startAdd).count();

            auto startRem = std::chrono::high_resolution_clock::now();
            table->remove(singleKey);
            auto endRem = std::chrono::high_resolution_clock::now();
            totalRemNs += std::chrono::duration<double, std::nano>(endRem - startRem).count();
        }

        delete table;

        double avgAdd = totalAddNs / REPETITIONS;
        double avgRem = totalRemNs / REPETITIONS;

        file << name << ",Sredni,Dodawanie," << elementsCount << "," << avgAdd << "\n";
        file << name << ",Sredni,Usuwanie," << elementsCount << "," << avgRem << "\n";

        std::cout << name << " [Sredni | N=" << elementsCount << "]\n"
            << "  -> Pojedyncze Dodanie: " << avgAdd << " ns\n"
            << "  -> Pojedyncze Usuwanie: " << avgRem << " ns\n";
    }

    static void runPessimisticCase(std::function<HashTableBase<long long, long long>* ()> createTable, const std::string& name, int elementsCount, std::ofstream& file) {
        HashTableBase<long long, long long>* table = createTable();
        long long cap = table->getCapacity();
        long long stepBreaker = cap - 1; 

        for (long long i = 1; i <= elementsCount; ++i) {
            long long badKey = i * cap * stepBreaker;
            table->insert(badKey, badKey);
        }

        double totalAddNs = 0.0;
        double totalRemNs = 0.0;

        for (int r = 1; r <= REPETITIONS; ++r) {
            long long singleKey = (elementsCount + r) * cap * stepBreaker;

            auto startAdd = std::chrono::high_resolution_clock::now();
            table->insert(singleKey, singleKey);
            auto endAdd = std::chrono::high_resolution_clock::now();
            totalAddNs += std::chrono::duration<double, std::nano>(endAdd - startAdd).count();

            auto startRem = std::chrono::high_resolution_clock::now();
            table->remove(singleKey);
            auto endRem = std::chrono::high_resolution_clock::now();
            totalRemNs += std::chrono::duration<double, std::nano>(endRem - startRem).count();
        }

        delete table;

        double avgAdd = totalAddNs / REPETITIONS;
        double avgRem = totalRemNs / REPETITIONS;

        file << name << ",Pesymistyczny,Dodawanie," << elementsCount << "," << avgAdd << "\n";
        file << name << ",Pesymistyczny,Usuwanie," << elementsCount << "," << avgRem << "\n";

        std::cout << name << " [Pesymistyczny | N=" << elementsCount << "]\n"
            << "  -> Pojedyncze Dodanie: " << avgAdd << " ns\n"
            << "  -> Pojedyncze Usuwanie: " << avgRem << " ns\n";
    }
};

#endif