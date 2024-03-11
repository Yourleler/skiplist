#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include<vector>
#include<math.h>
#include<string>
#include<iomanip>
#include<vector>
#include<algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <ctime>
#include <chrono>

#define NUM_RANDOM 1000
#define RANGE 1000

using namespace std;


template<class K, class E>
class skipnode
{
public:
    typedef pair<K, E> pairtype;

    pairtype element;//键值对
    vector<skipnode<K, E>*> next;//指针数组，存的是下一个节点，next[i]就是第i级跳表

    skipnode(pairtype p = { 0,0 }, int level = 0)
    {
        next.resize(level + 1, NULL);
        element = p;
    }
};

template<class K, class E>
class Strictskiplist
{
private:
    double cutoff; //层级上升的概率,用来确定层数
    int levels; //当前跳表的最大层数
    int maxlevel; //限定最大层数
    K tailKey; //最大的关键字
    skipnode<K, E>* headerNode; //头指针
    skipnode<K, E>* tailNode; //尾指针
    skipnode<K, E>** last; //查找过程中在进入下一层前此层最后停留的节点
    
public:
    Strictskiplist(K maxkey = 0, int maxnum = 0, double prob = 0.5);
    Strictskiplist(int i,vector<pair<K, E>> arr, K maxkey = 0, int maxnum = 0, double prob = 0.5);
    skipnode<K, E>* search(K key);
    skipnode<K, E>* search1(K key);
    pair<K, E>* find(const K& key);//查找
    int getlevel();//插入的时候获取层数
    void insert(pair<K, E> thepair, int level = 0);//插入
    void insert1(pair<K, E> thepair, int level = 0);//插入
    void erase(const K& key);//删除指定元素
    void erasemax();//删除最大元素
    void erasemin();//删除最小元素
    int getnewlevel(int x);//reset重置时对最底层的元素进行获取他在严格跳表中的层数
    void clear();//清空跳表，为下面的重置做准备
    ~Strictskiplist();
    void reset();//重置为严格跳表
    void output();
    int control; //判断重构跳表的条件，当此搜索遍历节点个数超过限定，重构
    int dsize; //元素总数

};