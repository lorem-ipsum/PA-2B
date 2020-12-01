#include <cassert>
#include <cstdio>
#include <iostream>
#include <utility>

// #define DEBUG

#define BNP BinNode*

#define RED false
#define BLACK true
#define PARENT(x) x->parent
#define UNCLE(x)                                                             \
  (x->parent == x->parent->parent->leftChild ? x->parent->parent->rightChild \
                                             : x->parent->parent->leftChild)
#define SIBLING(x) \
  (x == x->parent->leftChild ? x->parent->rightChild : x->parent->leftChild)
#define HASREDCHILD(x)                            \
  (x->leftChild && x->leftChild->color_ == RED || \
   x->rightChild && x->rightChild->color_ == RED)
#define GPARENT(x) x->parent->parent
struct BinNode {
  int value_;
  BNP parent;
  BNP leftChild;
  BNP rightChild;
  bool color_;  // red: 0, black: 1

  BinNode(int v, BNP p = nullptr, bool rb = RED)
      : value_(v),
        parent(p),
        leftChild(nullptr),
        rightChild(nullptr),
        color_(rb) {}
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
#ifdef DEBUG
    printf("insert(%d)\n", v);
#endif
    // 插入空树中
    if (size == 0) {
      root = new BinNode(v, nullptr, BLACK);
    }
    // 插入非空树
    else {
      std::pair<BNP, BNP> res = search(v);
      BNP tmp = new BinNode(v, res.first, RED);
      if (v > res.first->value_) {
        res.first->rightChild = tmp;
      } else {
        res.first->leftChild = tmp;
      }
      // 插入时父亲为红色
      if (res.first->color_ == RED) solveDoubleRed(tmp);
    }
    ++size;
  }
  void solveDoubleRed(BNP x) {
    while (true) {
      if (x == root) {
        x->color_ = BLACK;
        break;
      } else if (PARENT(x)->color_ == BLACK) {
        break;
      } else {
        // 叔叔为红色，将父节点和叔叔节点变成黑色，把祖父节点变为红色
        if (UNCLE(x) && UNCLE(x)->color_ == RED) {
          PARENT(x)->color_ = BLACK;
          UNCLE(x)->color_ = BLACK;
          GPARENT(x)->color_ = RED;
          x = GPARENT(x);
        }
        // 叔叔不存在或为黑，分为四种情况
        else {
          BNP p = PARENT(x);
          BNP g = GPARENT(x);
          BNP gg = GPARENT(x)->parent;
          if (x == p->leftChild && p == g->leftChild) {
            /**
             *
             *         g(BLACK)
             *       /
             *     p(RED)
             *   /
             * x(RED)
             *
             */
            p->color_ = BLACK;
            g->color_ = RED;
            connect34(x, p, g, x->leftChild, x->rightChild, p->rightChild,
                      g->rightChild, gg, gg != nullptr && gg->leftChild == g);

          } else if (x == p->rightChild && p == g->rightChild) {
            /**
             *
             * g(BLACK)
             *   \
             *     p(RED)
             *       \
             *         x(RED)
             *
             */
            p->color_ = BLACK;
            g->color_ = RED;
            connect34(g, p, x, g->leftChild, p->leftChild, x->leftChild,
                      x->rightChild, gg, gg != nullptr && gg->leftChild == g);
          } else if (x == p->rightChild && p == g->leftChild) {
            /**
             *
             *               g(BLACK)
             *          /
             *     p(RED)
             *       \
             *         x(RED)
             *
             */
            x->color_ = BLACK;
            g->color_ = RED;
            connect34(p, x, g, p->leftChild, x->leftChild, x->rightChild,
                      g->rightChild, gg, gg != nullptr && gg->leftChild == g);
          } else if (x == p->leftChild && p == g->rightChild) {
            /**
             *
             *     g(BLACK)
             *          \
             *               p(RED)
             *             /
             *           x(RED)
             *
             */
            x->color_ = BLACK;
            g->color_ = RED;
            connect34(g, x, p, g->leftChild, x->leftChild, x->rightChild,
                      p->rightChild, gg, gg != nullptr && gg->leftChild == g);
          }
          break;
        }
      }
    }
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

  void connect34(BNP a, BNP b, BNP c, BNP p1, BNP p2, BNP p3, BNP p4,
                 BNP handler, bool nullOrLeft) {
    b->leftChild = a;
    b->rightChild = c;
    a->parent = b;
    c->parent = b;
    a->leftChild = p1;
    a->rightChild = p2;
    c->leftChild = p3;
    c->rightChild = p4;
    if (p1) p1->parent = a;
    if (p2) p2->parent = a;
    if (p3) p3->parent = c;
    if (p4) p4->parent = c;

    b->parent = handler;
    if (!handler) {
      root = b;
    } else if (nullOrLeft) {
      handler->leftChild = b;
    } else {
      handler->rightChild = b;
    }
  }

  void remove(int x) {
#ifdef DEBUG
    printf("remove(%d)\n", x);
#endif
    if (size == 1) {
      root = nullptr;
      size = 0;
      return;
    }
    std::pair<BNP, BNP> res = search(x);
    BNP cur = res.second;
    BNP par = cur->parent;
    // 假如该节点有右子树，接下来取 cur = successor(cur)
    if (cur->rightChild) {
#ifdef DEBUG
      printf("假如该节点有右子树，接下来取 cur = successor(cur)\n");
#endif
      BNP succ = successor(cur);
      // succ必然不存在左子树
      cur->value_ = succ->value_;
      cur = succ;
      par = cur->parent;
      // 此时cur必然没有左子树，且cur不为根
      // 假如cur和右孩子中有且仅有一个为红色
      if (((cur->color_ == RED) ^
           (cur->rightChild && cur->rightChild->color_ == RED))) {
#ifdef DEBUG
        printf("假如cur和右孩子中有且仅有一个为红色\n");
#endif
        if (cur->rightChild) cur->rightChild->color_ = BLACK;
        if (cur->rightChild) cur->rightChild->parent = par;
        if (par->leftChild == cur) {
          par->leftChild = cur->rightChild;
        } else {
          par->rightChild = cur->rightChild;
        }
        --size;
        delete cur;
        return;
      }
      // 假如cur双黑或没有右子树(等价于par在cur这一支上损失一个黑高度)
      else {
#ifdef DEBUG
        printf(
            "假如cur双黑或没有右子树(等价于par在cur这一支上损失一个黑高度)\n");
#endif
        BNP sib = SIBLING(cur);  // 如果需要消除双黑缺陷，sib != nullptr;
        if (cur->rightChild) cur->rightChild->parent = par;
        if (par->leftChild == cur) {
          par->leftChild = cur->rightChild;
        } else {
          par->rightChild = cur->rightChild;
        }
        --size;
        delete cur;
        solveDoubleBlack(par, sib);
        return;
      }
    }
    // 假如该节点没有右子树，弃用succ方法
    else {
#ifdef DEBUG
      printf("假如该节点没有右子树，弃用succ方法\n");
#endif
      // 假如cur是树根
      if (cur == root) {
        root = cur->leftChild;
        root->parent = nullptr;
        root->color_ = BLACK;
        --size;
        delete cur;
        return;
      }
      // 以下情况，cur非树根
      // 假如cur和左孩子中有且仅有一个为红色
      if (((cur->color_ == RED) ^
           (cur->leftChild && cur->leftChild->color_ == RED))) {
#ifdef DEBUG
        printf("假如cur和左孩子中有且仅有一个为红色\n");
#endif
        if (cur->leftChild) cur->leftChild->color_ = BLACK;
        if (cur->leftChild) cur->leftChild->parent = par;
        if (par->leftChild == cur) {
          par->leftChild = cur->leftChild;
        } else {
          par->rightChild = cur->leftChild;
        }
        --size;
        delete cur;
        return;
      }
      // 假如cur双黑或没有左子树
      else {
#ifdef DEBUG
        printf("假如cur双黑或没有左子树\n");
#endif
        BNP sib = SIBLING(cur);  // 如果需要消除双黑缺陷，sib != nullptr;
        if (cur->rightChild) cur->rightChild->parent = par;
        if (par->leftChild == cur) {
          par->leftChild = cur->rightChild;
        } else {
          par->rightChild = cur->rightChild;
        }
        --size;
        delete cur;
        solveDoubleBlack(par, sib);
        return;
      }
    }
  }

  void solveDoubleBlack(BNP par, BNP s) {
    // 假如cur双黑或没有左子树
    // 假如s为黑色
    BNP gg = par->parent;
    BNP r = SIBLING(s);
    if (s->color_ == BLACK) {
      // BB-1 s有红孩子，即s是富裕的 (CHECKED)
      if (HASREDCHILD(s)) {
        if (par->leftChild == s) {
          // s的左孩子为红色
          if (s->leftChild && s->leftChild->color_ == RED) {
            BNP c = s->leftChild;
            /**
             *
             *              P(?)                            S(?)
             *            /      \                        /     \
             *        S(B)         r(B)    ======>    C(B)       P(B)
             *      /     \                              \          \
             *  C(R)       CC(?)                        CC(?)     r(B)(?=null)
             *
             */
            // colors
            c->color_ = BLACK;
            s->color_ = par->color_;
            par->color_ = BLACK;
            connect34(c, s, par, c->leftChild, c->rightChild, s->rightChild,
                      par->rightChild, gg,
                      gg != nullptr && gg->leftChild == par);
          }
          // s的右孩子为红色
          else {
            BNP c = s->rightChild;
            /**
             *
             *              P(?)                             C(?)
             *            /      \                         /     \
             *        S(B)         r(B)      ======>   S(B)       P(B)
             *      /     \                           /              \
             *  CC(?)       C(R)                 CC(?)             r(B)(?=null)
             *
             */
            // colors
            c->color_ = par->color_;
            par->color_ = BLACK;
            connect34(s, c, par, s->leftChild, c->leftChild, c->rightChild,
                      par->rightChild, gg,
                      gg != nullptr && gg->leftChild == par);
          }
        } else {
          // s的右孩子为红色
          if (s->rightChild && s->rightChild->color_ == RED) {
            BNP c = s->rightChild;
            /**
             *
             *              P(?)         |                   S(?)
             *            /      \       |                 /     \
             *        S(B)         r(B)  |  ======>    C(B)       P(B)
             *      /     \              |                \          \
             *  C(R)       CC(?)         |               CC(?)      r(B)(?=null)
             *
             */
            // colors
            c->color_ = BLACK;
            s->color_ = par->color_;
            par->color_ = BLACK;
            connect34(par, s, c, par->leftChild, s->leftChild, c->leftChild,
                      c->rightChild, gg, gg != nullptr && gg->leftChild == par);
          }
          // s的左孩子为红色
          else {
            BNP c = s->leftChild;
            /**
             *
             *              P(?)        |                    C(?)
             *            /      \      |                  /     \
             *        S(B)         r(B) |   ======>    S(B)       P(B)
             *      /     \             |            /               \
             *  CC(?)       C(R)        |       CC(?)               r(B)(?=null)
             *
             */
            // colors
            c->color_ = par->color_;
            par->color_ = BLACK;
            connect34(par, c, s, par->leftChild, c->leftChild, c->rightChild,
                      s->rightChild, gg, gg != nullptr && gg->leftChild == par);
          }
        }
      }
      // BB-2 s没有红孩子，需要向par索要 (CHECKED)
      else {
        // BB-2R p为红色，异常简单
        if (par->color_ == RED) {
          par->color_ = BLACK;
          s->color_ = RED;
        }
        // BB-2B p为黑色，需要迭代进行
        else {
          s->color_ = RED;
          if (par != root) {
            solveDoubleBlack(PARENT(par), SIBLING(par));
          }
        }
      }
    }
    // BB-3 s为红色 (CHECKED)
    else {
      if (par->leftChild == s) {
        s->color_ = BLACK;
        par->color_ = RED;

        BNP p2 = s->rightChild;
        par->leftChild = p2;
        s->rightChild = par;
        if (!gg) {
          root = s;
        } else if (gg->leftChild == par) {
          gg->leftChild = s;
        } else {
          gg->rightChild = s;
        }
        if (p2) p2->parent = par;
        par->parent = s;
        s->parent = gg;
        solveDoubleBlack(par, par->leftChild);
      } else {
        s->color_ = BLACK;
        par->color_ = RED;

        BNP p2 = s->leftChild;
        par->rightChild = p2;
        s->leftChild = par;
        if (!gg) {
          root = s;
        } else if (gg->leftChild == par) {
          gg->leftChild = s;
        } else {
          gg->rightChild = s;
        }
        if (p2) p2->parent = par;
        par->parent = s;
        s->parent = gg;
        solveDoubleBlack(par, par->rightChild);
      }
    }
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
  // for (int i = 1000; i >= 0; --i) {
  //   bst.insert(i);
  // }
  // // bst.traverse();
  // for (int i = 0; i < 100; ++i) {
  //   bst.remove(i);
  //   bst.traverse();
  // }
  // for (int i = 0; i < 510; ++i) {
  //   printf("find(%d) = %d\n", i * 233 % 2333,
  //          bst.findEqualToOrLessThan(i * 233 % 2333));
  // }
  // printf("\n");
  for (int i = 0; i < 30; ++i) {
    bst.insert(i);
    bst.traverse();
  }
  for (int i = 29; i >= 0; --i) {
    bst.remove(i);
    bst.traverse();
  }
  for (int i = 500; i < 510; ++i) {
    printf("%d ", bst.findEqualToOrLessThan(i * 233 % 19260817));
  }
  // bst.traverse();
  // for (int i = 0; i < 1000000; ++i) {
  //   bst.insert(i);
  // }
  // bst.traverse();
#endif
  return 0;
}