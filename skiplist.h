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

    pairtype element;//��ֵ��
    vector<skipnode<K, E>*> next;//ָ�����飬�������һ���ڵ㣬next[i]���ǵ�i������

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
    double cutoff; //�㼶�����ĸ���,����ȷ������
    int levels; //��ǰ�����������
    int maxlevel; //�޶�������
    K tailKey; //���Ĺؼ���
    skipnode<K, E>* headerNode; //ͷָ��
    skipnode<K, E>* tailNode; //βָ��
    skipnode<K, E>** last; //���ҹ������ڽ�����һ��ǰ�˲����ͣ���Ľڵ�
    
public:
    Strictskiplist(K maxkey = 0, int maxnum = 0, double prob = 0.5);
    Strictskiplist(int i,vector<pair<K, E>> arr, K maxkey = 0, int maxnum = 0, double prob = 0.5);
    skipnode<K, E>* search(K key);
    skipnode<K, E>* search1(K key);
    pair<K, E>* find(const K& key);//����
    int getlevel();//�����ʱ���ȡ����
    void insert(pair<K, E> thepair, int level = 0);//����
    void insert1(pair<K, E> thepair, int level = 0);//����
    void erase(const K& key);//ɾ��ָ��Ԫ��
    void erasemax();//ɾ�����Ԫ��
    void erasemin();//ɾ����СԪ��
    int getnewlevel(int x);//reset����ʱ����ײ��Ԫ�ؽ��л�ȡ�����ϸ������еĲ���
    void clear();//�������Ϊ�����������׼��
    ~Strictskiplist();
    void reset();//����Ϊ�ϸ�����
    void output();
    int control; //�ж��ع�������������������������ڵ���������޶����ع�
    int dsize; //Ԫ������

};