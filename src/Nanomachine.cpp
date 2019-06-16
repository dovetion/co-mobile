//
// Created by 葛鑫 on 2019-06-03.
//

#include "../inc/Nanomachine.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <cmath>
#include <random>
#include "../inc/aux.h"

#define NANOMACHINE_DEBUG 1

int Nanomachine::rand_seed = 6;
extern int B_timeout;
extern double B_rangeout;
extern int now;


const double time_losing_factor = 1.0;
const double dist_losing_factor = 1.0;
const double delta_step = 2.0;
const double dist_threashold = 50.0;
const double single_step = 1.0;
const double A_threashold = A_CONCENTRATION_MAX / 700.0;
const double B_threashold = B_CONCENTRATION_MAX / 300.0;
const double MAX_X = 2000.0;
const double MAX_Y = 2000.0;
const double direction_coe = 4.0; // must greater or equal to 1.0
const int num_direction = 12;
const double PI_2 = PI * 2.0;

std::vector<r_B> B_log;

double Nanomachine::getA(double _x, double _y) const {
    double distance = dist(0.0, 0.0, _x, _y);
    // in target area
    if (distance <= dist_threashold) {
        return -1.0;
    }
    return A_CONCENTRATION_MAX / (distance);
}

double Nanomachine::getB(double _x, double _y) const {
    double ret = 0.0;
    for (std::vector<r_B>::reverse_iterator it = B_log.rbegin(); it != B_log.rend(); ++it) {
        r_B &p = (*it);

        // it's too early to calculate that release
        if (now - p.t > B_timeout)
            break;
        if (now - p.t == 0)
            continue;

        // it's too far to calculate that release
        double distance = dist(_x, _y, p.x, p.y);
        if (distance > B_rangeout)
            continue;

        ret += time_losing_factor * 1.0 / abs(now - p.t) * dist_losing_factor / (distance * B_CONCENTRATION_MAX);
    }

    return ret;
}

void Nanomachine::move() {
    double C_A = getA(this->X, this->Y);
    double C_B = co_mobile ? getB(this->X, this->Y) : 0.0;


    double cur_angle = 0.0;
    double angle_step = 2.0 * PI / num_direction;
    double max_angle = 0.0;
    double max_concentration = -1.0;

    // if detect A with enough concentration, then stop
    // DETECTED AB : 3
    if (C_A < 0.0) {
        this->type = DETECTED_AB;
        return;
    }


    // if not detected both A and B, random walk
    // RANDOM WALK : 00
    else if (C_A > 0.0 && C_A < A_threashold && C_B < B_threashold) {
        this->type = RANDOMWALK;
        this->angle += rand_double(-PI / direction_coe, PI / direction_coe);
        X += delta_step * cos(this->angle);
        Y += delta_step * sin(this->angle);
    }


    // then if detected A, heading to target area without minding B
    // DETECTED A : 1
    else if (C_A >= A_threashold) {
        this->type = DETECTED_A;
        if (this->co_mobile && now - B_timeout >= lastReleaseTimeofB) {
            this->release_B(X, Y);
            setLastReleaseTimeofB(now);
        }
        double dis = dist(0, 0, X, Y);

        X += single_step * (-X) / dis;
        Y += single_step * (-Y) / dis;

#ifdef  NANOMACHINE_DEBUG
        printf("[DEBUG] heading max A : angle = %5.3f\n", this->angle);
#endif
    }


        // then if detected only B, heading to place where the concentration of B is most higher
    // DETECTED B : 2
    else if (C_B >= B_threashold) {
        this->type = DETECTED_B;
        for (int i = 0; i < num_direction; i++) {
            double delta_x = X + single_step * cos(cur_angle);
            double delta_y = Y + single_step * sin(cur_angle);
            double temp_B = getB(delta_x, delta_y);
            if (temp_B >= max_concentration) {
                max_concentration = temp_B;
                max_angle = cur_angle;
            }
            cur_angle += angle_step;

        }
        // release attractant B
        if (this->co_mobile && max_concentration < B_threashold) {
            this->release_B(X, Y);
            setLastReleaseTimeofB(now);
        }

        // find the best direction
        this->angle = max_angle;
        X += single_step * cos(this->angle);
        Y += single_step * sin(this->angle);

#ifdef  NANOMACHINE_DEBUG
        printf("[DEBUG] heading max B : angle = %5.3f\n", cur_angle);
#endif
    }


    // bounded environment
    if (this->X > MAX_X)
        this->X = 2.0 * MAX_X - this->X;
    else if (this->X < -MAX_X)
        this->X = -2.0 * MAX_X - this->X;

    if (this->Y > MAX_Y)
        this->Y = 2.0 * MAX_Y - this->Y;
    else if (this->Y < -MAX_Y)
        this->Y = -2.0 * MAX_Y - this->Y;
}

//TODO : need to be more accurate
void Nanomachine::release_B(double _x, double _y) {
    r_B tmp(_x, _y, now, ID);
    B_log.push_back(tmp);
    printf("[INFO] [%05d] Nanomachine %02d at [%4.3f, %4.3f] released B\n", now, ID, X, Y);
}

double Nanomachine::getAngle() const {
    return angle;
}

double Nanomachine::getSpeed() const {
    return speed;
}

int Nanomachine::getId() const {
    return ID;
}

double Nanomachine::getX() const {
    return X;
}

double Nanomachine::getY() const {
    return Y;
}

int Nanomachine::getType() const {
    return type;
}

void Nanomachine::setAngle(double angle) {
    Nanomachine::angle = angle;
}

void Nanomachine::setSpeed(double speed) {
    Nanomachine::speed = speed;
}

void Nanomachine::setId(int id) {
    ID = id;
}

void Nanomachine::setX(double x) {
    X = x;
}

void Nanomachine::setY(double y) {
    Y = y;
}

void Nanomachine::setType(int type) {
    Nanomachine::type = type;
}

int Nanomachine::getFinalReachTime() const {
    return final_reach_time;
}

void Nanomachine::setFinalReachTime(int finalReachTime) {
    final_reach_time = finalReachTime;
}

bool Nanomachine::isCoMobile() const {
    return co_mobile;
}

void Nanomachine::setCoMobile(bool coMobile) {
    co_mobile = coMobile;
}

void Nanomachine::print() const {
    printf("[Nano] #%02d : type = [%02d], at [%8.3f, %8.3f], dist = [%8.3f], getA = [%7.5f], getB = [%7.5f], ang = [%4.2f]\n",
           this->getId(), this->getType(), this->getX(), this->getY(), dist(0, 0, X, Y), this->getA(this->X, this->Y),
           this->getB(this->X, this->Y), this->angle);

}

std::ostream &operator<<(std::ostream &os, const Nanomachine &nanomachine) {
    os << "speed: " << nanomachine.speed << " angle: " << nanomachine.angle << " ID: " << nanomachine.ID << " X: "
       << nanomachine.X << " Y: " << nanomachine.Y << " type: " << nanomachine.type << " final_reach_time: "
       << nanomachine.final_reach_time;
    return os;
}

int Nanomachine::getLastReleaseTimeofB() const {
    return lastReleaseTimeofB;
}

void Nanomachine::setLastReleaseTimeofB(int t) {
    Nanomachine::lastReleaseTimeofB = t;
}



