#ifndef AVL_DEVICE_DATABASE_H
#define AVL_DEVICE_DATABASE_H

#include "Models.h"
#include <string>
#include <vector>

// ---------------------------------------------------------
// AVLDeviceDatabase
//
// A self-balancing Binary Search Tree (AVL Tree), keyed by
// deviceId, storing NetworkDevice records (BST & AVL Trees
// requirement - "Store telecom databases").
//
// Why an AVL tree over a plain BST?
//   A plain BST degrades to O(n) search/insert in the worst
//   case (e.g. devices added in sorted ID order, which is
//   common when IDs are allocated sequentially as new towers
//   are commissioned). An AVL tree rebalances itself via
//   rotations after every insertion, GUARANTEEING O(log n)
//   height and therefore O(log n) search/insert/delete even
//   in this worst case - critical for a telecom database that
//   is queried constantly for live network status.
// ---------------------------------------------------------
class AVLDeviceDatabase {
private:
    struct Node {
        NetworkDevice data;
        Node* left;
        Node* right;
        int height;
        Node(const NetworkDevice& d) : data(d), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int height(Node* n) const { return n ? n->height : 0; }
    int balanceFactor(Node* n) const { return n ? height(n->left) - height(n->right) : 0; }
    void updateHeight(Node* n) { n->height = 1 + std::max(height(n->left), height(n->right)); }

    // Standard AVL rotations
    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);

    Node* insert(Node* node, const NetworkDevice& d);
    Node* search(Node* node, const std::string& deviceId) const;
    void inorder(Node* node, std::vector<NetworkDevice>& out) const;
    void destroy(Node* node);

public:
    AVLDeviceDatabase() : root(nullptr) {}
    ~AVLDeviceDatabase() { destroy(root); }

    // Insert a device record - O(log n), tree stays balanced via rotations
    void insertDevice(const NetworkDevice& d);

    // Search for a device by ID - O(log n)
    const NetworkDevice* findDevice(const std::string& deviceId) const;

    // Return all devices in sorted order by deviceId (in-order traversal) - O(n)
    std::vector<NetworkDevice> allDevicesSorted() const;

    int treeHeight() const { return height(root); }
};

#endif // AVL_DEVICE_DATABASE_H
