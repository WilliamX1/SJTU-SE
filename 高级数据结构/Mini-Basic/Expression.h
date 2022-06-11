#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QString>

enum ExpCases {
    Compound, Constant, Identifier
};

class Expression {
protected:
    QString _contents;
public:
    Expression(QString str): _contents(str){};
    virtual bool checkUserInputValid(QString& errorName) = 0;
    virtual ~Expression(){};
    static ExpCases whichExpMightBe(QString str);
};

#endif // EXPRESSION_H
