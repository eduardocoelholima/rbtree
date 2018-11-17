#ifndef _RBTREE_CPP_
#define _RBTREE_CPP_
#include <iostream>
#include "RBTree.hpp"
using namespace std;


template <typename T>
RBTree<T>::RBTree() {
    sentynel = new Node<T>("<sentynel>", BLACK, sentynel, sentynel, sentynel);
    sentynel->parent = sentynel;
    sentynel->left = sentynel;
    sentynel->right = sentynel;
	root = sentynel;
}


template <class T>
RBTree<T>::~RBTree() {
	clear(root);
    delete sentynel;
}


template <class T>
void RBTree<T>::leftRotate(Node<T> *x) {
	// y = x's right child
	Node<T> *y = x->right;
	x->right = y->left;
	if (y->left != sentynel) {
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == sentynel) {
		root = y;
	} else if (x == x->parent->left) {
		x->parent->left = y;
	} else {
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}


template <class T>
void RBTree<T>::rightRotate(Node<T> *y) {
	Node<T> *x = y->left;
	y->left = x->right;
	if (x->right != sentynel) {
		x->right->parent = y;
	}
	x->parent = y->parent;
	if (y->parent == sentynel) {
		root = x;
	} else if (y == y->parent->right) {
		y->parent->right = x;
	} else {
		y->parent->left = x;
	}
	x->right = y;
	y->parent = x;
}


template <class T>
void RBTree<T>::insert(T key) {
    cout<<"> Inserting key "<<key<<"...";
    // if the key already exists
    if (find(key)!=sentynel) {
        cout<<" already present, nothing to be done."<<endl;
        return;
	}

    // if the key is not present in the tree 
	Node<T> *node = new Node<T>(key, BLACK, sentynel, sentynel, sentynel);

	// walks the tree downwards to find the spot for the new node to be inserted
    // after the loop, y = x's parent, x = spot for the node to be inserted
	Node<T> *y = sentynel;
	Node<T> *x = root;
	while (x != sentynel) {
		y = x;
		if (node->key < x->key) {
			x = x->left;
		} else {
			x = x->right;
		}
	}
	node->parent = y;
	if (y == sentynel) {
		root = node;
	} else if (node->key < y->key) {
		y->left = node;
	} else {
		y->right = node;
	}
	node->left = sentynel;
	node->right = sentynel;
	node->color = RED;

	// fix up the tree balancing
	insertFixUp(node);
    cout<<" success."<<endl;
}


template <class T>
void RBTree<T>::insertFixUp(Node<T> *node) {
	Node<T> *p, *pp;
	// p = node's parent
    // pp = node's grandparent
	while ((p = node->parent) && p->color == RED) {
		pp = p->parent;
		if (p == pp->left) {
			// case 1
			Node<T> *uncle = pp->right;
			if (uncle && uncle->color == RED) {
				p->color = BLACK;
				uncle->color = BLACK;
				pp->color = RED;
				node = pp;
			} else if (node == p->right) { // case 2
				Node<T> *tmp;
				leftRotate(p);
				tmp = p;
				p = node;
				node = tmp;
			} else if (node == p->left) { // case 3
				p->color = BLACK;
				pp->color = RED;
				rightRotate(pp);
			}
		} else {
			Node<T> *u = pp->left;
			if (u && u->color == RED) {
				p->color = BLACK;
				u->color = BLACK;
				pp->color = RED;
				node = pp;
			} else if (node == p->left) {
				Node<T> *tmp;
				rightRotate(p);
				tmp = p;
				p = node;
				node = tmp;
			} else if (node == p->right) {
				p->color = BLACK;
				pp->color = RED;
				leftRotate(pp);
			}
		}
	}
	root->color = BLACK;
}

//moves node v to u's position
template <class T>
void RBTree<T>::transplant(Node<T> *u, Node<T> *v) {
    if (u->parent == sentynel) {
        root = v;
    }
    else {
        if (u == u->parent->left) {
            if (u->parent!=sentynel) u->parent->left = v;
        }
        else if (u->parent!=sentynel) u->parent->right = v;
    }
    if (v!=sentynel) 
        v->parent = u->parent;
}

//locates tree's node with the least key
template <class T>
Node<T>* RBTree<T>::treeMinimum(Node<T> *x) {
    while (x->left != sentynel) x = x->left;
    return x;
}

//deletes the node with the given key
template <class T>
void RBTree<T>::erase(T key) {;
    cout<<"> Erasing key "<<key<<"...";
    Node<T> *z = find(key); // z is the node that will be erased
    if (z == sentynel) {
        cout<<": not found, nothing to be done."<<endl;
        return;
    }
    cout<<": found. ";
    Node<T> *x, *y; // x keeps track of color balancing
    int yOriginalColor;
    y = z;
    yOriginalColor = y->color;
    if (z->left == sentynel) { // if z has no left childs, z right child can replace it
        x = z->right;
        transplant(z, z->right);
    }
    else {
        if (z->right == sentynel) { // if z as no right child, z left child can replace it
            x = z->left;
            transplant(z, z->left);
        }
        else { // z has both children
            y = treeMinimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z)
                x->parent = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
    }
    //delete z;
    if (yOriginalColor == BLACK and x!=sentynel)
        eraseFixUp(x);
    cout<<" Removed. "<<endl;

    // per project requirement, will call print and check for every successful removal
    print();
    cout<<"Tree check:"<<endl;
    check();

}


// eraseFixUp
template <class T>
void RBTree<T>::eraseFixUp(Node<T> *x) {
    Node<T> *brother; // brother = w in cormen's pseudocode
    while(x != root && (x->color == BLACK || x == sentynel)) {
        if (x == x->parent->left) { // x is the left child
            brother = x->parent->right;
            if (brother->color == RED) {
                // case 1: brother is a red node
                brother->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                brother = x->parent->right;
            } 
            if ((brother->left->color == BLACK || brother->left == sentynel) &&
                (brother->right->color == BLACK || brother->right == sentynel)) {
                // case 2: brother is a black node and both its children are black
                brother->color = RED;
                x = x->parent;
            }
            else {
                if (brother->right->color == BLACK || brother->right == sentynel) {
                    // case 3: brother's right child is black, left child is red
                    brother->left->color = BLACK;
                    brother->color = RED;
                    rightRotate(brother);
                    brother = x->parent->right;
                }

                // case 4: brother's right child is red
                brother->color = x->parent->color;
                x->parent->color = BLACK;
                brother->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else { //x is the right child
            brother = x->parent->left;
            if (brother->color == RED) {
                // case 1: brother is a red node
                brother->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                brother = x->parent->left;
            } 
            if ((brother->right->color == BLACK || brother->right == sentynel) &&
                (brother->left->color == BLACK || brother->left == sentynel)) {
                // case 2: brother is a black node and both its children are black
                brother->color = RED;
                x = x->parent;
            }
            else {
                if (brother->left->color == BLACK || brother->left == sentynel) {
                    // case 3: brother's left child is black, right child is red
                    brother->right->color = BLACK;
                    brother->color = RED;
                    leftRotate(brother);
                    brother = x->parent->left;
                }
                // case 4: brother's left child is red
                brother->color = x->parent->color;
                x->parent->color = BLACK;
                brother->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}


// search for a key and prints it (very similar to find, but
// prints the result and has no return
template <class T>
void RBTree<T>::search(T key){
    Node<T> *result = find(root, key);
    if (result == sentynel) cout<<"Not found."<<endl;
    else cout<<"Found key "<<key<<"."<<endl;
}


// find the node from a subtree that has a given key and returns it
// will return the sentynel if the key is not found
template <class T>
Node<T>* RBTree<T>::find(Node<T>* node, T key) {
    if (node==sentynel) return sentynel;
    if (node->key == key) return node;
    if (key < node->key)
        return find(node->left, key);
    else
        return find(node->right, key);
}


// find the node that has a given key and returns it
template <class T>
Node<T>* RBTree<T>::find(T key) {
    return find(root, key);
}


// removes every node from memory
template <class T>
void RBTree<T>::clear(Node<T> * &node) {
	if (node == sentynel) return;
	if (node->left != sentynel) {
		return clear(node->left);
	}
	if (node->right != sentynel) {
		return clear(node->right);
	}
	delete node;
	node = sentynel;
}


// return the black height of a node, which is the number
// of black nodes down a leaf, excluding the node itself
// leafs counts as a black node
template <class T>
int RBTree<T>::height(Node<T> *node){
    int height = 0;
    while (node != sentynel) {
        if (node->left->color==BLACK) height++;
        node = node->left;
    }
    return height;
}


// prints the (sub)tree from a given node
template <class T>
void RBTree<T>::print(Node<T> *node){
    if (node != sentynel) {
        print(node->left);
        cout << node->key << " ";
        print(node->right);
    }
}


// prints the tree keys from the root, adding openning and closing parenthesis
template <class T>
void RBTree<T>::print() {
    if (root == sentynel) {
        cout << "Tree print:\n <empty tree>" << endl;
        return;
    }
    cout << "Tree print:\n ";
    print(root);
    cout << " " << endl;
}

//check call
template <class T>
void RBTree<T>::check(Node<T> *node) {
    if (node != sentynel) {
        string line = " ( ";
        if (node->parent==sentynel) line+="NIL, ";
        else { line +=node->parent->key; line += ", ";}
        line += node->key;
        line += ", ";
        if (node->color==RED) line += "vermelho, ";
        else line += "preto, ";
        line += to_string(height(node));
        line +=", ";
        if (node->left==sentynel) line+="NIL, ";
        else {line += node->left->key; line += ", ";}
        if (node->right==sentynel) line += "NIL )\n";
        else {line += node->right->key; line += " )\n";}
        cout << line;
        check(node->left);
        check(node->right);
    }
}

//check call from root
template <class T>
void RBTree<T>::check() {
    if (root!=sentynel) check(root);
    else cout << " <empty tree>" << endl;
}


#endif // !_RBTREE_CPP_