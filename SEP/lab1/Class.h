#ifndef CLASS_H_
#define CLASS_H_

#include "Student.h"
#include <string>
#include <vector>

class Class {
private:
    std::vector<StudentWrapper> students;

public:
    const std::string name;
    const int point;
    Class(const std::string &name, const int point)
        : name(name), point(point) { }

    void addStudent(const Student &st);
    StudentWrapper &getStudentWrapper(const std::string &studentId);
    double getAvgScore();
};

#endif // CLASS_H_
