#ifndef HASHMAP_H
#define HASHMAP_H

#include "HashMapInterface.h"
#include "Pair.h"

template <typename K, typename V>
class HashMap : public HashMapInterface<K, V> {
private:
	size_t capacity;
	Pair<std::string, V>* hashTable;
	size_t numKeys;
public:
	HashMap() : capacity(DEFAULT_MAP_HASH_TABLE_SIZE), numKeys(0),
				hashTable(new Pair<std::string, V>[DEFAULT_MAP_HASH_TABLE_SIZE]) {}
	~HashMap() { delete[] hashTable; }

	/**
	 * Read/write index access operator.
	 * If the key is not found, an entry is made for it.
	 * @return Read and write access to the value mapped to the provided key.
	 */
	V& operator[](const K& key) {
		double loadFactor = double(numKeys) / capacity * 100;
		if (loadFactor > LOAD_THRESHOLD) {
			rehash();
		}
		size_t hashIndex = hashEncoder(key) % capacity;
		int k = 1;
		while (1) {
			if (hashTable[hashIndex].first.size() == 0) {
				hashTable[hashIndex].first = key;
				++numKeys;
				break;
			}
			if (hashTable[hashIndex].first == key) break;
			hashIndex = (hashIndex + k) % capacity;
			k = k + 2;
		}
		return hashTable[hashIndex].second;
	}

	//NOTE: possibly put this function into a specialized string struct
	/** Generate a hash code for a string */
	size_t hashEncoder(std::string hashStr) {
		long long hashValue = 0;
		for (size_t i = 0; i < hashStr.size(); i++) {
			hashValue = (hashValue * HASH_CONSTANT) + hashStr.at(i);
		}
		return size_t(hashValue);
	}
	
	/** Copy the current hash table to a new hash table with increased capacity */
	void rehash() {
		numKeys = 0;
		capacity *= 2;
		Pair<std::string, V>* newTable = new Pair<std::string, V>[capacity];
		for (size_t i = 0; i < (capacity / 2); i++) {
			if (hashTable[i].first.size() != 0) {
				size_t hashIndex = hashEncoder(hashTable[i].first) % capacity;
				int k = 1;
				while (1) {
					if (newTable[hashIndex].first.size() == 0) {
						newTable[hashIndex] = hashTable[i];
						++numKeys;
						break;
					}
					else {
						hashIndex = (hashIndex + k) % capacity;
						k = k + 2;
					}
				}
			}
		}
		delete[] hashTable;
		hashTable = newTable;
	}

	/** @return: the number of elements that match the key in the Map. */
	size_t count(const K& key) {
		for (size_t i = 0; i < capacity; i++) {
			if (hashTable[i].first == key) {
				return 1;
			}
		}
		return 0;
	}

	/** Removes all items from the Map. */
	void clear(void) {
		delete[] hashTable;
		hashTable = new Pair<std::string, V>[DEFAULT_MAP_HASH_TABLE_SIZE];
	}

	/** @return: number of Key-Value pairs stored in the Map. */
	size_t size(void) const {
		return numKeys;
	}

	/** @return: maximum number of Key-Value pairs that the Map can hold. */
	size_t max_size(void) const {
		return capacity;
	}

	/** @return: string representation of Key-Value pairs in Map. */
	std::string toString(void) const {
		std::stringstream oss;
		oss << numKeys << "/" << capacity << std::endl;
		for (size_t i = 0; i < capacity; i++) {
			if (hashTable[i].first.size() != 0) {
				oss << "[" << i << ":" << hashTable[i].first << "->" 
					<< hashTable[i].second << "]" << std::endl;
			}
		}
		return oss.str();
	}

	friend std::ostream& operator<<(std::ostream& oss, const HashMap& hashmap) {
		oss << hashmap.toString();
		return oss;
	}
};
#endif