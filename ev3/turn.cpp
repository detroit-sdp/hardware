#include <iostream>
#include <chrono>
#include <thread>

#include "ev3dev.cpp"
#include "ev3dev.h"

using namespace std;
using namespace std::chrono_literals;
using namespace ev3dev;

int main ()
{
  large_motor drive{OUTPUT_A};
  medium_motor turn{OUTPUT_B};
  touch_sensor right{INPUT_1};
  touch_sensor left{INPUT_2};

  int speed = 150;
  int max_turn = 600;
  bool block = false;

  while (!button::back.pressed()) {
    if (button::up.pressed() && speed < 1000 && !block) {
      speed += 100;
      block = true;
    } else if (button::down.pressed() && speed > -1000 && !block) {
      speed -= 100;
      block = true;
    } else if (!button::down.pressed() && !button::up.pressed() && block) {
      block = false;
    }

    if (right.is_pressed() && left.is_pressed()) {
        turn.set_stop_action("brake").stop();
        drive.set_speed_sp(speed).run_forever();
    } else if (right.is_pressed() && turn.position() < max_turn) {
        turn.set_speed_sp(500).run_forever();
    } else if (left.is_pressed() && turn.position() > -max_turn) {
        turn.set_speed_sp(-500).run_forever();
    } else {
        turn.set_stop_action("brake").stop();
        drive.set_stop_action("brake").stop();
    }
  }


  turn.set_position_sp(0).run_to_abs_pos();
  this_thread::sleep_for(5000ms);  

  drive.set_stop_action("brake").stop();
  // turn.set_stop_action("break").stop();
  
  return 0;
}