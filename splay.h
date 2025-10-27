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

      Node(const string &title, double ign_rating, const vector<string> &genre, const string& platform, const string &user_rating);

    };

  private:

    Node* root;
    bool empty;

  public:

    // Splay functions

    Splay();

    void deleteSplay(Node* root);

    ~Splay();

    Node* rightRotation(Node* root);

    Node* leftRotation(Node* root);

    Node* zig(Node* root);

    Node* zigZig(Node* root);

    Node* zigZag(Node* root);

    Node* splay(Node* root, const string& title);

    Node* recursiveSplayInsert(Node* root, const string& title, double ign_rating, const vector<string>& genre, const string& platform, const string& user_rating);

    void insertSplay(const string& title, double ign_rating, const vector<string>& genre, const string& platform, const string &user_rating);

    Node* splayRecursiveSearch(Node* root, Node* &found, const string& title);

    Node* splaySearch(const string& title);

    void splayPrintPreorderRecursive(Node* root);

    void splayPrintPreorder();

  };

