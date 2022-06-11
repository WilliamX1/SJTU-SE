#ifndef COMPOUNDEXP_H
#define COMPOUNDEXP_H

#include "Expression.h"

class CompoundExp : public Expression {

public:
    CompoundExp(QString str);

    /*override*/
    bool checkUserInputValid(QString& errorName);
    /*override*/
    ~CompoundExp();
};

#endif // COMPOUNDEXP_H
