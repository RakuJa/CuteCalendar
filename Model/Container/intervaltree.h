#ifndef INTERVALTREE_H
#define INTERVALTREE_H

#include <list>
#include <cstddef> // std::size_t

#include <iostream> // test

#include "invalidinterval.h"

enum RBTreeColor{BLACK, RED};

// i valori di maxBV e minBV sono necessari per la gestione di intervalli aperti
// consideriamo intervalli del tipo [a, b] o (-infty, b] o [a, +infty)
// DT -> DataType, BT -> BoundType, minBV -> min bound value, maxBV -> max bound value

template<class DT, class BT, BT minBV, BT maxBV>
class IntervalTree {
private:
    class Interval {
    public:
        BT _lowBound, _highBound;
        Interval(const BT& = minBV, const BT& = maxBV); // TODO: aggiungere eccezioni se minBound > maxBound
        // Interval& operator=(const Interval&) il funzionamento del costruttore di copia sarebbe uguale a quello standard

        bool overlaps(const Interval&) const;
        bool operlapsExclusive(const Interval&) const;
        bool isPoint() const;
        bool within(const Interval&) const;
        BT size() const;
        bool operator==(const Interval&) const;
    };

    class Node {
    public:
        DT _info;
        Interval _interval;
        BT _maxBoundChild;
        RBTreeColor _color;
        Node *_parent, *_right, *_left;


        Node(const DT&, const Interval& = Interval(), RBTreeColor = RED,
               Node* = nullptr, Node* = nullptr, Node* = nullptr);
        ~Node();

        bool isRight() const;
        bool isLeft() const;
        bool isRoot() const;
        BT getKey() const;
    };

    Node* _root;
    std::size_t _size;

    void insert_fixup(Node*); // O(log N)
    void left_rotate(Node*);
    void right_rotate(Node*);
    void transplant(Node*, Node*);
    void delete_fixup(Node*);
    void recalculate_max(Node*); // O(log N)
public:

    class BaseIterator {
        friend class IntervalTree<DT, BT, minBV, maxBV>;
    private:
        Node* _node;
        BaseIterator(Node*);
    public:
        BaseIterator();
        DT& operator*() const;
        DT* operator->() const;
        BaseIterator& operator++(); // TODO
        BaseIterator& operator--(); // TODO
        bool operator==(const BaseIterator&) const; // SI RIFERISCE ALL'AREA DI MEMORIA, NON ALL'OGGETTO PUNTATO
        bool operator!=(const BaseIterator&) const;
        BaseIterator right() const;
        BaseIterator left() const;
        BaseIterator parent() const;
    };

    // member functions
    IntervalTree(); // O(1)
    IntervalTree(const IntervalTree&);
    IntervalTree& operator=(const IntervalTree&);
    ~IntervalTree(); // O(n)

    // Iterators
    BaseIterator begin() const;
    BaseIterator end() const;
   //  ConstIterator cbegin() const;
   //  ConstIterator cend() const;

    // Capacity
    bool empty() const; // O(1)
    std::size_t size() const; // O(1)

    // Element access
    DT& front();
    const DT& front() const;
    DT& back();
    const DT& back() const;

    // Modifiers
    void clear();
    BaseIterator insert(const DT&, const BT&, const BT&); // O(log N)
    BaseIterator erase(const BaseIterator&);

    // Operations
    std::list<DT*> intersects(const BT&, const BT&) const;
    DT* intervalSearch(const BT&, const BT&) const;
};

// Interval class
template<class DT, class BT, BT minBV, BT maxBV>
IntervalTree<DT, BT, minBV, maxBV>::Interval::Interval(const BT& lowBound, const BT& highBound)
    : _lowBound(lowBound), _highBound(highBound) {}

template<class DT, class BT, BT minBV, BT maxBV>
bool IntervalTree<DT, BT, minBV, maxBV>::Interval::overlaps(const Interval& i) const {
    return _lowBound <= i._highBound && i._lowBound <= _highBound;
}

