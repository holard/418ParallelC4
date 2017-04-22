
#include "board.h"

class Node {
    public:
    Node* next;
    Board data;
    int score;
    Node() {
        next = NULL;
    }
};

/*
 * Thread-safe hash table for Board objects
 */
class BoardMap {

    private:
        int width;
        Node** bins;
    public:
        BoardMap(int num_bins) {
            width = num_bins;
            bins = new Node*[num_bins];
            for (int i = 0; i < num_bins; i++) {
                bins[i] = new Node();
            }
        }

        int count(Board& b) {
            int bin = b.hash() % width;
            Node* curr = bins[bin];
            while (curr->next != NULL) {
                curr = curr->next;
                if (curr->data == b) {
                    return 1;
                }
            }
            return 0;
        }

        int get(Board& key) {
            int bin = key.hash() % width;
            Node* curr = bins[bin];
            while (curr->next != NULL) {
                curr = curr->next;
                if (curr->data == key) {
                    return curr->score;
                }
            }
            return 0;
        }

        void put(Board& key, int score) {
            Node* n = new Node();
            n->data = key;
            n->score = score;
            int bin = key.hash() % width;
            Node* curr = bins[bin];
            while (true) {
                if (curr->next == NULL) {
                    if (__sync_bool_compare_and_swap (&(curr->next), NULL, n)) {
                        return;
                    }
                }
                curr = curr->next;
                if (curr->data == key) {
                    delete n;
                    return;
                }
            }
        }
};

