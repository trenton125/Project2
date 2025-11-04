#ifndef HEAP_H
#define HEAP_H
#include <iostream>
#include <vector>
#include <string>
#include <initializer_list>
using namespace std;

struct Game {
    string title;
    int releaseYear;
    double rating;
    vector<string> genres;
    string platforms;
    string userRating;

    Game() {}

    Game(string t, int y, double r, vector<string> g, string p, string ur) {
        title = t;
        releaseYear = y;
        rating = r;
        genres = g;
        platforms = p;
        userRating = ur;
    }
};

class MaxHeap {
private:
    vector<Game*> heap;

    int parentIndex(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }

    void heapifyUp(int i);
    void heapifyDown(int i);

public:

    MaxHeap() {}
    ~MaxHeap();

    void insertHeap(const string &title, int year, double rating,
                    const vector<string> &genres, const string &platforms, const string &userRating);

    bool search(const string &title) const;
    Game* searchGame(const string &title) const;
    void display() const;
    Game* getMax() const;

    int size() const { return heap.size(); }
    bool empty() const { return heap.empty(); }
};

#endif
