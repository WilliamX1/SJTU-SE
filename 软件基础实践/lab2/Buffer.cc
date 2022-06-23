#include <fstream>
#include <iostream>
#include "Buffer.h"

//TODO: your code here
//implement the functions in ListBuffer

Buffer::Buffer() {
    head = new node("This line is a sentinel!");
    currentLineNum = totalLineNum = 0;
}

Buffer::~Buffer() {
    node* tmp = head, *cur;
    while(tmp != NULL){
        cur = tmp->next;
        delete tmp;
        tmp = cur;
    }
}

void Buffer::writeToFile(const string &filename) {

    std::fstream File(filename, std::ios::out | std::ios::trunc);

    node* cur = head;
    for (int i = 1; i <= totalLineNum; i++)
    {
        cur = cur->next;
        File << cur->content << '\n';
    }

    //***************************************奇怪***************************************
    //直接用File.tellg()不行??
    int filelen = File.tellg();
    printf("%d byte(s) written\n", filelen);

    File.close();
    return;
}

void Buffer::showLines(int from, int to, bool flag){
    if (flag)
    {
        if (from == 1)
            to = totalLineNum;
        else throw "Bad/Unknown command";
    }
    if (from > to)
        throw "Number range error";
    if (to > totalLineNum || from <= 0) throw "Line number out of range";


    node* cur = head;
    for (int i = 1; i <= from; i++) cur = cur->next;

    for (int i = from; i <= to; i++)
    {
        std::cout << i << '\t' << cur->content << std::endl;
        cur = cur->next;
    }
    currentLineNum = to;
    return;
}

void Buffer::deleteLines(int from, int to){
    if (from > to)
        throw "Delete range error";
    if (to > totalLineNum || from <= 0) throw "Line number out of range";


    node* cur1 = head;
    for (int i = 1; i < from; i++) cur1 = cur1->next;
    node* tmp = cur1, *cur2 = cur1->next;
    for (int i = from; i <= to; i++)
    {
        tmp = cur2;
        cur2 = cur2->next;
        delete tmp;
    }
    cur1->next = cur2;

    if (to == totalLineNum) currentLineNum = from - 1;
    else currentLineNum = from;
    totalLineNum -= to - from + 1;

    return;
}

void Buffer::insertLine(const string &text){
    //if (text == "") return;

    node* cur = head;
    for (int i = 1; i < currentLineNum; i++)
        cur = cur->next;
    cur->next = new node(text, cur->next);
    currentLineNum++;
    totalLineNum++;
    return;
}

void Buffer::appendLine(const string &text){
    //if (text == "") return;

    node* cur = head;
    for (int i = 1; i <= currentLineNum; i++) cur = cur->next;
    cur->next = new node(text, cur->next);
    currentLineNum++;
    totalLineNum++;
    return;
}

const string &Buffer::moveToLine(int idx) {
    if (idx < 0 || idx > totalLineNum)
        throw "Line number out of range";

    currentLineNum = idx;
    node* cur = head;
    while(idx--) cur = cur->next;
    return cur->content;
}
