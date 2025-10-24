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
      double ign_rating;
      vector<string> genre;
      string platform;
      string user_rating;

      // Tree info

      Node* left;
      Node* right;

      Node(string title, double ign_rating, vector<string> genre, string platform, string user_rating);

    };

  private:

    Node* root;
    bool empty;

  public:

    // Splay functions

    Splay();

    void deleteSplay(Node* root);

    ~Splay();

    Node* recursiveSplayInsert(Node* root, string title, double ign_rating, vector<string> genre, string platform, string user_rating);

    void insertSplay(string title, double ign_rating, vector<string> genre, string platform, string user_rating);

    Node* splaySearch(string title);

    void splayPrintInorderRecursive(Node* root);

    void splayPrintInorder();

  };

