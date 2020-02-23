#include <iostream>
#include <chrono>
#include <thread>

#include "ev3dev.cpp"
#include "ev3dev.h"

using namespace std;
using namespace ev3dev;

class Lift {
    large_motor drive_a;
    large_motor drive_b;
    touch_sensor bottom;
    touch_sensor top;

    int speed;

    public:
    Lift (int s) : 
        drive_a(OUTPUT_A), 
        drive_b(OUTPUT_B), 
        top(INPUT_1), 
        bottom(INPUT_2), 
        speed(s)
        {}

    void go_up () 
    {
        drive_a.set_speed_sp(-speed).run_forever();
        drive_b.set_speed_sp(-speed).run_forever();
        while (!top.is_pressed() && !button::enter.pressed());
        drive_a.set_stop_action(motor::stop_action_brake).stop();
        drive_b.set_stop_action(motor::stop_action_brake).stop();
    }

    void go_down () 
    {
        drive_a.set_speed_sp(speed).run_forever();
        drive_b.set_speed_sp(speed).run_forever();
        while (!bottom.is_pressed() && !button::enter.pressed());
        drive_a.set_stop_action(motor::stop_action_brake).stop();
        drive_b.set_stop_action(motor::stop_action_brake).stop();
    }

    void change_speed_by (int s)
    {
        int new_speed = speed + s;
        if (new_speed > 0 && new_speed <= 1000) speed = new_speed;
    }
};

void lift_test (Lift lift, int count) {
    for (int i = 0; i < count; i++) {
        lift.go_up();
        if (button::enter.pressed()) break;
        lift.go_down();
        if (button::enter.pressed()) break;
    }
}

int main () 
{
    Lift lift {200};

    bool block = false;

    while (!button::back.pressed()) 
    {
        if (button::up.pressed()) lift_test(lift, 10);
        else if (button::right.pressed()) { 
            if (!block) lift.change_speed_by(100);
            block = true;
        } else if (button::left.pressed()) {
            if (!block) lift.change_speed_by(-100);
            block = true;
        } else block = false;
    }
}

