//
// Created by Wu Chenghao on 2022/10/8.
//

#ifndef ASSIGNMENT2_NODE_H
#define ASSIGNMENT2_NODE_H


#include <string>

class Node {
public:
    friend class DomTree;

    Node();

    Node(std::string t, std::string o);

    Node(std::string t, std::string o, std::string c, std::string e);

    void print_outer_html(int indent) const;

    void print_text() const;

private:
    std::string type;
    std::string opening_tag;
    std::string closing_tag;
    std::string element;
    Node *parent;
    Node *child;
    Node *brother;
};


#endif //ASSIGNMENT2_NODE_H
