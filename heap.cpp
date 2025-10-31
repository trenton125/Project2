#include "heap.h"
#include <iomanip>

void MaxHeap::heapifyUp(int i) {
    while (i > 0 && heap[parentIndex(i)].title < heap[i].title) {
        swap(heap[i], heap[parentIndex(i)]);
        i = parentIndex(i);
    }
}

void MaxHeap::heapifyDown(int i) {
    int largest = i;
    int l = leftChild(i);
    int r = rightChild(i);

    if (l < static_cast<int>(heap.size()) && heap[l].title > heap[largest].title)
        largest = l;
    if (r < static_cast<int>(heap.size()) && heap[r].title > heap[largest].title)
        largest = r;

    if (largest != i) {
        swap(heap[i], heap[largest]);
        heapifyDown(largest);
    }
}

void MaxHeap::insertHeap(const string &title, int year, double rating,
                         const vector<string> &genres, const string &platforms, const string &userRating) {
    Game g(title, year, rating, genres, platforms, userRating);
    heap.push_back(g);
    heapifyUp(static_cast<int>(heap.size()) - 1);
}

bool MaxHeap::search(const string &title) const {
    for (const auto &g : heap) {
        if (g.title == title) {
            cout << "Game found:\n";
            cout << " Title: " << g.title << "\n";
            cout << " Year: " << g.releaseYear << "\n";
            cout << " Rating: " << g.rating << "\n";
            cout << " Genres: ";
            for (const auto &genre : g.genres) cout << genre << " ";
            cout << "\n Platforms: " << g.platforms;
            cout << "\n User Rating: " << g.userRating << "\n";
            return true;
        }
    }
    cout << "Game not found.\n";
    return false;
}
Game* MaxHeap::searchGame(const string &title) {
    for (auto &g : heap) {
        if (g.title == title) {
            return &g;
        }
    }
    return nullptr;
}

void MaxHeap::display() const {
    cout << std::left << setw(30) << "Title"
         << setw(8) << "Year"
         << setw(8) << "Rating"
         << setw(30) << "Genres"
         << setw(15) << "Platforms"
         << setw(10) << "UserRt" << endl;

    cout << string(100, '-') << endl;

    for (const auto &g : heap) {
        string genreStr;
        for (const auto &genre : g.genres) {
            if (!genreStr.empty()) genreStr += ", ";
            genreStr += genre;
        }

        cout << std::left << setw(30) << g.title
             << setw(8) << g.releaseYear
             << setw(8) << g.rating
             << setw(30) << genreStr
             << setw(15) << g.platforms
             << setw(10) << g.userRating << endl;
    }
}

Game MaxHeap::getMax() const {
    if (heap.empty()) throw runtime_error("Heap is empty");
    return heap[0];
}
