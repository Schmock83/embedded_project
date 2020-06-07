#pragma once

#include "adafruitmotorhat.h"

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

/*
* Wrapper Klasse die den MotorHat steuert und indirekt die motoren verwaltet
*/

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
    void turnLeft(int degrees);
    void turnRight(int degrees);
    const int getSpeed(void);
    const int getSpeed(int index);
    void drive(ControllerCommand command);
    void driveDistance(AdafruitController::ControllerCommand cmd, double cm);

private:
    AdafruitMotorHAT hat;
};