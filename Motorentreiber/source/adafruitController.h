#pragma once

#include "adafruitmotorhat.h"

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

class AdafruitController
{
public:
    enum ControllerCommand
    {
        kForward,
        kBackward,
        kBrake,
        kRelease,
        kLeft,
        kRight,
        kInvalidCommand = -1,
    };
    ~AdafruitController();
    void setSpeed(int speed=100);
    void setSpeed(int speed, int index);
    bool motorsAvailable(void);
    bool isValid(void);

    void drive(ControllerCommand command);

private:
    AdafruitMotorHAT hat;
};