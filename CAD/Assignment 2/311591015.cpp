#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <list>
#include <stack>
#include <deque>
#include <queue>

#include <iterator>

using namespace std;

void infileV(ifstream &inV);
void infilePat(ifstream &inPat, stringstream &ss2);
void infilePat2(ifstream &inPat);
void infileLib(ifstream &inLib);
void outfileLoad(ofstream &outLoad);
void outfileDelay(ofstream &outDelay);
void outfilePath(ofstream &outPath);
void calDelay(istringstream &ss);
void getSignal(istringstream &ss);
void outfile(ofstream &outDelay, ofstream &outPath, stringstream &ss2);

// void outfile(ofstream &outDelay, ofstream &outPath, vector<vector<int>> &patSignal);
// next = s.find_first_of(",\";\{()\\}:", init);

void dealVS1(string &s)
{
    size_t beg = s.find("/*"), end;
    while (beg != string::npos)
    {
        end = s.find("/", beg + 1);
        s.replace(beg, end - beg + 1, 1, ' ');
        beg = s.find("/*");
    }
}

void dealVS2(string &s)
{
    size_t beg = s.find(',');
    while (beg != string::npos)
    {
        s.replace(beg, 1, 1, ' ');
        if (beg + 1 >= s.size())
        {
            break;
        }
        beg = s.find(',', beg + 1);
    }
}

void dealSpace(string s, vector<string> &temp)
{
    int init = 0;
    int next;
    while (true)
    {
        next = s.find_first_of("(.) ", init);
        if (next != init)
        {
            string tmp = s.substr(init, next - init);
            if (tmp.size() != 0)
                temp.push_back(tmp);
        }
        if (next == string::npos)
            break;
        init = next + 1;
    }
}

void dealString(string &s)
{
    int init = 0;
    int next;
    // while (true)
    // {
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '(' || s[i] == ')' || s[i] == '\"' || s[i] == ',')
        {
            s[i] = ' ';
        }
    }
    //}
}

void splitStr2Vec(string s, vector<string> &buf)
{
    int current = 0, next;
    while (true)
    {
        next = s.find_first_of(",;:( )\t\"", current);
        if (next != current)
        {
            string tmp = s.substr(current, next - current);
            if (tmp.size() != 0)
                buf.push_back(tmp);
        }
        if (next == string::npos)
            break;
        current = next + 1;
    }
}

struct Wire;

struct Gate
{
    Gate();
    int typeGate;
    bool isvisit, sensization;
    double cap, inTransTime, outTransTime, outLoadCap, delay, accuDelay;
    string strGate, strtypeGate, gateId, strWireIn1, strWireIn2, strwireOut;
    Wire *wireIn1, *wireIn2, *wireOut;
};

struct Wire
{
    Wire();
    int signal;
    bool isend;
    double transition;
    string strWire, idxIn;
    // vector<int> vecWireData;
    // vector<string> vecIdxOut;
    //**
    Gate *inGate;
    vector<Gate *> vecOutGate;
    // vector<Gate *> vecInGate, vecOutGate;
};

struct Graph
{
    Graph();
    string strGraph;
    // vector<string> vecPortGraph;
    // vector<Wire *> vecInWireGraph, vecOutWireGraph, vecwireGraph;
    // vector<Gate *> vecGateGraph;
};

struct Table
{
    Table();
    double in1Cap, in2Cap, outCap;
    // double vecOutCap[7], vecInTrans[7];
    double tableCellRise[7][7], tableCellFall[7][7], tableRiseTrans[7][7], tableFallTrans[7][7];
    string strTable;
    // vector<double> vecOutCap, vecInTrans;
    // vector<vector<double>> tableCellRise, tableCellFall, tableRiseTrans, tableFallTrans;
    // pair<string, string> pairString;
};

// Table inverter;
// Table nand;
// Table nor;
double vecOutCap[7], vecInTrans[7];
vector<string> vecWire, vecPatWire;
vector<vector<int>> patSignal;
map<string, Gate *> gateMap;
unordered_map<string, Wire *> cktMap;    // check
unordered_map<string, Table *> tableMap; // check
map<string, Wire *> inCktMap;
map<string, Wire *> outCktMap;
list<Gate *> gateList;
list<Wire *> wireList;

// stringstream ss2;

void getss(istringstream &ss, stringstream &ss2)
{
    // stringstream ss2;
    ss2 << ss.rdbuf();

    cout << ss.str() << endl;
    cout << "g " << ss2.str() << "f" << endl;
}

int main(int argc, char *argv[])
{

    // ifstream inPat(argv[1]);
    // infilePat(inPat);
    // inPat.close();
    stringstream ss2;

    ifstream inV(argv[1]);
    ifstream inPat(argv[3]);
    ifstream inLib(argv[5]);
    ofstream outLoad;
    ofstream outDelay;
    ofstream outPath;

    infileV(inV);
    infilePat(inPat, ss2);
    infileLib(inLib);

    string testCase = argv[1];
    string caseName = testCase.substr(0, testCase.size() - 2);
    outLoad.open("311591015_" + caseName + "_load.txt");
    outDelay.open("311591015_" + caseName + "_delay.txt");
    outPath.open("311591015_" + caseName + "_path.txt");
    outfileLoad(outLoad);
    outfile(outDelay, outPath, ss2);

    // outfileDelay(outDelay);
    // outfilePath(outPath);

    inV.close();
    inPat.close();
    inLib.close();
    outLoad.close();
    outDelay.close();
    outPath.close();
    return 0;
}

