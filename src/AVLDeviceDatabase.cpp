#include "AVLDeviceDatabase.h"
#include <algorithm>

AVLDeviceDatabase::Node* AVLDeviceDatabase::rotateRight(Node* y) {
    Node* x = y->left;
    Node* t2 = x->right;

    x->right = y;
    y->left = t2;

    updateHeight(y);
    updateHeight(x);
    return x;
}

AVLDeviceDatabase::Node* AVLDeviceDatabase::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* t2 = y->left;

    y->left = x;
    x->right = t2;

    updateHeight(x);
    updateHeight(y);
    return y;
}

AVLDeviceDatabase::Node* AVLDeviceDatabase::insert(Node* node, const NetworkDevice& d) {
    if (!node) return new Node(d);

    if (d.deviceId < node->data.deviceId)
        node->left = insert(node->left, d);
    else if (d.deviceId > node->data.deviceId)
        node->right = insert(node->right, d);
    else {
        node->data = d; // update existing device record
        return node;
    }

    updateHeight(node);
    int balance = balanceFactor(node);

    // Left Left case
    if (balance > 1 && d.deviceId < node->left->data.deviceId)
        return rotateRight(node);

    // Right Right case
    if (balance < -1 && d.deviceId > node->right->data.deviceId)
        return rotateLeft(node);

    // Left Right case
    if (balance > 1 && d.deviceId > node->left->data.deviceId) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left case
    if (balance < -1 && d.deviceId < node->right->data.deviceId) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

AVLDeviceDatabase::Node* AVLDeviceDatabase::search(Node* node, const std::string& deviceId) const {
    if (!node) return nullptr;
    if (deviceId == node->data.deviceId) return node;
    if (deviceId < node->data.deviceId) return search(node->left, deviceId);
    return search(node->right, deviceId);
}

void AVLDeviceDatabase::inorder(Node* node, std::vector<NetworkDevice>& out) const {
    if (!node) return;
    inorder(node->left, out);
    out.push_back(node->data);
    inorder(node->right, out);
}

void AVLDeviceDatabase::destroy(Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

void AVLDeviceDatabase::insertDevice(const NetworkDevice& d) {
    root = insert(root, d);
}

const NetworkDevice* AVLDeviceDatabase::findDevice(const std::string& deviceId) const {
    Node* n = search(root, deviceId);
    return n ? &n->data : nullptr;
}

std::vector<NetworkDevice> AVLDeviceDatabase::allDevicesSorted() const {
    std::vector<NetworkDevice> out;
    inorder(root, out);
    return out;
}
