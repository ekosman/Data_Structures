#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_

//-------------------------------------- AVL TREE EXCEPTIONS
class AVLTreeException: public std::exception {
};
class AVLTreeKeyNotFoundException: public AVLTreeException {
};
class AVLTreeKeyAlreadyExistsException: public AVLTreeException {
};
class AVLTreeInvalidArgException: public AVLTreeException {
};
class AVLTreeNullArgException: public AVLTreeException {
};
class AVLTreeIteratorReachedEnd: public AVLTreeException {
};
class AVLTreeIteratorReachedRoot: public AVLTreeException {
};

//-------------------------------------- HashMap EXCEPTIONS
class HashMapException: public std::exception {
};
class HashMapKeyNotFoundException: public HashMapException {
};
class HashMapKeyAlreadyExistsException: public HashMapException {
};

#endif /* EXCEPTIONS_HPP_ */