Gate::Gate()
{
    cap = 0;
    typeGate = -1;
    isvisit = false, sensization = true;
    inTransTime = 0, outTransTime = 0;
    outLoadCap = 0;
    delay = 0, accuDelay = 0;
    strGate = "", strtypeGate = "", gateId = "", strWireIn1 = "", strWireIn2 = "", strwireOut = "";
    wireIn1 = nullptr, wireIn2 = nullptr, wireOut = nullptr;
}

Wire::Wire()
{
    strWire = "", idxIn = "";
    signal = -1;
    isend = false;
    transition = 0;
    // vecWire = "";
    // vecWireData = 0;
    // vecIdxOut = "";
    inGate = nullptr;
    // , vecOutGate = nullptr;
}
Graph::Graph()
{
    strGraph = "";
    // vecPortGraph = "";
    // vecInWireGraph = nullptr, vecOutWireGraph = nullptr, vecwireGraph = nullptr;
    // vecGateGraph = nullptr;
}
Table::Table()
{
    in1Cap = 0, in2Cap = 0, outCap = 0;
    // vecOutCap[7] = {0}, vecInTrans[7] = {0};
    tableCellRise[7][7] = {0}, tableCellFall[7][7] = {0}, tableRiseTrans[7][7] = {0}, tableFallTrans[7][7] = {0};
    strTable = "";
}

void getSignal(istringstream &ss)
{
    vector<int> vecSignal;
    vector<int> v;
    do
    {
        // v.clear();
        for (int s; ss >> s;)
        {
            v.push_back(s);
            vecSignal.push_back(s);
        }
        patSignal.push_back(v);
        if (ss.fail())
        {
            ss.clear();
            string token;
            ss >> token;
        }

    } while (!ss.eof());

    // for (size_t i = 0; i < vecSignal.size(); ++i)
    // {
    //     cout << vecSignal[i] << " ";
    // }
    // cout << endl;
    // for (int i = 0; i < patSignal.size(); i++)
    // {
    //     for (int j = 0; j <= 4; j++)
    //     {
    //         cout << "pat: " << patSignal[i][j];
    //     }
    //     cout << endl;
    // }
}

