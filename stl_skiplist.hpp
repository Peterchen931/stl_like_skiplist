# include <vector>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <random>
using namespace std;

#ifndef SKIPLISTPRO_NODE_H
#define SKIPLISTPRO_NODE_H


template <class Value>
struct Skiplist_node{
    typedef Skiplist_node<Value>* Link_type;
    typedef int level_type;

    vector<Link_type> next;
    Value value_field;
    level_type node_level;
    Skiplist_node();
    Skiplist_node(Value v, level_type level): value_field(v), node_level(level), next(level+1, nullptr) {}
    Skiplist_node(level_type level):node_level(level), next(level+1, nullptr) {}
    Skiplist_node(const Skiplist_node<Value> &x ) = default;
};


template<class Value, class Ref, class Ptr>
struct Skiplist_iterator{
    // 类型声明
    typedef forward_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
    
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;

    typedef Skiplist_iterator<Value, Value&, Value*> iterator;
    typedef Skiplist_iterator<Value, const Value&, const Value*> const_iterator;
    typedef Skiplist_iterator<Value, Ref, Ptr> self;

    typedef Skiplist_node<Value>* Link_type;

    Link_type node;

    // 构造函数
    Skiplist_iterator() {}
    Skiplist_iterator(Link_type x) {node = x;}
    Skiplist_iterator(const iterator& it) {node = it.node;}

    reference operator*() const { return node->value_field; }   // 这里的operator*的行为是取nodevalue，因为要对使用者屏蔽node的概念
    pointer operator->() const { return &(operator*()); }
    self& operator++() { node = node->next[0]; return *this; }
    self  operator++(int) { self tmp = *this; node = node->next[0]; return tmp; }
};
template<class Value, class Ref, class Ptr>
bool operator==(const Skiplist_iterator<Value, Ref, Ptr>& x, const Skiplist_iterator<Value, Ref, Ptr>& y){
    return x.node == y.node;
}
template<class Value, class Ref, class Ptr>
bool operator!=(const Skiplist_iterator<Value, Ref, Ptr>& x, const Skiplist_iterator<Value, Ref, Ptr>& y){
    return x.node != y.node;
}



template<class Key, class Value, class KeyofValue, class Compare, class Alloc = std::allocator<Skiplist_node<Value>>>
class Skiplist{
public:
    typedef Key                     key_type;
    typedef Value                   value_type;
    typedef value_type*             pointer;
    typedef const value_type*       const_pointer;
    typedef value_type&             reference;
    typedef const value_type&       const_reference;
    
    typedef Skiplist_node<Value>*   Link_type;
    typedef Skiplist_node<Value>    Node_type;
    typedef ptrdiff_t               difference_type;
    typedef size_t                  size_type;
    typedef int                     level_type;

    typedef Alloc                                                           allocator_type;
    typedef Skiplist_iterator<value_type, reference, pointer>               iterator;
    typedef Skiplist_iterator<value_type, const_reference, const_pointer>   const_iterator;
private:
    Link_type get_node(level_type level){
        Link_type node = allocator.allocate(1); 
        allocator.construct(node, level);
        return node;
    }
    Link_type get_node(level_type level, value_type value){
        // 使用allocate traits重写
        Link_type node = allocator.allocate(1); 
        allocator.construct(node, value, level);
        return node;
    }
    Link_type get_node(const Skiplist_node<Value>& x){
        Link_type node = allocator.allocate(1);
        allocator.construct(node, x);
        return node;
    }
    void rm_node(Link_type node){
        allocator.destroy(node);
        allocator.deallocate(node, 1);
    }
    int get_random_level()const{
        // 随机生成level
        // level要求P[i] = 2P[i+1]
        // 根据从最低位开始连续的1的数量进行模拟(n次独立重复实验)
        // 根据level进行时间优化
        random_device rd;  // 将用于为随机数引擎获得种子
        default_random_engine gen(rd()); // 以播种标准 mersenne_twister_engine
        uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);
        uint64_t i = dis(gen);
        int bit = 1;
        int rand_level = 0;
        // rand_level范围限制在[0，max_level+1]
        for(; (bit & i) && (rand_level < max_level+1); rand_level++){
            bit = bit << 1;
        }
        return rand_level;
    }
    void copy_list(const Skiplist<Key, Value, KeyofValue, Compare, Alloc> &x){
        max_level = x.max_level;
        node_count = x.node_count;
        head = get_node(*x.head);
        vector<Link_type> update(max_level+1, head);

        for(Link_type node = x.head->next[0]; node != x.foot; node = node->next[0]){
            Link_type new_node = get_node(*node);
            for(level_type i = 0; i <= node->node_level; i++){
                update[i]->next[i] = new_node;
                update[i] = new_node;
            }
        }
        for(level_type i = 0; i < update.size(); i++){
            update[i]->next[i] = foot;
        }
    }
    bool is_equal(const Key &k1, const Key &k2)const{
        return !key_compare(k1, k2) && !key_compare(k2, k1);
    }
