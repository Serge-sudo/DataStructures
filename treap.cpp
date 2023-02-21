#include <bits/stdc++.h>

using namespace std;

class BestTreap {
public:
  struct Node {
    long long data;
    long long leftest;
    long long rightest;
    long long Pass_Val;
    long long sum;
    long long size;
    long long priority;
    bool PassZero;
    bool isIncreasing;
    bool isDecreasing;
    bool PassReverse;
    Node *left;
    Node *right;
    Node(long long d)
        : data(d), leftest(d), rightest(d), Pass_Val(0), sum(d), size(1),
          priority(rand()), PassZero(0), isIncreasing(1), isDecreasing(1),
          PassReverse(0), left(0), right(0) {}
    Node(long long dt, long long leftest, long long rightest,
         long long Pass_Val, long long sum, long long size, long long priority,
         bool z, bool i, bool d, bool re, Node *l, Node *r)
        : data(dt), leftest(leftest), rightest(rightest), Pass_Val(Pass_Val),
          sum(sum), size(size), priority(priority), PassZero(z),
          isIncreasing(i), isDecreasing(d), PassReverse(re), left(l), right(r) {
    }
  };
  Node *root;

  long long size(Node *node) { return node ? node->size : 0; }

  void update(Node *node) {
    if (!node)
      return;
    just_push(node->left);
    just_push(node->right);
    node->size = (node->left ? node->left->size : 0) +
                 (node->right ? node->right->size : 0) + 1;
    node->sum = (node->left ? node->left->sum : 0LL) +
                (node->right ? node->right->sum : 0LL) + node->data;
    node->leftest = (!node->left ? node->data : node->left->leftest);
    node->rightest = (!node->right ? node->data : node->right->rightest);
    node->isIncreasing = (node->left ? node->left->isIncreasing &&
                                           (node->left->rightest <= node->data)
                                     : true) &&
                         (node->right ? node->right->isIncreasing &&
                                            (node->right->leftest >= node->data)
                                      : true);
    node->isDecreasing = (node->left ? node->left->isDecreasing &&
                                           (node->left->rightest >= node->data)
                                     : true) &&
                         (node->right ? node->right->isDecreasing &&
                                            (node->right->leftest <= node->data)
                                      : true);
  }

  Node *merge(Node *l, Node *r) {
    just_push(l);
    just_push(r);
    if (!l)
      return r;
    if (!r)
      return l;
    Node *res = NULL;
    if (l->priority > r->priority) {
      res = new Node(l->data, l->leftest, l->rightest, l->Pass_Val, l->sum,
                     l->size, l->priority, l->PassZero, l->isIncreasing,
                     l->isDecreasing, l->PassReverse, l->left,
                     merge(l->right, r));
      delete l;
    } else {
      res = new Node(r->data, r->leftest, r->rightest, r->Pass_Val, r->sum,
                     r->size, r->priority, r->PassZero, r->isIncreasing,
                     r->isDecreasing, r->PassReverse, merge(l, r->left),
                     r->right);
      delete r;
    }
    update(res);
    return res;
  }

  std::pair<Node *, Node *> split(Node *node, long long indx) {
    if (!node)
      return {NULL, NULL};
    just_push(node);
    long long pos = (node->left ? node->left->size : 0) + 1;
    Node *l = NULL;
    Node *r = NULL;
    pair<Node *, Node *> smth;
    if (indx < pos) {
      if (!node->left)
        l = NULL;
      else
        smth = split(node->left, indx);
      l = smth.first;
      r = new Node(node->data, node->leftest, node->rightest, node->Pass_Val,
                   node->sum, node->size, node->priority, node->PassZero,
                   node->isIncreasing, node->isDecreasing, node->PassReverse,
                   smth.second, node->right);
      delete node;
      update(r);
    } else {
      if (!node->right)
        r = NULL;
      else
        smth = split(node->right, indx - pos);
      r = smth.second;
      l = new Node(node->data, node->leftest, node->rightest, node->Pass_Val,
                   node->sum, node->size, node->priority, node->PassZero,
                   node->isIncreasing, node->isDecreasing, node->PassReverse,
                   node->left, smth.first);
      delete node;
      update(l);
    }
    return make_pair(l, r);
  }

  void insert(long long pos, long long val) {
    auto tmp = split(root, pos);
    Node *node = new Node(val);
    root = merge(merge(tmp.first, node), tmp.second);
  }

  void print(Node *node, string &str) {
    if (!node)
      return;
    just_push(node);
    if (node) {
      print(node->left, str);
      str += to_string(node->data) + " ";
      print(node->right, str);
    }
  }

