#ifndef SEPARATE_CHAINING_HPP
#define SEPARATE_CHAINING_HPP

#include "HashTableBase.hpp"
#include <stdexcept>

template <typename K, typename V>
struct HashNode {
    K key;
    V value;
    HashNode* next;

    HashNode(K k, V v) : key(k), value(v), next(nullptr) {}
};

template <typename K, typename V>
class SeparateChaining : public HashTableBase<K, V> {
private:
    HashNode<K, V>** memoryBlock;

    unsigned int calculateIndex(const K& targetKey) const {
        return static_cast<unsigned int>(targetKey % this->maxCapacity);
    }

    void expandAndMigrate() {
        unsigned int previousCapacity = this->maxCapacity;
        HashNode<K, V>** oldData = memoryBlock;

        this->maxCapacity = previousCapacity * 2;
        this->currentSize = 0;

        memoryBlock = new HashNode<K, V>* [this->maxCapacity];
        for (unsigned int i = 0; i < this->maxCapacity; ++i) {
            memoryBlock[i] = nullptr;
        }

        for (unsigned int i = 0; i < previousCapacity; ++i) {
            HashNode<K, V>* current = oldData[i];
            while (current != nullptr) {
                this->insert(current->key, current->value);
                HashNode<K, V>* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
        delete[] oldData;
    }

public:
    SeparateChaining(unsigned int initialSize) : HashTableBase<K, V>(initialSize) {
        memoryBlock = new HashNode<K, V>* [this->maxCapacity];
        for (unsigned int i = 0; i < this->maxCapacity; ++i) {
            memoryBlock[i] = nullptr;
        }
    }

    ~SeparateChaining() override {
        for (unsigned int i = 0; i < this->maxCapacity; ++i) {
            HashNode<K, V>* current = memoryBlock[i];
            while (current != nullptr) {
                HashNode<K, V>* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
        delete[] memoryBlock;
    }

    void insert(K incomingKey, V incomingValue) override {
        if ((this->currentSize * 100) / this->maxCapacity >= 75) {
            expandAndMigrate();
        }

        unsigned int position = calculateIndex(incomingKey);
        HashNode<K, V>* current = memoryBlock[position];

        while (current != nullptr) {
            if (current->key == incomingKey) {
                current->value = incomingValue;
                return;
            }
            current = current->next;
        }

        HashNode<K, V>* newNode = new HashNode<K, V>(incomingKey, incomingValue);
        newNode->next = memoryBlock[position];
        memoryBlock[position] = newNode;
        this->currentSize++;
    }

    void remove(K targetKey) override {
        unsigned int position = calculateIndex(targetKey);
        HashNode<K, V>* current = memoryBlock[position];
        HashNode<K, V>* previous = nullptr;

        while (current != nullptr) {
            if (current->key == targetKey) {
                if (previous == nullptr) {
                    memoryBlock[position] = current->next;
                }
                else {
                    previous->next = current->next;
                }
                delete current;
                this->currentSize--;
                return;
            }
            previous = current;
            current = current->next;
        }
    }

    V get(K targetKey) override {
        unsigned int position = calculateIndex(targetKey);
        HashNode<K, V>* current = memoryBlock[position];

        while (current != nullptr) {
            if (current->key == targetKey) {
                return current->value;
            }
            current = current->next;
        }

        throw std::invalid_argument("Klucz nie istnieje");
    }
};

#endif