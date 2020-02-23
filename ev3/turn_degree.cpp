#include <iostream>
#include <chrono>
#include <thread>

#include "ev3dev.cpp"
#include "ev3dev.h"

using namespace std;
using namespace ev3dev;

class Robot {
  large_motor  drive;
  medium_motor turn;
  gyro_sensor  gyro;

  int speed;
  int max_turn;

  public:
  Robot(int s, int t) : 
    drive(OUTPUT_A), 
    turn(OUTPUT_B), 
    gyro(INPUT_3), 
    speed(s), 
    max_turn(t) 
    {}

  void turn_by(int deg) {
    if (deg == 0) return;
    
    drive.set_stop_action("brake").stop();
    if (deg > 0) {
      turn.set_speed_sp(500).set_position_sp(max_turn).run_to_abs_pos();
      while (turn.position() < max_turn);
      int dest  = gyro.value() + deg;
      drive.set_speed_sp(speed).run_forever();
      while (gyro.value() < dest);
      drive.set_stop_action("brake").stop();
      turn.set_speed_sp(-500).set_position_sp(0).run_to_abs_pos();
      while (turn.position() > 0);
    } else {
      turn.set_speed_sp(-500).set_position_sp(-max_turn).run_to_abs_pos();
      while (turn.position() > -max_turn);
      int dest  = gyro.value() + deg;
      drive.set_speed_sp(speed).run_forever();
      while (gyro.value() > dest);
      drive.set_stop_action("brake").stop();
      turn.set_speed_sp(500).set_position_sp(0).run_to_abs_pos();
      while (turn.position() < 0);
    }
  }

  void run() {
    drive.set_speed_sp(speed).run_forever();
  }

  void stop() {
    drive.set_stop_action(motor::stop_action_brake).stop();
    turn.set_stop_action(motor::stop_action_brake).stop();
  }

  int get_speed() { return(speed); }
  void set_speed(int s) { speed = s; }
};

int main ()
{
  touch_sensor right{INPUT_1};
  touch_sensor left{INPUT_2};

  Robot r {250, 600};

  bool scanning = false;
  bool block = false;
  bool run = false;
  int acc = 0;

  while (!button::back.pressed()) {
    if (button::up.pressed() && r.get_speed() < 1000 && !block) {
      r.set_speed(r.get_speed() + 100);
      block = true;
    } else if (button::down.pressed() && r.get_speed() > -1000 && !block) {
      r.set_speed(r.get_speed() - 100);
      block = true;
    } else if (!button::down.pressed() && !button::up.pressed() && block) {
      block = false;
    }

    if (!scanning) {
      acc = 0;
      if (right.is_pressed() || left.is_pressed()) {
        this_thread::sleep_for(70ms);
        if (right.is_pressed() && left.is_pressed()) {
          r.run();
          run = true;
        } else if (run) {
          r.stop();
          run = false;
        } else {
          scanning = true;
        }
      }
    } else {
      auto start = chrono::steady_clock::now();
      auto curr = chrono::steady_clock::now();
      while (chrono::duration_cast<chrono::milliseconds>(curr - start).count() < 1000) {
        curr = chrono::steady_clock::now();
        if (block && !right.is_pressed() && !left.is_pressed()) 
          block = false;

        if (right.is_pressed() && !block) {
          acc++;
          block = true;
          start = chrono::steady_clock::now();
        } else if (left.is_pressed() && !block) {
          acc--;
          block = true;
          start = chrono::steady_clock::now();
        }
      }

      r.turn_by(acc * 45);
      scanning = false;
    }
  }

  r.stop();

  return 0;
}