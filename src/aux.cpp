//
// Created by 葛鑫 on 2019-06-12.
//

// struct and vector for record attractant B release
#include <time.h>
#include <iostream>
#include <string.h>
#include <random>
#include <cmath>
#include "../inc/aux.h"


double rand_double(double _min, double _max) {
    double coe = (double) rand() / (double) RAND_MAX;
    return (_max - _min) * coe + _min;
}

double dist(double _x1, double _y1, double _x2, double _y2) {
    return sqrt(abs(_x1 - _x2) * abs(_x1 - _x2) + abs(_y1 - _y2) * abs(_y1 - _y2));
}

std::string getCurrentTimeStr() {
    time_t t = time(nullptr);
    char ch[100] = {0};
    strftime(ch, sizeof(ch) - 1, "%Y-%m-%d-%H-%M-%S", localtime(&t));     //年-月-日 时-分-秒
    std::string ret;
    int idx = 0;
    while (ch[idx] != 0)
        ret.append(1, ch[idx++]);
    return ret;
}

void msg(std::string _s) {
    std::cout << "[INFO] " << _s << std::endl;
}

