#include "DomTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>

using namespace std;

int main() {
    ifstream html_doc;
    while (!html_doc.is_open()) {
        cout << "please enter file path or url > ";
        string path_or_url;
        cin >> path_or_url;
        if (path_or_url[0] == '/') {    // if the input is a local path
            html_doc.open(path_or_url);
            if (!html_doc.is_open()) {
                cout << "file does not exist, please try again" << endl;
            }
        } else if (path_or_url.substr(0, 4) == "http") {    // if the input is a url
            cout << "url-parser still undone" << endl;
            // parse a url
            // TO DO
        } else {
            cout << "illegal input, please try again" << endl;
        }
    }
    stringstream buffer;
    buffer << html_doc.rdbuf();
    string html_content = buffer.str(); // read all
    html_doc.close();

    DomTree tree(html_content);
    int command = 0;
    string command_line;
    getline(cin, command_line);
    string xpath;
    map<string, int> command_map = {
            {"CheckHTML", 0},
            {"OutHTML", 1},
            {"Text", 2},
            {"Exit", 3}
    };
    while (command != 3) {  // select function
        cout << "please enter command > ";
        getline(cin, command_line);
        size_t p = command_line.find(' ');
        if (p != string::npos) {
            xpath = command_line.substr(p + 1);
            command_line.erase(p, command_line.size() - p);
        } else {
            xpath.clear();
        }
        if (command_map.count(command_line) > 0) {
            command = command_map[command_line];
        } else {
            command = 4;
        }
        switch (command) {
            case 0: {
                bool check = tree.check();
                if (!check) {
                    command = 3;
                    cout << "html is illegal" << endl
                         << "error position: " << tree.get_error_message() << endl;
                } else {
                    cout << "html is legal" << endl;
                }
                break;
            }
            case 1: {
                tree.print_outer_html(xpath);
                break;
            }
            case 2: {
                tree.print_text(xpath);
                break;
            }
            case 3:
                cout << "quit successfully" << endl;
                break;
            default:
                cout << "undefined command, please try again" << endl;
                break;
        }
    }

    return 0;
}
