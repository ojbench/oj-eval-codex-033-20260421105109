#pragma once
#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>

namespace sjtu {

template <typename T>
class list {
  private:
    struct node {
      node *prev;
      node *next;
      T value;
      template <typename U>
      explicit node(U &&v) : prev(nullptr), next(nullptr), value(std::forward<U>(v)) {}
    };

    node *head; // sentinel head (no value). We'll use a circular sentinel.
    size_t n;

    void init_empty() {
      head = reinterpret_cast<node*>(::operator new(sizeof(node)));
      head->prev = head;
      head->next = head;
      // Do not construct T for sentinel
      n = 0;
    }

    static void link_nodes(node *a, node *b) {
      a->next = b;
      b->prev = a;
    }

  public:
    class iterator;
    class const_iterator;

    list() { init_empty(); }

    list(const list &other) { init_empty(); for (const auto &x : other) push_back(x); }

    list &operator=(const list &other) {
      if (this == &other) return *this;
      clear();
      for (const auto &x : other) push_back(x);
      return *this;
    }

    ~list() { clear(); ::operator delete(reinterpret_cast<void*>(head)); head = nullptr; }

    class iterator {
      friend class list;
      friend class const_iterator;
      node *p;
      const list *owner;
      iterator(node *ptr, const list *own) : p(ptr), owner(own) {}
     public:
      iterator() : p(nullptr), owner(nullptr) {}
      iterator &operator++() { p = p->next; return *this; }
      iterator &operator--() { p = p->prev; return *this; }
      iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
      iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }
      T &operator*() const noexcept { return p->value; }
      T *operator->() const noexcept { return &p->value; }
      friend bool operator==(const iterator &a, const iterator &b) { return a.p == b.p; }
      friend bool operator!=(const iterator &a, const iterator &b) { return !(a == b); }
    };

    class const_iterator {
      friend class list;
      const node *p;
      const list *owner;
      const_iterator(const node *ptr, const list *own) : p(ptr), owner(own) {}
     public:
      const_iterator() : p(nullptr), owner(nullptr) {}
      const_iterator(const iterator &it) : p(it.p), owner(it.owner) {}
      const_iterator &operator++() { p = p->next; return *this; }
      const_iterator &operator--() { p = p->prev; return *this; }
      const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
      const_iterator operator--(int) { const_iterator tmp = *this; --(*this); return tmp; }
      const T &operator*() const noexcept { return p->value; }
      const T *operator->() const noexcept { return &p->value; }
      friend bool operator==(const const_iterator &a, const const_iterator &b) { return a.p == b.p; }
      friend bool operator!=(const const_iterator &a, const const_iterator &b) { return !(a == b); }
      friend bool operator==(const const_iterator &a, const iterator &b) { return a.p == b.p; }
      friend bool operator!=(const const_iterator &a, const iterator &b) { return !(a == b); }
      friend bool operator==(const iterator &a, const const_iterator &b) { return a.p == b.p; }
      friend bool operator!=(const iterator &a, const const_iterator &b) { return !(a == b); }
    };

    T &front() noexcept { return *begin(); }
    T &back() noexcept { auto it = end(); --it; return *it; }
    const T &front() const noexcept { return *cbegin(); }
    const T &back() const noexcept { auto it = cend(); --it; return *it; }

    iterator begin() noexcept { return iterator(head->next, this); }
    const_iterator cbegin() const noexcept { return const_iterator(head->next, this); }
    iterator end() noexcept { return iterator(head, this); }
    const_iterator cend() const noexcept { return const_iterator(head, this); }

    bool empty() const noexcept { return n == 0; }
    size_t size() const noexcept { return n; }

    void clear() noexcept {
      node *cur = head->next;
      while (cur != head) {
        node *nx = cur->next;
        cur->value.~T();
        ::operator delete(reinterpret_cast<void*>(cur));
        cur = nx;
      }
      head->next = head->prev = head;
      n = 0;
    }

    iterator insert(iterator pos, const T &value) {
      if (pos.owner != this) throw std::invalid_argument("iterator does not belong to this list");
      node *posn = pos.p;
      node *prev = posn->prev;
      node *nn = reinterpret_cast<node*>(::operator new(sizeof(node)));
      try {
        new (&nn->value) T(value);
      } catch (...) {
        ::operator delete(reinterpret_cast<void*>(nn));
        throw;
      }
      nn->prev = prev; nn->next = posn;
      prev->next = nn; posn->prev = nn;
      ++n;
      return iterator(nn, this);
    }

    iterator erase(iterator pos) noexcept {
      node *cur = pos.p;
      // assume valid and not end()
      node *p = cur->prev;
      node *nx = cur->next;
      p->next = nx; nx->prev = p;
      cur->value.~T();
      ::operator delete(reinterpret_cast<void*>(cur));
      --n;
      return iterator(nx, this);
    }

    void push_front(const T &value) { insert(begin(), value); }
    void push_back(const T &value) { insert(end(), value); }

    void pop_front() noexcept { erase(begin()); }
    void pop_back() noexcept { auto it = end(); --it; erase(it); }
};

} // namespace sjtu

#endif // SJTU_LIST_HPP

