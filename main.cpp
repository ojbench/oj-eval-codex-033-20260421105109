#include <bits/stdc++.h>
#include "list.hpp"

// Simple driver that reads commands to operate on sjtu::list<int>.
// Protocol (for OJ test harness in this repo):
// First line: Q (number of operations)
// Each next line: command among:
//   pf x  -> push_front x
//   pb x  -> push_back x
//   pof   -> pop_front (if not empty)
//   pob   -> pop_back (if not empty)
//   fr    -> print front (or "EMPTY")
//   bk    -> print back (or "EMPTY")
//   sz    -> print size
// All prints are on their own line.

int main() {
  using namespace std;
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  sjtu::list<long long> lst;
  int Q; if (!(cin >> Q)) return 0;
  while (Q--) {
    string op; cin >> op;
    if (op == "pf") {
      long long x; cin >> x; lst.push_front(x);
    } else if (op == "pb") {
      long long x; cin >> x; lst.push_back(x);
    } else if (op == "pof") {
      if (!lst.empty()) lst.pop_front();
    } else if (op == "pob") {
      if (!lst.empty()) lst.pop_back();
    } else if (op == "fr") {
      if (lst.empty()) cout << "EMPTY\n"; else cout << lst.front() << "\n";
    } else if (op == "bk") {
      if (lst.empty()) cout << "EMPTY\n"; else cout << lst.back() << "\n";
    } else if (op == "sz") {
      cout << lst.size() << "\n";
    }
  }
  return 0;
}

