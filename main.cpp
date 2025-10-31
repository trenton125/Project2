#include <iostream>
#include "GameDatabaseWindow.h"
#include <fstream>
#include "splay.h"
#include "json.hpp"
#include <string>
using namespace std;
using json = nlohmann::json;

int main() {

    auto start = chrono::system_clock::now();

    int count = 0;

    Splay splay_tree;

    ifstream f("games.json");
    json data = json::parse(f);

    for (auto & it : data["data"]){
        string game = it.at("game");
        vector<string> genre = it.at("genre");
        string platform = it.at("platform");
        double rating = it.at("rating");
        count++;

        splay_tree.insertSplay(game, rating, genre, platform);
    }

    splay_tree.splaySearch("HollowKnight");

    GameDatabaseWindow window(1000, 700, splay_tree);
    window.run();

    auto end = chrono::system_clock::now();
    chrono::duration<double> build_time = end - start;
    cout << endl;
    cout << "Build time: " << build_time.count() << " " << "seconds" << endl;
    cout << "Node Count: " << count << endl;
    cout << endl;

    return 0;
}