public:
    Skiplist(): max_level(-1), node_count(0), key_compare(Compare()), kov(KeyofValue()), allocator(Alloc()) {
        head = get_node(0);
        foot = get_node(-1);
        head->next[0] = foot;
    }
    Skiplist(const allocator_type& a): max_level(-1), node_count(0), key_compare(Compare()), kov(KeyofValue()), allocator(a) {
        head = get_node(0);
        foot = get_node(-1);
        head->next[0] = foot;
    }
    Skiplist(const Skiplist<Key, Value, KeyofValue, Compare, Alloc> &x): key_compare(x.key_compare), allocator(x.allocator) {
        foot = get_node(-1);
        copy_list(x);
    }
    ~Skiplist(){
        Link_type p = head;
        while (p != foot) {
            Link_type q = p->next[0];
            rm_node(p);
            p = q;
        }
        rm_node(foot);
    }
    
    Skiplist<Key, Value, KeyofValue, Compare, Alloc>& operator=(const Skiplist<Key, Value, KeyofValue, Compare, Alloc> &x){
        clear();
        copy_list(x);
        return *this;
    }

    allocator_type get_allocator() const { return allocator_type(); }

    void clear(){
        Link_type p = head;
        while (p != foot) {
            Link_type q = p->next[0];
            rm_node(p);
            p = q;
        }
        head = get_node(0);
        head->next[0] = foot;
        max_level = -1;
        node_count = 0;
    }
    
    iterator begin()const{ return head->next[0]; }   // 会自动构造iterator
    iterator end()const{ return foot; }
    const_iterator cbegin()const{ return head->next[0]; }
    const_iterator cend()const{ return foot; }

    bool empty()const{ return node_count==0; }
    size_type size()const{ return node_count; }
    size_type max_size()const{ return std::numeric_limits<difference_type>::max(); }

    pair<iterator,bool> insert_unique(const value_type& x) {
        vector<Link_type> update(max_level+1);
        Link_type node = head;
        for(int i = max_level; i >=0; i--){
            while( node->next[i] != foot && key_compare(kov(node->next[i]->value_field), kov(x)) ){
                node = node->next[i];
            }
            update[i] = node;
        }

        if( node->next[0] != foot && is_equal(kov(node->next[0]->value_field), kov(x)) ){
            return make_pair(node, false);
        }

        level_type new_level = get_random_level();
        if( new_level > max_level ){
            new_level = ++max_level;
            if( max_level != 0 ){
                head->next.push_back(foot);
            }
            update.push_back(head);
        }
        Link_type new_node = get_node(new_level, x);
        for(level_type i = new_level; i >= 0; i--){
            new_node->next[i] = update[i]->next[i];
            update[i]->next[i] = new_node;
        }
        node_count++;
        return make_pair(node, true);
    }
    pair<iterator, bool> insert_equal(const value_type& x){
        vector<Link_type> update(max_level+1);
        Link_type node = head;
        for(int i = max_level; i >=0; i--){
            while( node->next[i] != foot && key_compare( kov(node->next[i]->value_field), x ) ){
                node = node->next[i];
            }
            update[i] = node;
        }

        level_type new_level = get_random_level();
        if( new_level > max_level ){
            new_level = ++max_level;
            if( max_level != 0 ){
                head->next.push_back(foot);
            }
            update.push_back(head);
        }
        Link_type new_node = get_node(new_level, x);
        for(level_type i = new_level; i >= 0; i--){
            new_node->next[i] = update[i]->next[i];
            update[i]->next[i] = new_node;
        }
        node_count++;
        return make_pair(node, true);
    }   
    void insert_unique(iterator first, iterator last){
        for(iterator tmp = first; tmp != last; tmp++){
            key_type k = kov(tmp->value_field);
            insert_unique(k);
        }
    }
    void insert_equal(iterator first, iterator last){
        for(iterator tmp = first; tmp != last; tmp++){
            key_type k = kov(tmp->value_field);
            insert_equal(k);
        }
    }
    
    size_type erase(const key_type& x){
        vector<Link_type> update(max_level+1);
        Link_type node = head;

        for( level_type i = max_level; i >= 0; --i ){
            while( node->next[i] != foot && key_compare( kov(node->next[i]->value_field), x ) ){
                node = node->next[i];
            }
            update[i] = node;
        }
        int erase_count = 0;

        while( node->next[0] != foot && !key_compare( x, kov(node->next[0]->value_field) ) ){
            Link_type need_erase = node->next[0];
            for( level_type i = 0; i <= need_erase->node_level; ++i ){
                update[i]->next[i] = need_erase->next[i];
            }
            while( max_level >= 0 && head->next[max_level] == foot ){
                --max_level;
            }
            --node_count;
            erase_count++;

            rm_node(need_erase);
        }
        return erase_count;
    }
    
    iterator find(const key_type& x)const{
        Link_type node = lower_bound(x).node;
        return node == foot || is_equal(kov(node->value_field), x) ? node : foot;
    }
    size_type count(const key_type& x)const{
        Link_type node = find(x).node;
        int count = 0;
        while( node != foot && !key_compare( x, kov(node->value_field) ) ){
            count++;
            node = node->next[0];
        }
        return count;
    }
    iterator lower_bound(const key_type& x)const{
        // 注，如果其返回的迭代器为end()
        Link_type node = head;
        for(level_type i = max_level; i >= 0; --i){
            while( node->next[i] != foot && ( key_compare( kov(node->next[i]->value_field), x ) ) ){
                node = node->next[i];
            }
        }
        // 出循环后，node->next[0]是第一个不小于x的节点，即node为lower_bound_before
        return node->next[0];
    }
    iterator upper_bound(const key_type& x)const{
        Link_type node = head;
        for(level_type i = max_level; i >= 0; --i){
            while(  node->next[i] != foot && 
                    ( key_compare( kov(node->next[i]->value_field), x ) || is_equal( kov(node->next[i]->value_field), x ) ) ){ // 化简
                node = node->next[i];
            }
        }
        // 出循环后，node->next[0]是第一个大于x的节点，即node为upper_bound
        return node->next[0];
    }

    void swap(Skiplist<Key, Value, KeyofValue, Compare, Alloc>& t){
        std::swap(head, t.head);
        std::swap(foot, t.foot);
        std::swap(node_count, t.node_count);
        std::swap(key_compare, t.key_compare);
        std::swap(allocator, t.allocator);
    }

    Compare key_comp()const{ return key_compare; }

