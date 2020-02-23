#include <iostream>
#include "ev3dev.cpp"
#include "ev3dev.h"

using namespace std;
using namespace ev3dev;

int main ()
{
  large_motor drive{OUTPUT_A};
  drive.set_speed_sp(200).run_forever();

  int i = 1;
  while (i > 0) {
    cout << "Enter 0 to stop the motor: ";
    cin >> i;
  }

  drive.set_stop_action("brake").stop();

  cout << "Motor stopped\n";
  
  return 0;
}