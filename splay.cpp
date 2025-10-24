//
// Created by trent on 10/23/2025.
//

#include <iostream>
#include "splay.h"
using namespace std;

Splay::Splay() {

  this->root = nullptr;
  this->empty = true;

}

Splay::Node::Node(string title, double ign_rating, vector<string> genre, string platform, string user_rating) {

  this->title = title;
  this->ign_rating = ign_rating;
  this->genre = genre;
  this->user_rating = user_rating;
  this->left = nullptr;
  this->right = nullptr;

}

void Splay::deleteSplay(Node* root) {

  if (empty == false) {
    if (root != nullptr) {
      deleteSplay(root->left);
      deleteSplay(root->right);
      delete root;
    }
  }
}

Splay::~Splay() {

  deleteSplay(root);

}

Splay::Node *Splay::recursiveSplayInsert(Node *root, string title, double ign_rating, vector<string> genre, string platform, string user_rating) {

  // Recursive Insert

  if (root == nullptr) {
    empty == false;
    return new Node(title, ign_rating, genre, platform, user_rating);
  }
  else if (title < root->title) {
    root->left = recursiveSplayInsert(root, title, ign_rating, genre, platform, user_rating);
  }
  else if (title > root->title) {
    root->right = recursiveSplayInsert(root, title, ign_rating, genre, platform, user_rating);
  }

  // Moving inserted node to root through splay operations

  // SPLAY BALANCING CODE HERE

}


void Splay::insertSplay(string title, double ign_rating, vector<string> genre, string platform, string user_rating) {

  root = recursiveSplayInsert(root, title, ign_rating, genre, platform, user_rating);

}

Splay::Node* Splay::splaySearch(string title) {

  Node* temp = root;
  if (empty == true) {
    return nullptr;
  }

  while (temp != nullptr) {
    if (title < temp->title) {
      temp = temp->left;
    }
    else if (title > temp->title) {
      temp = temp->right;
    }
    else if (title == temp->title) {
      return temp;
    }
  }

}

void Splay::splayPrintInorderRecursive(Node* root) {

  // For testing

  if (root == nullptr) {
    return;
  }
  else {
    splayPrintInorderRecursive(root->left);
    cout << root->title << " ";
    splayPrintInorderRecursive(root->right);
  }

}

void Splay::splayPrintInorder() {
  splayPrintInorderRecursive(root);
}