#ifdef DEBUG
    void showEachLevelSize()const{
        Link_type node = head->next[0];
        vector<level_type> level_table(max_level);
        while(node != foot){
            level_table[node->nodeLevel]++;
            node = node->next[0];
        }
        for(auto i : level_table){
            cout << i << '|';
        }
        cout << endl;
    }
    void showAllNodes()const{
        cout << "skiplist:" << endl;
        for(int i = 0; i <= max_level; i++){
            cout << "line " << i << "\t";
        }
        cout << endl;
        Link_type tmp = head->next[0];
        while(tmp != foot){
            for(int i =0; i <= tmp->node_level; i++){
                cout << tmp->value_field << "\t";
            }
            tmp = tmp->next[0];
            cout << endl;
        }
        cout << endl;
    }
#endif

protected:
    size_type node_count;
    level_type max_level;
    Compare key_compare;
    KeyofValue kov;
    Link_type head;
    Link_type foot;
    allocator_type allocator;
};

template<class Key, class Value, class KeyofValue, class Compare, class Alloc = std::allocator<Skiplist_node<Value>>>
bool operator==(const Skiplist<Key, Value, KeyofValue, Compare, Alloc>& x, 
                const Skiplist<Key, Value, KeyofValue, Compare, Alloc>& y){
    return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
}

template<class Key, class Value, class KeyofValue, class Compare, class Alloc = std::allocator<Skiplist_node<Value>>>
bool operator!=(const Skiplist<Key, Value, KeyofValue, Compare, Alloc>& x, 
                const Skiplist<Key, Value, KeyofValue, Compare, Alloc>& y){
    return !(x == y);
}


#endif //SKIPLISTPRO_NODE_H
