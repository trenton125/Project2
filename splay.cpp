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

  if (root != nullptr) {
    deleteSplay(root->left);
    deleteSplay(root->right);
    delete root;
  }
  else {
    return;
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

  if (root == nullptr || root->title == title) {
    return root;
  }
  if (title < root->title) {
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

  else {
    if (root->right == nullptr) {
      return root;
    }
    if (title > root->right->title) {
      root->right->right = splay(root->right->right, title);
      root = leftRotation(root);
    }
    else if (title < root->right->title) {
      root->right->left = splay(root->right->left, title);
      if (root->right->left != nullptr) {
        root->right = rightRotation(root->right);
      }
    }
    if (root->right == nullptr) {
      return root;
    }
    else {
      return leftRotation(root);
    }
  }

  // if (root->right != nullptr) {
  //   if (title == root->right->title) {
  //     root = leftRotation(root);
  //     return root;
  //   }
  //   if (root->right->left != nullptr) {
  //     if (title == root->right->left->title) {
  //       root = rightRotation(root);
  //       root = leftRotation(root);
  //       return root;
  //     }
  //   }
  // }
  // else if (root->left != nullptr) {
  //   if (title == root->left->title) {
  //     root = rightRotation(root);
  //     return root;
  //   }
  //   if (root->left->right != nullptr) {
  //     if (title == root->left->right->title) {
  //       root = leftRotation(root);
  //       root = rightRotation(root);
  //       return root;
  //     }
  //   }
  // }

  return root;

}

Splay::Node *Splay::recursiveSplayInsert(Node *root, const string& title, double ign_rating, const vector<string>& genre, const string& platform) {

  // Recursive Insert

  if (root == nullptr) {
    return new Node(title, ign_rating, genre, platform);
  }
  if (title < root->title) {
    root->left = recursiveSplayInsert(root->left, title, ign_rating, genre, platform);
  }
  else if (title > root->title) {
    root->right = recursiveSplayInsert(root->right, title, ign_rating, genre, platform);
  }

  root = splay(root, title);

  return root;

}


void Splay::insertSplay(const string& title, double ign_rating, const vector<string>& genre, const string& platform) {

  root = recursiveSplayInsert(root, title, ign_rating, genre, platform);

}

Splay::Node *Splay::splayRecursiveSearch(Node* root, Node* &found, const string& title) {
  if (root == nullptr) {
    return root;
  }
  if (title < root->title) {
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

  return root;

}

Splay::Node *Splay::splaySearch(const string& title) {

  Node* found;
  auto start = chrono::high_resolution_clock::now();
  root = splayRecursiveSearch(root, found, title);
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