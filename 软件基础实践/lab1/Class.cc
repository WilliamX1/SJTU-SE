#include "Class.h"
#include <string>
#include "Student.h"

void Class::addStudent(const Student &st)
{
    StudentWrapper sw(st.id, st);
    students.push_back(sw);
}

StudentWrapper &Class::getStudentWrapper(const std::string &studentId)
{
    std::vector<StudentWrapper>::iterator it = students.begin();
    while (it != students.end())
    {
        if (it->id == studentId)
            return *it;
        it++;
    }
    // if the student not in this class, return nullptr
    StudentWrapper *opps = nullptr;
    return *opps;
}

double Class::getAvgScore()
{
    // TODO: implement getAvgScore.
    double sum = 0.0;  //学生总分
    int students_num = 0;  //课程人数

    for (std::vector<StudentWrapper>::iterator it = students.begin();
        it != students.end();
         ++it){
        sum += it->getScore();
        students_num++;
    }

    if (students_num) return sum / students_num;
    return 0.0;
}
