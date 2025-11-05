//
// Created by trent on 10/23/2025.
//

#include <iostream>
#include <string>
#include <chrono>
#include "splay.h"
using namespace std;

Splay::Splay() {

  this->root = nullptr;
  this->empty = true;
  this->lastSearchTime = 0.0;

}

Splay::Node::Node(const string &title, double ign_rating, const vector<string> &genre, const string& platform) {

  this->title = title;
  this->ign_rating = ign_rating;
  this->genre = genre;
  this->platform = platform;
  this->user_rating = "-1";
  this->left = nullptr;
  this->right = nullptr;

}

void Splay::deleteSplay(const Node* root) {

  if (root == nullptr) {
    return;
  }
  deleteSplay(root->left);
  deleteSplay(root->right);
  delete root;
}

Splay::~Splay() {

  deleteSplay(root);
  root = nullptr;

}

Splay::Node *Splay::rightRotation(Node *root) {

  //Modified code from lecture slides

  if (root == nullptr || root->left == nullptr) {
    return root;
  }

  Node* grandchild = root->left->right;
  Node* newParent = root->left;
  newParent->right = root;
  root->left = grandchild;
  return newParent;

}

Splay::Node *Splay::leftRotation(Node *root) {

  //Modified code for lecture slides

  if (root == nullptr || root->right == nullptr) {
    return root;
  }

  Node* grandchild = root->right->left;
  Node* newParent = root->right;
  newParent->left = root;
  root->right = grandchild;
  return newParent;

}

Splay::Node* Splay::splay(Node* root, const string& title) {

  //What rotation is needed?

  //Modified code from AVL Tree Project but updated for splay

  if (root == nullptr || root->title == title) {
    return root;
  }

  if (title > root->title) {
    if (root->right == nullptr) {
      return root;
    }
    if (title < root->right->title) {
      root->right->left = splay(root->right->left, title);
      if (root->right->left != nullptr) {
        root->right = rightRotation(root->right);
      }
    }
    else if (title > root->right->title) {
      root->right->right = splay(root->right->right, title);
      root = leftRotation(root);
    }
    if (root->right == nullptr) {
      return root;
    }
    else {
      return leftRotation(root);
    }
  }

  else {
    if (root->left == nullptr) {
      return root;
    }
    if (title < root->left->title) {
      root->left->left = splay(root->left->left, title);
      root = rightRotation(root);
    }

    else if (title > root->left->title) {
      root->left->right = splay(root->left->right, title);
      if (root->left->right != nullptr) {
        root->left = leftRotation(root->left);
      }
    }
    if (root->left == nullptr) {
      return root;
    }
    else {
      return rightRotation(root);
    }
  }

}

Splay::Node *Splay::SplayInsertActual(Node *root, const string& title, double ign_rating, const vector<string>& genre, const string& platform) {

  // Insert Node if Empty

  if (root == nullptr) {
    return new Node(title, ign_rating, genre, platform);
  }

  //Splay the Tree

  root = splay(root, title);

  //New Game

  Node* newGame = new Node(title, ign_rating, genre, platform);

  //Replace root with newGame

  if (title > root->title) {

    newGame->left = root;
    newGame->right = root->right;
    root->right = nullptr;

  }
  else {

    newGame->right = root;
    newGame->left = root->left;
    root->left = nullptr;

  }

  //root = newGame

  return newGame;

}


void Splay::insertSplay(const string& title, double ign_rating, const vector<string>& genre, const string& platform) {

  root = SplayInsertActual(root, title, ign_rating, genre, platform);

}

Splay::Node *Splay::splaySearchActual(Node* root, const string& title) {

  root = splay(root, title);
  if (root != nullptr) {
    if (root->title == title) {
      return root;
    }
  }

  //Not found

  return nullptr;

}

Splay::Node *Splay::splaySearch(const string& title) {

  //Start Clock

  auto start = chrono::high_resolution_clock::now();

  Node* found = splaySearchActual(root, title);

  //End Clock

  auto end = chrono::high_resolution_clock::now();

  chrono::duration<double> elapsed_time = end - start;
  lastSearchTime = elapsed_time.count();
  splayPrintNode(found, elapsed_time);
  return found;

}

void Splay::splayPrintPreorderRecursive(const Node* root) {

  // For testing

  if (root == nullptr) {
    return;
  }
  else {
    cout << root->title << " ";
    splayPrintPreorderRecursive(root->left);
    splayPrintPreorderRecursive(root->right);
  }

}

void Splay::splayPrintPreorder() const {
  splayPrintPreorderRecursive(root);
}

void Splay::splayPrintNode(Node* node, chrono::duration<double> elapsed_time) {

  //For Testing

  if (node == nullptr) {
    cout << "Game not Found!" << endl;
    return;
  }
  cout << node->title << endl;
  cout << node->ign_rating << endl;
  for (const auto & i : node->genre) {
    cout << i << " ";
  }
  cout << endl;
  cout << node->platform << endl;
  cout << endl;
  cout << "Search Time: " << elapsed_time.count() << " " << "seconds" << endl;
}