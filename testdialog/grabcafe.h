#ifndef GRABCAFE_H
#define GRABCAFE_H
#include <alproxies/almotionproxy.h>
#include <alproxies/alrobotpostureproxy.h>


class grabCafe
{
public:
  grabCafe(string robotIP);
  void test();
  void action();

private:
  static int current_x;
  static int current_y;
  static double current_theta;

  AL::ALMotionProxy* motion;
  AL::ALRobotPostureProxy* posture;

  void aboutFace();
  void walkForward();
  void walkBack();
  void grabIt();
  void handIn();
};

#endif // GRABCAFE_H
