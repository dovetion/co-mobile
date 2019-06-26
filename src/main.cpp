#include <iostream>
#include "../inc/Nanomachine.h"
#include "../inc/aux.h"
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

// global veriable

int B_timeout = 10;
int now = 0;
const int num_nanomachine = 10;
const bool is_comobile = false;
const int max_time = 20000;
double target_area = 30.0;
double B_rangeout = 200.0;
double propotional_coe = 1.0;
const double init_distance = 900.0;


// function declaration
void setup();

void loop();

void collect();

vector<Nanomachine> nano_v, final_v;
vector<pair<double, double>> loc_mark[num_nanomachine];

int main() {
    msg("Co-Mobile Simulation Running");
    setup();
    loop();
    collect();
    return 0;
}

void setup() {
    // set random seed
    // Nanomachine::rand_seed = 1;
    srand(Nanomachine::rand_seed);
    rand();
    printf("[INFO] random seed set to %d\n", Nanomachine::rand_seed);

    // init nanomachine
    printf("[INFO] Initialize nanomachines' parameter\n");
    for (int i = 0; i < num_nanomachine; i++) {
        Nanomachine m;
        m.setFinalReachTime(-1);
        m.setId(i);
        m.setSpeed(1.0);
        m.setType(RANDOMWALK);
        m.setAngle(rand_double(0.0, 2.0 * PI));
        m.setCoMobile(is_comobile);
        m.setLastReleaseTimeofB(-1);
        double angle = rand_double(0, 2 * PI);
        m.setX(sin(angle) * init_distance);
        m.setY(cos(angle) * init_distance);
        printf("\t\t -> Nanomachine #%02d : angle = %02.3f, coordinates: [%7.3f, %7.3f] \n", m.getId(), m.getAngle(),
               m.getX(), m.getY());
        nano_v.push_back(m);
    }


}

void loop() {
    int num_targetarea = 0;
    const int num_threashold = num_nanomachine * propotional_coe;
    printf("[INFO] Into Loop, now all nanomachines are trying to find target area\n");
    while (num_targetarea < num_threashold) {
        num_targetarea = 0;
        now += 1;
        for (int i = 0; i < nano_v.size(); i++) {
            int ID = nano_v[i].getId();
            if (nano_v[i].getType() == DETECTED_AB) {
                printf("\t\t -> Naonomachine #%02d : Already in Target Area\n", ID);
                num_targetarea += 1;
                nano_v[i].setFinalReachTime(now);
                final_v.push_back(*(nano_v.begin() + i));
                nano_v.erase(nano_v.begin() + i);
                //sleep(2);
                continue;
            }
            double lastX = nano_v[i].getX();
            double lastY = nano_v[i].getY();

            loc_mark[ID].push_back({lastX, lastY});
            nano_v[i].move();
            //printf("\t\t -> Nanomachine #%02d type[%02d] when %d : from [%7.3f, %7.3f] moved to [%7.3f, %7.3f]\n", i, nanomachine[i].getType(), now, lastX, lastY, nanomachine[i].getX(), nanomachine[i].getY());
            nano_v[i].print();
            //printf("%10.5f %10.5f\n", lastX, lastY);
        }
        if (now >= max_time) {
            for (auto &m: nano_v)
                final_v.push_back(m);
            break;
        }
    }
}

void collect() {
    printf("[INFO] simulation is done, now collecting data ... \n");
    printf("\n[INFO] now generating the data for each Nanomachine ...\n");

    // Generate data report for each nanomachine
    int allReachTime = 0, reached_num = 0;
    for (auto &m: final_v) {
        int finaltime = m.getFinalReachTime();
        if (finaltime == -1)
            printf("\t\t --> [NANO] #%02d: NEVER reached target area\n", m.getId());
        else {
            printf("\t\t --> [NANO] #%02d: reached target area after [%07d]\n", m.getId(), finaltime);
            allReachTime += finaltime;
            reached_num += 1;
        }
    }
    printf("[INFO] %5.3f of Nanomachines reached target area\n", 1.0 * reached_num / num_nanomachine);
    if (reached_num)
        printf("[INFO] average reach time is [%05d]\n", allReachTime / reached_num);
    else
        printf("[INFO] average reach time is [0.00000]\n");

    // output the trace for each nanomachine
    string out = getCurrentTimeStr();
    string path = "../data/" + out + ".txt";
    printf("\n[INFO] now collecting the trace of each Nanomachine, output in \"%s\" \n", path.c_str());
    freopen(path.c_str(), "w", stdout);
    for (int i = 0; i < num_nanomachine; i++) {
        printf("%d %d\n", i, (int) loc_mark[i].size());
        for (auto x: loc_mark[i]) {
            printf("%.5f %.5f\n", x.first, x.second);
        }
    }
}

