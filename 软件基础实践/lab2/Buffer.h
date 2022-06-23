#pragma once

#include <string>

using std::string;

class Buffer {
private:
    int currentLineNum;
    int totalLineNum;
    // TODO: add a List here for storing the input lines
    struct node{
        string content;
        node* next;
        node(string s = "", node* n = NULL):content(s), next(n){};
    };
    node* head;
public:
    Buffer();
    ~Buffer();

    void writeToFile(const string &filename);

    const string &moveToLine(int idx);

    void showLines(int from, int to, bool flag);

    void deleteLines(int from, int to);
    void insertLine(const string &text);
    void appendLine(const string &text);
};
