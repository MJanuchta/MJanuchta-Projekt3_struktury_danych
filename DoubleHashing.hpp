#ifndef DOUBLE_HASHING_HPP
#define DOUBLE_HASHING_HPP

#include "HashTableBase.hpp"
#include <stdexcept>

template <typename K, typename V>
class DoubleHashing : public HashTableBase<K, V> {
private:
    TableCell<K, V>* memoryBlock;

    unsigned int calculatePrimaryIndex(const K& targetKey) const {
        return static_cast<unsigned int>(targetKey % this->maxCapacity);
    }

    unsigned int calculateStepSize(const K& targetKey) const {
        unsigned int step = static_cast<unsigned int>(targetKey % (this->maxCapacity - 1));
        if (step % 2 == 0) {
            step++;
        }
        return step;
    }

    void expandAndMigrate() {
        unsigned int previousCapacity = this->maxCapacity;
        TableCell<K, V>* oldData = memoryBlock;

        this->maxCapacity = previousCapacity * 2;
        this->currentSize = 0;

        memoryBlock = new TableCell<K, V>[this->maxCapacity];

        for (unsigned int i = 0; i < previousCapacity; ++i) {
            if (oldData[i].state == CellState::OCCUPIED) {
                this->insert(oldData[i].key, oldData[i].value);
            }
        }

        delete[] oldData;
    }

public:
    DoubleHashing(unsigned int initialSize) : HashTableBase<K, V>(initialSize) {
        memoryBlock = new TableCell<K, V>[this->maxCapacity];
    }

    ~DoubleHashing() override {
        delete[] memoryBlock;
    }

    void insert(K incomingKey, V incomingValue) override {
        if ((this->currentSize * 100) / this->maxCapacity >= 60) {
            expandAndMigrate();
        }

        unsigned int position = calculatePrimaryIndex(incomingKey);
        unsigned int step = calculateStepSize(incomingKey);

        for (unsigned int attempt = 0; attempt < this->maxCapacity; ++attempt) {
            TableCell<K, V>& currentCell = memoryBlock[position];

            if (currentCell.state == CellState::FREE || currentCell.state == CellState::ERASED) {
                currentCell.key = incomingKey;
                currentCell.value = incomingValue;
                currentCell.state = CellState::OCCUPIED;
                this->currentSize++;
                return;
            }

            if (currentCell.state == CellState::OCCUPIED && currentCell.key == incomingKey) {
                currentCell.value = incomingValue;
                return;
            }

            position = (position + step) % this->maxCapacity;
        }

        throw std::overflow_error("Brak miejsca w pamieci");
    }

    void remove(K targetKey) override {
        unsigned int position = calculatePrimaryIndex(targetKey);
        unsigned int step = calculateStepSize(targetKey);

        for (unsigned int attempt = 0; attempt < this->maxCapacity; ++attempt) {
            TableCell<K, V>& currentCell = memoryBlock[position];

            if (currentCell.state == CellState::FREE) {
                return;
            }

            if (currentCell.state == CellState::OCCUPIED && currentCell.key == targetKey) {
                currentCell.state = CellState::ERASED;
                this->currentSize--;
                return;
            }

            position = (position + step) % this->maxCapacity;
        }
    }

    V get(K targetKey) override {
        unsigned int position = calculatePrimaryIndex(targetKey);
        unsigned int step = calculateStepSize(targetKey);

        for (unsigned int attempt = 0; attempt < this->maxCapacity; ++attempt) {
            TableCell<K, V>& currentCell = memoryBlock[position];

            if (currentCell.state == CellState::FREE) {
                break;
            }

            if (currentCell.state == CellState::OCCUPIED && currentCell.key == targetKey) {
                return currentCell.value;
            }

            position = (position + step) % this->maxCapacity;
        }

        throw std::invalid_argument("Klucz nie istnieje");
    }
};

#endif