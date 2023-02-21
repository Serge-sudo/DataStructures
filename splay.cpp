#include <iostream>

using namespace std;
#define MOD static_cast < int > (1e9)

class SplayTree {
    private:
        struct Node {
            int value;
            long long sum;
            Node * left, * right, * parent;
            Node() {
                sum = 0;
                left = right = parent = NULL;
            }
            Node(int value, long long sum) {
                this -> value = value;
                this -> sum = sum;
                left = right = parent = NULL;

            }

        };

    void update(Node * v) {
        if (v == NULL) return;
        v -> sum = static_cast<long long > (v -> value + (v -> left ? v -> left -> sum : 0) + (v -> right ? v -> right -> sum : 0));
        if (v -> left != NULL) {
            v -> left -> parent = v;
        }
        if (v -> right != NULL) {
            v -> right -> parent = v;
        }
    }

    void grannyFix(Node * grand, Node * parent, Node * t) {
        t -> parent = grand;
        if (grand == NULL) return;
        if (grand -> left == parent) grand -> left = t;
        else grand -> right = t;
    }

    void zag(Node * t) {
        Node * parent = t -> parent;
        Node * grand = parent -> parent;
        Node * tmp = t -> left;
        t -> left = parent;
        parent -> right = tmp;
        update(parent);
        update(t);
        grannyFix(grand, parent, t);
    }

    void zig(Node * t) {

        Node * parent = t -> parent;
        Node * grand = parent -> parent;
        Node * tmp = t -> right;
        t -> right = parent;
        parent -> left = tmp;
        update(parent);
        update(t);
        grannyFix(grand, parent, t);

    }

    void zig_zig(Node * t, Node * parent) {
        zig(parent);
        zig(t);
    }
    void zag_zag(Node * t, Node * parent) {
        zag(parent);
        zag(t);
    }
    void zig_zag(Node * t, Node * parent) {
        zag(t);
        zig(t);
    }
    void zag_zig(Node * t, Node * parent) {
        zig(t);
        zag(t);
    }

    Node * splay(Node * v) {
        if (v == NULL) return NULL;
        while (v -> parent != NULL) {

            if (v -> parent -> parent == NULL) {
                Node * parent = v -> parent;
                if (parent == NULL) break;
                if (parent -> left == v) zig(v);
                else zag(v);
                break;
            }
            Node * parent = v -> parent;
            Node * granny = parent -> parent;
            if (parent -> left == v && granny -> left == parent) zig_zig(v,parent);
            else if (parent -> right == v && granny -> right == parent) zag_zag(v, parent);
            else if (parent -> right == v && granny -> left == parent) zig_zag(v, parent);
            else zag_zag(v, parent);
        }
        return v;
    }

    pair < Node * , Node * > find(Node * root, int value) {
        pair < Node * , Node * > res;
        Node * curr = root, * prev = root;
        while (curr) {
            prev = curr;
            if ((curr -> value >= value && res.first == NULL) || (curr -> value >= value && curr -> value < res.first -> value)) res.first = curr;
            if (curr -> value == value) break;
            if (curr -> value > value) curr = curr -> left;
            else curr = curr -> right;
        }
        root = splay(prev);
        res.second = root;
        return res;
    }

    pair < Node * , Node * > split(Node * root, int val) {
        pair < Node * , Node * > result;
        pair < Node * , Node * > div = find(root, val);
        root = div.second;
        if (div.first == NULL) {
            return make_pair(root, nullptr);
        }
        div.first = splay(div.first);
        result.first = div.first -> left;
        div.first -> left = NULL;
        result.second = div.first;
        if (result.first != NULL) result.first -> parent = NULL;
        update(result.first);
        update(result.second);
        return result;
    }

    Node * merge(Node * tree1, Node * tree2) {
        if (tree1 == NULL) return tree2;
        if (tree2 == NULL) return tree1;
        while (tree2 -> left) tree2 = tree2 -> left;
        tree2 = splay(tree2);
        tree2 -> left = tree1;
        update(tree2);
        return tree2;
    }

    void destroyRecursive(Node * node) {
        if (node) {
            destroyRecursive(node -> left);
            destroyRecursive(node -> right);
            delete node;
        }
    }

    Node * root;

    public:

        SplayTree() {
            root = NULL;
        }~SplayTree() {
            destroyRecursive(root);
        }

    void insert(int x) {
        Node * new_root = NULL;
        pair < Node * , Node * > gap = split(root, x);
        if (!gap.second || gap.second -> value != x) new_root = new Node(x, x);
        root = merge(merge(gap.first, new_root), gap.second);
    }

    long long sum(int from, int to) {
        pair < Node * , Node * > div1 = split(root, from);
        Node * part1 = div1.first;
        Node * part2 = div1.second;
        pair < Node * , Node * > div2 = split(part2, to + 1);
        part2 = div2.first;
        Node * part3 = div2.second;
        long long result = static_cast<long long> (part2 != NULL ? part2 -> sum : 0);
        part2 = merge(part1, part2);
        root = merge(part2, part3);
        return result;
    }

};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n, start, end;
    long long result = 0;
    cin >> n;
    SplayTree tree;
    string current = "", previous = "";
    for (int i = 0; i < n; ++i) {
        cin >> current >> start;
        if (current == "+") {
            if (previous == "?") tree.insert((result + start) % MOD);
            else tree.insert(start);
        } else {
            cin >> end;
            result = tree.sum(start, end);
            cout << result << endl;
        }
        previous = current;
    }

    return 0;
}