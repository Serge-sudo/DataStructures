#include <iostream>
#include <optional>
using namespace std;

class AVLTree {
    private:
        struct Node {
            int b_factor;
            int value;
            Node * left;
            Node * right;
            int height;
            Node(int val) {
                value = val;
                b_factor = 0;
                left = NULL;
                right = NULL;
                height = 0;
            }
        };

    Node * root;

    int height() {
        return (root == NULL) ? 0 : root -> height;
    }

    bool exists(Node * node, int value) {
        if (node == NULL) return false;

        if (value < node -> value) return exists(node -> left, value);

        else if (value > node -> value) return exists(node -> right, value);

        return true;
    }

    Node * insert(Node * node, int value) {

        if (node == NULL) return new Node(value);

        if (value < node -> value) {
            node -> left = insert(node -> left, value);

        } else {
            node -> right = insert(node -> right, value);
        }

        update(node);

        return balance(node);
    }

    void update(Node * node) {
        int leftTreeHeight = (node -> left == NULL) ? -1 : node -> left -> height;
        int rightTreeHeight = (node -> right == NULL) ? -1 : node -> right -> height;

        node -> height = 1 + std::max(leftTreeHeight, rightTreeHeight);

        node -> b_factor = rightTreeHeight - leftTreeHeight;
    }

    Node * balance(Node * node) {

        if (node -> b_factor == -2) {

            if (node -> left -> b_factor <= 0) {
                return rightRotation(node);

            } else {
                node -> left = leftRotation(node -> left);
                return rightRotation(node);
            }

        } else if (node -> b_factor == 2) {

            if (node -> right -> b_factor >= 0) {
                return leftRotation(node);

            } else {
                node -> right = rightRotation(node -> right);
                return leftRotation(node);
            }
        }

        return node;
    }

    Node * leftRotation(Node * node) {
        Node * newParent = node -> right;
        node -> right = newParent -> left;
        newParent -> left = node;
        update(node);
        update(newParent);
        return newParent;
    }

    Node * rightRotation(Node * node) {
        Node * newParent = node -> left;
        node -> left = newParent -> right;
        newParent -> right = node;
        update(node);
        update(newParent);
        return newParent;
    }

    void destroyRecursive(Node * node) {
        if (node) {
            destroyRecursive(node -> left);
            destroyRecursive(node -> right);
            delete node;
        }
    }

    Node * remove(Node * node, int elem) {
        if (node == NULL) return NULL;

        if (elem < node -> value) {
            node -> left = remove(node -> left, elem);

        } else if (elem > node -> value) {
            node -> right = remove(node -> right, elem);

        } else {
            if(node -> left == NULL && node -> right == NULL){
                return NULL;
            }
            else if (node -> left == NULL) {
                return node -> right;

            } else if (node -> right == NULL) {
                return node -> left;

            } else {

                    int exchangeval = findMin(node -> right);
                    node -> value = exchangeval;
                    node -> right = remove(node -> right, exchangeval);

            }
        }

        update(node);

        return balance(node);
    }

    int findMin(Node * node) {
        while (node -> left != NULL) node = node -> left;
        return node -> value;
    }



    Node * next(int x, Node * t) {
        
        if (t == NULL) return NULL;

        if (t -> value > x) {
                Node* node = next(x,t->left);
                return node != NULL ? node: t;
        } else
                return next(x,t->right);

    }

    Node * prev(int x, Node * t) {
        
        if (t == NULL) return NULL;

        if (t -> value < x) {
                Node* node = prev(x,t->right);
                return node != NULL ? node: t;
        } else
                return prev(x,t->left);

    }






    public:
        bool exists(int value) {
            return exists(root, value);
        }

    bool insert(int value) {
        if (!exists(root, value)) {

            root = insert(root, value);
            return true;
        }
        return false;
    }

    bool remove(int elem) {

        if (exists(root, elem)) {
            root = remove(root, elem);
            return true;
        }

        return false;
    }

    std::optional<int> next(int x) {
        Node * tmp = next(x, root);
        if(tmp)
            return tmp->value;
        else
            return std::nullopt;
    }
    std::optional<int> prev(int x) {
        Node * tmp = prev(x, root);
        if(tmp)
            return tmp->value;
        else
            return std::nullopt;
    }

    AVLTree() {
        root = NULL;
    }
    ~AVLTree() {
        destroyRecursive(root);
    }

};
int main() {
    AVLTree t;
    string s;
    int num;
    while (cin >> s) {
        cin >> num;
        if (s == "insert") {
            t.insert(num);
        } else if (s == "exists") {
            if (t.exists(num))
                cout << "true" << endl;
            else
                cout << "false" << endl;
        } else if (s == "delete") {
            t.remove(num);
        } else if (s == "next") {
           auto x = t.next(num);
           
            if (x.has_value())
                cout << * x << endl;
            else
                cout << "none" << endl;

        } else if (s == "prev") {
            std::optional<int> x = t.prev(num);
            // cout << *x;
            if (x.has_value())
                cout << * x << endl;
            else
                cout << "none" << endl;
        }
    }
}