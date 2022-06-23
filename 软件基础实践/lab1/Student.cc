#include "Student.h"

#include <string>
#include <sstream>
#include "Class.h"

std::string Student::toString() const
{
    // TODO: uncomment the following code after implementing class Student.
    std::ostringstream oss;
    oss << "Student Information:"
        << "\n\tid: " << id
        << "\n\tname: " << name
        << "\n\tenrollment year: " << year
        << "\n\tdegree: " << (degree == graduate ? "graduate" : "undergraduate")
        << std::endl;
    return oss.str();
    return "";
}

// TODO: implement functions which are declared in Student.h.

void Student::addClass(Class* c){
    classes.push_back(c);
    return;
}

double Undergraduate::getGrade(){
    double score_sum = 0.0;
    int classes_num = 0;

    for (std::vector<Class *>::iterator it = classes.begin();
         it != classes.end();
         it++){

        if (&(*it)->getStudentWrapper(id) == nullptr)
            continue;

        classes_num += (*it)->point;

        score_sum += (*it)->point * (*it)->getStudentWrapper(id).getScore() / 20;
    }

    if (classes_num) return score_sum / classes_num;
    return 0.0;
}

double Graduate::getGrade(){
    double score_sum = 0.0;
    int classes_sum = 0;

    for (std::vector<Class *>::iterator it = classes.begin();
         it != classes.end();
         it++){

        if (&(*it)->getStudentWrapper(id) == nullptr)
            continue;

        classes_sum += (*it)->point;

        switch(int((*it)->getStudentWrapper(id).getScore() / 10))
        {
        case 10: case 9: score_sum += (*it)->point * 4.0; break;
        case 8: score_sum += (*it)->point * 3.5; break;
        case 7: score_sum += (*it)->point * 3.0; break;
        case 6: score_sum += (*it)->point * 2.5; break;
        default: score_sum += (*it)->point * 2.0; break;
        }
    }

    if (classes_sum) return score_sum / classes_sum;
    return 0.0;
}