void infileV(ifstream &inV)
{
    // cout << "inv" << endl;
    string str, line;
    // istringstream ss;
    vector<string> vecData;
    bool preCom = false;
    bool posCom = false;

    if (!inV.is_open())
    {
        cout << "can not open .v file" << endl;
    }
    else
    {
        getline(inV, str);
        while (str.find("module") == string::npos)
        {
            getline(inV, str);
            // cout << str << " ";
        }
        // cout << endl;
        istringstream ss;
        while (true)
        {
            // cout << "in: " << endl;
            // cout << endl;
            ss.str("");
            ss.clear();
            getline(inV, str);
            ss.str(str);
            ss >> line;
            if (line.find("endmodule") != string::npos)
            {
                break;
            }
            else if (line.find("/*") != string::npos)
            {
                preCom = true;
                line = ss.str();
                while (line.find("*/") == string::npos)
                {
                    posCom = true;
                    getline(inV, line);
                }
                continue;
            }
            else if (line.find("//") != string::npos)
            {
                continue;
            }
            else
            {
                if (line == "input")
                {
                    string s = ss.str();
                    getline(ss, s, ';');
                    dealVS1(s);
                    // cout << "1: " << endl;
                    // cout << s << endl;
                    dealVS2(s);
                    // cout << "2: " << endl;
                    // cout << s << endl;
                    ss.str("");
                    ss.clear();
                    ss.str(s);

                    Wire *wire;
                    string name;
                    // cout << "input name: " << endl;
                    while (ss >> name)
                    {
                        wire = new Wire;
                        wire->strWire = name;

                        // cout << wire->strWire << endl;
                        pair<string, Wire *> toMap(name, wire);
                        cktMap.insert(toMap);
                        inCktMap.insert(toMap);
                        wireList.push_back(wire);
                        // cout<<wireList<<endl;
                    }
                }

                else if (line == "output")
                {
                    string s = ss.str();
                    getline(ss, s, ';');
                    dealVS1(s);
                    // cout << "1: " << endl;
                    // cout << s << endl;
                    dealVS2(s);
                    // cout << "2: " << endl;
                    // cout << s << endl;
                    ss.str("");
                    ss.clear();
                    ss.str(s);

                    Wire *wire;
                    string name;
                    // cout << "output name: " << endl;
                    while (ss >> name)
                    {
                        wire = new Wire;
                        wire->strWire = name;

                        // cout << wire->strWire << endl;
                        pair<string, Wire *> toMap(name, wire);
                        cktMap.insert(toMap);
                        outCktMap.insert(toMap);
                        wireList.push_back(wire);
                        // cout<<wireList<<endl;
                    }
                }

                else if (line == "wire")
                {
                    string s = ss.str();
                    getline(ss, s, ';');
                    dealVS1(s);
                    // cout << "1: " << endl;
                    // cout << s << endl;
                    dealVS2(s);
                    // cout << "2: " << endl;
                    // cout << s << endl;
                    ss.str("");
                    ss.clear();
                    ss.str(s);

                    Wire *wire;
                    string name;
                    // cout << "wire name: " << endl;
                    while (ss >> name)
                    {
                        wire = new Wire;
                        wire->strWire = name;

                        // cout << wire->strWire << endl;
                        pair<string, Wire *> toMap(name, wire);
                        cktMap.insert(toMap);
                        wireList.push_back(wire);
                        // cout<<wireList<<endl;
                    }
                }
                else
                {
                    if (line == "NOR2X1")
                    {
                        string ckt, pin;
                        string s = ss.str();
                        // cout << "s: " << s << endl;
                        getline(ss, s, ';');
                        // cout << "s after getline: " << s << endl;
                        dealVS1(s);
                        // cout << "1: ";
                        // cout << s << endl;
                        dealVS2(s);
                        // cout << "2: ";
                        // cout << s << endl;
                        ss.str("");
                        ss.clear();
                        ss.str(s);
                        // cout << "after ss: " << s << endl;

                        Gate *gate;
                        gate = new Gate;
                        ss >> gate->strGate;
                        // cout << "strGate: " << gate->strGate << endl;
                        gate->strtypeGate = "NOR2X1";
                        // cout << "strtypeGate: " << gate->strtypeGate << endl;
                        vecData.clear();
                        ss.str("");
                        ss.clear();

                        dealSpace(s, vecData);
                        // cout << "deal space: ";
                        // for (int j = 0; j < vecData.size(); j++)
                        // {
                        //     cout << vecData[j] << " ";
                        // }

                        // cout << endl;
                        // cout << "vecData size: " << vecData.size() << endl;
                        stringstream os;

                        copy(vecData.begin(), vecData.end(), ostream_iterator<string>(os, "\n"));
                        // cout << os << endl;
                        os >> ckt;
                        gate->gateId = ckt;
                        // cout << "gateId discard: " << ckt << endl;
                        for (int i = 0; i <= 2; i++)
                        {
                            os >> ckt;
                            // cout << "begin: " << ckt;
                            // cout << endl;
                            // cout << "in foor loop: "
                            //      << "begin_ " << ckt << endl;

                            if (ckt == "ZN")
                            {
                                os >> pin;
                                // cout << "ckt: " << ckt;
                                // cout << " pin: " << pin;
                                // cout << " ss: " << ss << endl;
                                gate->wireOut = cktMap[pin];
                                cktMap[pin]->inGate = gate;
                            }

                            else if (ckt == "A1")
                            {
                                os >> pin;
                                // cout << "ckt: " << ckt;
                                // cout << " pin: " << pin;
                                // cout << " ss: " << ss << endl;
                                gate->wireIn1 = cktMap[pin];
                                cktMap[pin]->vecOutGate.push_back(gate);
                            }

                            else if (ckt == "A2")
                            {
                                os >> pin;
                                // cout << "ckt: " << ckt;
                                // cout << " pin: " << pin;
                                // cout << " ss: " << ss << endl;
                                gate->wireIn2 = cktMap[pin];
                                cktMap[pin]->vecOutGate.push_back(gate);
                            }

                            else
                            {
                                cout << "Error Message: "
                                     << "with in NOR2X1, error!!" << endl;
                                cout << "line: " << line << " "
                                     << ", ckt: " << ckt << endl;
                            }
                            // cout << endl;
                        }
                        gateList.push_back(gate);
                        gateMap.insert(pair<string, Gate *>(gate->strGate, gate));
                    }

                    else if (line == "INVX1")
                    {
                        string ckt, pin;
                        string s = ss.str();
                        // cout << "s: " << s << endl;
                        getline(ss, s, ';');
                        // cout << "s after getline: " << s << endl;
                        dealVS1(s);
                        // cout << "1: ";
                        // cout << s << endl;
                        dealVS2(s);
                        // cout << "2: ";
                        // cout << s << endl;
                        ss.str("");
                        ss.clear();
                        ss.str(s);
                        // cout << "after ss: " << s << endl;

                        Gate *gate;
                        gate = new Gate;
                        ss >> gate->strGate;
                        // cout << "strGate: " << gate->strGate << endl;
                        gate->strtypeGate = "INVX1";
                        // cout << "strtypeGate: " << gate->strtypeGate << endl;
                        vecData.clear();
                        ss.str("");
                        ss.clear();

                        dealSpace(s, vecData);
                        // cout << "deal space: ";
                        // for (int j = 0; j < vecData.size(); j++)
                        // {
                        //     cout << vecData[j] << " ";
                        // }

                        // cout << endl;
                        // cout << "vecData size: " << vecData.size() << endl;
                        stringstream os;

                        copy(vecData.begin(), vecData.end(), ostream_iterator<string>(os, "\n"));
                        // cout << os << endl;
                        os >> ckt;
                        gate->gateId = ckt;
                        // cout << "gateId discard: " << ckt << endl;
                        for (int i = 0; i <= 1; i++)
                        {
                            os >> ckt;
                            // cout << "begin: " << ckt;
                            // cout << endl;
                            // cout << "in foor loop: "
                            //      << "begin_ " << ckt << endl;

                            if (ckt == "ZN")
                            {
                                os >> pin;
                                // cout << "ckt: " << ckt;
                                // cout << " pin: " << pin;
                                // cout << " ss: " << ss << endl;
                                gate->wireOut = cktMap[pin];
                                cktMap[pin]->inGate = gate;
                            }

                            else if (ckt == "I")
                            {
                                os >> pin;
                                // cout << "ckt: " << ckt;
                                // cout << " pin: " << pin;
                                // cout << " ss: " << ss << endl;
                                gate->wireIn1 = cktMap[pin];
                                gate->wireIn2 = nullptr;
                                cktMap[pin]->vecOutGate.push_back(gate);
                            }

                            else
                            {
                                cout << "Error Message: "
                                     << "with in INVX1, error!!" << endl;
                                cout << "line: " << line << " "
                                     << ", ckt: " << ckt << endl;
                            }
                            // cout << endl;
                        }
                        gateList.push_back(gate);
                        gateMap.insert(pair<string, Gate *>(gate->strGate, gate));
                    }

                    else if (line == "NANDX1")
                    {
                        string ckt, pin;
                        string s = ss.str();
                        // cout << "s: " << s << endl;
                        getline(ss, s, ';');
                        // cout << "s after getline: " << s << endl;
                        dealVS1(s);
                        // cout << "1: ";
                        // cout << s << endl;
                        dealVS2(s);
                        // cout << "2: ";
                        // cout << s << endl;
                        ss.str("");
                        ss.clear();
                        ss.str(s);
                        // cout << "after ss: " << s << endl;

                        Gate *gate;
                        gate = new Gate;
                        ss >> gate->strGate;
                        // cout << "strGate: " << gate->strGate << endl;
                        gate->strtypeGate = "NANDX1";
                        // cout << "strtypeGate: " << gate->strtypeGate << endl;
                        vecData.clear();
                        ss.str("");
                        ss.clear();

                        dealSpace(s, vecData);
                        // cout << "deal space: ";
                        // for (int j = 0; j < vecData.size(); j++)
                        // {
                        //     cout << vecData[j] << " ";
                        // }

                        // cout << endl;
                        // cout << "vecData size: " << vecData.size() << endl;
                        stringstream os;

                        copy(vecData.begin(), vecData.end(), ostream_iterator<string>(os, "\n"));
                        // cout << os << endl;
                        os >> ckt;
                        gate->gateId = ckt;
                        // cout << "gateId discard: " << ckt << endl;
                        for (int i = 0; i <= 2; i++)
                        {
                            os >> ckt;
                            // cout << "begin: " << ckt;
                            // cout << endl;
                            // cout << "in foor loop: "
                            //      << "begin_ " << ckt << endl;

                            if (ckt == "ZN")
                            {
                                os >> pin;
                                // cout << "ckt: " << ckt;
                                // cout << " pin: " << pin;
                                // cout << " ss: " << ss << endl;
                                gate->wireOut = cktMap[pin];
                                cktMap[pin]->inGate = gate;
                            }

                            else if (ckt == "A1")
                            {
                                os >> pin;
                                // cout << "ckt: " << ckt;
                                // cout << " pin: " << pin;
                                // cout << " ss: " << ss << endl;
                                gate->wireIn1 = cktMap[pin];
                                cktMap[pin]->vecOutGate.push_back(gate);
                            }

                            else if (ckt == "A2")
                            {
                                os >> pin;
                                // cout << "ckt: " << ckt;
                                // cout << " pin: " << pin;
                                // cout << " ss: " << ss << endl;
                                gate->wireIn2 = cktMap[pin];
                                cktMap[pin]->vecOutGate.push_back(gate);
                            }

                            else
                            {
                                cout << "Error Message: "
                                     << "with in NANDX1, error!!" << endl;
                                cout << "line: " << line << " "
                                     << ", ckt: " << ckt << endl;
                            }
                            // cout << endl;
                        }
                        gateList.push_back(gate);
                        gateMap.insert(pair<string, Gate *>(gate->strGate, gate));
                    }

                    else
                    {
                        continue;
                    }
                }
            }
        }
    }
}

