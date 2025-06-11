#include "bst_tree.h"

#include <stack>

// Destructor — frees memory of the entire tree
bst_tree::~bst_tree() {
    std::stack<node*> nodes;
    if (root != nullptr) nodes.push(root);

    while (!nodes.empty()) {
        node* current = nodes.top();
        nodes.pop();

        if (current->left) nodes.push(current->left);
        if (current->right) nodes.push(current->right);
        delete current;
    }
}

// Insert a value into the tree (thread-safe)
void bst_tree::insert(int64_t data) {
    node** current = &root;
    node* parent = nullptr;

    while (true) {
        if (*current == nullptr) {
            // Lock parent before inserting new child
            if (parent != nullptr) parent->node_mutex.lock();
            *current = new node(data);
            if (parent != nullptr) parent->node_mutex.unlock();
            return;
        }

        node* currentNode = *current;
        std::unique_lock<std::mutex> lock(currentNode->node_mutex);

        parent = currentNode;
        if (data < currentNode->data) {
            current = &currentNode->left;
        } else {
            current = &currentNode->right;
        }
    }
}

// Build a balanced tree from sorted data (used for testing)
void bst_tree::build_from_sorted_data(const std::vector<long int>& values) {
    std::function<node*(int, int)> build = [&](int l, int r) -> node* {
        if (l >= r) return nullptr;

        int mid = l + (r - l) / 2;
        node* n = new node(values[mid]);
        n->left = build(l, mid);
        n->right = build(mid + 1, r);
        return n;
    };

    root = build(0, static_cast<int>(values.size()));
}