  void Next_Permut(long long l, long long r) {
    auto tmp = split(root, r + 1);

    auto tmp2 = split(tmp.first, l);
    // if(tmp.second)
    tmp2.second = Next_Permut(tmp2.second);

    root = merge(merge(tmp2.first, tmp2.second), tmp.second);
  }
  void Prev_Permut(long long l, long long r) {
    auto tmp = split(root, r + 1);
    auto tmp2 = split(tmp.first, l);
    // if(tmp.second)
    tmp2.second = Prev_Permut(tmp2.second);
    root = merge(merge(tmp2.first, tmp2.second), tmp.second);
  }
  void print() {
    string st = "";
    print(root, st);
    if (st != "")
      cout << st << endl;
  }
  void remove(long long pos) {
    auto tmp = split(root, pos);
    auto tmp2 = split(tmp.second, 1);
    if (tmp2.first)
      delete tmp2.first;
    root = merge(tmp.first, merge(nullptr, tmp2.second));
  }

  long long get_sum(long long l, long long r) {
    auto tmp = split(root, r + 1);
    auto tmp2 = split(tmp.first, l);
    long long s = 0;
    if (tmp2.second)
      s = tmp2.second->sum;
    root = merge(merge(tmp2.first, tmp2.second), tmp.second);
    return s;
  }
  void rev(long long l, long long r) {
    auto tmp = split(root, r + 1);
    auto tmp2 = split(tmp.first, l);
    tmp2.second->PassReverse = true;
    just_push(tmp2.second);
    root = merge(merge(tmp2.first, tmp2.second), tmp.second);
  }

  void add_flag(long long l, long long r, long long x) {
    auto tmp = split(root, r + 1);
    auto tmp2 = split(tmp.first, l);
    if (tmp2.second) {
      tmp2.second->Pass_Val += x;
      tmp2.second->data += x;
      tmp2.second->rightest += x;
      tmp2.second->leftest += x;
      tmp2.second->sum += size(tmp2.second) * x;
    }
    root = merge(merge(tmp2.first, tmp2.second), tmp.second);
  }

  void set_flag(long long l, long long r, long long x) {
    auto tmp = split(root, r + 1);
    auto tmp2 = split(tmp.first, l);
    if (tmp2.second) {
      tmp2.second->PassZero = true;
      tmp2.second->isDecreasing = tmp2.second->isIncreasing = 1;
      tmp2.second->Pass_Val = x;
      tmp2.second->data = x;
      tmp2.second->rightest = x;
      tmp2.second->leftest = x;
      tmp2.second->sum = size(tmp2.second) * x;
    }
    root = merge(merge(tmp2.first, tmp2.second), tmp.second);
  }

  void push_add(Node *node) {
    if (!node)
      return;
    if (node->left) {
      node->left->data += node->Pass_Val;
      node->left->leftest += node->Pass_Val;
      node->left->rightest += node->Pass_Val;
      node->left->sum += node->Pass_Val * size(node->left);
      node->left->Pass_Val += node->Pass_Val;
    }
    if (node->right) {
      node->right->data += node->Pass_Val;
      node->right->rightest += node->Pass_Val;
      node->right->rightest += node->Pass_Val;
      node->right->sum += node->Pass_Val * size(node->right);
      node->right->Pass_Val += node->Pass_Val;
    }
    node->Pass_Val = 0;
  }

  void push_set(Node *node) {
    if (!node)
      return;
    if (node->left) {
      node->left->data = 0;
      node->left->rightest = 0;
      node->left->leftest = 0;
      node->left->sum = 0;
      node->left->Pass_Val = 0;
      node->left->isDecreasing = 1;
      node->left->isIncreasing = 1;
      node->left->PassZero = true;
    }
    if (node->right) {
      node->right->data = 0;
      node->right->leftest = 0;
      node->right->rightest = 0;
      node->right->sum = 0;
      node->right->Pass_Val = 0;
      node->right->isDecreasing = 1;
      node->right->isIncreasing = 1;
      node->right->PassZero = true;
    }
    node->PassZero = false;
  }
  void push_reverse(Node *node) {
    if (!node)
      return;

    std::swap(node->isDecreasing, node->isIncreasing);
    std::swap(node->leftest, node->rightest);
    std::swap(node->left, node->right);
    if (node->left) {
      node->left->PassReverse ^= true;
    }

    if (node->right) {
      node->right->PassReverse ^= true;
    }
    node->PassReverse = false;
  }

  void just_push(Node *node) {
    if (!node)
      return;
    if (node->PassReverse)
      push_reverse(node);
    if (node->PassZero)
      push_set(node);
    if (node->Pass_Val)
      push_add(node);
  }

