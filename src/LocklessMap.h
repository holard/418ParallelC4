
#include <string>

class Node {
    public:
    Node* next;
    std::string data;
    int score;
    Node() {
        next = NULL;
    }
};

/*
 * Thread-safe hash table for Board objects
 */
class LocklessMap {

    private:
        int width;
        Node** bins;
        std::hash<std::string> hash;
    public:
        LocklessMap(int num_bins) {
            width = num_bins;
            bins = new Node*[num_bins];
            for (int i = 0; i < num_bins; i++) {
                bins[i] = new Node;
            }
        }

        int count(std::string& b) {
            int bin = hash(b) % width;
            Node* curr = bins[bin];
            while (curr->next != NULL) {
                curr = curr->next;
                if (curr->data == b) {
                    return 1;
                }
            }
            return 0;
        }

        int get(std::string& key) {
            int bin = hash(key) % width;
            Node* curr = bins[bin];
            while (curr->next != NULL) {
                curr = curr->next;
                if (curr->data == key) {
                    return curr->score;
                }
            }
            return 0;
        }

        bool put(std::string& key, int score) {
            Node* n = new Node();
            n->data = key;
            n->score = score;
            int bin = hash(key) % width;
            Node* curr = bins[bin];
            while (true) {
                if (curr->next == NULL) {
                    if (__sync_bool_compare_and_swap (&(curr->next), NULL, n)) {
                        return true;
                    }
                }
                curr = curr->next;
                if (curr->data == key) {
                    delete n;
                    return false;
                }
            }
        }

        bool put_seq(std::string& key, int score) {
            int bin = hash(key) % width;
            Node* curr = bins[bin];
            while (true) {
                if (curr->next == NULL) {
                    Node* n = new Node();
                    n->data = key;
                    n->score = score;
                    curr->next = n;
                    return true;
                }
                curr = curr->next;
                if (curr->data == key) {
                    return false;
                }
            }
        }
};

