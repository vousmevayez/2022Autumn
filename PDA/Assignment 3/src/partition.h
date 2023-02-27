#ifndef _PARTITION_H_
#define _PARTITION_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <iomanip>
#include <random>
#include <deque>
using namespace std;

#define N 250000

int numTerminal, numCell;
int flag;

class Data
{
public:
    string name, type;
    int x, y, w, h, num, chipIdx;
    int xr, yt, area, idx; // xr=x+w, yt=y+h
    // string name, int x, int y, int w, int h
    Data() : chipIdx(0), area(0) {}
};

class Vertex
{
public:
    int locked, part, gain, net, area;
    // vector<int> vecNet;
    vector<pair<int, int>> vecNet;
    Vertex() : locked(0), part(0), gain(0), net(0), area(0) {}
};

class Row
{
public:
    int w, x, y;
    int rownum;
    int /*ri,*/ rowb, rowt, roww;
    // Row() : ri(0) {}
};

class Partition
{
public:
    void inputFile(ifstream &fin);
    void outputFile(ofstream &fout);
    int initoverlap();
    int partoverlap();
    void part();
    int costfunction(vector<Data *> a, vector<Data *> b);
    void solve();
    void update(vector<Data *> &a, vector<Data *> b);
    void placement();
    void final();
    void pushTerminal();

    int cw, ch, rh, rn, terminalArea, cellArea, distX, distY, overlapArea, poverlapArea;
    int offset, totalArea, splitArea, totalNet;
    int besArea, bestNet;
    int cost;
    vector<int> vecChipIdx, bestSplit, maxGain;
    vector<Row *> row1, row2;
    vector<Data *> cell, terminal, tc, cell0, cell1, sortedCell, cell_;
    vector<Vertex *> vertex;
    vector<vector<list<int>>> vecVertex;
    deque<deque<Data *>> vec;
    vector<vector<Data *>> vecterminal;
    map<string, Data *> mapData;
    vector<vector<pair<int, int>>> coord;

    Partition() : terminalArea(0), cellArea(0), distX(0), distY(0), overlapArea(0), poverlapArea(0), offset(0), totalArea(0), splitArea(0), totalNet(0) {}
} p;

bool sortCell(const Data *c1, const Data *c2)
{
    if (c1->x == c2->x)
    {
        return c1->w < c2->w;
    }
    else
    {
        return c1->x < c2->x;
    }
}

bool sortcell(const Data *c1, const Data *c2)
{
    // if (c1->x == c2->x)
    // {
    //     return c1->w < c2->w;
    // }
    // else
    // {
    return c1->x < c2->x;
    // }
}

bool sortIdx(const Data *c1, const Data *c2)
{
    return c1->idx < c2->idx;
}

bool sortRow(const Row *r1, const Row *r2)
{
    if (r1->w == r2->w)
    {
        return r1->y < r2->y;
    }
    else
    {
        return r1->w < r2->w;
    }
}

#endif
