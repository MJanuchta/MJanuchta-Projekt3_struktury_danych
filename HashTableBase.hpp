#ifndef HASH_TABLE_BASE_HPP
#define HASH_TABLE_BASE_HPP

#include <stdexcept>
enum class CellState {
    FREE,       
    ERASED,     
    OCCUPIED    
};

template <typename K, typename V>
struct TableCell {
    CellState state = CellState::FREE;
    K key;
    V value;
};

template <typename K, typename V>
class HashTableBase {
protected:
    int currentSize;
    int maxCapacity;

public:
    HashTableBase(int capacity) : currentSize(0), maxCapacity(capacity) {}

    virtual ~HashTableBase() = default;
    virtual void insert(K key, V value) = 0;
    virtual void remove(K key) = 0;
    virtual V get(K key) = 0;

    int getSize() const {
        return currentSize;
    }

    int getCapacity() const {
        return maxCapacity;
    }

    float getLoadFactor() const {
        return static_cast<float>(currentSize) / static_cast<float>(maxCapacity);
    }
};

#endif