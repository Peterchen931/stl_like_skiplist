#include "stl_skiplist.hpp"

using namespace std;
template<class Key, class Value, class Compare = less<Key>, class Alloc = std::allocator< Skiplist_node<pair<const Key, Value>> >>
class slmap{
public:
    typedef Key                     key_type;       // 键值类型
    typedef Value                   data_type;      // 数据类型
    typedef pair<const Key, Value>  value_type;     // 元素类型(键值/实值)
    typedef Compare                 key_compare;    // 键值比较函数
private:
    typedef Skiplist<key_type, value_type, _Select1st<value_type>, key_compare, Alloc> SKL_type;

    typedef typename SKL_type::pointer                  pointer;
    typedef typename SKL_type::const_pointer            const_pointer;
    typedef typename SKL_type::reference                reference;
    typedef typename SKL_type::const_reference          const_reference;
    typedef typename SKL_type::iterator                 iterator;
    typedef typename SKL_type::const_iterator           const_iterator;
    typedef typename SKL_type::size_type                size_type;
    typedef typename SKL_type::difference_type          difference_type;
    typedef typename SKL_type::allocator_type           allocator_type;


// private:
public:
    SKL_type skl;
public:
    slmap() = default;
    slmap(const allocator_type& a):skl(a){}
    slmap(const slmap &x):skl(x.skl){}
    ~slmap() = default;


    allocator_type get_allocator() const { return skl.get_allocator(); }

    void clear(){ skl.clear(); }

    iterator begin() { return skl.begin(); }
    iterator end() { return skl.end(); }
    const_iterator cbegin() { return skl.cbegin(); }
    const_iterator cend() { return skl.cend(); }
    bool empty() const { return skl.empty(); }
    size_type size() const { return skl.size(); }

    data_type& operator[](const key_type& k){
        iterator i = lower_bound(k);
        if( i == end() || skl.key_compare( k, i.first ) ){
            i = insert(i, value_type(k, data_type()));
        }
        return (*i).second;
    }
    
    pair<iterator,bool> insert(const value_type& x) { return skl.insert_unique(x); }
    void insert(iterator first, iterator last){ skl.insert_unique(first, last); }
    
    size_type erase(const key_type& x){ return skl.erase(x); };
    
    iterator find(const key_type& x)const{ return skl.find(x); }
    size_type count(const key_type& x)const{ return skl.count(x); }
    iterator lower_bound(const key_type& x)const{ return skl.lower_bound(x); }
    iterator upper_bound(const key_type& x)const{ return skl.upper_bound(x); }

    void swap(slmap<Key,Value,Compare,Alloc>& x) { skl.swap(x.skl); }
    
    // slmap<Key,Value,Compare,Alloc>& operator=(const slmap<Key,Value,Compare,Alloc>& x){ return skl = x.skl; }

};
template<class Key, class Value, class Compare, class Alloc>
bool operator==(const slmap<Key, Value, Compare, Alloc> &x, const slmap<Key, Value, Compare, Alloc> &y){
    return x.skl==y.skl;
}
template<class Key, class Value, class Compare, class Alloc>
bool operator!=(const slmap<Key, Value, Compare, Alloc> &x, const slmap<Key, Value, Compare, Alloc> &y){
    return x.skl!=y.skl;
}