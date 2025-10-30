#include <iostream>
#include <fstream>
#include "splay.h"
#include "json.hpp"
#include <string>
using namespace std;
using json = nlohmann::json;

int main() {

    Splay splay_tree;

    ifstream f("../games.json");
    json data = json::parse(f);

    for (auto & it : data["data"]){
        string game = it.at("game");
        vector<string> genre = it.at("genre");
        string platform = it.at("platform");
        double rating = it.at("rating");

        splay_tree.insertSplay(game, rating, genre, platform);
    }

    splay_tree.splayPrintPreorder();


    return 0;
}
