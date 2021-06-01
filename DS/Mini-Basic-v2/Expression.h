#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QString>

enum ExpCases {
    Compound, Constant, Identifier, String
};

class Expression {
public:
    QString _contents;
    Expression(QString str): _contents(str){};
    virtual bool checkUserInputValid(QString& errorName) = 0;
    virtual ~Expression(){};
    static ExpCases whichExpMightBe(QString str);
};

#endif // EXPRESSION_H
