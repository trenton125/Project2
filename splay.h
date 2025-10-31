//
// Created by trent on 10/23/2025.
//

#pragma once
#include <iostream>
#include <vector>
#include <chrono>
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

      Node(const string &title, double ign_rating, const vector<string> &genre, const string& platform);

    };

  private:

    Node* root;
    bool empty;
    double lastSearchTime;

  public:

    // Splay functions

    Splay();

    void deleteSplay(const Node* root);

    ~Splay();

    static Node* rightRotation(Node* root);

    static Node* leftRotation(Node* root);

    static Node* splay(Node* root, const string& title);

    static Node* recursiveSplayInsert(Node* root, const string& title, double ign_rating, const vector<string>& genre, const string& platform);

    void insertSplay(const string& title, double ign_rating, const vector<string>& genre, const string& platform);

    static Node* splayRecursiveSearch(Node* root, Node* &found, const string& title);

    Node* splaySearch(const string& title);

    static void splayPrintPreorderRecursive(const Node* root);

    void splayPrintPreorder() const;

    static void splayPrintNode(Node* node, chrono::duration<double> elapsed_time);

    double getLastSearchTime() const { return lastSearchTime; }

  };

