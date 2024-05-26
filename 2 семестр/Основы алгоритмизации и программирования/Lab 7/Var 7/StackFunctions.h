#pragma once

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

template<typename T>
struct Node {
    T data;
    Node<T>* next;
    Node(T val) : data(val), next(nullptr) {}
};

template<typename T>
class Stack {
private:
    Node<T>* top;

public:
    Stack();
    ~Stack();
    void push(T val);
    void pop();
    T peek();
    bool isEmpty();
    void clear();
    void saveToFile(const std::string& filename);
    void readFromFile(const std::string& filename);
    int countDuplicates();
    void print();
};
