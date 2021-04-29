//
// Created by Wang Allen on 2018/2/6.
//

#ifndef SKIPLISTPRO_SKIPLIST_H
#define SKIPLISTPRO_SKIPLIST_H

#include <cstddef>
#include <cassert>
#include <ctime>
#include <random>
#include "Node.h"

using namespace std;

// #define DEBUG

template<typename K, typename V>
class SkipList {
friend class skip_list_test;
public:
    SkipList();
    ~SkipList();

    Node<K, V> *find(K key) const;
    bool insert(K key, V value);
    bool erase(K key);
    void clear();
    int size();
    bool empty();
    V& operator[](K key);
    int count(K key);
    SkipList<K,V>& operator=(const SkipList<K,V>& other);
    void merge(SkipList<K,V> other);
    Node<K, V> * lower_bound();
    Node<K, V> * upper_bound();


private:
    void freeList();
    int getRandomLevel();
    void showAllNodes();
    void showLineNodes(int k);
    void showEachLevelSize();

private:
    // 单链表，每一层都是环形链表begin=head，end=head
    int level;
    size_t nodeCount;
    Node<K, V> *header;
};

template<typename K, typename V>
SkipList<K, V>::SkipList():level(-1), nodeCount(0){
    header = new Node<K, V>(0);
    header->forward[0] = header;
}

template<typename K, typename V>
SkipList<K, V>::~SkipList(){
    freeList();
}

template<typename K, typename V>
void SkipList<K, V>::freeList() {
    Node<K, V> *p = header->forward[0];
    Node<K, V> *q;
    while (p != header) {
        q = p->forward[0];
        delete p;
        p = q;
    }
    delete p;
}

template<typename K, typename V>
void SkipList<K, V>::clear() {
    freeList();
    level = -1;
    nodeCount = 0;
    header = new Node<K, V>(-1, -1, 0);
    header->forward[0] = header;
}

template<typename K, typename V>
int SkipList<K, V>::size() {
    return nodeCount;
}

template<typename K, typename V>
bool SkipList<K, V>::empty(){
    return nodeCount == 0;
}

template<typename K, typename V>
Node<K, V> *SkipList<K, V>::find(const K key) const {
    Node<K, V> *node = header;
    // 找到level0中大于等于key的第一项
    for (int i = level; i >= 0; --i) {
        while ( node->forward[i] != header && (node->forward[i])->key < key ) { // 下一个点是header代表转了一圈，即当前为末尾
            node = node->forward[i];
        }
    }
    // 到这里有两种可能，一是node到了末尾，二是node到了目标位置之前
    // 第一个大于等于key的数字，也是目标
    if( node->forward[0] == header ){
        return nullptr;
    }
    return node->forward[0]->key == key ? node->forward[0] : nullptr;
};

template<typename K, typename V>
bool SkipList<K, V>::insert(K key, V value) {
    vector<Node<K, V>*> update(level+1);  // 用于标记各层需要更新的位置
    Node<K, V> *node = header;

    // 确认节点插入的位置
    for (int i = level; i >= 0; --i) {
        while ( node->forward[i] != header && (node->forward[i])->key < key ) {
            node = node->forward[i];
        }
        update[i] = node;
    }

    // 如果该节点不是末尾且值右与key一致，代表有重复，不用插入
    node = node->forward[0];
    if(node != header && node->key == key){
        // 行为应该是更改值
        return false;
    }

    // 新建一个节点，level随机
    int nodeLevel = getRandomLevel();
    if (nodeLevel > level) {    // 如果新节点大于当前level则将level加一
        level++;
        if( level != -1 ){
            header->forward.push_back(header); // 对key泛型
        }
        update.push_back(header);
    }

    //创建新结点并插入
    Node<K, V> *newNode = new Node<K, V>(key, value, nodeLevel);
    for (int i = nodeLevel; i >= 0; --i) {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
    ++nodeCount;

#ifdef DEBUG
    showAllNodes();
#endif

    return true;
};

template<typename K, typename V>
bool SkipList<K, V>::erase(K key) {
    vector<Node<K, V>*> update(level+1);
    Node<K, V> *node = header;

    // 确认节点删除的位置
    for (int i = level; i >= 0; --i) {
        while ( node->forward[i] != header && (node->forward[i])->key < key ) {
            node = node->forward[i];
        }
        update[i] = node;
    }

    //如果结点是末尾，或者没有节点与key相等就返回false
    node = node->forward[0];
    if( node == header || node->key != key ){
        return false;
    }
    // 处理连接
    for (int i = 0; i <= node->nodeLevel; ++i) {
        update[i]->forward[i] = node->forward[i];
    }
    //释放结点
    delete node;
    //更新level的值
    while (level > 0 && header->forward[level] == header) {
        --level;
    }
    --nodeCount;

#ifdef DEBUG
    showAllNodes();
#endif

    return true;
};

template<typename K, typename V>
int SkipList<K, V>::getRandomLevel() {
    // 随机生成level
    // level要求P[i] = 2P[i+1]
    // 根据从最低位开始连续的1的数量进行模拟(n次独立重复实验)
    // 根据level进行时间优化
    random_device rd;  // 将用于为随机数引擎获得种子
    default_random_engine gen(rd()); // 以播种标准 mersenne_twister_engine
    uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);
    uint64_t i = dis(gen);

    // int i = rand();
    int bit = 1;
    int rand_level = 0;
    // rand_level范围限制在[0，max_level+1]
    for(; (bit & i) && (rand_level < level+1); rand_level++){
        bit = bit << 1;
    }
    return rand_level;
}

template<typename K, typename V>
void SkipList<K, V>::showEachLevelSize() {
    Node<int, int> *node = header->forward[0];
    vector<int> level_table(level);
    while(node != header){
        level_table[node->nodeLevel]++;
        node = node->forward[0];
    }
    for(auto i : level_table){
        cout << i << '|';
    }
    cout << endl;
}

template<typename K, typename V>
void SkipList<K, V>::showAllNodes() {
    for(int i = 0; i <= level; i++){
        showLineNodes(i);
    }
    cout << endl;
}

template<typename K, typename V>
void SkipList<K, V>::showLineNodes(int i) {
    Node<K, V> *tmp = header->forward[i];
    cout << "line " << i << ": ";
    while (tmp != header) {
        cout << tmp->key << '|';
        tmp = tmp->forward[i];
    }
    cout << endl;
}

#endif //SKIPLISTPRO_SKIPLIST_H
