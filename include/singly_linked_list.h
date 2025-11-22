#pragma once

#include "node.h"
#include <memory_resource>
#include <utility>
#include <iostream>
#include <iterator>

template <typename T>
class singly_linked_list {
private:
    Node<T>* head = nullptr;
    Node<T>* tail = nullptr;
    std::pmr::polymorphic_allocator<Node<T>> alloc;

    class iterator {
    private:
        Node<T>* current;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit iterator(Node<T>* ptr) : current(ptr) {}

        reference operator*() const { return current->data; }
        pointer operator->() const { return &(current->data); }
        
        iterator& operator++() {
            current = current->next;
            return *this;
        }
        
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        bool operator==(const iterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

public:
    using iterator = iterator;

    explicit singly_linked_list(std::pmr::memory_resource* resource = std::pmr::get_default_resource())
        : alloc(resource) {}

    ~singly_linked_list() {
        clear();
    }

    singly_linked_list(const singly_linked_list&) = delete;
    singly_linked_list& operator=(const singly_linked_list&) = delete;

    singly_linked_list(singly_linked_list&& other) noexcept
        : head(other.head), tail(other.tail), alloc(other.alloc) {
        other.head = other.tail = nullptr;
    }

    void push_back(const T& value) {
        Node<T>* new_node = alloc.allocate(1);
        try {
            alloc.construct(new_node, value, nullptr);
        } catch (...) {
            alloc.deallocate(new_node, 1);
            throw;
        }

        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    void push_back(T&& value) {
        Node<T>* new_node = alloc.allocate(1);
        try {
            alloc.construct(new_node, std::move(value), nullptr);
        } catch (...) {
            alloc.deallocate(new_node, 1);
            throw;
        }

        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            alloc.destroy(temp);
            alloc.deallocate(temp, 1);
        }
        tail = nullptr;
    }

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }

    bool empty() const { return head == nullptr; }

    T& front() { 
        return head->data; 
    }

    std::size_t size() {
        std::size_t count = 0;
        for (auto it = begin(); it != end(); ++it) {
            ++count;
        }
        return count;
    }

    void print() {
        for (auto it = begin(); it != end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
};