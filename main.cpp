#include <bits/stdc++.h>

#define printd(expr) std::cout << #expr " = " << (expr) << endl
#define null nullptr

// красно черные свойства
// 1) каждая вершина красная или черная
// 2) корень черный
// 3) каждый лист черный
// 4) если вершина красная то обо ее ребенка черные
// 5) все пути идущие от вершины до листьев которые являются потомками данной вершины
//    содержат одинаковое количество черных вершин [черная высота вершины]

// bh(x) - черная высота вершины x
// пусть bh(x) = k
// bh(leaf) = 0 - черная высота листа
// 2^0-1 = 1-1 = 0
// bh(x.left) >= k-1
// bh(x.right) >= k-1
// 2^(k-1)-1+2^(h-1)-1+1=2^k-1
// n >= 2^bh(root)-1
// n+1 >= 2^bh(root)
// log(n+1) >= bh(root)
// высота дерева может быть больше
// черной высоты корня не более чем
// в два раза то есть
// 2log(n+1) >= h

//  n - вершина которую вставили
//  p - родитель n
//  возможны следующие нарушния
//  1) p == leaf && n.color == red (свойство 2)
//  2) p.color == red && n.color == red (свойство 4)

//  p - вершина которую надо удалить
//  n - вершина которой будет заменена p
//  возможны следующие нарушения
//  1) p == root && n.color == red (свойство 2)
//  2) p.prev.color == red && n.color == red (свойство 4)
//  3) p.color == black && n.color == black (свойство 5)

class red_black_tree {
  enum colors{none, red, black};
  struct node {
    node *left{null};
    node *right{null};
    node *prev{null};
    colors color{none};
    int key{0};
  };
  typedef node* pnode;
  pnode leaf{new node{null, null, null, black, 0}};
  pnode root{leaf};
public:
  void insert(int key) {
    auto p = &root;
    auto prev = leaf;
    while (*p != leaf && (*p)->key != key) {
      prev = *p;
      if ((*p)->key > key)
        p = &(*p)->left;
      else
        p = &(*p)->right;
    }
    if (*p == leaf) {
      *p = new node{leaf, leaf, prev, red, key};
      insert_fix(*p);
    }
  }
  void erase(int key) {
    auto p = root;
    while (p != leaf && p->key != key) {
      if (p->key > key)
        p = p->left;
      else
        p = p->right;
    }
    if (p != leaf) {
      if (p->left != leaf && p->right != leaf)
        replace(p);
      else
        erase(p);
    }
  }
  void print() {
    print(root, 0);
  }
private:
  void lrot(pnode p) {
    auto n = p->right;
    p->right = n->left;
    n->left = p;
    n->prev = p->prev;
    p->right->prev = p;
    if (p->prev == leaf)
      root = n;
    else if (p == p->prev->left)
      p->prev->left = n;
    else
      p->prev->right = n;
    p->prev = n;
  }
  void rrot(pnode p) {
    auto n = p->left;
    p->left = n->right;
    n->right = p;
    n->prev = p->prev;
    p->left->prev = p;
    if (p->prev == leaf)
      root = n;
    else if (p == p->prev->left)
      p->prev->left = n;
    else
      p->prev->right = n;
    p->prev = n;
  }
  void insert_fix(pnode p) {
    while (p->prev->color == red) {
      if (p->prev == p->prev->prev->left) {
        auto u = p->prev->prev->right;
        if (u->color == red) {
          p->prev->color = black;
          u->color = black;
          p->prev->prev->color = red;
          p = p->prev->prev;
        } else {
          if (p == p->prev->right) {
            p = p->prev;
            lrot(p);
          }
          auto g = p->prev->prev;
          p->prev->color = black;
          g->color = red;
          rrot(g);
        }
      } else {
        auto u = p->prev->prev->left;
        if (u->color == red) {
          p->prev->color = black;
          u->color = black;
          p->prev->prev->color = red;
          p = p->prev->prev;
        }
        else {
          if (p == p->prev->left) {
            p = p->prev;
            rrot(p);
          }
          auto g = p->prev->prev;
          p->prev->color = black;
          g->color = red;
          lrot(g);
        }
      }
    }
    root->color = black;
  }
  void replace(pnode p) {
    auto rep = p->right;
    while (rep->left != leaf)
      rep = rep->left;
    p->key = rep->key;
    erase(rep);
  }
  void erase(pnode p) {
    auto n = p->left;
    if (p->right != leaf)
      n = p->right;
    if (p->color == black) {
      if (n->color == red)
        n->color = black;
      else
        erase_fix(p);
    }
    n->prev = p->prev;
    if (p->prev == leaf)
      root = n;
    else if (p == p->prev->left)
      p->prev->left = n;
    else
      p->prev->right = n;
    delete p;
  }
  void erase_fix(pnode p) {
    while (p != root && p->color == black) {
      if (p == p->prev->left) {
        auto s = p->prev->right;
        if (s->color == red) {
          p->prev->color = red;
          s->color = black;
          lrot(p->prev);
          s = p->prev->right;
        }
        if (s->left->color == black &&
            s->right->color == black) {
          s->color = red;
          p = p->prev;
        } else {
          if (s->right->color == black) {
            s->left->color = black;
            s->color = red;
            rrot(s);
            s = p->prev->right;
          }
          s->color = p->prev->color;
          p->prev->color = black;
          s->right->color = black;
          lrot(p->prev);
          p = root;
        }
      } else {
        auto s = p->prev->left;
        if (s->color == red) {
          p->prev->color = red;
          s->color = black;
          rrot(p->prev);
          s = p->prev->left;
        }
        if (s->left->color == black &&
            s->right->color == black) {
          s->color = red;
          p = p->prev;
        } else {
          if (s->left->color == black) {
            s->right->color = black;
            s->color = red;
            lrot(s);
            s = p->prev->left;
          }
          s->color = p->prev->color;
          p->prev->color = black;
          s->left->color = black;
          rrot(p->prev);
          p = root;
        }
      }
    }
    p->color = black;
  }
  void print(pnode p, int n) {
    if (p != leaf) {
      print(p->right, n + 1);
      for (int i = 0; i < n; i++)
        std::cout << "    ";
      if (p->color == red)
        std::cout << "\033[1;31m";
      else
        std::cout << "\033[1;30m";
      std::cout << p->key << std::endl;
      std::cout << "\033[0m";
      print(p->left, n + 1);
    }
  }
};

using namespace std;

int main() {
  red_black_tree t;

  for (int i = 0; i < 50; i++)
    t.insert(i);

  t.print();

  return 0;
}
