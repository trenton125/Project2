//
// Created by trent on 10/23/2025.
//

#include <iostream>
#include <string>
#include "splay.h"
using namespace std;

Splay::Splay() {

  this->root = nullptr;
  this->empty = true;

}

Splay::Node::Node(const string &title, double ign_rating, const vector<string> &genre, const string& platform, const string &user_rating) {

  this->title = title;
  this->ign_rating = ign_rating;
  this->genre = genre;
  this->platform = platform;
  this->user_rating = user_rating;
  this->left = nullptr;
  this->right = nullptr;

}

void Splay::deleteSplay(const Node* root) {

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

Splay::Node *Splay::rightRotation(Node *root) {

  Node* grandchild = root->left->right;
  Node* newParent = root->left;
  newParent->right = root;
  root->left = grandchild;
  return newParent;

}

Splay::Node *Splay::leftRotation(Node *root) {

  Node* grandchild = root->right->left;
  Node* newParent = root->right;
  newParent->left = root;
  root->right = grandchild;
  return newParent;

}

Splay::Node* Splay::splay(Node* root, const string& title) {

  // Zig or Zag?

  if (root->right == nullptr && root->left == nullptr) {
    return root;
  }
  if (root->right != nullptr) {
    if (title == root->right->title) {
      root = leftRotation(root);
      return root;
    }
    if (root->right->left != nullptr) {
      if (title == root->right->left->title) {
        root = rightRotation(root);
        root = leftRotation(root);
        return root;
      }
    }
  }
  else if (root->left != nullptr) {
    if (title == root->left->title) {
      root = rightRotation(root);
      return root;
    }
    if (root->left->right != nullptr) {
      if (title == root->left->right->title) {
        root = leftRotation(root);
        root = rightRotation(root);
        return root;
      }
    }
  }

  return root;

}

Splay::Node *Splay::recursiveSplayInsert(Node *root, const string& title, double ign_rating, const vector<string>& genre, const string& platform, const string& user_rating) {

  // Recursive Insert

  if (root == nullptr) {
    return new Node(title, ign_rating, genre, platform, user_rating);
  }
  if (title < root->title) {
    root->left = recursiveSplayInsert(root->left, title, ign_rating, genre, platform, user_rating);
  }
  else if (title > root->title) {
    root->right = recursiveSplayInsert(root->right, title, ign_rating, genre, platform, user_rating);
  }

  root = splay(root, title);

  return root;

}


void Splay::insertSplay(const string& title, double ign_rating, const vector<string>& genre, const string& platform, const string &user_rating) {

  root = recursiveSplayInsert(root, title, ign_rating, genre, platform, user_rating);

}

Splay::Node *Splay::splayRecursiveSearch(Node* root, Node* &found, const string& title) {
  if (root == nullptr) {
    return root;
  }
  if (stoi(title) < stoi(root->title)) {
    root->left = splayRecursiveSearch(root->left, found, title);
  }
  else if (title > root->title) {
    root->right = splayRecursiveSearch(root->right, found, title);
  }
  else if (title == root->title) {
    found = root;
    return root;
  }
  else {
    found = nullptr;
    return root;
  }

  if (found != nullptr) {
    root = splay(root, title);
    return root;
  }

}

Splay::Node *Splay::splaySearch(const string& title) {

  Node* found;
  root = splayRecursiveSearch(root, found, title);
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





