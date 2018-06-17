#ifndef AVLTREE_HPP_
#define AVLTREE_HPP_

//
//	File		: avltree.hpp
//	Description	: Generic implementation of an AVL tree.
//

#include <exception>
#include <iostream>
#include "assert.h"
#include "exceptions.hpp"
#include <cstdlib>
#include <cmath>

template<class T, typename KeyType> class AVLTree {

protected:

	//
	//	Class		: Node
	//	Description : AVL Tree node object class
	//
	class Node {
	private:
		KeyType _key;
		T _data;
		Node* _left;
		Node* _right;
		Node* _parent;
		int _left_height;
		int _right_height;

		//
		// Constants
		//
		static const int HEIGHT_ZERO = 0;

	public:
		Node() :
				_left(NULL), _right(NULL), _parent(NULL), _left_height(
						HEIGHT_ZERO), _right_height(HEIGHT_ZERO) {
		}

		// Node contstructor
		Node(KeyType key, T data) :
				_key(key), _data(data), _left(NULL), _right(NULL), _parent(
						NULL), _left_height(HEIGHT_ZERO), _right_height(
						HEIGHT_ZERO) {
		}

		//
		//	Node public methods
		//

		// Name			: getLeft
		// Description	: Returns a pointer to the left son of the node
		// Parameters	: None
		// Return Value : pointer to the left son
		Node* getLeft(void) {
			return _left;
		}

		// Name			: getRight
		// Description	: Returns a pointer to the right son of the node
		// Parameters	: None
		// Return Value : pointer to the right son
		Node* getRight(void) {
			return _right;
		}

		// Name			: getParent
		// Description	: Returns a pointer to the node's parent
		// Parameters	: None
		// Return Value : pointer to the node's parent
		Node* getParent(void) {
			return _parent;
		}

		// Name			: getData
		// Description	: Returns a pointer to the node's data
		// Parameters	: None
		// Return Value : pointer to the node's data
		T& getData(void) {
			return _data;
		}

		// Name			: isLeaf
		// Description	: This function returns whether the node is a leaf
		//	or not.
		// Parameters	: None
		// Return Value : true if leaf, false otherwise
		bool isLeaf(void) {
			return (_left == NULL && _right == NULL);
		}

		// Name			: isFull
		// Description	: Checks if the node has both left and right children.
		// Parameters	: None
		// Return Value : true if full, false otherwise
		bool isFull(void) {
			return (_left != NULL && _right != NULL);
		}

		// Name			: getBalance
		// Description	: Returns the balance factor of the node. The balance
		//	factor is the the height of the left subtree minus the height
		// 	of the right subtree.
		// Parameters	: None
		// Return Value : the node's balance factor
		int getBalance(void) {
			return _left_height - _right_height;
		}

		// Name			: setLeft
		// Description	: This function sets the left son of the node.
		// Parameters	:
		// 	@son - the requested node to set
		// Return Value : None
		void setLeft(Node * son) {
			_left = son;
			updateLeftHeight();
		}

		// Name			: setRight
		// Description	: This function sets the right son of the node.
		// Parameters	:
		// 	@son - the requested node to set
		// Return Value : none
		void setRight(Node* son) {
			_right = son;
			updateRightHeight();
		}

		// Name			: setData
		// Description	: This function sets the node's data
		// Parameters	:
		//	data - the new data to set
		// Return Value : none
		void setData(T & data) {
			_data = data;
		}

		// Name			: setKey
		// Description	: This function sets the node's key
		// Parameters	:
		//	key - the new key to set
		// Return Value : none
		void setKey(KeyType key) {
			_key = key;
		}

		// Name			: setParent
		// Description	: sets the node parent
		// Parameters	:
		// 	@parent - the requested node to set
		// Return Value : none
		void setParent(Node* parent) {
			_parent = parent;
		}

		// Name			: updateLeftHeight
		// Description	: Updates the left height of the given node.
		// Parameters	:
		//	@change - the new value to update
		// Return Value : None
		void updateLeftHeight() {
			_left_height = (_left == NULL) ? 0 : _left->getHeight();
		}

		// Name			: updateRightHeight
		// Description	: Updates the right height of the given node.
		// Parameters	:
		//	@change - the new value to update
		// Return Value : None
		void updateRightHeight() {
			_right_height = (_right == NULL) ? 0 : _right->getHeight();
		}
		// Name			: getHeight
		// Description	: This function returns the height of the node.
		// The height is defined as the maximum subtree size + 1.
		// Parameters	: None
		// Return Value : the height of the node
		int getHeight() {
			return std::max(_left_height, _right_height) + 1;
		}

		// Name			: disconnectFromParent
		// Description	: This function disconnect the node from its parent.
		// Parameters	: None
		// Return Value	: None
		void disconnectFromParent(void) {
			if (_parent != NULL) {
				if (_parent->getLeft() == this) {
					_parent->setLeft(NULL);
				} else {
					_parent->setRight(NULL);
				}
			}
			setParent(NULL);
		}

		// Name			: attachParentAndSon
		// Description	: This function attaches between the node's parent
		// and his son. Ths function assumes that the node has only one son.
		// Parameters	: None
		// Return Value	: None
		void attachParentAndSon(void) {
			if (_parent != NULL) {
				if (getRight() != NULL) {
					if (isLeftRight() == true) {
						_parent->setRight(_right);
					} else {
						_parent->setLeft(_right);
					}
					if (_right != NULL) {
						_right->setParent(_parent);
					}
				} else {
					if (isLeftRight() == true) {
						_parent->setRight(_left);
					} else {
						_parent->setLeft(_left);
					}
					if (_left != NULL) {
						_left->setParent(_parent);
					}

				}
			} else {
				if (_right != NULL) {
					_right->setParent(NULL);
				} else if (_left != NULL) {
					_left->setParent(NULL);
				}
			}
			setParent(NULL);
		}

		// Name			: isLeftRight
		// Description	: This function tests whether the node is a right son
		// of his parent or a left son.
		// Parameters	: None
		// Return Value	: true - if right son, false otherwise
		// If the node doesn't have any parent, an exception will be raised.
		bool isLeftRight(void) {
			if (_parent == NULL) {
				throw AVLTreeNullArgException();
			}
			return (_parent->getRight() == this);
		}

		// Name			: swap
		// Description	: This function swaps between the current node to the
		// given node.
		// Parameters	:
		//	@node - the node to swap with
		// Return Value	: None
		void swap(Node * node) {
			if (node == NULL) {
				throw AVLTreeNullArgException();
			}

			T temp_data = _data;
			_data = node->getData();
			node->setData(temp_data);

			KeyType temp_key = _key;
			_key = node->getKey();
			node->setKey(temp_key);
		}

		// Node destructor
		~Node() {
		}

		// Name			: getKey
		// Description	: This function returns the node's key
		// Parameters	: None
		// Return Value	: the node's key
		KeyType getKey(void) {
			return _key;
		}

		bool operator<(const Node& node) const {
			return _key < node._key;
		}

		bool operator>(const Node& node) const {
			return node < *this;
		}

		bool operator==(const Node& node) const {
			return !(*this > node) && !(*this < node);
		}
	};

private:
	Node * root;
	Node * minimal;
	int size;

	//
	// Constants
	//
	static const int UNBALANCED_FACTOR_SON_ZERO = 0;
	static const int UNBALANCED_FACTOR_SON = 1;
	static const int UNBALANCED_FACTOR_SON_NEGATIVE = -1;
	static const int UNBALANCED_FACTOR = 2;
	static const int UNBALANCED_FACTOR_NEGATIVE = -2;
	static const int NEW_NODE_HEIGHT = 1;
	static const int INITIAL_SIZE = 0;
	static const int UNBALANCED_TREE = 2;
	static const int POWER_TWO = 2;
	/*
	 int pow(int base, int exp){
	 int res = 1;
	 while (exp != 0) res *= base;

	 return res;
	 }
	 */
	// Name			: recursiveFind
	// Description	: Searches a given key recursively from the given tree node
	// Parameters	:
	//	@current - the node to start from
	//	@key - the key to find
	// Return Value : If the key was found, the suitable node will be returned,
	//				  otherwise, NULL.
	Node * recursiveFind(Node * current, KeyType key) {
		if (current == NULL) {
			return NULL;
		}

		if (current->getKey() > key) {
			return recursiveFind(current->getLeft(), key);
		} else if (current->getKey() < key) {
			return recursiveFind(current->getRight(), key);
		} else {
			return current;
		}
	}
	// Name			: rotateLL
	// Description	: Makes a LL rotation
	// Parameters	:
	//	@B - B node (See AVL's rotations for more info)
	// Return Value : This function returns the new root of the rotated subtree.
	Node* rotateLL(Node * B) {
		Node* B_Parent = B->getParent();
		Node* A = B->getLeft();
		Node* A_right = A->getRight();

		B->setLeft(A_right);
		if (A_right != NULL) {
			A_right->setParent(B);
		}

		A->setRight(B);
		B->setParent(A);
		A->setParent(B_Parent);

		if (B_Parent != NULL) {
			if (B_Parent->getLeft() == B) {
				B_Parent->setLeft(A);
			} else {
				B_Parent->setRight(A);
			}
		}

		if (root == B) {
			root = A;
		}

		return A;
	}

	// Name			: rotateRR
	// Description	: Makes a RR rotation
	// Parameters	:
	//	@A - A node (See AVL's rotations for more info)
	// Return Value : This function returns the new root of the rotated subtree.
	Node* rotateRR(Node * A) {
		Node* A_parent = A->getParent();
		Node* B = A->getRight();
		Node* B_left = B->getLeft();

		A->setRight(B_left);
		B->setLeft(A);

		if (B_left != NULL) {
			B_left->setParent(A);
		}

		A->setParent(B);
		B->setParent(A_parent);

		if (A_parent != NULL) {
			if (A_parent->getLeft() == A) {
				A_parent->setLeft(B);
			} else {
				A_parent->setRight(B);
			}
		}

		if (root == A) {
			root = B;
		}

		return B;
	}

	// Name		: rotateLR
	// Description	: Makes a LR rotation
	// Parameters	:
	//		@C - the 'C' node (Look on AVL's rotation for more info)
	// Return Value : This function returns the new root of the
	//	rotated subtree.
	Node* rotateLR(Node * C) {
		rotateRR(C->getLeft());
		return rotateLL(C);
	}

	// Name		: rotateRL
	// Description	: Makes a RL rotation
	// Parameters	:
	//		@C - the 'C' node (Look on AVL's rotation for more info)
	// Return Value : This function returns the new root of the
	//	rotated subtree.
	Node* rotateRL(Node * C) {
		rotateLL(C->getRight());
		return rotateRR(C);
	}

	// Name		: rotate
	// Description	: This function determines if a rotation should occur,
	// 	if so, it makes the rotation.
	// Parameters	:
	//		@node - the tested node
	// Return Value : None
	void rotate(Node * node) {
		if (node->getBalance() == UNBALANCED_FACTOR) {
			if (node->getLeft()->getBalance() >= UNBALANCED_FACTOR_SON_ZERO) {
				rotateLL(node);
			} else if (node->getLeft()->getBalance()
					== UNBALANCED_FACTOR_SON_NEGATIVE) {
				rotateLR(node);
			}
		} else if (node->getBalance() == UNBALANCED_FACTOR_NEGATIVE) {
			if (node->getRight()->getBalance() == UNBALANCED_FACTOR_SON) {
				rotateRL(node);
			} else if (node->getRight()->getBalance()
					<= UNBALANCED_FACTOR_SON_ZERO) {
				rotateRR(node);
			}
		}
	}

	// Name			: recursiveInsert
	// Description	: Insert a given node recursively to the tree.
	// Parameters	:
	//		@current - starting node
	// 	@to_insert - the inserted node
	// Return Value : None
	void recursiveInsert(Node * current, Node * to_insert) {
		if (current->isLeaf() == true) {
			if (*current < *to_insert) {
				current->setRight(to_insert);
			} else {
				current->setLeft(to_insert);
			}
			to_insert->setParent(current);
		} else if (current->isFull() == true) {
			if (*current < *to_insert) {
				recursiveInsert(current->getRight(), to_insert);
				current->updateRightHeight();
			} else {
				recursiveInsert(current->getLeft(), to_insert);
				current->updateRightHeight();
			}
		} else if (current->getLeft() != NULL) {
			if (*to_insert > *current) {
				current->setRight(to_insert);
				to_insert->setParent(current);
			} else {
				recursiveInsert(current->getLeft(), to_insert);
				current->updateLeftHeight();
			}
		} else {
			if (*to_insert < *current) {
				current->setLeft(to_insert);
				to_insert->setParent(current);
			} else {
				recursiveInsert(current->getRight(), to_insert);
				current->updateRightHeight();
			}
		}

		rotate(current);
	}

	// Name			: recursiveDelete
	// Description	: This function deletes the node that fits the given
	// 					key. We assume that the key exists
	// Parameters	:
	//	@current - the node to start from
	//	@key - the key represents the node to delete
	// Return Value : None
	void recursiveDelete(Node * current, const KeyType& key) {
		if (current->getKey() == key) {
			if (current->isLeaf() == true) {
				current->disconnectFromParent();
				if (current == root) {
					root = NULL;
				}
				delete current;
				return;
			} else if (current->isFull() == true) {
				Node* current_right = current->getRight();
				while (current_right->getLeft() != NULL) {
					current_right = current_right->getLeft();
				}
				current->swap(current_right);
				recursiveDelete(current->getRight(), key);
				if (current->getRight() != NULL) {
					current->updateRightHeight();
				}
			} else {
				current->attachParentAndSon();
				if (current == root) {
					if (current->getLeft() != NULL) {
						root = current->getLeft();
					} else {
						root = current->getRight();
					}
				}
				delete current;
				return;
			}
		} else {
			if (key > current->getKey()) {
				recursiveDelete(current->getRight(), key);
				if (current->getRight() != NULL) {
					current->updateRightHeight();
				}
			} else {
				recursiveDelete(current->getLeft(), key);
				if (current->getLeft() != NULL) {
					current->updateLeftHeight();
				}
			}
		}

		rotate(current);
	}

	// Name			: inorderOutputAux
	// Description	: This is an auxilary function for the  inorderOutput
	// function. It outputs the tree data to the given output stream.
	//	@current 	- the node to start from
	//	@output		- output stream
	// Return Value : None
	void inorderOutputAux(Node * current, std::ostream& output) const {
		if (current == NULL) {
			return;
		}
		inorderOutputAux(current->getLeft(), output);
		output << (current->getKey()) << ",";
		inorderOutputAux(current->getRight(), output);
	}

	// Name			: getMaxBalanceFactorAux
	// Description	: This is an auxilary function for the getMaxBalanceFactor
	// function. It searches for the maximum balance factor (abs) in the tree.
	// Parameters	:
	//	@current 	- the node to start from
	// Return Value : maximum balance factor
	int getMaxBalanceFactorAux(Node * current) {
		if (current == NULL) {
			return 0;
		}
		return std::max(abs(current->getBalance()),
				std::max(getMaxBalanceFactorAux(current->getRight()),
						getMaxBalanceFactorAux(current->getLeft())));
	}

	// Name			: recursiveDestruct
	// Description	: This function deletes all nodes in the tree, using post
	// order algorithm.
	// Parameters	:
	//	@current 	- the node to start the delete action from
	// Return Value : None
	void recursiveDestruct(Node* current) {
		if (current == NULL) {
			return;
		}
		recursiveDestruct(current->getLeft());
		recursiveDestruct(current->getRight());
		delete current;
		size--;
	}

	//
	//	Name		:	updateMinimal
	//	Parameters	:
	//		@current - current node
	//	Description	:	The function updates to minimal node field of
	//					tree to contain the minimal value in the tree.
	//	Return Value: None
	void updateMinimal(Node* current) {
		if (current == NULL) {
			minimal = NULL;
		} else if (current->getLeft() != NULL) {
			updateMinimal(current->getLeft());
		} else {
			minimal = current;
		}
	}

	//
	//	Name		:	extractTreeDataInOrderAux
	//	Description	:	This function extracts the data from the tree
	//					to the output array.
	//	Parameters	:
	//		@node - starting node
	//		@out - pointer to the output array
	//		@index - pointer to the index in the array
	//	Return Value: None
	void extractTreeDataInOrderAux(Node* node, T** out, int* index) {
		if (out == NULL || index == NULL) {
			throw AVLTreeNullArgException();
		}
		if (node == NULL)
			return;

		extractTreeDataInOrderAux(node->getLeft(), out, index);

		T& temp = node->getData();
		(out[*index]) = &temp;
		*index += 1;

		extractTreeDataInOrderAux(node->getRight(), out, index);

	}

	//
	//	Name		:	getTreeHeightByNodesCount
	//	Description	:	Given number of nodes, the function computes
	//					the height of the smallest "full-tree" which
	//					has more than @count nodes.
	//	Parameters	:
	//		@count - the number of nodes that must be in the tree
	//	Return Value: 	the suitable height
	//
	int getTreeHeightByNodesCount(int count) {
		if (count < 0) {
			throw AVLTreeInvalidArgException();
		}

		int height = 0;
		while (pow(POWER_TWO, height) - 1 < count) {
			height++;
		}

		return height;
	}

	//
	//	Name		:	createBlankTree
	//	Description	:	This function creates a new "full-tree" with
	//					the requested height (without data).
	//	Parameters	:
	//		@height - the height of the new tree
	//	Return Value: pointer to the new tree
	Node* createBlankTree(int height) {
		if (height == 0) {
			return NULL;
		}

		Node* root = new Node();
		Node* left = createBlankTree(height - 1);
		Node* right = createBlankTree(height - 1);

		root->setLeft(left);
		if (left != NULL) {
			left->setParent(root);
		}

		root->setRight(right);
		if (right != NULL) {
			right->setParent(root);
		}

		size++;
		return root;
	}

	//
	//	Name		:	fillTreeInOrder
	//	Description	:	The function fills a blank tree with in-order
	//					algorithm, based on the data in the given array.
	//					NOTE: the data should be ordered.
	//	Parameters	:
	//		@root - the tree root
	//		@ordered_data - input data
	//		@ordered_keys - input keys
	//		@index - arrays current index
	//	The keys and data arrays should be in the same order (for example
	//	the first data element should fit the first key).
	void fillTreeInOrder(Node* node, T* ordered_data, KeyType* ordered_keys,
			int * index) {
		if (node == NULL) {
			return;
		}
		if (ordered_data == NULL || ordered_keys == NULL || index == NULL) {
			throw AVLTreeInvalidArgException();
		}

		fillTreeInOrder(node->getLeft(), ordered_data, ordered_keys, index);
		node->setData(ordered_data[*index]);
		node->setKey(ordered_keys[*index]);
		(*index)++;
		fillTreeInOrder(node->getRight(), ordered_data, ordered_keys, index);
	}

	//
	//	Name		:	trimTreeInOrder
	//	Description	:	The function trims the tree so that after the
	//					operation, it has exactly the requested amount of nodes.
	//	Parameters	:
	//		@node - starting node
	//		@current_size - current tree size
	//		@requested_size - final size
	//	Return Value: None
	void trimTreeInOrder(Node* node, int* current_size, int requested_size) {
		if (*current_size == requested_size) {
			return;
		}
		if (node == NULL) {
			return;
		}

		if (current_size == NULL) {
			throw AVLTreeNullArgException();
		}

		trimTreeInOrder(node->getLeft(), current_size, requested_size);
		if (node->isLeaf()) {
			node->disconnectFromParent();
			delete node;
			size--;
			(*current_size)--;
			return;
		}
		trimTreeInOrder(node->getRight(), current_size, requested_size);

		node->updateLeftHeight();
		node->updateRightHeight();
	}

public:
	//
	//	Class		: iterator
	// 	Description	: An iterator. Used to iterate over the data in an AVLTree.
	//
	class iterator {
	private:
		Node* current;
		AVLTree * tree;
	public:
		// Constructs a new iterator that points to a given node in the given
		// tree.
		iterator(Node* node, AVLTree * tree) :
				current(node), tree(tree) {
		}

		// Name			: MoveLeft
		// Description	: Move the given iterator to left son
		// Parameters	: None
		// Return Value : Iterator to the left son
		iterator& MoveLeft(void) {
			if (current == NULL) {
				throw AVLTreeIteratorReachedEnd();
			}

			current = current->getLeft();
			return *this;
		}

		// Name			: MoveRight
		// Description	: Move the given iterator to right son
		// Parameters	: None
		// Return Value : Iterator to the right son
		iterator& MoveRight(void) {
			if (current == NULL) {
				throw AVLTreeIteratorReachedEnd();
			}

			current = current->getRight();
			return *this;
		}

		// Name			: moveToParent
		// Description	: Move the given iterator to its parent
		// Parameters	: None
		// Return Value : Iterator to parent
		iterator& moveToParent(void) {
			if (current == NULL) {
				throw AVLTreeIteratorReachedEnd();
			}
			Node * parent = current->getParent();
			if (parent == NULL) {
				throw AVLTreeIteratorReachedRoot();
			}

			current = parent;

			return *this;
		}

		// Dereference operator
		// Description	: Returns the iterator's data
		// Return Value : the iterator's saved data
		T& operator*() const {
			if (current == NULL) {
				throw AVLTreeIteratorReachedEnd();
			}

			return current->getData();
		}

		// Equal operator
		// Description	: Two iterators are equals if their keys are equals,
		// and they are part of the same tree.
		// Parameters	:
		//	@rhs - right side tree iterator
		// Return Value	: true if equal, false otherwise
		bool operator==(const iterator& rhs) const {
			return ((tree == rhs.tree) && (current == rhs.current));
		}

		// Not Equal operator
		// Description	: Two iterators are equals if their keys are equals,
		// and they are part of the same tree.
		// Parameters	:
		//	@rhs - right side tree iterator
		// Return Value	: true if not equal, false if equal
		bool operator!=(const iterator& rhs) const {
			return (!(*this == rhs));
		}

	};

	//
	// Public interface
	//
	template<class F, typename KeyTypeF>
	friend std::ostream& operator<<(std::ostream& output,
			const AVLTree<F, KeyTypeF>& tree);

	// AVLTree constructor
	AVLTree() :
			root(NULL), minimal(NULL), size(INITIAL_SIZE) {
	}
	;

	//	AVLTree destructor
	~AVLTree() {
		recursiveDestruct(root);
	}

	// Name			: Insert
	// Description	: This function inserts a new node to the tree.
	// Parameters	:
	//	@key 	- the node's key
	//	@data 	- the node's data
	// Return Value : None
	// 	If the key already exist, AVLTreeKeyAlreadyExistsException will be
	// thrown.
	void Insert(KeyType key, T const& data) {
		bool already_exist = true;
		if (Empty() == true) {
			root = new Node(key, data);
			size++;
			updateMinimal(root);
			return;
		}

		try {
			Find(key);
		} catch (AVLTreeKeyNotFoundException&) {
			recursiveInsert(root, new Node(key, data));
			already_exist = false;
		}
		if (already_exist == true) {
			throw AVLTreeKeyAlreadyExistsException();
		}

		size++;
		// Finally, update the minimal node
		updateMinimal(root);
	}

	// Name			: Delete
	// Description	: This function deletes a node from the tree, by the given
	// key.
	// Parameters	:
	//	@key - the key represents the node to delete
	// Return Value : None, if the key wasn't found a suitable exception will
	// be thrown (AVLTreeKeyNotFoundException).
	void Delete(const KeyType & key) {
		Find(key);

		recursiveDelete(root, key);

		size--;
		if (size == 0) {
			root = NULL;
		}
		// Finally, update minimal node
		updateMinimal(root);
	}

	// Name			: Find
	// Description	: This function searches the tree for a given key.
	// Parameters	: key - the key to search
	// Return Value : If the key was found in the tree, an iterator for
	// the suitable node will be returned. Otherwise, an exception will be
	// thrown.
	iterator Find(const KeyType & key) {
		Node * searched_node = recursiveFind(root, key);
		if (searched_node == NULL) {
			throw AVLTreeKeyNotFoundException();
		}

		return iterator(searched_node, this);
	}

	// Name			: getSize
	// Description	: This function returns the total number of nodes in the
	// tree.
	// Parameters	: None
	// Return Value : The tree size will be returned
	int getSize() const {
		return size;
	}

	// Name			: Empty
	// Description	: This function tests whether the tree is empty or not.
	// Parameters	: None
	// Return Value : true if the tree is empty, false otherwise
	bool Empty(void) {
		return (root == NULL || getSize() == 0);
	}

	// Name			: inorderOutput
	// Description	: This function prints the tree contents (inorder)
	// Parameters	:
	//	@output - output stream
	// Return Value : None
	void inorderOutput(std::ostream& output) const {
		inorderOutputAux(root, output);
	}

	// Name			: isBalanced
	// Description	: This function tests if the tree is balanced. It should
	//					always return true.
	// Parameters	: None
	// Return Value : true if balanced, false otherwise
	bool isBalanced(void) {
		return (getMaxBalanceFactorAux(root) < UNBALANCED_TREE);
	}

	//
	//	Name		:	inOrderExtract
	//	Description	:	The function returns an ordered array which
	//					contains the data contained in the tree.
	// Parameters	:	None
	// Return Value	: 	Pointer to a data array
	T** inOrderExtract(void) {
		T** data_array = new T*[getSize()];
		int index = 0;

		extractTreeDataInOrderAux(root, data_array, &index);

		return data_array;
	}

	//
	//	Name		:	generateInOrder
	//	Description	:	Based on the data in the given arrays, the
	//					function constructs and fills the tree. The
	//					new tree would be "almost-full", means it has
	//					only few leafs missing in the last level.
	// Parameters	:
	//	@arr_data - the input data
	//	@arr_keys - the input keys
	//	@count	- number of elements in the array
	// Return Value	: None
	void generateInOrder(T** arr_data, KeyType* arr_keys, int count) {
		if (count < 0) {
			throw AVLTreeInvalidArgException();
		} else if (count == 0) {
			return;
		}

		if (arr_data == NULL || arr_keys == NULL) {
			throw AVLTreeNullArgException();
		}

		int i = 0;

		T* copied_data = new T[count];
		for (int i = 0; i < count; i++) {
			copied_data[i] = T(*(arr_data[i]));
		}
		if (size != count) {
			// must reset the tree
			// Destruct the current tree
			recursiveDestruct(root);

			// Generate the new tree
			int requested_height = getTreeHeightByNodesCount(count);
			int current_size = std::pow(POWER_TWO, requested_height) - 1;
			root = createBlankTree(requested_height);
			trimTreeInOrder(root, &current_size, count);
		}

		fillTreeInOrder(root, copied_data, arr_keys, &i);

		updateMinimal(root);

		delete[] copied_data;
	}

	//
	//	Name		:	getMinimal
	//	Description	:	The function returns the iterator to the minimal node
	//					in the tree.
	//	Parameters	: 	None
	//	Return Value: 	returns an iterator to the minimal node in the tree.
	iterator getMinimal(void) {
		if (minimal == NULL) {
			throw AVLTreeKeyNotFoundException();
		}

		return iterator(minimal, this);
	}

};

// Name			: operator<<
// Description	: << Operator overload, used to print the tree (inorder).
// Parameters	:
//	@output - output stream
//	@tree	- the tree to print
// Return Value : the output stream is returned
template<class T, typename KeyType>
std::ostream& operator<<(std::ostream& output,
		const AVLTree<T, KeyType>& tree) {
	tree.inorderOutput(output);

	return output;
}

#endif /* AVLTREE_HPP_ */
