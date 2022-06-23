#include <vector>
#include <iostream>
#include <fstream>
#include "Class.h"
#include "Student.h"

using namespace std;

class AppX {
private:
    vector<Student *> studentVec;
    vector<Class *> classVec;

    void loadFiles();
    void inputScore();
    void printAvgScore();
    void printGpa();

public:
    AppX();
    ~AppX();
    int run();
};

AppX::~AppX()
{
    // You can use the traditional loop, which is more clear.
    for (vector<Class *>::iterator it = classVec.begin();
         it != classVec.end();
         ++it) {
        if (*it) delete (*it);
    }
    // You can use modern and simpler loops only if you know what it is doing.
    for (const auto &s: studentVec) {
        if (s) delete (s);
    }
}

AppX::AppX()
{
    loadFiles();
}

void AppX::loadFiles()
{
    string line;
    size_t pos1, pos2;
    vector<string> bufv;
    Student *st = nullptr;
    string clsname;
    int point;
    Class *cl = nullptr;

    // Open a file as a input stream.
    ifstream stfile("./Students.txt");
    if (!stfile) cout << "Students.txt not open!\n";

    while (getline(stfile, line)) {
        if (line.empty()) // It's an empty line.
            continue;
        if (line[0] == '#') // It's a comment line.
            continue;

        // The bufv vector stores each columnes in the line.
        bufv.clear();
        // Split the line into columes.
        //   pos1: begining the the column.
        //   pos2: end of the column.
        pos1 = 0;
        while (true) {
            pos2 = line.find(';', pos1 + 1);
            if (pos2 == string::npos) { // No more columns.
                // Save the last column (pos1 ~ eol).
                bufv.push_back(line.substr(pos1, string::npos));
                break;
            } else {
                // Save the column (pos1 ~ pos2).
                bufv.push_back(line.substr(pos1, pos2 - pos1));
            }
            pos1 = pos2 + 1;
        }

        // TODO: uncomment next lines after implementing class Undergraduate
        // and Graduate.
        if (bufv[3] == "U")
            st = new Undergraduate(bufv[0], bufv[1], bufv[2]);
        else if (bufv[3] == "G")
            st = new Graduate(bufv[0], bufv[1], bufv[2]);

        studentVec.push_back(st);
    }
    stfile.close();

    // TODO: load data from ./Classes.txt and push objects to the vector.
    // Hint: how is student information read?
    ifstream clfile("./Classes.txt");
    if (!clfile) cout << "Classes.txt not open!\n";

    while (getline(clfile, line)){
        if (line.empty())  // It's an empty line.
           continue;
        if (line[0] == '#') // It's a comment line
           continue;

        pos1 = line.find(':', 0);
        pos2 = line.find(':', pos1 + 1);
        clsname = line.substr(pos1 + 1, pos2 - pos1);

        getline(clfile, line);
        pos1 = line.find(':', 0);
        point = line[pos1 + 1] - '0';   // class point no more than 9

        cl = new Class(clsname, point);
        classVec.push_back(cl);

        while (getline(clfile, line)){
            if (line.empty())
                break;
            for (std::vector<Student *>::iterator it = studentVec.begin();
                 it != studentVec.end();
                 it++){
                if ((*it)->id == line)
                {
                    st = *it;
                    break;
                }
            }
            if (st == nullptr) throw("Student not find!");
            cl->addStudent(*st);
            st->addClass(cl);
        }
    }
    clfile.close();
}

void AppX::inputScore()
{
    // TODO: implement inputScore.
    // Hint: Take a look at printAvgScore().
    string get_score = ""; // to pre-handle the score to avoid that wrong input makes program crash.
    double score = 0.0;
    string sbuf;
    string student_id;
    Class *cl;
    StudentWrapper *st;

    while(true){
        flag1:;  // to symbolize this position;
        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        cl = nullptr;
        for (vector<Class *>::iterator it = classVec.begin();
            it != classVec.end();
            ++it){
            if ((*it)->name == sbuf){
                cl = *it;
                break;
            }
        }

        if (cl == nullptr)
        {
            cout << "No match class!\n";
            goto flag1;
        }

        flag2:;
        cout << "Please input the student id (or input q to quit): ";
        cin >> student_id;
        if (student_id == "q")
            goto flag1;

        st = &cl->getStudentWrapper(student_id);
        if (st == nullptr)
        {
            cout << "No match student!" << endl;
            goto flag2;
        }

        cout << st->toString(); //show the student basic information

        cout << "Please input the student score (or input q to quit): ";
        cin >> get_score;
        if (get_score == "q")
            goto flag2;

        if (get_score == "100") score = 100;  //judge 100 especially
        else if (get_score.length() == 1 && '0' <= get_score[0] && get_score[0] <= '9') score = get_score[0] - '0';
        else if (get_score.length() == 2 && '0' <= get_score[0] && get_score[0] <= '9'
                 && '0' <= get_score[1] && get_score[1] <= '9') score = (get_score[0] -'0') * 10 + (get_score[1] - '0');
        else {
            cout << "Wrong score!\n";
            goto flag2;
        }

        st->setScore(score);
        goto flag2;
        //cout << "Input score successfully!" << endl;
    }
}

void AppX::printAvgScore()
{
    string sbuf;
    Class *cl;

    while (true) {
        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        cl = nullptr;
        for (vector<Class *>::iterator it = classVec.begin();
             it != classVec.end();
             ++it) {
            if ((*it)->name == sbuf) {
                cl = *it;
                break;
            }
        }
        if (cl == nullptr) {
            cout << "No match class!\n";
            continue;
        }

        printf( "The average score is: %.2f\n", cl->getAvgScore());
    }
}

void AppX::printGpa()
{
    // TODO: implement printGpa.
    // Hint: Take a look at printAvgScore().
    string student_id;
    Student *st;

    while (true) {
        cout << "Please input the student id (or input q to quit): ";
        cin >> student_id;
        if (student_id == "q")
            break;

        st = nullptr;
        for (vector<Student *>::iterator it = studentVec.begin();
             it != studentVec.end();
             ++it){
            if ((*it)->id == student_id){
                st = *it;
                break;
            }
        }
        if (st == nullptr)
        {
            cout << "No match student!\n";
            continue;
        }

        st->toString();
        printf("GPA = %.2f\n", st->getGrade());
    }

}

int AppX::run()
{
    char cmd;
    while (true) {
        cout << "Command menu:\n"
            << "\ti: Input score\n"
            << "\ta: Compute average score of a class\n"
            << "\tg: Compute grade of a student\n"
            << "\tq: Quit\n"
            << "Please input the command: ";
        cin >> cmd;
        if (cmd == 'i') {
            inputScore();
        } else if (cmd == 'a') {
            printAvgScore();
        } else if (cmd == 'g') {
            printGpa();
        } else if (cmd == 'q') {
            break;
        } else {
            cout << "Invalid command!\n";
        }
    }
    return 0;
}

int main()
{
    AppX app;
    return app.run();
}
