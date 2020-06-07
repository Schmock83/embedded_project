#include "adafruitController.h"

#include "util.h"

#define DISTANCE_CM_SPEED_PER_SECOND 0.18 // wie viele cm pro Speed buggy pro sekunde faehrt
#define MILLISECONDS_PER_DEGREE 7.222222  // wie viele millisekunden (nach links bzw rechts) gefahren werden muss pro grad

using namespace std::chrono_literals;

AdafruitController::~AdafruitController()
{
    // release the motor after use
    if(hat.isValid())
    {
       drive(AdafruitController::kRelease); 
    }
}

void AdafruitController::driveDistance(AdafruitController::ControllerCommand cmd, double cm)
{
    int speed = -1;
    log::output("called");
    if (isValid() && (speed = getSpeed()) != -1)
    {
        drive(cmd);

        std::this_thread::sleep_for(std::chrono::milliseconds((int)((cm / (speed * DISTANCE_CM_SPEED_PER_SECOND)) * 1000))); //TODO
        log::output("ms: " + (int)((cm / (speed * DISTANCE_CM_SPEED_PER_SECOND)) * 1000));
    }else
    {
        log::output("else");
    }
    
}


void AdafruitController::turnLeft(int degrees)
{
    drive(AdafruitController::kLeft);
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(degrees * MILLISECONDS_PER_DEGREE)));
}

void AdafruitController::turnRight(int degrees)
{
    drive(AdafruitController::kRight);
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(degrees * MILLISECONDS_PER_DEGREE)));
}

bool AdafruitController::isValid(void)
{
    return hat.isValid();
}

void AdafruitController::setSpeed(int speed)
{
    if(motorsAvailable())
    {
        hat.getMotor(MOTOR_LEFT)->setSpeed(speed+5);
        hat.getMotor(MOTOR_RIGHT)->setSpeed(speed);   
    }
}

void AdafruitController::setSpeed(int speed, int index)
{
    if(motorsAvailable())
    {
        hat.getMotor(index)->setSpeed(speed);
    }
}

const int AdafruitController::getSpeed(void)
{
    if(!motorsAvailable())
    {
        return -1;
    }
    if(getSpeed(MOTOR_LEFT) == getSpeed(MOTOR_RIGHT))
        return getSpeed(MOTOR_LEFT);
    else
    {
        log::error("AdafruitController::getSpeed: Speed of left and right Motor different!");
        return getSpeed(MOTOR_RIGHT);
    }
}

const int AdafruitController::getSpeed(int index)
{
    if(hat.getMotor(index))
        return hat.getMotor(index)->getSpeed();
    else
    {
        log::error("AdafruitController::getSpeed: Couldn`t get a Motor for index: " + index);
        return -1;
    }
    
}

bool AdafruitController::motorsAvailable(void)
{
    if(!hat.isValid())
    {
        //error ausgeben wenn motorHat nicht erreichbar ist
        log::error("Motorhat konnte nicht erkannt werden");
        return false;
    }
    else if(!(hat.getMotor(MOTOR_RIGHT)) || !hat.getMotor(MOTOR_LEFT))
    {
        //error ausgeben wenn einer oder beide der Motoren nicht erkannt wurde
        log::error("Rechter und/oder Linker Motor konnten nicht erkannt werden");
        return false;
    }else
    {
        return true;
    }
}

// ausführen des eigendlichen kommandos für das bewegen der motoren
void AdafruitController::drive(ControllerCommand command)
{
    if(motorsAvailable())
    {

        switch(command)
        {
            case kForward:
                hat.getMotor(MOTOR_LEFT)->run(AdafruitDCMotor::kForward);
                hat.getMotor(MOTOR_RIGHT)->run(AdafruitDCMotor::kForward);
                break;
            case kBackward:
                hat.getMotor(MOTOR_LEFT)->run(AdafruitDCMotor::kBackward);
                hat.getMotor(MOTOR_RIGHT)->run(AdafruitDCMotor::kBackward);
                break;
            case kBrake:
                hat.getMotor(MOTOR_LEFT)->run(AdafruitDCMotor::kBrake);
                hat.getMotor(MOTOR_RIGHT)->run(AdafruitDCMotor::kBrake);
                break;
            case kRelease:
                hat.getMotor(MOTOR_LEFT)->run(AdafruitDCMotor::kRelease);
                hat.getMotor(MOTOR_RIGHT)->run(AdafruitDCMotor::kRelease);
                break;
            case kLeft:
                hat.getMotor(MOTOR_LEFT)->run(AdafruitDCMotor::kBackward);
                hat.getMotor(MOTOR_RIGHT)->run(AdafruitDCMotor::kForward);
                break;
            case kRight:
                hat.getMotor(MOTOR_LEFT)->run(AdafruitDCMotor::kForward);
                hat.getMotor(MOTOR_RIGHT)->run(AdafruitDCMotor::kBackward);
                break;
            default:
                log::error("Undefined Command");
                return;
        }
    }
}