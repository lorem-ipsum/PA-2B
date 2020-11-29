#include <cassert>
#include <cstdio>
#include <iostream>
#include <utility>

#define BNP BinNode*

#define RED false
#define BLACK true

struct BinNode {
  int value_;
  BNP parent;
  BNP leftChild;
  BNP rightChild;
  bool redOrBlack;  // red: 0, black: 1

  BinNode(int v, BNP p = nullptr, bool rb = RED)
      : value_(v),
        parent(p),
        leftChild(nullptr),
        rightChild(nullptr),
        redOrBlack(rb) {}
};

struct BST {
  BNP root;
  int size;
  BST() : root(nullptr), size(0) {}
  std::pair<BNP, BNP> search(int v) {
    if (size == 0) {
      return std::pair<BNP, BNP>(nullptr, nullptr);
    }
    BNP hot = nullptr;
    BNP res = root;
    while (true) {
      if (res == nullptr || v == res->value_) break;
      hot = res;
      if (v > res->value_) {
        res = res->rightChild;
      } else {
        res = res->leftChild;
      }
    }
    return std::pair<BNP, BNP>(hot, res);
  }
  void insert(int v) {
    // printf("insert(%d)\n", v);
    BNP newNode = new BinNode(v);
    // 插入空树中
    if (size == 0) {
      root = newNode;
    }
    // 插入非空树
    else {
      std::pair<BNP, BNP> res = search(v);
      BNP tmp = new BinNode(v, res.first);
      if (v > res.first->value_) {
        res.first->rightChild = tmp;
      } else {
        res.first->leftChild = tmp;
      }
    }
    ++size;
  }
  BNP successor(BNP r) {
    // 保证所研究的节点拥有右子树
    if (r->rightChild == nullptr) {
      assert(false);
    }
    BNP res = r->rightChild;  // res != nullptr
    while (res->leftChild) {
      res = res->leftChild;
    }
    return res;
  }
  void remove(int x) {
    // printf("remove(%d)\n", x);
    std::pair<BNP, BNP> res = search(x);
    BNP cur = res.second;
    BNP par = cur->parent;
    if (cur->leftChild == nullptr && cur->rightChild == nullptr) {
      // remove a node which has no children
      if (par == nullptr) {
        // remove the only node
        root = nullptr;
      } else {
        if (par->leftChild == cur) {
          par->leftChild = nullptr;
        } else {
          par->rightChild = nullptr;
        }
      }
    } else if (cur->leftChild == nullptr) {
      // remove a node which only has right child
      if (par == nullptr) {
        // remove the root node
        root = cur->rightChild;
        root->parent = nullptr;
      } else {
        if (cur == par->leftChild) {
          par->leftChild = cur->rightChild;
        } else {
          par->rightChild = cur->rightChild;
        }
      }
      cur->rightChild->parent = par;
    } else if (cur->rightChild == nullptr) {
      // remove a node which only has left child
      if (par == nullptr) {
        root = cur->leftChild;
      } else {
        if (cur == par->leftChild) {
          par->leftChild = cur->leftChild;
        } else {
          par->rightChild = cur->leftChild;
        }
      }
      cur->leftChild->parent = par;
    } else {
      // remove a node that has both children
      if (cur->leftChild->leftChild == nullptr &&
          cur->leftChild->rightChild == nullptr) {
        // cur's lc does not have children
        if (par == nullptr) {
          root = cur->leftChild;
          root->parent = nullptr;
          root->leftChild = nullptr;
          root->rightChild = cur->rightChild;
          cur->rightChild->parent = root;
        } else {
          if (cur == par->leftChild) {
            par->leftChild = cur->leftChild;
          } else {
            par->rightChild = cur->leftChild;
          }
          cur->leftChild->rightChild = cur->rightChild;
          cur->rightChild->parent = cur->leftChild;
          cur->leftChild->parent = par;
        }
      } else if (cur->rightChild->leftChild == nullptr &&
                 cur->rightChild->rightChild == nullptr) {
        // cur's rc does not have children
        if (par == nullptr) {
          root = cur->rightChild;
          root->parent = nullptr;
          root->rightChild = nullptr;
          root->leftChild = cur->leftChild;
          cur->leftChild->parent = root;
        } else {
          if (cur == par->leftChild) {
            par->leftChild = cur->rightChild;
          } else {
            par->rightChild = cur->rightChild;
          }
          cur->rightChild->leftChild = cur->leftChild;
          cur->leftChild->parent = cur->rightChild;
          cur->rightChild->parent = par;
        }
      } else {
        // normal case, find the successor
        BNP succ = successor(cur);
        BNP succParent = succ->parent;
        BNP succRightChild = succ->rightChild;
        succ->parent = par;
        if (par == nullptr) {
          // cur is the root node
          root = succ;
        } else {
          if (cur == par->leftChild) {
            par->leftChild = succ;
          } else {
            par->rightChild = succ;
          }
        }
        if (succParent != cur) {
          succ->leftChild = cur->leftChild;
          if (cur->leftChild) cur->leftChild->parent = succ;
          succ->rightChild = cur->rightChild;
          if (cur->rightChild) cur->rightChild->parent = succ;

          succParent->leftChild = succRightChild;
          if (succRightChild) succRightChild->parent = succParent;
        } else {
          succ->leftChild = cur->leftChild;
          if (cur->leftChild) cur->leftChild->parent = succ;
        }
      }
    }
    delete cur;
    --size;
  }

  void traverse() {
    printf("========== begin ==========\n");
    traverseR(root);
    printf("\n==========  end  ==========\n");
  }
  void traverseR(BNP r) {
    if (r == nullptr) return;
    traverseR(r->leftChild);
    printf("%d ", r->value_);
    traverseR(r->rightChild);
  }

  int findEqualToOrLessThan(int x) {
    if (size == 0) return -1;
    BNP r = root;
    int hot = -1;
    while (r) {
      if (r->value_ > x) {
        if (!r->leftChild) {
          return hot;
        } else {
          r = r->leftChild;
        }
      } else if (r->value_ == x) {
        return x;
      } else {
        // r->value_ < x
        hot = r->value_;
        if (!r->rightChild) {
          return hot;
        } else {
          r = r->rightChild;
        }
      }
    }
  }
};

// #define DEBUG

int main() {
  BST bst;
#ifndef DEBUG
  int n;
  scanf("%d", &n);
  for (int i = 0; i < n; ++i) {
    char abc;
    int x;
    scanf(" %c\n", &abc);
    scanf("%d", &x);
    if (abc == 'A') {
      bst.insert(x);
    } else if (abc == 'B') {
      bst.remove(x);
    } else {
      printf("%d\n", bst.findEqualToOrLessThan(x));
    }
  }
#else
  for (int i = 0; i < 1000; ++i) {
    bst.insert(i * 233 % 19260817);
    bst.traverse();
  }
  for (int i = 500; i < 510; ++i) {
    printf("%d ", bst.findEqualToOrLessThan(i * 233 % 19260817));
  }
  printf("\n");
  for (int i = 999; i >= 10; --i) {
    bst.remove(i * 233 % 19260817);
    bst.traverse();
  }
  for (int i = 500; i < 510; ++i) {
    printf("%d ", bst.findEqualToOrLessThan(i * 233 % 19260817));
  }
  bst.traverse();
#endif
  return 0;
}