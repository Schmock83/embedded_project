#pragma once

#include "adafruitmotorhat.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <wiringPi.h>
#include <iomanip>

#define GPIO_TRIGGER 1
#define GPIO_ECHO 5
#define BREMSLICHT 26

#define DISTANCE_CM_SPEED_PER_SECOND 0.18 // wie viele cm pro Speed buggy pro sekunde faehrt
#define MILLISECONDS_PER_DEGREE 7.222222  // wie viele millisekunden (nach links bzw rechts) gefahren werden muss pro grad
#define MINIMAL_DISTANCE_CM 15
#define INTERVALL_MS 50 // Intervall in dem geprueft werden soll ob hinderniss vor buggy
#define LOG_OUTPUT 1    // togglen der hilfs-ausgaben

#define BACKUP_INTERVALL 10 // wie viele cm buggy nach hinten fahren soll wenn rechts(45grad) und links(45grad) hindernisse sind

using namespace std::chrono_literals;

/*
* Wrapper Klasse die den MotorHat steuert und indirekt die motoren verwaltet
*/

#define command_time_pair std::pair<AdafruitController::ControllerCommand, double>

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
    AdafruitController::ControllerCommand getCommandForString(const char *command);
    void printCommand(AdafruitController::ControllerCommand cmd, double seconds);
    bool checkFrontalDistance(void);
    void evadeObstacle(void);
    void driveThreadFunc(std::vector <command_time_pair> commands);
    std::vector<command_time_pair> parseArguments(int argc, char *argv[], void (*signalHandler) (int));

private:
    AdafruitMotorHAT hat;
};