template<class DT, class BT, BT minBV, BT maxBV>
bool IntervalTree<DT, BT, minBV, maxBV>::Interval::operlapsExclusive(const Interval& i) const {
    return _lowBound < i._highBound && i._lowBound < _highBound;
}

template<class DT, class BT, BT minBV, BT maxBV>
bool IntervalTree<DT, BT, minBV, maxBV>::Interval::isPoint() const {
    return _lowBound == _highBound;
}

template<class DT, class BT, BT minBV, BT maxBV>
BT IntervalTree<DT, BT, minBV, maxBV>::Interval::size() const {
    return _highBound - _lowBound;
}

template<class DT, class BT, BT minBV, BT maxBV>
bool IntervalTree<DT, BT, minBV, maxBV>::Interval::within(const Interval& i) const {
    return _lowBound >= i._lowBound && _highBound <= i._highBound;
}

template<class DT, class BT, BT minBV, BT maxBV>
bool IntervalTree<DT, BT, minBV, maxBV>::Interval::operator==(const Interval& i) const {
    return _lowBound == i._lowBound && _highBound == i._highBound;
}

// Node class
template<class DT, class BT, BT minBV, BT maxBV>
IntervalTree<DT, BT, minBV, maxBV>::Node::Node(const DT& info,
       const Interval& interval, RBTreeColor color, Node* parent, Node* right, Node* left)
    : _info(info), _interval(interval), _maxBoundChild(interval._highBound), _color(color),
        _parent(parent), _right(right), _left(left) {}

template<class DT, class BT, BT minBV, BT maxBV>
bool IntervalTree<DT, BT, minBV, maxBV>::Node::isRight() const {
    return _parent && _parent->_right == this;
}

template<class DT, class BT, BT minBV, BT maxBV>
bool IntervalTree<DT, BT, minBV, maxBV>::Node::isLeft() const {
    return _parent && _parent->_left == this;
}

template<class DT, class BT, BT minBV, BT maxBV>
BT IntervalTree<DT, BT, minBV, maxBV>::Node::getKey() const {
    return _interval._lowBound;
}

template<class DT, class BT, BT minBV, BT maxBV>
bool IntervalTree<DT, BT, minBV, maxBV>::Node::isRoot() const {
    return !_parent;
}

template<class DT, class BT, BT minBV, BT maxBV>
IntervalTree<DT, BT, minBV, maxBV>::Node::~Node() {
    delete _left;
    delete _right;
}

// BaseIterator class
template<class DT, class BT, BT minBV, BT maxBV>
IntervalTree<DT, BT, minBV, maxBV>::BaseIterator::BaseIterator()
    : _node(nullptr) {}

template<class DT, class BT, BT minBV, BT maxBV>
IntervalTree<DT, BT, minBV, maxBV>::BaseIterator::BaseIterator(Node* n)
    : _node(n) {}

template<class DT, class BT, BT minBV, BT maxBV>
DT& IntervalTree<DT, BT, minBV, maxBV>::BaseIterator::operator*() const {
    return _node->_info;
}

template<class DT, class BT, BT minBV, BT maxBV>
DT* IntervalTree<DT, BT, minBV, maxBV>::BaseIterator::operator->() const {
    return &(_node->_info);
}

template<class DT, class BT, BT minBV, BT maxBV>
bool IntervalTree<DT, BT, minBV, maxBV>::BaseIterator::operator==(const BaseIterator& it) const {
    return _node == it._node; // confronto le aree di memoria
}

template<class DT, class BT, BT minBV, BT maxBV>
bool IntervalTree<DT, BT, minBV, maxBV>::BaseIterator::operator!=(const BaseIterator& it) const {
    return _node != it._node;
}

template<class DT, class BT, BT minBV, BT maxBV>
typename IntervalTree<DT, BT, minBV, maxBV>::BaseIterator IntervalTree<DT, BT, minBV, maxBV>::BaseIterator::right() const {
    return BaseIterator(_node->_right);
}

