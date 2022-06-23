#ifndef STUDENT_H_
#define STUDENT_H_

#include <string>
#include <vector>
#include <iostream>

class Class;

enum Degree {
    undergraduate,
    graduate
};

class Student {
    // TODO: implement class Student.
private:
    const std::string name;
    const std::string year;
    const enum Degree degree;
protected:
    std::vector<Class *> classes;
public:
    const std::string id;
    Student(const std::string &id, const std::string name, const std::string year, enum Degree degree)
        :id(id), name(name), year(year), degree(degree){};
    virtual double getGrade() = 0;
    std::string toString() const;
    void addClass(Class* c);
};

// TODO: implement class Graduate.
class Undergraduate : public Student{
public:
    Undergraduate(const std::string &id, const std::string name, const std::string year)
        :Student(id, name, year, undergraduate){};
    double getGrade();
};
// TODO: implement class Undergraduate.
class Graduate : public Student{
public:
    Graduate(const std::string &id, const std::string name, const std::string year)
        :Student(id, name, year, graduate){};
    double getGrade();
};

class StudentWrapper {
private:
    const Student &student;
    double score = 0.0;
public:
    const std::string id;
    // TODO: fix error, already fixed!
    StudentWrapper(const std::string &id, const Student &student)
        : student(student),id(id){};

    void setScore(double score)
    {
        this->score = score;
    }

    double getScore() const
    {
        return this->score;
    }

    std::string toString() const
    {
        return student.toString();
    }
};

#endif // STUDENT_H_
