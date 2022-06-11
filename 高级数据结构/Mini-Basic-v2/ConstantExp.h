#ifndef CONSTANTEXP_H
#define CONSTANTEXP_H

#include "Expression.h"

class ConstantExp: public Expression {
private:
    QString _var;
public:
    ConstantExp(QString str);
    /*override*/
    bool checkUserInputValid(QString& errorName);
    /*override*/
    ~ConstantExp();
};

#endif // CONSTANTEXP_H
