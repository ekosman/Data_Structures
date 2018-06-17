#ifndef HASH_MAP_HPP_
#define HASH_MAP_HPP_

#include "avltree.hpp"
#include "exceptions.hpp"

#define THRESHOLD_INCREASE 0.75
#define THRESHOLD_DECREASE 0.25

template<typename V, class K> class HashMap {
	//
	//	Class		: HashMap
	//	Description : Implementation of hash map, which uses
	//					dynamic chain hashing. It's entries are
	//					AVL trees

private:
	//
	// Constants
	//
	static const int EMPTY_TABLE = 0;
	static const int INITIAL_SIZE = 10;
	static const int INCREASE_FACTOR = 2;
	static const int DECREASE_FACTOR = 2;

	AVLTree<V*, K> *entries;
	int _size;
	int _count;

	// Name			: hashFunction
	// Description	: This function converts a given key to it's matching
	//					hashing value
	// Parameters	:
	//	@key 	- key with which result should be associated
	// Return Value : The hash value associated with the given key
	int hashFunction(const K& key) const {
		int res = ((int) key % _size);

		if (0 <= res) {
			return res;
		} else {
			return res + _size;
		}
	}

	// Name			: loadFactorCheckAndResize
	// Description	: This function checks if the current load factor of the
	//					hash table is valid (i.e between the 2 thresholds)
	// Parameters	: None
	// Return Value : None
	void loadFactorCheckAndResize() {
		double load_factor = (double) _count / _size;

		if (THRESHOLD_INCREASE <= load_factor) {
			Resize(true);
		} else if (THRESHOLD_DECREASE >= load_factor) {
			Resize(false);
		}
	}

	// Name			: Resize
	// Description	: This function resizes the array which it's entries
	//					contain the data of hash-map.
	//					The map won't change it's size to be less that it's
	//					initial size.
	// Parameters	:
	//	@increase 	- boolean parameter which indicated if the table has to
	//					be increased or decreased
	// Return Value : None
	// If memory allocation failes, a matching exception would be thrown by
	//	the system.
	void Resize(bool increase) {
		if (false == increase && _size == INITIAL_SIZE){
			return;
		}
		
		int old_size = _size;
		_size = (increase == true) ? (_size * INCREASE_FACTOR) :
				(_size / DECREASE_FACTOR);

		AVLTree<V*, K>* old_entries = entries;
		entries = new AVLTree<V*, K> [_size];
		_count = 0;

		for (int i = 0; i < old_size; i++) {
			int tree_size = old_entries[i].getSize();
			V*** tree_data = old_entries[i].inOrderExtractData();
			K* tree_keys = old_entries[i].inOrderExtractKeys();

			for (int j = 0; j < tree_size; j++) {
				K temp_key = tree_keys[j];
				V* temp_obj = *(tree_data[j]);

				int entry_index = hashFunction(temp_key);
				entries[entry_index].Insert(temp_key, temp_obj);
				_count++;
			}
			
			delete[] tree_data;
			delete[] tree_keys;
		}

		delete[] old_entries;
	}

public:

	// HashMap contstructor
	HashMap() :
			_size(INITIAL_SIZE), _count(EMPTY_TABLE) {
		entries = new AVLTree<V*, K> [INITIAL_SIZE];
	}

	// Name			: Insert
	// Description	: This function inserts an element to the map
	// Parameters	:
	//	@key 	- key with which the specified value is to be associated
	//	@data 	-  value to be associated with the specified key
	// Return Value : Reference to the inserted object
	// 	If the key already exist, HashMapKeyAlreadyExistsException will be
	// thrown.
	V* Insert(K key, const V& obj) {
		if (Contains(key)) {
			throw HashMapKeyAlreadyExistsException();
		}

		int entry_index = hashFunction(key);
		V* to_add = new V(obj);
		
		
		// Key doesn't exist for sure, no need to use try/catch blocks
		// Insert the pointer to the tree
		entries[entry_index].Insert(key, to_add);
		_count++;

		loadFactorCheckAndResize();

		return to_add;
	}

	// Name			: Delete
	// Description	: Removes the mapping for the specified key from this map if present.
	// Parameters	:
	//	@key - key whose mapping is to be removed from the map
	// Return Value : None, if the key wasn't found a suitable exception will
	// be thrown (HashMapKeyNotFoundException).
	void Delete(const K & key) {
		if (!Contains(key)) {
			throw HashMapKeyNotFoundException();
		}

		int entry_index = hashFunction(key);

		// Key exists for sure, no need to use try/catch blocks
		// Delete the pointer and then delete from the tree
		auto tree_it = entries[entry_index].Find(key);
		delete *tree_it;
		entries[entry_index].Delete(key);
		_count--;

		loadFactorCheckAndResize();

	}

	// Name			: Find
	// Description	: Finds an element with key equivalent to key.
	// Parameters	:
	//	key - key value of the element to search for
	// Return Value : Reference to an element with key equivalent to key.
	//					If no such element is found, an exception would be
	// 					thrown.
	V& Find(const K& key) const {
		if (!Contains(key)) {
			throw HashMapKeyNotFoundException();
		}

		int entry_index = hashFunction(key);

		// Key exists for sure, no need to use try/catch blocks
		auto tree_it = entries[entry_index].Find(key);
		return *(*tree_it);
	}

	// Name			: isEmpty
	// Description	: This function tests whether the map is empty or not.
	// Parameters	: None
	// Return Value : true if this map contains no key-value mappings
	bool Empty() const {
		return (EMPTY_TABLE == _count);
	}

	// Name			: Contains
	// Description	: Tests if this map contains a mapping for the specified key.
	// Parameters	:
	//	key - The key whose presence in this map is to be tested
	// Return Value : true if this map contains a mapping for the specified key
	bool Contains(const K& key) const {
		int entry_index = hashFunction(key);

		try {
			entries[entry_index].Find(key);
		} catch (AVLTreeKeyNotFoundException&) {
			return false;
		}

		return true;
	}

	// Name			: getSize
	// Description	: Returns the number of key-value mappings in this map.
	// Parameters	: None
	// Return Value : the number of key-value mappings in this map
	int getSize() const {
		return _count;
	}

	// HashMap destructor
	~HashMap() {
		// Delete the pointers in all the tree nodes
		// Then, destroy all the entries
		for (int i = 0; i < _size; i++) {
			int tree_size = entries[i].getSize();
			V*** tree_data = entries[i].inOrderExtractData();
			// The above array cointains pointers to pointers of V type

			for (int j = 0; j < tree_size; j++) {
				delete *(tree_data[j]);
			}
			
			delete[] tree_data;
		}

		
		delete[] entries;
	}
};

#endif /* HASH_MAP_HPP_ */
