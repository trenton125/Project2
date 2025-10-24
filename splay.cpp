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

Splay::Node::Node(string title, string release_year, string ign_rating, string genre, vector<string> platforms, string user_rating) {

  this->title = title;
  this->release_year = release_year;
  this->ign_rating = ign_rating;
  this->genre = genre;
  this->user_rating = user_rating;
  this->left = nullptr;
  this->right = nullptr;

}






