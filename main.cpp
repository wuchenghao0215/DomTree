#include "DomTree.h"
#include "httplib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>

using namespace std;

int main() {
    string html_content;
    while (html_content.empty()) {
        cout << "please enter file path or url > ";
        string path_or_url;
        cin >> path_or_url;
        if (path_or_url[0] == '/') {    // if the input is a local path
            ifstream html_doc;
            html_doc.open(path_or_url);
            if (!html_doc.is_open()) {
                cout << "file does not exist, please try again" << endl;
            } else {
                stringstream buffer;
                buffer << html_doc.rdbuf();
                html_content = buffer.str(); // read all
                html_doc.close();
            }
        } else if (path_or_url.substr(0, 4) == "http") {    // if the input is a url
            httplib::Client cli(path_or_url);
            httplib::Headers headers;
            headers.insert({"Accept", "text/html,application/xhtml+xml,application/xml;"});
            headers.insert({"User-Agent", "Mozilla/5.0"});
            auto res = cli.Get("/", headers);
            if (res.error() == httplib::Error::Success) {
                html_content = res->body;
            } else {
                cout << "request denied, please try again" << endl;
            }
        } else {
            cout << "illegal input, please try again" << endl;
        }
    }

    DomTree tree(html_content);
    int command = 0;
    string command_line;
    getline(cin, command_line);
    string xpath;
    map<string, int> command_map = {
            {"CheckHTML", 0},
            {"OutHTML", 1},
            {"Text", 2},
            {"cd", 3},
            {"ls", 4},
            {"Exit", 5}
    };
    while (command != 5) {  // select function
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
                if (!tree.check()) {
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
            case 3: {
                tree.cd(xpath);
                break;
            }
            case 4: {
                tree.ls();
                break;
            }
            case 5: {
                cout << "quit successfully" << endl;
                break;
            }
            default: {
                cout << "undefined command, please try again" << endl;
            }
        }
    }

    return 0;
}
