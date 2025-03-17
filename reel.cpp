#include "reel.h"
#include "phil_funcs.h"
#include <iostream>
#include <cmath>

static int gluttony = -1;

Reel* reels[3];

GLfloat get_rand( GLfloat max ){
	return ( 1 + (float) ( max * (rand() / (RAND_MAX + 1.0))) );
}

Reel::Reel() {
    start();
}

void Reel::start() {
    stopped = false;
    speed = get_rand(2);
    degree = get_rand(360);
    begin = 0;
    begin = speed;
}

void Reel::stop() {
    int r = 0;
    for (int x = 0; x < 3; ++x) {
        if (reels[x]->speed < reels[r]->speed) {
            r = x;
        }
    }
    reels[r]->stopped = true;
}

int Reel::winner() {
    int win = reels[0]->position();
    for (int x = 1; x < 3; ++x) {
        if (win != reels[x]->position()) {
            return -1;
        }
    }
    return win;
}

int Reel::num_stopped() {
    int num = 0;
    for (int x = 0; x < 3; ++x) {
        if (reels[x]->stopped) {
            ++num;
        }
    }
    return num;
}

bool Reel::all_stopped() {
    return num_stopped() == 3;
}

int Reel::position() {
    return static_cast<int>(roundf((degree - 30) / 40));
}

bool Reel::at_stop_position() {
    return (static_cast<GLint>(roundf(degree + 10)) % 40 == 0);
}

int Reel::stop_position(Reel *reel) {
    int position = reel->position();
    if (gluttony != -1) {
        return gluttony;
    }
    std::cout << "Results: " << Reel::num_stopped() << std::endl;
    switch (Reel::num_stopped()) {
        case 0:
            return position;
        case 1:
            for (int x = 0; x < 3; ++x) {
                if (reels[x]->stopped) {
                    return reels[x]->position();
                }
            }
            return position;
        default:
            int reel_stopped;
            for (int x = 0; x < 3; ++x) {
                if (reels[x]->stopped) {
                    reel_stopped = reels[x]->position();
                }
            }
            std::cout << "Second Results: " << reel_stopped << std::endl;

            int w = static_cast<int>(get_rand(3));
            std::cout << "Final Reel FTW?: " << (w == 1) << std::endl;
            if (w == 1) {
                return gluttony = reel_stopped;
            } else {
                if (position == reel_stopped) {
                    if (++position > 9) {
                        position = 0;
                    }
                }
                return gluttony = position;
            }
    }
}

void Reel::spin() {
    if (this->stopped) {
        return;
    }
    if (speed < 0.2) {
        if (this->at_stop_position()) {
            if (Reel::stop_position(this) == this->position()) {
                stopped = true;
                std::cout << "Reel halts on: " << this->position() << std::endl;
                if (Reel::all_stopped()) {
                    int winner = Reel::winner();
                    std::cout << "Winner: " << winner << std::endl;
                }
            }
        }
    } else {
        speed -= 0.001;
    }
    degree += speed;
    if (degree > 360) {
        degree -= 360;
    }
}