void infilePat2(ifstream &inPat)
{
    // cout << "inpat" << endl;
    string str, line, patName;
    istringstream os;
    vector<string> vecData, temp;
    vector<int> vec;
    bool preCom = false;
    bool posCom = false;
    size_t i = 0;

    if (!inPat.is_open())
    {
        cout << "can not open .pat file" << endl;
    }
    else
    {
        getline(inPat, str);

        splitStr2Vec(str, vecData);
        while (!inPat.eof())
        {
            temp.clear();
            // vec.clear();
            getline(inPat, str);
            splitStr2Vec(str, temp);

            for (int i = 0; i < temp.size(); i++)
            {
                if (temp[i] != "0" && temp[i] != "1")
                    continue;
                vec.push_back(stoi(temp[i]));
                // copy(vecData.begin(), vecData.end(), ostream_iterator<string>(os, "\n"));
            }
            // //  dealSpace(str, vecData);
            // // stringstream os;
            // // copy(vecData.begin(), vecData.end(), ostream_iterator<string>(os, "\n"));
            // // string pat;
            // // os >> pat;
            // // cout << pat << endl;
            // for (int i = 0; i < vec.size(); i++)
            // {
            //     cout << vec[i] << " ";
            // }
            // cout << endl;
            if (str == ".end")
                break;
        }
        // for (int i = 0; i < vec.size(); i++)
        // {
        //     cout << vec[i] << " ";
        // }
        // cout << endl;
    }

    //     }
    // }
}

