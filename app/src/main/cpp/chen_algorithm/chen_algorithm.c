//
// Created by CHENGUANG491 on 2018/8/14.
//
#include "chen_algorithm.h"

//数学次方计算
double power(double base, int exponent) {
    int result = base;
    int i;
    if (exponent == 0) {
        return 1;
    }
    for (i = 1; i < exponent; ++i) {
        result = result * base;
    }
    return result;
}

//数学加法计算
double addFunc(double a, double b) {
    return a + b;
}