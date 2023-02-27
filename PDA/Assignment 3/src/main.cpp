#include "partition.h"
// #include "partition.cpp"
#include "new.cpp"

int main(int argc, char *argv[])
{

    ifstream fin(argv[1]);
    ofstream fout(argv[2]);
    clock_t start, end;

    start = clock();

    p.inputFile(fin);
    p.part();
    p.placement();
    p.pushTerminal();
    // p.initoverlap();
    // p.partoverlap();
    // p.costfunction(cell, cell);
    // p.final();
    p.outputFile(fout);
    end = clock();
    double tookTime = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time taken by lab3 is " << fixed << setprecision(6)
         << tookTime << " secends." << endl;

    fin.close();
    fout.close();
    return 0;
}
