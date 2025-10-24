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
    splay.insertSplay("HollowKnight: Silksong", 10, {"souls-like", "metroidvania"}, "PC", "10");
    splay.splayPrintInorder();

    return 0;
}