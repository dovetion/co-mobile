//
// Created by 葛鑫 on 2019-06-03.
//

#ifndef CO_MOBILE_NANOMACHINE_H
#define CO_MOBILE_NANOMACHINE_H


#define RANDOMWALK 0
#define DETECTED_A 1
#define DETECTED_B 2
#define DETECTED_AB 3
#define PI (double)3.141592653589793
#define B_CONCENTRATION_MAX 1.0
#define A_CONCENTRATION_MAX 1.0
#define DEBUG_NANOMACHINE

#include <string>
#include <ostream>

class Nanomachine {

private:
    // nanomachines' moving speed
    double speed;
    // nanomachines' heading direction
    double angle;
    // nanomachines' ID
    int ID;
    // nanomachines' position
    double X, Y;
    //co-mobile scheme
    bool co_mobile;
public:
    int getLastReleaseTimeofB() const;

    void setLastReleaseTimeofB(int lastReleaseTimeofB);

private:
    // last release time of B
    int lastReleaseTimeofB;
public:
    bool isCoMobile() const;

    void setCoMobile(bool coMobile);

public:
    friend std::ostream &operator<<(std::ostream &os, const Nanomachine &nanomachine);

private:
    // nanomachines' status type
    // RANDOMWALK -> not detected attractant A and B
    // DETECTED_A -> only detected attractant A
    // DETECTED_B -> only detected attractant B
    // DETECTED_AB -> detected both A and B
    int type;

    int final_reach_time;
public:
    void setFinalReachTime(int finalReachTime);

public:
    int getFinalReachTime() const;


public:
    // random seed
    static int rand_seed;

    Nanomachine() {
        angle = 0.0;
        speed = 100.0;
        X = Y = 0.0;
        ID = -1;
        type = RANDOMWALK;
    }

    Nanomachine(int _id, double _x, double _y) {
        X = _x;
        Y = _y;
        ID = _id;
        type = RANDOMWALK;
        angle = 0.0;
        speed = 0.0;
    }

    // move nanomachine to the next position
    void move();

    // once found attrctant A, release attractant B
    void release_B(double _x, double _y);

    double getA(double _x, double _y) const;

    double getB(double _x, double _y) const;

    void print() const;

    double getAngle() const;

    double getSpeed() const;

    int getId() const;

    double getX() const;

    double getY() const;

    int getType() const;

    void setAngle(double angle);

    void setSpeed(double speed);

    void setId(int id);

    void setX(double x);

    void setY(double y);

    void setType(int type);
};


#endif //CO_MOBILE_NANOMACHINE_H