void infilePat(ifstream &inPat, stringstream &ss2)
{
    // cout << "inpat" << endl;
    string str, line, patName;
    istringstream os;
    // stringstream ss2;
    vector<string> vecData, vecNode;
    bool preCom = false;
    bool posCom = false;
    size_t i = 0;

    if (!inPat.is_open())
    {
        cout << "can not open .pat file" << endl;
    }
    else
    {
        // while (true)
        // {
        inPat >> str;
        // cout << "1: " << str << endl;
        getline(inPat, str);
        // cout << "2: " << str << endl;

        while (i < str.size())
        {
            i = str.find(i, ',');
            if (i == string::npos)
            {
                break;
            }
            str.replace(i, 1, 1, ' ');
            i++;
        }
        // cout << "bef: " << str << endl;
        istringstream ss(str);
        // cout << "aft: " << str << endl;
        while (ss >> patName)
        {
            dealVS2(patName);
            vecWire.push_back(patName);
        }
        // for (int i = 0; i < vecWire.size(); i++)
        //     cout << "vecWire: " << vecWire[i] << " ";
        // cout << endl;
        getline(inPat, str);
        while (str.find(".end") == string::npos)
        {
            ss.str("");
            ss.clear();
            ss.str(str);
            // cout << str << endl;
            // cout << "get .pat signal: " << endl;
            getSignal(ss);
            // getss(ss, ss2);
            // ss2 << ss.rdbuf();
            std::stringstream ss2(ss.str());
            // cout << ss.str() << endl;
            // cout << ss2.str() << endl;
            getline(inPat, str);
        }

        //     }
        // }
    }
}

void infileLib(ifstream &inLib)
{
    // cout << "inlib" << endl;
    // int cell;
    string str, cell;
    vector<string> vecData;
    bool preCom = false;
    bool posCom = false;
    Table *table;

    if (!inLib.is_open())
    {
        cout << "can not open .lib file" << endl;
    }
    else
    {
        while (str != "index_1")
        {
            // cout << str << endl;
            inLib >> str;
        }
        // cout << "after index1" << endl;
        getline(inLib, str);
        // cout << "after getline" << endl;

        dealString(str);
        // cout << "after deal string" << endl;

        istringstream ss(str);
        // cout << "after ss" << endl;
        // cout << "in veccap for loop" << endl;

        for (int i = 0; i <= 6; i++)
        {
            // cout << "in veccap for loop" << endl;
            ss >> vecOutCap[i];
            // cout << vecOutCap[i] << " ";
        }
        // cout << endl;

        // cout << "after vecoutcap" << endl;

        inLib >> str;
        while (str != "index_2")
        {
            cout << str << endl;
            inLib >> str;
        }
        // cout << "after index_2" << endl;

        getline(inLib, str);
        // cout << "after getline" << endl;

        dealString(str);
        // cout << "after dealstring" << endl;

        ss.str("");
        ss.clear();
        ss.str(str);
        // cout << ss.str() << endl;
        // cout << "in vecintrans for loop" << endl;

        for (int i = 0; i <= 6; i++)
        {
            // cout << "in vecintrans for loop" << endl;
            ss >> vecInTrans[i];
            // cout << vecInTrans[i] << " ";
        }
        // cout << endl;
        // choose which cell
        inLib >> str;
        for (int i = 0; i <= 2; i++)
        {
            table = new Table;
            while (str != "capacitance")
            {
                inLib >> str;
            }
            inLib >> str;
            inLib >> table->in1Cap;
            // cout << "capacitance1 " << table->in1Cap << endl;
            if (i != 1)
            {
                inLib >> str;
                while (str != "capacitance")
                {
                    inLib >> str;
                }
                inLib >> str;
                inLib >> table->in2Cap;
                // cout << "capacitance2 " << table->in2Cap << endl;
            }

            inLib >> str;
            while (str != "cell_rise(table10){")
            {
                inLib >> str;
            }
            inLib >> str;

            // cout << "tableCellRise " << endl;
            for (int j = 0; j <= 6; j++)
            {
                getline(inLib, str);
                dealString(str);
                ss.str("");
                ss.clear();
                ss.str(str);
                // cout << endl;
                for (int k = 0; k <= 6; k++)
                {
                    ss >> table->tableCellRise[j][k];
                    // cout << table->tableCellRise[j][k] << " ";
                }
                // cout << endl;
            }

            inLib >> str;
            while (str != "cell_fall(table10){")
            {
                inLib >> str;
            }
            inLib >> str;

            // cout << "tableCellFall " << endl;
            for (int j = 0; j <= 6; j++)
            {
                getline(inLib, str);
                dealString(str);
                ss.str("");
                ss.clear();
                ss.str(str);
                // cout<<endl;
                for (int k = 0; k <= 6; k++)
                {
                    ss >> table->tableCellFall[j][k];
                    // cout << table->tableCellFall[j][k] << " ";
                }
                // cout << endl;
            }

            inLib >> str;
            while (str != "rise_transition(table10){")
            {
                inLib >> str;
            }
            inLib >> str;

            // cout << "tableRiseTrans " << endl;
            for (int j = 0; j <= 6; j++)
            {
                getline(inLib, str);
                dealString(str);
                ss.str("");
                ss.clear();
                ss.str(str);
                // cout << endl;
                for (int k = 0; k <= 6; k++)
                {
                    ss >> table->tableRiseTrans[j][k];
                    // cout << table->tableRiseTrans[j][k] << " ";
                }
                // cout << endl;
            }

            inLib >> str;
            while (str != "fall_transition(table10){")
            {
                inLib >> str;
            }
            inLib >> str;

            // cout << "tableFallTrans " << endl;
            for (int j = 0; j <= 6; j++)
            {
                getline(inLib, str);
                dealString(str);
                ss.str("");
                ss.clear();
                ss.str(str);
                for (int k = 0; k <= 6; k++)
                {
                    ss >> table->tableFallTrans[j][k];
                    // cout << table->tableFallTrans[j][k] << " ";
                }
                // cout << endl;
            }
            switch (i)
            {
            case 0:
                cell = "NOR2X1";
                break;
            case 1:
                cell = "INVX1";
                break;
            default:
                cell = "NANDX1";
                break;
            }
            tableMap.insert(pair<string, Table *>(cell, table));
        }
        // free(table);
    }
}

