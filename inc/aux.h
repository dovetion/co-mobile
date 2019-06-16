//
// Created by 葛鑫 on 2019-06-12.
//

#ifndef CO_MOBILE_AUX_H
#define CO_MOBILE_AUX_H

#endif //CO_MOBILE_AUX_H


// struct and vector for record attractant B release
struct r_B {
    double x, y;
    int t, id;

    r_B(double _x, double _y, int _t, int _id) : x(_x), y(_y), t(_t), id(_id) {}
};

std::string getCurrentTimeStr();

void msg(std::string _s);

double rand_double(double _min, double _max);

double dist(double _x1, double _y1, double _x2, double _y2);