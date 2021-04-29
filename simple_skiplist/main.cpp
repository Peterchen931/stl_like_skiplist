#include <iostream>
#include <vector>
#include <time.h>
#include "SkipList.h"

using namespace std;

class skip_list_test{
public:
    void test_random_level(int len){
        cout << "skiplist random level test start..." << endl;

        SkipList<int, int> skipList;
        for(int i = 0; i < len; i++){
            skipList.insert(i,i);
        }
        skipList.showEachLevelSize();
        cout << "skiplist random level test finish..." << endl;
    }
    void test_init(){
        cout << "skiplist construction test start..." << endl;
        SkipList<int, int> skipList;
        assert(skipList.nodeCount == 0);
        assert(skipList.header->forward[0] == skipList.header);
        assert(skipList.level == -1);
        cout << "skiplist construction test finish..." << endl;
    }
    void test_insert(){
        cout << "skiplist insert test now starts..." << endl;

        //给一个最大的整型值
        SkipList<int, int> skipList;
        int length = 10;
        for (int i = 1; i <= length; ++i) {
            skipList.insert(i, i);
        }
        assert(skipList.size() == 10);
        cout << "skiplist insert test now finish..." << endl;
    }
    void test_search(){
        cout << "skiplist search test now starts..." << endl;

        //给一个最大的整型值
        SkipList<int, int> skipList;
        int length = 10;
        for(int i = 1; i <= length; ++i){
            skipList.insert(i, i);
        }
        for(int i = 1; i <= length; i++){
            auto tmp = skipList.find(i);
            assert(tmp != nullptr);
            assert(tmp->getKey() == i);
            assert(tmp->getValue() == i);
        }
        assert( skipList.find(length+1) == nullptr );

        cout << "skiplist find test now finish..." << endl;
    }
    void test_erase(){
        cout << "skiplist erase test now starts..." << endl;

        //给一个最大的整型值
        SkipList<int, int> skipList;
        int length = 10;
        for(int i = 1; i <= length; ++i){
            skipList.insert(i, i);
        }
        for(int i = 1; i <= length; ++i){
            skipList.erase(i);
        }

        cout << "skiplist erase test now finish..." << endl;
    }
    void test_insert_speed(){
        int count1 = 1 << 10;
        int count2 = 1 << 20;
        int ntime = 1 << 10;
        SkipList<int, int> skipList;
        clock_t t1;
        clock_t t2;
        // 在数据量为2^20与2^24情况进行2^4次插入，对比时间，预计时间比为1：1.2
        for(int i = 0; i < count1; i++){
            int tmp = rand();
            skipList.insert(tmp, tmp);
        }

        t1 = clock();
        for(int i = 0; i < ntime; i++){
            int tmp = rand();
            skipList.insert(tmp, tmp);
        }
        t2 = clock();
        double d1 = (clock() - t1) * 1.0 / CLOCKS_PER_SEC * 1000;
        cout << "insert 2^20 " << d1 << "ms" << endl;

        skipList.clear();
        for(int i = 0; i < count2; i++){
            int tmp = rand();
            skipList.insert(tmp, tmp);
        }
        t1 = clock();
        for(int i = 0; i < ntime; i++){
            int tmp = rand();
            skipList.insert(tmp, tmp);
        }
        t2 = clock();
        double d2 = (clock() - t1) * 1.0 / CLOCKS_PER_SEC * 1000;
        cout << "insert 2^24 " << d2 << "ms" << endl;
        cout << "ratio is:" << d2 / d1 << endl;
    }
    void test_search_speed(){
        int count1 = 1 << 10;
        int count2 = 1 << 20;
        int ntime = 1 << 10;
        SkipList<int, int> skipList;
        clock_t t1;
        clock_t t2;
        // 在数据量为2^10与2^16情况进行对比，预计时间比为1:1.6
        for(int i = 0; i < count1; i++){
            int tmp = rand();
            skipList.insert(tmp, tmp);
        }
        t1 = clock();
        for(int i = 0; i < ntime; i++){
            int tmp = rand();
            skipList.find(tmp);
        }
        t2 = clock();
        double d1 = (clock() - t1) * 1.0 / CLOCKS_PER_SEC * 1000;
        cout << "find 2^10 " << d1 << "ms" << endl;

        skipList.clear();
        for(int i = 0; i < count2; i++){
            int tmp = rand();
            skipList.insert(tmp, tmp);
        }
        t1 = clock();
        for(int i = 0; i < ntime; i++){
            int tmp = rand();
            skipList.find(tmp);
        }
        t2 = clock();
        double d2 = (clock() - t1) * 1.0 / CLOCKS_PER_SEC * 1000;
        cout << "find 2^20 " << d2 << "ms" << endl;
        cout << "ratio is:" << d2 / d1 << endl;
    }
    void test_erase_speed(){
        int count1 = 1 << 10;
        int count2 = 1 << 20;
        int ntime = 1 << 10;
        SkipList<int, int> skipList;
        clock_t t1;
        clock_t t2;
        // 在数据量为2^20与2^24情况进行2^4次插入，对比时间，预计时间比为1：1.2
        for(int i = 0; i < count1; i++){
            int tmp = rand();
            skipList.insert(tmp, tmp);
        }

        t1 = clock();
        for(int i = 0; i < ntime; i++){
            int tmp = rand();
            skipList.erase(tmp);
        }
        t2 = clock();
        double d1 = (clock() - t1) * 1.0 / CLOCKS_PER_SEC * 1000;
        cout << "erase 2^20 " << d1 << "ms" << endl;

        skipList.clear();
        for(int i = 0; i < count2; i++){
            int tmp = rand();
            skipList.insert(tmp, tmp);
        }
        t1 = clock();
        for(int i = 0; i < ntime; i++){
            int tmp = rand();
            skipList.erase(tmp);
        }
        t2 = clock();
        double d2 = (clock() - t1) * 1.0 / CLOCKS_PER_SEC * 1000;
        cout << "erase 2^24 " << d2 << "ms" << endl;
        cout << "ratio is:" << d2 / d1 << endl;
    }
};






int main() {
    skip_list_test test;
    test.test_random_level(1L << 20);
    // test.test_init();
    // test.test_insert();
    // test.test_search();
    // test.test_erase();
    // test.test_insert_speed();
    // test.test_search_speed();
    // test.test_erase_speed();


    return 0;
}