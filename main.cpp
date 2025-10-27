#include <iostream>
#include "splay.h"
#include <string>
using namespace std;
// #include "Bridges.h"
// #include "DataSource.h"
// #include "data_src/Game.h"

// using namespace bridges;

int main() {

    Splay splay;
    splay.insertSplay("D", 10, {"souls-like", "metroidvania"}, "PC", "10");
    splay.insertSplay("E", 10, {"souls-like", "metroidvania"}, "PC", "10");
    splay.insertSplay("A", 10, {"souls-like", "metroidvania"}, "PC", "10");
    splay.splayPrintPreorder();
    splay.splaySearch("D");
    cout << endl << " " << endl;
    splay.splayPrintPreorder();

    return 0;
}