bool sortLoad(const Gate *a, const Gate *b);
bool sortDelay(const Gate *a, const Gate *b);
void getDelay(Gate *gate, double &outputTransition, double &gateDelay);

void outfileLoad(ofstream &outLoad)
{
    for (auto it = gateList.begin(); it != gateList.end(); it++)
    {
        Wire *wire = (*it)->wireOut;
        double load = 0.0;
        for (auto itGate = wire->vecOutGate.begin(); itGate != wire->vecOutGate.end(); itGate++)
        {
            string type = (*itGate)->strtypeGate;
            if (wire == (*itGate)->wireIn1)
            {
                load += tableMap[type]->in1Cap;
            }
            else if (wire == (*itGate)->wireIn2)
            {
                load += tableMap[type]->in2Cap;
            }
            else
            {
                cout << "calLoad error" << endl;
            }
        }
        if (outCktMap.find(wire->strWire) != outCktMap.end())
        {
            load += 0.03;
        }
        (*it)->outLoadCap = load;
    }
    gateList.sort(sortLoad);

    for (auto it = gateList.begin(); it != gateList.end(); it++)
    {
        outLoad << (*it)->gateId << ' ' << (*it)->outLoadCap << "\n";
    }
}

bool sortLoad(const Gate *a, const Gate *b)
{
    if (a->outLoadCap == b->outLoadCap)
    {
        string a_name = a->strGate;
        string b_name = b->strGate;
        int pos = 0;
        for (int i = 0; i < a_name.size(); i++)
        {
            if (a_name[i] >= '0' && a_name[i] <= '9')
            {
                break;
            }
            pos++;
        }
        a_name.erase(0, pos);

        pos = 0;
        for (int i = 0; i < b_name.size(); i++)
        {
            if (b_name[i] >= '0' && b_name[i] <= '9')
            {
                break;
            }
            pos++;
        }
        b_name.erase(0, pos);

        return stoi(a_name) < stoi(b_name);
    }
    else
    {
        return a->outLoadCap > b->outLoadCap;
    }
}

bool sortDelay(const Gate *a, const Gate *b)
{
    return a->delay > b->delay;
}

