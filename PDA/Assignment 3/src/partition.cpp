#include "partition.h"

void Partition::inputFile(ifstream &fin)
{

    string info;
    fin >> info >> cw >> ch;
    fin >> info >> rh >> rn;
    fin >> info >> numTerminal;
    if (numTerminal != 0)
    {
        for (int i = 0; i < numTerminal; i++)
        {
            Data *t = new Data;
            fin >> t->name >> t->x >> t->y >> t->w >> t->h;
            t->xr = t->x + t->w;
            t->yt = t->y + t->h;
            terminalArea += t->w * t->h;
            t->area += t->w * t->h;
            terminal.push_back(t);
            // tc.push_back(t);
        }
    }

    fin >> info >> numCell;
    for (int i = 0; i < numCell; i++)
    {
        Data *c = new Data;
        fin >> c->name >> c->x >> c->y >> c->w >> c->h;
        c->xr = c->x + c->w;
        c->yt = c->y + c->h;
        c->idx = i;
        cellArea += c->w * c->h;
        c->area += c->w * c->h;
        cell.push_back(c);
        // tc.push_back(c);
    }
    sortedCell = cell;
    sort(sortedCell.begin(), sortedCell.end(), sortCell);
    // int a = costfunction(cell, cell);
    // cout << a << "offset" << endl;
}

void Partition::outputFile(ofstream &fout)
{
    sort(cell.begin(), cell.end(), sortIdx);
    for (int i = 0; i < numCell; i++)
    {
        fout << "c" << cell[i]->idx << " " << cell[i]->x << " " << cell[i]->y << " " << cell[i]->chipIdx << endl;
    }
}

// ===========================================
// partition
// ===========================================
void Partition::part()
{
    int div = numCell / 2;
    for (int i = 0; i < div; i++)
    {
        Data *c0 = new Data;
        c0->name = cell[i]->name;
        c0->x = cell[i]->x;
        c0->y = cell[i]->y;
        c0->w = cell[i]->w;
        c0->h = cell[i]->h;
        c0->xr = cell[i]->xr;
        c0->yt = cell[i]->yt;
        c0->idx = cell[i]->idx;
        c0->chipIdx = 0;
        cell0.push_back(c0);
        // cout << c0->chipIdx << endl;
    }
    for (int i = div; i < numCell; i++)
    {
        Data *c1 = new Data;
        c1->name = cell[i]->name;
        c1->x = cell[i]->x;
        c1->y = cell[i]->y;
        c1->w = cell[i]->w;
        c1->h = cell[i]->h;
        c1->xr = cell[i]->xr;
        c1->yt = cell[i]->yt;
        c1->idx = cell[i]->idx;
        c1->chipIdx = 1;
        cell1.push_back(c1);
        // cout << c1->chipIdx << endl;
    }
    cout << "c0.size: " << cell0.size() << endl;
    cout << "c1.size: " << cell1.size() << endl;
}

int Partition::costfunction(vector<Data *> a, vector<Data *> b)
{
    sort(a.begin(), a.end(), sortIdx);
    sort(b.begin(), b.end(), sortIdx);
    // offset
    int offset = 0;
    for (int i = 0; i < int(a.size()); i++)
    {
        offset += (abs(a[i]->x - b[i]->x) + abs(a[i]->y - b[i]->y));
    }
    // initoverlap();
    // partoverlap();
    // cost = offset * (poverlapArea / overlapArea);
    // cout << cost << endl;
    // cout << offset << endl;
    return offset;
}

void Partition::update(vector<Data *> &a, vector<Data *> b)
{
    a.clear();
    for (int i = 0; i < int(b.size()); i++)
    {
        Data *u = new Data;
        u->name = b[i]->name;
        u->x = b[i]->x;
        u->y = b[i]->y;
        u->w = b[i]->w;
        u->h = b[i]->h;
        u->xr = b[i]->xr;
        u->yt = b[i]->yt;
        u->idx = b[i]->idx;
        u->chipIdx = b[i]->chipIdx;
        a.push_back(u);
    }
}

