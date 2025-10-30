#include <iostream>
#include "splay.h"
#include "heap.h"
#include <string>
using namespace std;
// #include "Bridges.h"
// #include "DataSource.h"
// #include "data_src/Game.h"

// using namespace bridges;

int main() {
    MaxHeap heap;



    heap.insertHeap("Zelda: Breath of the Wild", 2017, 9.8, {"Adventure", "Open World"}, "Switch", "9.7");
    heap.insertHeap("Halo: Combat Evolved", 2001, 9.0, {"Shooter", "Sci-Fi"}, "Xbox", "9.1");
    heap.insertHeap("Animal Crossing: New Horizons", 2020, 8.5, {"Simulation"}, "Switch", "8.9");
    heap.insertHeap("Mario Kart 8 Deluxe", 2017, 9.3, {"Racing", "Multiplayer"}, "Switch", "9.5");
    heap.insertHeap("God of War", 2018, 9.6, {"Action", "Adventure"}, "PlayStation", "9.8");


    heap.display();
    heap.insertHeap("Zzzz Ultimate Game", 2025, 10.0, {"Fantasy", "RPG"}, "PC", "10.0");

    heap.getMax();

    heap.display();

}

