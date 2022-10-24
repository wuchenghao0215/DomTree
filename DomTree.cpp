//
// Created by Wu Chenghao on 2022/10/7.
//

#include "DomTree.h"
#include <queue>
#include <set>
#include <stack>
#include <sstream>
#include <utility>
#include <regex>
#include <iostream>

using namespace std;

DomTree::DomTree() : root(new Node), is_legal(false) {}

DomTree::DomTree(string c) : root(new Node), content(std::move(c)), is_legal(false) {
    set<string> unclosed_tag = {"br", "hr", "base", "meta", "img", "input", "link"};
    stack<Node *> nodes;
    string tag, element, type;
    bool is_tag;
    for (char i: content) {
        if (i == '<') {
            is_tag = true;
            if (!element.empty()) {
                size_t n = element.find_last_not_of(" \n\r\t");
                if (n != string::npos) {
                    element.erase(n + 1, element.size() - n);
                }
                n = element.find_first_not_of(" \n\r\t");
                if (n != 0) {
                    element.erase(0, n);
                }
            }
            if (!element.empty()) {
                auto *node = new Node("IText", "", "", element);
                if (!nodes.empty()) {
                    node->parent = nodes.top();
                } else {
                    node->parent = root;
                }
                if (node->parent->child == nullptr) {
                    node->parent->child = node;
                } else {
                    Node *p = node->parent->child;
                    while (p->brother != nullptr) {
                        p = p->brother;
                    }
                    p->brother = node;
                }
                element.clear();
            }
        } else if (i == '>') {
            is_tag = false;
            istringstream ss(tag);
            ss >> type;
            if (type[0] == '!') {   // ignore commentary
                tag.clear();
                continue;
            }
            if (unclosed_tag.count(type) != 0) {    // if the tag doesn't need closing
                auto *node = new Node(type, tag);
                if (!nodes.empty()) {
                    node->parent = nodes.top();
                } else {
                    node->parent = root;
                }
                if (node->parent->child == nullptr) {
                    node->parent->child = node;
                } else {
                    Node *p = node->parent->child;
                    while (p->brother != nullptr) {
                        p = p->brother;
                    }
                    p->brother = node;
                }
            } else if (type[0] != '/') {    // if the tag need closing and is an opening tag
                auto *node = new Node(type, tag);
                if (!nodes.empty()) {
                    node->parent = nodes.top();
                } else {
                    node->parent = root;
                }
                if (node->parent->child == nullptr) {
                    node->parent->child = node;
                } else {
                    Node *p = node->parent->child;
                    while (p->brother != nullptr) {
                        p = p->brother;
                    }
                    p->brother = node;
                }
                nodes.push(node);
                element.clear();
            } else {    // if the tag is a closing tag
                string t_type = type.substr(1);
                if (t_type != nodes.top()->type) {  // illegal
                    is_legal = false;
                    Node *p = nodes.top();
                    stack<Node *> error_nodes;
                    while (p != root) {
                        error_nodes.push(p);
                        p = p->parent;
                    }
                    while (!error_nodes.empty()) {
                        error_message.push_back('/');
                        error_message.append(error_nodes.top()->type);
                        error_nodes.pop();
                    }
                    break;
                } else {
                    nodes.top()->closing_tag = tag;
                    nodes.pop();
                }
            }
            tag.clear();
        } else if (is_tag) {
            tag.push_back(i);
        } else {
            element.push_back(i);
        }
    }
    if (nodes.empty()) {
        is_legal = true;
    }
}

DomTree::~DomTree() {
    queue<Node *> q;
    q.push(root);
    while (!q.empty()) {
        if (q.front()->child != nullptr) {
            q.push(q.front()->child);
        }
        if (q.front()->brother != nullptr) {
            q.push(q.front()->brother);
        }
        delete q.front();
        q.pop();
    }
}

bool DomTree::check() const {
    return is_legal;
}

string DomTree::get_error_message() const {
    return error_message;
}

void DomTree::print_outer_html(string &xpath) const {
    if (xpath[0] == '/') {
        xpath.erase(0, 1);
    }
    if (!current_xpath.empty()) {
        xpath = current_xpath + '/' + xpath;
    }
    if (xpath.empty()) {
        xpath = "html";
    }
    regex pattern("/");
    sregex_token_iterator begin(xpath.begin(), xpath.end(), pattern, -1), end;
    vector<string> path_layer = {begin, end};
    Node *p = root;
    queue<Node *> q;
    q.push(p);
    for (const auto &i: path_layer) {
        size_t n = q.size();
        for (int j = 0; j < n; ++j) {
            p = q.front()->child;
            q.pop();
            while (p != nullptr) {
                if (p->type == i) {
                    q.push(p);
                }
                p = p->brother;
            }
        }
        if (q.empty()) {
            cout << "xpath does not exist" << endl;
            cout << xpath << endl;
            break;
        }
    }
    while (!q.empty()) {
        p = q.front();
        p->print_outer_html(0);
        q.pop();
    }
}

void DomTree::print_text(string &xpath) const {
    if (xpath[0] == '/') {
        xpath.erase(0, 1);
    }
    if (!current_xpath.empty()) {
        xpath = current_xpath + xpath;
    }
    if (xpath.empty()) {
        xpath = "html";
    }
    regex pattern("/");
    sregex_token_iterator begin(xpath.begin(), xpath.end(), pattern, -1), end;
    vector<string> path_layer = {begin, end};
    Node *p = root;
    queue<Node *> q;
    q.push(p);
    for (const auto &i: path_layer) {
        size_t n = q.size();
        for (int j = 0; j < n; ++j) {
            p = q.front()->child;
            q.pop();
            while (p != nullptr) {
                if (p->type == i) {
                    q.push(p);
                }
                p = p->brother;
            }
        }
        if (q.empty()) {
            cout << "xpath does not exist" << endl;
            cout << xpath << endl;
            break;
        }
    }
    while (!q.empty()) {
        p = q.front();
        p->print_text();
        q.pop();
    }
}

void DomTree::cd(std::string &xpath) {
    if (xpath[0] == '/') {
        xpath.erase(0, 1);
    }
    if (xpath.empty()) {
        return;
    }
    regex pattern("/");
    sregex_token_iterator begin(xpath.begin(), xpath.end(), pattern, -1), end;
    vector<string> path_layer = {begin, end};
    Node *p = root;
    queue<Node *> q;
    q.push(p);
    for (const auto &i: path_layer) {
        size_t n = q.size();
        for (int j = 0; j < n; ++j) {
            p = q.front()->child;
            q.pop();
            while (p != nullptr) {
                if (p->type == i) {
                    q.push(p);
                }
                p = p->brother;
            }
        }
        if (q.empty()) {
            cout << "xpath does not exist" << endl;
            cout << xpath << endl;
            return;
        }
    }
    for (const auto &i: path_layer) {
        current_xpath += i + '/';
    }
}

void DomTree::ls() const {

}