// ===========================================
// placement
// ===========================================
void Partition::placement()
{
    // bottom
    for (int i = 0; i < int(cell0.size()); i++)
    {
        for (int next = i + 1; next < int(cell0.size()); next++)
        {
            // on the left side of leftmost
            // below the side of bottom
            Row *row = new Row;
            int ri = 0;
            // int rowb = rh * ri;
            // int rowt = rh + rowb;
            // int roww = cw;
            // row->ri = 0;
            row->rowb = rh * ri;
            row->rowt = rh + row->rowb;
            row->roww = cw;
            if (cell0[i]->xr < cell0[next]->x && cell0[i]->yt < cell0[next]->y) //&& cell0[i]->xr < terminal[i]->x && cell0[i]->yt < terminal[i]->y
            {
                cell0[i]->x = cell0[i]->x;
                cell0[i]->xr = cell0[i]->x + cell0[i]->w;
                cell0[i]->y = row->rowb;
            }
            // overlap
            else
            {
                // if (cell0[i]->xr > terminal[i]->x)
                // {
                //     cell0[i]->x = terminal[i]->xr;
                //     cell0[i]->xr = cell0[i]->x + cell0[i]->w;
                //     cell0[i]->y = rowb;
                // }
                // cell0[next]->x = cell0[i]->xr;
                // cell0[next]->y = rowb;
                if (cell0[i]->xr > terminal[i]->x)
                {
                    cell0[i]->x = terminal[i]->xr;
                    cell0[i]->xr = cell0[i]->x + cell0[i]->w;
                    cell0[i]->y = row->rowb;
                }
                cell0[next]->x = cell0[i]->xr;
                cell0[next]->xr = cell0[next]->x + cell0[next]->w;
                cell0[next]->y = cell0[i]->y;
                if (row->roww - cell0[i]->w - terminal[i]->w < cell0[next]->w)
                {
                    // roww = 0;
                    cell0[next]->x = row->rowb;
                    cell0[next]->xr = cell0[next]->x + cell0[next]->w;
                    cell0[next]->y = row->rowt;
                }
            }
        }
    }
    // top
    for (int i = 0; i < int(cell1.size()); i++)
    {
        for (int next = i + 1; next < int(cell1.size()); next++)
        {
            // on the left side of leftmost
            // below the side of bottom
            Row *row = new Row;
            int ri = 0;
            row->rowb = rh * ri;
            row->rowt = rh + row->rowb;
            row->roww = cw;
            if (cell1[i]->xr < cell1[next]->x && cell1[i]->yt < cell1[next]->y) //&& cell1[i]->xr < terminal[i]->x && cell1[i]->yt < terminal[i]->y
            {
                cell1[i]->x = cell1[i]->x;
                cell1[i]->xr = cell1[i]->x + cell1[i]->w;
                cell1[i]->y = cell1[i]->y;
            }
            // overlap
            else
            {
                if (cell1[i]->xr > terminal[i]->x)
                {
                    cell1[i]->x = terminal[i]->xr;
                    cell1[i]->xr = cell1[i]->x + cell1[i]->w;
                    cell1[i]->y = cell1[i]->y;
                }
                cell1[next]->x = cell1[i]->xr;
                cell1[next]->xr = cell1[next]->x + cell[next]->w;
                cell1[next]->y = cell1[i]->y;
                if (row->roww - cell1[i]->w - terminal[i]->w < cell1[next]->w)
                {
                    cell1[next]->x = row->rowb;
                    cell1[next]->xr = cell1[next]->x + cell1[next]->w;
                    cell1[next]->y = row->rowt;
                }
            }
        }
    }
    vector<Data *> temp;
    temp.clear();
    for (int i = 0; i < numCell; i++)
    {
        for (int j = 0; j < int(cell0.size()); j++)
        {
            if (cell[i]->idx == cell0[j]->idx)
            {
                Data *t0 = new Data;
                t0->x = cell0[j]->x;
                t0->y = cell0[j]->y;
                t0->idx = cell0[j]->idx;
                t0->chipIdx = cell0[j]->chipIdx;
                // cout << "c0 " << t0->idx << " " << t0->chipIdx << endl;
                temp.push_back(t0);
            }
        }
        for (int k = 0; k < int(cell1.size()); k++)
        {
            if (cell[i]->idx == cell1[k]->idx)
            {
                Data *t1 = new Data;
                t1->x = cell1[k]->x;
                t1->y = cell1[k]->y;
                t1->idx = cell1[k]->idx;
                t1->chipIdx = cell1[k]->chipIdx;
                // cout << "c1 " << t1->idx << " " << t1->chipIdx << endl;
                temp.push_back(t1);
            }
        }
    }
    // cout << cell.size() << endl;
    cell.clear();
    // cout << temp.size() << endl;
    cell = temp;
    for (int i = 0; i < int(cell.size()); i++)
    {
        cout << "c" << cell[i]->idx << " " << cell[i]->x << " " << cell[i]->y << " " << cell[i]->chipIdx << endl;
    }
}

int Partition::initoverlap()
{
    // for (int i = 0; i < numTerminal; i++)
    // {
    //     for (int next = i + 1; next < numTerminal; next++)
    //     {
    //         if (terminal[i]->xr < terminal[next]->x)
    //         {
    //             overlapArea += 0;
    //         }
    //         else if (terminal[i]->yt < terminal[next]->y)
    //         {
    //             overlapArea += 0;
    //         }
    //         else
    //         {
    //             distX = min(terminal[i]->xr, terminal[next]->xr) - max(terminal[i]->x, terminal[next]->x);
    //             distY = min(terminal[i]->yt, terminal[next]->yt) - max(terminal[i]->y, terminal[next]->y);
    //             if (distX > 0 && distY > 0)
    //             {
    //                 overlapArea += distX * distY;
    //             }
    //         }
    //     }
    // }
    // cout << "terminal overlap" << overlapArea << endl;
    for (int i = 0; i < numCell; i++)
    {
        Vertex *v = new Vertex;
        v->area = cell[i]->area;
        vertex.push_back(v);
        splitArea += v->area;
    }
    for (int i = 0; i < numCell; i++)
    {
        for (int next = i + 1; next < numCell; next++)
        {
            if (cell[i]->xr < cell[next]->x)
            {
                overlapArea += 0;
            }
            else if (cell[i]->yt < cell[next]->y)
            {
                overlapArea += 0;
            }
            else
            {
                distX = min(cell[i]->xr, cell[next]->xr) - max(cell[i]->x, cell[next]->x);
                distY = min(cell[i]->yt, cell[next]->yt) - max(cell[i]->y, cell[next]->y);
                if (distX > 0 && distY > 0)
                {
                    overlapArea += distX * distY;
                }
            }
        }
    }
    // cout << "initoverlap: " << overlapArea << endl;
    return overlapArea;
}