template<class DT, class BT, BT minBV, BT maxBV>
typename IntervalTree<DT, BT, minBV, maxBV>::BaseIterator IntervalTree<DT, BT, minBV, maxBV>::BaseIterator::left() const {
    return BaseIterator(_node->_left);
}

template<class DT, class BT, BT minBV, BT maxBV>
typename IntervalTree<DT, BT, minBV, maxBV>::BaseIterator IntervalTree<DT, BT, minBV, maxBV>::BaseIterator::parent() const {
    return BaseIterator(_node->_parent);
}

// Interval Tree class
template<class DT, class BT, BT minBV, BT maxBV>
IntervalTree<DT, BT, minBV, maxBV>::IntervalTree() : _root(nullptr), _size(0) {}

// Interval Tree - Capacity
template<class DT, class BT, BT minBV, BT maxBV>
bool IntervalTree<DT, BT, minBV, maxBV>::empty() const {
    return _size == 0;
}

template<class DT, class BT, BT minBV, BT maxBV>
std::size_t IntervalTree<DT, BT, minBV, maxBV>::size() const {
    return _size;
}

// Interval Tree - Modifiers
template<class DT, class BT, BT minBV, BT maxBV>
void IntervalTree<DT, BT, minBV, maxBV>::clear() {
    delete _root;
    _size = 0;
}

template<class DT, class BT, BT minBV, BT maxBV>
void IntervalTree<DT, BT, minBV, maxBV>::left_rotate(Node* x) {
       std::cout << "Left rotate" << std::endl;

    if(!x->_right) {
     //   return;
    std::cout << "Caso" << std::endl;
     }

    Node* y = x->_right;
    x->_right = y->_left;

    if(y->_left)
        y->_left->_parent = x;
    y->_parent = x->_parent;
    if(!x->_parent)
        _root = y;
    else if(x->isLeft())
        x->_parent->_left = y;
    else
        x->_parent->_right = y;

    y->_left = x;
    x->_parent = y;

    if (x->_left && x->_right)
                   x->_maxBoundChild = std::max(x->_interval._highBound, std::max(x->_left->_maxBoundChild, x->_right->_maxBoundChild));
               else if (x->_left)
                   x->_maxBoundChild = std::max(x->_interval._highBound, x->_left->_maxBoundChild);
               else if (x->_right)
                   x->_maxBoundChild = std::max(x->_interval._highBound, x->_right->_maxBoundChild);
               else
                   x->_maxBoundChild = x->_interval._highBound;

               if (y->_right)
                   y->_maxBoundChild = std::max(y->_interval._highBound, std::max(y->_right->_maxBoundChild, x->_maxBoundChild));
               else
                   y->_maxBoundChild = std::max(y->_interval._highBound, x->_maxBoundChild);
}

template<class DT, class BT, BT minBV, BT maxBV>
void IntervalTree<DT, BT, minBV, maxBV>::right_rotate(Node* y) {
    std::cout << "Right rotate" << std::endl;
    if(!y->_right) {
        std::cout << "Caso" << std::endl;
       //  return;
    }

    Node* x = y->_left;
      y->_left = x->_right;

      if (x->_right)
          x->_right->_parent = y;

      x->_parent= y->_parent;
      if (!y->_parent)
          _root = x;
      else if (y->isLeft())
          y->_parent->_left = x;
      else
          y->_parent->_right = x;

      x->_right = y;
      y->_parent = x;

      if (y->_left && y->_right)
         y->_maxBoundChild = std::max(y->_interval._highBound, std::max(y->_left->_maxBoundChild, y->_right->_maxBoundChild));
     else if (y->_left)
         y->_maxBoundChild = std::max(y->_interval._highBound, y->_left->_maxBoundChild);
     else if (y->_right)
         y->_maxBoundChild = std::max(y->_interval._highBound, y->_right->_maxBoundChild);
     else
         y->_maxBoundChild = y->_interval._highBound;

     if (x->_left)
         x->_maxBoundChild = std::max(x->_interval._highBound, std::max(x->_left->_maxBoundChild, y->_maxBoundChild));
     else
         x->_maxBoundChild = std::max(x->_interval._highBound, y->_maxBoundChild);
}

