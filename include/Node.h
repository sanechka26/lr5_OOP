#pragma once
#include <utility>

template <typename T>
struct Node {
    T data;
    Node* next;

    explicit Node(const T& d, Node* n = nullptr) : data(d), next(n) {}

    explicit Node(T&& d, Node* n = nullptr) : data(std::move(d)), next(n) {}
};