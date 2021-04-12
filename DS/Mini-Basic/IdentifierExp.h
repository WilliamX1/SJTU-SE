#ifndef IDENTIFIEREXP_H
#define IDENTIFIEREXP_H

#include "Expression.h"

class IdentifierExp: public Expression {
private:
    QString _var;
public:
    IdentifierExp(QString str);
    /*override*/
    bool checkUserInputValid(QString& errorName);
    /*override*/
    ~IdentifierExp();
};

#endif // IDENTIFIEREXP_H
