//
// Created by trent on 10/23/2025.
//

#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Splay {

  public:

    struct Node {

      // Game info

      string title;
      string release_year;
      string ign_rating;
      string genre;
      vector<string> platforms;
      string user_rating;

      // Tree info

      Node* left;
      Node* right;

      Node(string title, string release_year, string ign_rating, string genre, vector<string> platforms, string user_rating);

    };

  private:

    Node* root;
    bool empty;

  public:

    Splay();

    void deleteSplay(Node* root);

    ~Splay();

  //hello world

  };

