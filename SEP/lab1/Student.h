#ifndef STUDENT_H_
#define STUDENT_H_

#include <string>
#include <vector>

class Class;

enum Degree {
    undergraduate,
    graduate
};

class Student {
    // TODO: implement class Student.
private:

protected:

public:
    const std::string id;
    std::string toString() const;
};

// TODO: implement class Graduate.

// TODO: implement class Undergraduate.

class StudentWrapper {
private:
    const Student &student;
    double score = 0.0;
public:
    const std::string id;
    // TODO: fix error
    StudentWrapper(const std::string &id, const Student &student) {
        this->id = id;
        this->student = student;
    }

    void setScore(double score)
    {
        if (score < 0 || score > 100)
            throw "Invalid Score!";
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
