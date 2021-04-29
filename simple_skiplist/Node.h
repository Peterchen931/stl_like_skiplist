# include <vector>
using namespace std;

#ifndef SKIPLISTPRO_NODE_H
#define SKIPLISTPRO_NODE_H

//forward declaration
template<typename K, typename V>
class SkipList;

template<typename K, typename V>
class Node {
    friend class SkipList<K, V>;
    friend class skip_list_test;
    
public:
    Node() {}
    Node(K k, V v, int level): key(k), value(v), nodeLevel(level), forward(level+1){}
    Node(int level): key(K()), value(V()), nodeLevel(level), forward(level+1){}
    ~Node(){};
    K getKey() const;
    V getValue() const;

private:
    K key;
    V value;
    vector<Node<K,V>*> forward; // 一个level就会有一个指针，所以是一个指针数组
    int nodeLevel;
};

template<typename K, typename V>
K Node<K, V>::getKey() const {
    return key;
}

template<typename K, typename V>
V Node<K, V>::getValue() const {
    return value;
}

#endif //SKIPLISTPRO_NODE_H