template<class DT, class BT, BT minBV, BT maxBV>
typename IntervalTree<DT, BT, minBV, maxBV>::BaseIterator
    IntervalTree<DT, BT, minBV, maxBV>::insert(const DT& data, const BT& lowBound, const BT& highBound) {

    Node *x = _root,
         *y = nullptr,
         *z = new Node(data, Interval(lowBound, highBound));

    // inserisco il nuovo nodo mantenendo le caratteristiche di un BST
    while(x) {
        y = x;
        if(lowBound < x->_interval._lowBound)
            x = x->_left;
        else
            x = x->_right;
    }

    z->_parent = y;
    if(!y)
        _root = z;
    else if(lowBound < y->_interval._lowBound)
       y->_left = z;
    else y->_right = z;

    std::cout << "Inserito, inizio il fixup..." << std::endl;
    // fix per mantenere le caratteristiche di un RBTree
     insert_fixup(z);
    std::cout << "Passato fixup" << std::endl;
    // fix per il massimo in ogni nodo (IntervalTree)
    recalculate_max(z);
    std::cout << "Calcolato il massimo" << std::endl;

    _size++;
    return BaseIterator(z);
}

template<class DT, class BT, BT minBV, BT maxBV>
void IntervalTree<DT, BT, minBV, maxBV>::insert_fixup(Node* z) {

    Node *parent = z->_parent;

    while(z->_parent && z->_parent->_parent && z->_parent->_color == RED) {
        Node *parent = z->_parent,
             *anchestor = parent->_parent;

        std::cout << "Sono entrato nel ciclo del fixup" << std::endl;

        if(parent == anchestor->_left) {
            Node* y = anchestor->_right;
            std::cout << "Primo ramo dell'if" << std::endl;
            if(y->_color == RED) {
                parent->_color = BLACK;
                y->_color = BLACK;
                anchestor->_color = RED;
                z = anchestor;
            } else{
                if(z->isRight()) {
                    z = parent;
                    left_rotate(z);
                }

                parent->_color = BLACK;
                anchestor->_color = RED;
                right_rotate(anchestor);
            }
        } else {
            Node* y = anchestor->_left;

            if(y->_color == RED) {
                parent->_color = BLACK;
                y->_color = BLACK;
                anchestor->_color = RED;
                z = anchestor;
            } else{
                if(z->isLeft()) {
                    z = parent;
                    right_rotate(z);
                }

                parent->_color = BLACK;
                anchestor->_color = RED;
                left_rotate(anchestor);
            }
        }
    }

    _root->_color = BLACK;
}

// ripercorro al contrario l'albero aggiornando il massimo
// finchè non trovo un nodo con max maggiore del nodo corrente --> il massimo in questo caso sarà dato
// dall'altro figlio = non devo aggiornare più niente
template<class DT, class BT, BT minBV, BT maxBV>
void IntervalTree<DT, BT, minBV, maxBV>::recalculate_max(Node* z) {
    Node* tmp = z->_parent;
    while(tmp && tmp->_maxBoundChild <= z->_maxBoundChild) {
        if(tmp->_left && tmp->_left->_maxBoundChild > tmp->_maxBoundChild)
            tmp->_maxBoundChild = tmp->_left->_maxBoundChild;
        if(tmp->_right && tmp->_right->_maxBoundChild > tmp->_maxBoundChild)
            tmp->_maxBoundChild = tmp->_right->_maxBoundChild;

        tmp = tmp->_parent;
    }
}

template<class DT, class BT, BT minBV, BT maxBV>
typename IntervalTree<DT, BT, minBV, maxBV>::BaseIterator
    IntervalTree<DT, BT, minBV, maxBV>::erase(const BaseIterator& it) {

    return BaseIterator();
}

template<class DT, class BT, BT minBV, BT maxBV>
IntervalTree<DT, BT, minBV, maxBV>::~IntervalTree() {
    delete _root;
}

#endif // INTERVALTREE_H