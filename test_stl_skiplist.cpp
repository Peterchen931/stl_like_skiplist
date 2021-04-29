#define DEBUG

#include "stl_skiplist.hpp"
#include "stl_slmap.hpp"
#include <functional>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const pair<int, int> &p){
    os << p.first << "(" << p.second << ")";
}

// void test_sklist(){
//     Skiplist<int, int, _Identity<int>, less<int>> skl;
//     for(int i = 0; i < 20; i++){
//         // skl.insert_unique(i);
//         // skl.insert_unique(rand()%10);
//         skl.insert_equal(rand()%10);
//         // skl.showAllNodes();
//     }
//     skl.showAllNodes();
//     auto iter = skl.find(3);
//     iter = skl.lower_bound(3);
//     iter = skl.upper_bound(3);
//     int k = skl.count(21);
//     int l = skl.count(3);

//     Skiplist<int, int, _Identity<int>, less<int>> skl1 = skl;
//     cout << (skl==skl1) << endl;
//     skl1.showAllNodes();
//     skl1 = skl;
//     skl1.showAllNodes();
//     for(int i = 0; i < 10; i++){
//         int tmp = rand() % 10;
//         int c = skl.erase(tmp);
//         // skl.showAllNodes();
//     }
    
//     cout << "dfdsfs" << endl;
//     auto print = [](const int& n) { std::cout << " " << n; };
//     for_each(skl1.begin(), skl1.end(), print);
//     cout << endl;
//     skl1.swap(skl);
//     for_each(skl1.begin(), skl1.end(), print);
//     cout << endl;
//     for_each(skl.begin(), skl.end(), print);
//     cout << endl;
// }

void test_slmap(){
    slmap<int, int> slm;
    for(int i = 0; i < 20; i++){
        // skl.insert_unique(i);
        // skl.insert_unique(rand()%10);
        pair<int, int> t(rand()%10, rand()%10);
        slm.insert(t);
        // slm.showAllNodes();
    }
    slm.skl.showAllNodes();
    auto iter = slm.find(3);
    cout << iter->first << iter->second << endl;
    // iter->first++;
    iter->second++;
    slm.skl.showAllNodes();
    auto iter1 = slm.cbegin();
    // iter1->second++;
    iter1++;

    iter = slm.lower_bound(3);
    iter = slm.upper_bound(3);
    int k = slm.count(21);
    int l = slm.count(3);

    slmap<int, int> slm1 = slm;
    cout << (slm==slm1) << endl;
    slm1.skl.showAllNodes();
    slm1 = slm;
    slm1.skl.showAllNodes();
    for(int i = 0; i < 10; i++){
        int tmp = rand() % 10;
        int c = slm.erase(tmp);
        // slm.showAllNodes();
    }
    
    cout << "dfdsfs" << endl;
    auto print = [](const pair<int, int>& n) { std::cout << n << "|"; };
    for_each(slm1.begin(), slm1.end(), print);
    cout << endl;
    slm1.swap(slm);
    for_each(slm1.begin(), slm1.end(), print);
    cout << endl;
    for_each(slm.begin(), slm.end(), print);
    cout << endl;
}


int main(void){
    // cout << make_pair(3,4) << endl;
    // test_sklist();
    test_slmap();

    // pair<int, int> a;
    // pair<int, int> b;
    // const int c = 3;
    // cout << less<int>()(_Select1st<pair<int, int>>()(a), c) << endl;

    return 0;
}