  std::pair<Node *, Node *> split_monotonous(Node *node, long long key,
                                             bool ineq) {
    if (!node)
      return {NULL, NULL};
    just_push(node);
    pair<Node *, Node *> res;
    if (!ineq ? key < node->data : key > node->data) {
      res = split_monotonous(node->left, key, ineq);
      node->left = res.second;
      res.second = node;
    } else {
      res = split_monotonous(node->right, key, ineq);
      node->right = res.first;
      res.first = node;
    }
    update(node);
    return res;
  }

  long long find_start_of_monotonous(Node *node, bool type) {

    if (!node)
      return 0;
    just_push(node);
    if (node->left) {
      just_push(node->left);
      if (node->left->left)
        just_push(node->left->left);
      if (node->left->right)
        just_push(node->left->right);
    }
    if (node->right) {
      just_push(node->right);
      if (node->right->left)
        just_push(node->right->left);
      if (node->right->right)
        just_push(node->right->right);
    }

    if (type) {
      if (node->right) {
        if (!(node->right->isIncreasing)) {
          return size(node->left) +
                 find_start_of_monotonous(node->right, type) + 1;
        }
        if (node->data > node->right->leftest) {
          return size(node->left) + 1;
        }
      }
      if (node->left && node->data < node->left->rightest) {
        return size(node->left);
      }

    } else {

      if (node->right) {

        if (!(node->right->isDecreasing)) {
          return size(node->left) +
                 find_start_of_monotonous(node->right, type) + 1;
        }
        if (node->data < node->right->leftest) {
          return size(node->left) + 1;
        }
      }
      if (node->left && node->data > node->left->rightest) {

        return size(node->left);
      }
    }

    return find_start_of_monotonous(node->left, type);
  }

  Node *Next_Permut(Node *node) {
    if (!node)
      return NULL;
    if (node->isDecreasing) {
      node->PassReverse = true;
      just_push(node);
      return node;
    }

    long long beg = find_start_of_monotonous(node, false);

    pair<Node *, Node *> tmp1 = split(node, beg - 1);
    pair<Node *, Node *> tmp2 = split(tmp1.second, 1);

    if (tmp2.second) {
      tmp2.second->PassReverse = true;
      just_push(tmp2.second);
    }

    pair<Node *, Node *> tmp3 =
        split_monotonous(tmp2.second, tmp2.first->data, 0);

    pair<Node *, Node *> tmp4 = split(tmp3.second, 1);
    node = merge(
        merge(merge(merge(tmp1.first, tmp4.first), tmp3.first), tmp2.first),
        tmp4.second);
    return node;
  }

  Node *Prev_Permut(Node *node) {
    if (!node)
      return NULL;
    if (node->isIncreasing) {
      node->PassReverse = true;
      just_push(node);
      return node;
    }
    long long beg = find_start_of_monotonous(node, true);
    pair<Node *, Node *> tmp1 = split(node, beg - 1);
    pair<Node *, Node *> tmp2 = split(tmp1.second, 1);
    if (tmp2.second) {
      tmp2.second->PassReverse = true;
      just_push(tmp2.second);
    }
    pair<Node *, Node *> tmp3 =
        split_monotonous(tmp2.second, tmp2.first->data, 1);
    pair<Node *, Node *> tmp4 = split(tmp3.second, 1);
    node = merge(
        merge(merge(merge(tmp1.first, tmp4.first), tmp3.first), tmp2.first),
        tmp4.second);
    return node;
  }

  BestTreap() { root = NULL; }
  ~BestTreap() { DestroyRecursive(root); }
  void DestroyRecursive(Node *node) {
    if (node) {
      DestroyRecursive(node->left);
      DestroyRecursive(node->right);
      delete node;
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  BestTreap root;
  long long num, tmp, n;
  cin >> num;
  for (long long i = 0; i < num; ++i) {
    cin >> tmp;
    root.insert(i, tmp);
  }
  cin >> n;
  for (long long i = 0; i < n; i++) {
    long long oper;
    cin >> oper;
    long long x, pos, l, r;
    if (oper == 1) {
      cin >> l >> r;
      cout << root.get_sum(l, r) << endl;
    } else if (oper == 2) {
      cin >> x >> pos;
      root.insert(pos, x);
    } else if (oper == 3) {
      cin >> pos;
      root.remove(pos);
    } else if (oper == 4) {
      cin >> x >> l >> r;
      root.set_flag(l, r, x);

    } else if (oper == 5) {
      cin >> x >> l >> r;
      root.add_flag(l, r, x);
    } else if (oper == 6) {
      cin >> l >> r;
      root.Next_Permut(l, r);
    } else if (oper == 7) {
      cin >> l >> r;
      root.Prev_Permut(l, r);
    } else if (oper == 8) {
      cin >> l >> r;
      root.rev(l, r);
    } else if (oper == 9) {
      // cin >> l >> r;
      root.print();
      cout << endl;
    }
  }
  root.print();
}