#include <iostream>
#include "GameDatabaseWindow.h"
#include <fstream>
#include "splay.h"
#include "heap.h"
#include "json.hpp"
#include <string>
#include <chrono>
using namespace std;
using json = nlohmann::json;



int main() {

    int count = 0;

    Splay splay_tree;
    MaxHeap max_heap;

    // Load JSON data
    ifstream f("../games.json");
    json data = json::parse(f);

    auto splay_start = chrono::high_resolution_clock::now();

    for (auto & it : data["data"]){
        string game = it.at("game");
        vector<string> genre = it.at("genre");
        string platform = it.at("platform");
        double rating = it.at("rating");

        splay_tree.insertSplay(game, rating, genre, platform);
        count++;
    }
    for (int i = 0; i < 82466; i++) {
        string game = to_string(i);
        vector<string> genre = {"N/A", "N/A"};
        string platform = "N/A";
        double rating = 10;

        splay_tree.insertSplay(game, rating, genre, platform);
        count++;
    }

    auto splay_end = chrono::high_resolution_clock::now();

    chrono::duration<double> splay_build_time = splay_end - splay_start;

    // Build Max Heap with timing
    auto heap_start = chrono::high_resolution_clock::now();

    for (auto & it : data["data"]){
        string game = it.at("game");
        vector<string> genre = it.at("genre");
        string platform = it.at("platform");
        double rating = it.at("rating");

        max_heap.insertHeap(game, 0, rating, genre, platform, "-1");
    }
    for (int i = 0; i < 82466; i++) {
        string game = to_string(i);
        vector<string> genre = {"N/A", "N/A"};
        string platform = "N/A";
        double rating = 10;

        max_heap.insertHeap(game, 0, rating, genre, platform, "-1");
    }

    auto heap_end = chrono::high_resolution_clock::now();

    chrono::duration<double> heap_build_time = heap_end - heap_start;

    // Launch GUI with both data structures and timing info
    GameDatabaseWindow window(1000, 750, splay_tree, max_heap,
                             splay_build_time.count(), heap_build_time.count());
    window.run();

    cout << endl;
    cout << "Node Count: " << count << endl;
    cout << endl;

    return 0;
}