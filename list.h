//
// Created by borisshapa on 22.06.19.
//

#ifndef LIST_LIST_H
#define LIST_LIST_H

#include <iterator>

template<typename T>
struct list {
    list();

    list (list const& other);

    ~list();

    list &operator=(list const &);

    bool empty() const;

    void clear();

    void push_back(T const &);

    void pop_back();

    void push_front(T const &);

    void pop_front();

    T &back();

    T const &back() const;

    T &front();

    T const &front() const;

private:
    template <typename U>
    struct list_iterator;

public:
    typedef list_iterator<T> iterator;
    typedef list_iterator<T const> const_iterator;
    typedef std::reverse_iterator<list_iterator<T>> reverse_iterator;
    typedef std::reverse_iterator<list_iterator<T const>> const_reverse_iterator;

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    iterator insert(const_iterator pos, T const &);

    iterator erase(const_iterator);

    void splice(const_iterator pos, list &other, const_iterator first, const_iterator last);

    template<typename U>
    friend void swap(list<U> &, list<U> &);

private:
    struct node_base {
        node_base() : next(nullptr), prev(nullptr) {}

        node_base(node_base *prev, node_base *next) : prev(prev), next(next) {}

        node_base *next;
        node_base *prev;
    };

    struct node : node_base {
        node() = delete;

        node(T value, node_base *prev, node_base *next) : node_base(prev, next), value(value) {}

        T value;
    };

    template<typename U>
    struct list_iterator : std::iterator<std::bidirectional_iterator_tag, U> {
        list_iterator() = default;
        friend list<T>;
        list_iterator(list_iterator<T> const &other) : ptr(other.ptr) {}

        list_iterator &operator++() {
            ptr = ptr->next;
            return *this;
        };

        list_iterator operator++(int) {
            list_iterator <U> tmp(*this);
            ++*this;
            return tmp;
        };

        list_iterator &operator--() {
            ptr = ptr->prev;
            return *this;
        };

        list_iterator operator--(int) {
            list_iterator <U> tmp(*this);
            --*this;
            return tmp;
        };

        U &operator*() const {
            return static_cast<node *>(ptr)->value;
        };

        U *operator->() const {
            return &static_cast<node *>(ptr)->value;
        };

        template<typename UU>
        bool operator==(list_iterator<UU> const &other) const {
            return ptr == other.ptr;
        }

        template<typename UU>
        bool operator!=(list_iterator<UU> const &other) const {
            return ptr != other.ptr;
        }

    private :
        node_base *ptr;
        list_iterator(node_base *node) : ptr(node) {}
    };

    node_base fake_node;

    node_base * & first();

    node_base * & last();

};

template<typename T>
list<T>::list() {
    first() = &fake_node;
    last() = &fake_node;
}

template<typename T>
list<T>::~list() {
    clear();
}

template<typename T>
list<T> &list<T>::operator=(list const &other) {
    list<T> tmp(other);
    swap(*this, tmp);
    return *this;
}

template<typename U>
void swap(list<U> &a, list<U> &b) {
    auto a_left = a.last();
    auto a_right = a.first();
    auto b_left = b.last();
    auto b_right = b.first();

    a_left->next = a_right->prev = &b.fake_node;
    b_left->next = b_right->prev = &a.fake_node;
    std::swap(a.fake_node, b.fake_node);
}

template<typename T>
bool list<T>::empty() const {
    return &fake_node == fake_node.next;
}

template<typename T>
void list<T>::clear() {
    node_base *cur = first();
    while (cur != &fake_node) {
        node *to_del = static_cast<node*>(cur);
        cur = cur->next;
        delete to_del;
    }
    first() = &fake_node;
    last() = &fake_node;
}

template<typename T>
void list<T>::push_back(const T &value) {
    node_base *last_ = last();
    last() = new node(value, last_, &fake_node);
    last_->next = last();
}

template<typename T>
void list<T>::pop_back() {
    node_base *tmp = last();
    last()->prev->next = &fake_node;
    last() = last()->prev;
    delete static_cast<node*>(tmp);
}

template<typename T>
void list<T>::push_front(const T &value) {
    node_base *first_ = first();
    first() = new node(value, &fake_node, first_);
    first_->prev = first();
}

template<typename T>
void list<T>::pop_front() {
    node_base *tmp = first();
    first()->next->prev = &fake_node;
    first() = first()->next;
    delete static_cast<node*>(tmp);
}

template<typename T>
T &list<T>::back() {
    return static_cast<node *>(fake_node.prev)->value;
}

template<typename T>
T const &list<T>::back() const {
    return static_cast<node *>(fake_node.prev)->value;
}

template<typename T>
T &list<T>::front() {
    return static_cast<node *>(fake_node.next)->value;
}

template<typename T>
T const &list<T>::front() const {
    return static_cast<node *>(fake_node.next)->value;
}

template<typename T>
typename list<T>::iterator list<T>::begin() {
    return iterator(fake_node.next);
}

template<typename T>
typename list<T>::const_iterator list<T>::begin() const {
    return iterator(fake_node.next);
}

template<typename T>
typename list<T>::iterator list<T>::end() {
    return iterator(&fake_node);
}

template<typename T>
typename list<T>::const_iterator list<T>::end() const {
    return const_iterator(const_cast<node_base*>(&fake_node));
}

template<typename T>
typename list<T>::reverse_iterator list<T>::rbegin() {
    return reverse_iterator(end());
}

template<typename T>
typename list<T>::reverse_iterator list<T>::rend() {
    return reverse_iterator(begin());
}

template<typename T>
typename list<T>::const_reverse_iterator list<T>::rbegin() const {
    return const_reverse_iterator(end());
}

template<typename T>
typename list<T>::const_reverse_iterator list<T>::rend() const {
    return const_reverse_iterator(begin());
}

template<typename T>
typename list<T>::iterator list<T>::insert(list::const_iterator pos, const T & val) {
    node_base* cur = static_cast<node_base*>(new node(val, pos.ptr->prev, pos.ptr));
    cur->prev->next = cur;
    cur->next->prev = cur;
    return iterator(cur);
}

template<typename T>
typename list<T>::iterator list<T>::erase(list::const_iterator pos) {
    pos.ptr->prev->next = pos.ptr->next;
    pos.ptr->next->prev = pos.ptr->prev;
    iterator tmp(pos.ptr->next);
    delete static_cast<node*>(pos.ptr);
    return tmp;
}

template<typename T>
void list<T>::splice(list::const_iterator pos, list &other, list::const_iterator first, list::const_iterator last) {
    node_base * tmp = first.ptr->prev;
    pos.ptr->prev->next = first.ptr;
    first.ptr->prev = pos.ptr->prev;

    last.ptr->prev->next = pos.ptr;
    pos.ptr->prev = last.ptr->prev;

    last.ptr->prev = tmp;
    tmp->next = last.ptr;
}

template<typename T>
list<T>::list(const list &other) : list() {
    for (auto x : other) {
        push_back(x);
    }
}

template<typename T>
typename list<T>::node_base * &list<T>::first() {
    return fake_node.next;
}

template<typename T>
typename list<T>::node_base * &list<T>::last() {
    return fake_node.prev;
}

#endif //LIST_LIST_H
