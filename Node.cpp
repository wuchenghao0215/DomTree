//
// Created by Wu Chenghao on 2022/10/8.
//

#include "Node.h"
#include <utility>
#include <iostream>

using namespace std;

Node::Node() : parent(nullptr), child(nullptr), brother(nullptr) {}

Node::Node(std::string t, std::string o) :
        type(std::move(t)), opening_tag(std::move(o)),
        parent(nullptr), child(nullptr), brother(nullptr) {}

Node::Node(std::string t, std::string o, std::string c, std::string e) :
        type(std::move(t)), opening_tag(std::move(o)), closing_tag(std::move(c)), element(std::move(e)),
        parent(nullptr), child(nullptr), brother(nullptr) {}

void Node::print_outer_html(int indent) const {
    for (int i = 0; i < 4 * indent; ++i) {
        cout << ' ';
    }
    if (!opening_tag.empty()) {
        cout << '<' << opening_tag << '>' << endl;
    } else {    // is iText
        cout << element << endl;
        return;
    }
    if (!closing_tag.empty()) { // has child(ren)
        Node *p = child;
        while (p != nullptr) {
            p->print_outer_html(indent + 1);
            p = p->brother;
        }
        for (int i = 0; i < 4 * indent; ++i) {
            cout << ' ';
        }
        cout << '<' << closing_tag << '>' << endl;
    }
}

void Node::print_text() const {
    if (opening_tag.empty()) {  // iText
        cout << element << endl;
    } else {
        Node *p = child;
        while (p != nullptr) {
            p->print_text();
            p = p->brother;
        }
    }
}