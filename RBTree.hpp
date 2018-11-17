#ifndef _RBTree_
#define _RBTree_

#define RED	0
#define BLACK 1

template <class T>
class Node {
public:
	T key;
	int color;
	Node *left;
	Node *right;
	Node *parent;
	Node(T _key, int _color, Node *_left, Node *_right, Node *_parent)
		:key(_key), color(_color), left(_left), right(_right), parent(_parent) {}
};

template <class T>
class RBTree {
private:
	Node<T> *root;
	Node<T> *sentynel;
	void leftRotate(Node<T> *x);
	void rightRotate(Node<T> *y);
	void insertFixUp(Node<T> *node);
	void eraseFixUp(Node<T> *x);
    Node<T>* find(Node<T> *node, T key);
	void clear(Node<T> * &node);
	void print(Node<T> *node);
	void check(Node<T> *node);
	void transplant(Node<T> *u, Node <T> *v);
	Node<T>* treeMinimum(Node<T> *x);
	int height(Node<T> *node);

public:
	RBTree();
	void insert(T key);
	void erase(T key);
	Node<T>* find(T key);
	void print();
	void check();
	~RBTree();
	void search(T key);
};

#include "RBTree.cpp"

#endif // !_RBTree_