void outfile(ofstream &outDelay, ofstream &outPath, stringstream &ss2)
{
    for (auto i = gateList.begin(); i != gateList.end(); i++)
    {
        (*i)->isvisit = false;
        (*i)->accuDelay = 0.0;
    }

    for (auto i = wireList.begin(); i != wireList.end(); i++)
    {
        (*i)->isend = false;
    }
    queue<Wire *> wireQue;
    queue<Gate *> gateQue;
    /* Initialization */
    for (int i = 0; i < vecPatWire.size(); i++)
    {
        Wire *wire = cktMap[vecPatWire[i]];
        ss2 >> wire->signal;
        wire->transition = 0.0;
        for (auto igate = wire->vecOutGate.begin(); igate != wire->vecOutGate.end(); igate++)
        {
            string nameGate = (*igate)->gateId; /// check
            if (!(*igate)->isvisit)
            {
                gateQue.push(*igate);
            }
        }
        wire->isend = true;
    }

    Gate *igate;
    while (!gateQue.empty())
    {
        igate = gateQue.front();
        if (igate->strtypeGate == "INVX1")
        { // check
            if (!igate->wireIn1->isend)
            {
                gateQue.pop();
                gateQue.push(igate);
                continue;
            }
        }
        else
        {
            if (!igate->wireIn1->isend || !igate->wireIn2->isend)
            {
                gateQue.pop();
                gateQue.push(igate);
                continue;
            }
        }

        double outputTransition;
        double gateDelay;

        getDelay(igate, outputTransition, gateDelay);
        igate->delay = gateDelay;
        igate->accuDelay += gateDelay;
        igate->wireOut->transition = outputTransition;
        igate->isvisit = true;
        for (auto fgate = igate->wireOut->vecOutGate.begin(); fgate != igate->wireOut->vecOutGate.end(); fgate++)
        {
            if (!(*fgate)->isvisit)
            {
                gateQue.push(*fgate);
            }
        }

        igate->wireOut->isend = true;
        gateQue.pop();
    }

    gateList.sort(sortDelay); //
    for (auto i = gateList.begin(); i != gateList.end(); i++)
    {
        outDelay << (*i)->gateId << ' ' << (*i)->wireOut->signal << ' ' << (*i)->delay << ' ' << (*i)->wireOut->transition << endl;
    }
    outDelay << endl;

    Wire *maxWireDelay = outCktMap.begin()->second;
    double max = maxWireDelay->inGate->accuDelay;
    Wire *minWireDelay = outCktMap.begin()->second;
    double min = minWireDelay->inGate->accuDelay;

    for (auto i = outCktMap.begin(); i != outCktMap.end(); i++)
    {
        double d = i->second->inGate->accuDelay;
        if (d > max)
        {
            max = d;
            maxWireDelay = i->second;
        }
        if (d < min)
        {
            min = d;
            minWireDelay = i->second;
        }
    }

    outPath << "Longest delay = " << max << ", the path is: ";
    vector<Wire *> maxWirePath;
    Wire *wire = maxWireDelay;
    while (true)
    {
        maxWirePath.push_back(wire);
        if (inCktMap.find(wire->strWire) != inCktMap.end())
        { // check name?strWire
            break;
        }
        else
        {
            if (wire->inGate->sensization)
            {
                wire = wire->inGate->wireIn1;
            }
            else
            {
                wire = wire->inGate->wireIn2;
            }
        }
    }
    for (int i = maxWirePath.size() - 1; i >= 0; i--)
    {
        if (i != maxWirePath.size() - 1)
        {
            outPath << " -> ";
        }
        outPath << maxWirePath[i]->strWire;
        // check name?strWire
    }
    outPath << endl;

    outPath << "Shortest delay = " << min << ", the path is: ";
    vector<Wire *> minWirePath;
    wire = minWireDelay;
    while (true)
    {
        minWirePath.push_back(wire);
        if (inCktMap.find(wire->strWire) != inCktMap.end())
        {
            break;
        }
        else
        {
            if (wire->inGate->sensization)
            {
                wire = wire->inGate->wireIn1;
            }
            else
            {
                wire = wire->inGate->wireIn2;
            }
        }
    }
    for (int i = minWirePath.size() - 1; i >= 0; i--)
    {
        if (i != minWirePath.size() - 1)
        {
            outPath << " -> ";
        }
        outPath << minWirePath[i]->strWire;
    }
    outPath << endl;
}

double calculation(double cap1, double cap2, double in_trans1, double in_trans2, double n1, double n2, double n3, double n4, double cap, double trans)
{
    double ans1, ans2, result;
    if ((cap >= cap1) && (cap <= cap2))
    {
        if ((trans >= in_trans1) && (trans <= in_trans2))
        {
            ans1 = (cap2 - cap) * n1 / (cap2 - cap1) + (cap - cap1) * n2 / (cap2 - cap1);
            ans2 = (cap2 - cap) * n3 / (cap2 - cap1) + (cap - cap1) * n4 / (cap2 - cap1);
            result = (trans - in_trans1) * ans2 / (in_trans2 - in_trans1) + (in_trans2 - trans) * ans1 / (in_trans2 - in_trans1);
        }
        else if (trans < in_trans1)
        {
            ans1 = (cap2 - cap) * n1 / (cap2 - cap1) + (cap - cap1) * n2 / (cap2 - cap1);
            ans2 = (cap2 - cap) * n3 / (cap2 - cap1) + (cap - cap1) * n4 / (cap2 - cap1);
            result = ans2 - (ans2 - ans1) * (in_trans2 - trans) / (in_trans2 - in_trans1);
        }
        else if (trans > in_trans2)
        {
            ans1 = (cap2 - cap) * n1 / (cap2 - cap1) + (cap - cap1) * n2 / (cap2 - cap1);
            ans2 = (cap2 - cap) * n3 / (cap2 - cap1) + (cap - cap1) * n4 / (cap2 - cap1);
            result = ans1 + (ans2 - ans1) * (trans - in_trans1) / (in_trans2 - in_trans1);
        }
    }
    else if (cap < cap1)
    {
        if (trans < in_trans1)
        {
            ans1 = n2 - ((n2 - n1) * (cap2 - cap) / (cap2 - cap1));
            ans2 = n4 - ((n4 - n3) * (cap2 - cap) / (cap2 - cap1));
            result = ans2 - (ans2 - ans1) * (in_trans2 - trans) / (in_trans2 - in_trans1);
        }
        else if ((trans >= in_trans1) && (trans <= in_trans2))
        {
            ans1 = n2 - ((n2 - n1) * (cap2 - cap) / (cap2 - cap1));
            ans2 = n4 - ((n4 - n3) * (cap2 - cap) / (cap2 - cap1));
            result = (trans - in_trans1) * ans2 / (in_trans2 - in_trans1) + (in_trans2 - trans) * ans1 / (in_trans2 - in_trans1);
        }
    }
    else if (cap > cap2)
    {
        if (trans > in_trans2)
        {
            ans1 = n1 + (n2 - n1) * (cap - cap1) / (cap2 - cap1);
            ans2 = n3 + (n4 - n3) * (cap - cap1) / (cap2 - cap1);
            result = ans1 + (ans2 - ans1) * (trans - in_trans1) / (in_trans2 - in_trans1);
        }
        else if ((trans >= in_trans1) && (trans <= in_trans2))
        {
            ans1 = n1 + (n2 - n1) * (cap - cap1) / (cap2 - cap1);
            ans2 = n3 + (n4 - n3) * (cap - cap1) / (cap2 - cap1);
            result = (trans - in_trans1) * ans2 / (in_trans2 - in_trans1) + (in_trans2 - trans) * ans1 / (in_trans2 - in_trans1);
        }
    }
    return result;
}

