#include <fstream>
#include <iostream>
#include "Buffer.h"

//TODO: your code here
//implement the functions in ListBuffer

Buffer::Buffer() {}

Buffer::~Buffer() {}

void Buffer::writeToFile(const string &filename) const {}

void Buffer::showLines(int from, int to) const {}

void Buffer::deleteLines(int from, int to){}

void Buffer::insertLine(const string &text){}

void Buffer::appendLine(const string &text){}

const string &Buffer::moveToLine(int idx) const { }