int Partition::partoverlap()
{
    int div = numCell / 2;
    for (int i = 0; i < div; i++)
    {
        Data *c0 = new Data;
        c0->name = cell[i]->name;
        c0->x = cell[i]->x;
        c0->y = cell[i]->y;
        c0->w = cell[i]->w;
        c0->h = cell[i]->h;
        c0->xr = cell[i]->xr;
        c0->yt = cell[i]->yt;
        c0->idx = cell[i]->idx;
        c0->chipIdx = 0;
        cell0.push_back(c0);
    }
    for (int i = div; i < numCell; i++)
    {
        Data *c1 = new Data;
        c1->name = cell[i]->name;
        c1->x = cell[i]->x;
        c1->y = cell[i]->y;
        c1->w = cell[i]->w;
        c1->h = cell[i]->h;
        c1->xr = cell[i]->xr;
        c1->yt = cell[i]->yt;
        c1->idx = cell[i]->idx;
        c1->chipIdx = 1;
        cell1.push_back(c1);
    }
    for (int i = 0; i < int(cell0.size()); i++)
    {
        for (int next = i + 1; next < int(cell0.size()); next++)
        {
            // on the left side of leftmost
            if (cell0[i]->xr < cell0[next]->x)
            {
                poverlapArea += 0;
            }
            // below the side of bottom
            else if (cell0[i]->yt < cell0[next]->y)
            {
                poverlapArea += 0;
            }
            // overlap
            else
            {
                distX = min(cell0[i]->xr, cell0[next]->xr) - max(cell0[i]->x, cell0[next]->x);
                distY = min(cell0[i]->yt, cell0[next]->yt) - max(cell0[i]->y, cell0[next]->y);
                if (distX > 0 && distY > 0)
                {
                    poverlapArea += distX * distY;
                }
            }
        }
    }
    for (int i = 0; i < int(cell1.size()); i++)
    {
        for (int next = i + 1; next < int(cell1.size()); next++)
        {
            // on the left side of leftmost
            if (cell1[i]->xr < cell1[next]->x)
            {
                poverlapArea += 0;
            }
            // below the side of bottom
            else if (cell1[i]->yt < cell1[next]->y)
            {
                poverlapArea += 0;
            }
            // overlap
            else
            {
                distX = min(cell1[i]->xr, cell1[next]->xr) - max(cell1[i]->x, cell1[next]->x);
                distY = min(cell1[i]->yt, cell1[next]->yt) - max(cell1[i]->y, cell1[next]->y);
                if (distX > 0 && distY > 0)
                {
                    poverlapArea += distX * distY;
                }
            }
        }
    }
    // cout << "partoverlap: " << poverlapArea << endl;
    return poverlapArea;
}

void Partition::pushTerminal()
{
    vec.resize(rn);
    // Row *row = new Row;
    // int ri = 0;
    // row->rowb = rh * ri;
    // row->rowt = rh + row->rowb;
    // row->roww = cw;
    for (int j = 0; j < numTerminal; j++)
    {
        int flag1 = 0, flag2 = 0;
        Data *t = terminal[j];
        int t1 = t->y;
        int t2 = t->y + t->h;
        // cout << "t1 " << t1 << " " << t2 << endl;
        for (int i = 0; i < rn - 1; i++)
        {
            int y1 = rh * i;
            int y2 = y1 + rh;
            // cout << "y1 " << y1 << " " << y2 << endl;
            if (t1 >= y1 && t1 < y2)
            {
                flag1 = i;
            }
            if (t2 > y1 && t2 <= y2)
            {
                flag2 = i;
                break;
            }
        }
        // cout << "flag " << flag1 << " " << flag2 << endl;
        for (int k = flag1; k <= flag2; k++)
        {
            Data *d = new Data;
            d->x = terminal[j]->x;
            d->y = terminal[j]->y;
            d->w = terminal[j]->w;
            d->h = terminal[j]->h;
            d->name = terminal[j]->name;
            d->idx = terminal[j]->idx;
            // cout << "in" << endl;
            vec[k].push_back(d);
        }
        // cout << "fininsh" << endl;
    }
    // cout << "size = " << vec.size() << endl;
    // for (int i = 0; i < vec.size(); i++)
    // {
    //     cout << "row = " << i << endl;
    //     for (int j = 0; j < vec[i].size(); j++)
    //         cout << vec[i][j] << " ";
    // }
    // cout << endl;
}