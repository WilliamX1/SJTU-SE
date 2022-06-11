#ifndef STRINGEXP_H
#define STRINGEXP_H

#include "Expression.h"

class StringExp: public Expression
{
private:
    QString _var;
public:
    StringExp(QString str);
    bool checkUserInputValid(QString& errorName) override;
    ~StringExp() override;
};

#endif // STRINGEXP_H
