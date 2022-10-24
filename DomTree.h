//
// Created by Wu Chenghao on 2022/10/7.
//

#ifndef ASSIGNMENT2_DOMTREE_H
#define ASSIGNMENT2_DOMTREE_H


#include "Node.h"
#include <string>

class DomTree {
public:
    DomTree();

    explicit DomTree(std::string c);

    ~DomTree();

    bool check() const;

    std::string get_error_message() const;

    std::string get_real_xpath(std::string &xpath) const;

    void print_outer_html(std::string &xpath) const;

    void print_text(std::string &xpath) const;

    void cd(std::string &xpath);

    void ls() const;

private:
    Node *root;
    std::string content;
    bool is_legal;
    std::string error_message;
    std::string current_xpath;
};


#endif //ASSIGNMENT2_DOMTREE_H
