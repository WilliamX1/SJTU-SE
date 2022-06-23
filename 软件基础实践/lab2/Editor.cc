#include <iostream>
#include <sstream>
#include "Editor.h"

using namespace std;

Editor::Editor()
{
    buffer = new Buffer();
}
Editor::~Editor()
{
    // TODO: Implement destructor
    buffer->~Buffer();
    delete buffer;
}

void Editor::run()
{
    string cmd;
    while (true)
    {
        cout << "cmd> ";
        cout.flush();
        getline(cin, cmd);
        if (cmd == "Q")
            break;
        if (cmd == "") continue;


        try {
            dispatchCmd(cmd);
        } catch (const char *e) {
            cout << "? " << e << endl;
        } catch (const out_of_range &oor) {
            cout << "? " << oor.what() << endl;
        } catch (const range_error &re) {
            cout << "? " << re.what() << endl;
        }
    }
}
void Editor::cmdAppend()
{
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    // TODO: finish cmdAppend.
    string str;

    while(1){
        getline(cin, str, '\n');
        if (str == ".") break;

        buffer->appendLine(str);
    }

    return;
}

void Editor::cmdInsert()
{
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    bool firstLine = true;
    while (true)
    {
        string text;
        getline(cin, text, '\n');
        if (text == ".") break;

        if (firstLine) {
            buffer->insertLine(text);
            firstLine = false;
        }  else {
            buffer->appendLine(text);
        }
    }
}

void Editor::cmdDelete(int start, int end)
{
    buffer->deleteLines(start, end);
}

void Editor::cmdNull(int line)
{
    cout << buffer->moveToLine(line) << endl;
}

void Editor::cmdNumber(int start, int end, bool flag)
{
    buffer->showLines(start, end, flag);
}

void Editor::cmdWrite(const string &filename)
{
    buffer->writeToFile(filename);
}

void Editor::dispatchCmd(const string &cmd)
{
    if (cmd == "a") {
        cmdAppend();
        return;
    }
    if (cmd == "i") {
        cmdInsert();
        return;
    }
    if (cmd[0] == 'w') {
        // TODO: call cmdWrite with proper arguments
        if (cmd.length() == 1 || cmd[2] == ' ' || cmd[2] == '\n' || cmd[2] == '\0')
            throw "Filename not specified";
        if (cmd[1] != ' ') throw "Bad/Unknown command";

        cmdWrite(cmd.substr(2));
        return;
    }
    // TODO: handle special case "1,$n".
    int start, end;
    char comma, type, dollar;
    stringstream ss(cmd);
    ss >> start;
    if (ss.eof()) {
        cmdNull(start);
        return;
    }

    //*************
    ss >> comma >> end >> type;
    if (comma != ',') throw "Bad/Unknown command";

    if (ss.good()) {
        if (type == 'n') {
            cmdNumber(start, end);
            return;
        } else if (type == 'd') {
            cmdDelete(start, end);
            return;
        }
    }
    else{
        ss.clear();
        ss << cmd;
        ss >> dollar >> type;
        if (dollar == '$' && type == 'n'){
            cmdNumber(start, 0, true);
            return;
        }
    }
    throw "Bad/Unknown command";
}
