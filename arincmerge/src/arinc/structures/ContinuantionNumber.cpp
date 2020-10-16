#include "ContinuantionNumber.h"

ContinuantionNumber::ContinuantionNumber(char c) : value(c)
{}

char ContinuantionNumber::next()
{
    if ('9' == value)
        value = 'A';
    else
        ++value;
    return value;
}