void getDelay(Gate *gate, double &outputTransition, double &gateDelay)
{
    Wire *A1 = gate->wireIn1;
    Wire *A2 = gate->wireIn2;
    double delay1;
    if (inCktMap.find(A1->strWire) != inCktMap.end())
    {
        delay1 = 0.0;
    }
    else
    {
        delay1 = A1->inGate->accuDelay;
    }
    double delay2;
    if (gate->strtypeGate != "INVX1")
    {
        if (inCktMap.find(A2->strWire) != inCktMap.end())
            delay2 = 0.0;
        else
            delay2 = A2->inGate->accuDelay;
    }
    else
    {
        delay2 = -10000.0;
    }
    Table *table = tableMap[gate->strtypeGate];

    Wire *thiswire;
    double initdelay;
    int fsignal;
    if (gate->strtypeGate == "INVX1")
    {
        thiswire = A1;
        initdelay = delay1;
        gate->sensization = true;
        if (A1->signal == 1)
            fsignal = 0;
        else
            fsignal = 1;
    }
    else if (gate->strtypeGate == "NANDX1")
    {
        if (A1->signal == 0)
        {
            if (A2->signal == 0)
            {
                if (delay1 < delay2)
                {
                    gate->sensization = true;
                    gate->accuDelay = delay1;
                    thiswire = A1;
                    initdelay = delay1;
                }
                else
                {
                    gate->sensization = false;
                    gate->accuDelay = delay2;
                    thiswire = A2;
                    initdelay = delay2;
                }
                fsignal = 1;
            }
            else
            {
                gate->sensization = true;
                gate->accuDelay = delay1;
                thiswire = A1;
                initdelay = delay1;
                fsignal = 1;
            }
        }
        else if (A2->signal == 0)
        {
            gate->sensization = false;
            gate->accuDelay = delay2;
            thiswire = A2;
            initdelay = delay2;
            fsignal = 1;
        }
        else
        {
            if (delay1 > delay2)
            {
                gate->sensization = true;
                gate->accuDelay = delay1;
                thiswire = A1;
                initdelay = delay1;
            }
            else
            {
                gate->sensization = false;
                gate->accuDelay = delay2;
                thiswire = A2;
                initdelay = delay2;
            }
            fsignal = 0;
        }
    }
    else if (gate->strtypeGate == "NOR2X1")
    {
        if (A1->signal == 1)
        {
            if (A2->signal == 1)
            {
                if (delay1 < delay2)
                {
                    gate->sensization = true;
                    gate->accuDelay = delay1;
                    thiswire = A1;
                    initdelay = delay1;
                }
                else
                {
                    gate->sensization = false;
                    gate->accuDelay = delay2;
                    thiswire = A2;
                    initdelay = delay2;
                }
            }
            else
            {
                gate->sensization = true;
                gate->accuDelay = delay1;
                thiswire = A1;
                initdelay = delay1;
            }
            fsignal = 0;
        }
        else if (A2->signal == 1)
        {
            gate->sensization = false;
            gate->accuDelay = delay2;
            thiswire = A2;
            initdelay = delay2;
            fsignal = 0;
        }
        else
        {
            if (delay1 > delay2)
            {
                gate->sensization = true;
                gate->accuDelay = delay1;
                thiswire = A1;
                initdelay = delay1;
            }
            else
            {
                gate->sensization = false;
                gate->accuDelay = delay2;
                thiswire = A2;
                initdelay = delay2;
            }
            fsignal = 1;
        }
    }
    else
    {
        cout << "getdelay error" << endl;
    }
    gate->wireOut->signal = fsignal;

    if (fsignal == 1)
    {
        cout << "fsignal " << fsignal << endl;
    }
    else
    {
        cout << "fsignal " << fsignal << endl;
    }
    gate->accuDelay = initdelay;
}