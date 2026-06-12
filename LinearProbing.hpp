#ifndef LINEAR_PROBING_HPP
#define LINEAR_PROBING_HPP

#include "HashTableBase.hpp"
#include <stdexcept>

template <typename K, typename V>
class LinearProbing : public HashTableBase<K, V> {
private:
    TableCell<K, V>* memoryBlock;

    unsigned int calculateIndex(const K& targetKey) const {
        return static_cast<unsigned int>(targetKey % this->maxCapacity);
    }

    void expandAndMigrate() {
        unsigned int previousCapacity = this->maxCapacity;
        TableCell<K, V>* oldData = memoryBlock;

        this->maxCapacity = previousCapacity * 2;
        this->currentSize = 0;

        memoryBlock = new TableCell<K, V>[this->maxCapacity];

        for (TableCell<K, V>* ptr = oldData; ptr < oldData + previousCapacity; ++ptr) {
            if (ptr->state == CellState::OCCUPIED) {
                this->insert(ptr->key, ptr->value);
            }
        }

        delete[] oldData;
    }

public:
    LinearProbing(unsigned int initialSize) : HashTableBase<K, V>(initialSize) {
        memoryBlock = new TableCell<K, V>[this->maxCapacity];
    }

    ~LinearProbing() override {
        delete[] memoryBlock;
    }

    void insert(K incomingKey, V incomingValue) override {
        if ((this->currentSize * 100) / this->maxCapacity >= 60) {
            expandAndMigrate();
        }

        unsigned int position = calculateIndex(incomingKey);

        for (unsigned int step = 0; step < this->maxCapacity; ++step) {
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

            position++;
            if (position >= this->maxCapacity) {
                position = 0;
            }
        }

        throw std::overflow_error("Brak miejsca w pamieci");
    }

    void remove(K targetKey) override {
        unsigned int position = calculateIndex(targetKey);

        for (unsigned int step = 0; step < this->maxCapacity; ++step) {
            TableCell<K, V>& currentCell = memoryBlock[position];

            if (currentCell.state == CellState::FREE) {
                return;
            }

            if (currentCell.state == CellState::OCCUPIED && currentCell.key == targetKey) {
                currentCell.state = CellState::ERASED;
                this->currentSize--;
                return;
            }

            position++;
            if (position >= this->maxCapacity) {
                position = 0;
            }
        }
    }

    V get(K targetKey) override {
        unsigned int position = calculateIndex(targetKey);

        for (unsigned int step = 0; step < this->maxCapacity; ++step) {
            TableCell<K, V>& currentCell = memoryBlock[position];

            if (currentCell.state == CellState::FREE) {
                break;
            }

            if (currentCell.state == CellState::OCCUPIED && currentCell.key == targetKey) {
                return currentCell.value;
            }

            position++;
            if (position >= this->maxCapacity) {
                position = 0;
            }
        }

        throw std::invalid_argument("Klucz nie istnieje");
    }
};

#endif