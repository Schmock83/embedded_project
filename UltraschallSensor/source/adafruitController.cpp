#include "adafruitController.h"

#include "util.h"

using namespace std::chrono_literals;

AdafruitController::~AdafruitController()
{
    // release the motor after use
    if(hat.isValid())
    {
       drive(AdafruitController::kRelease); 
    }
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
                return; //soll nicht schlafen
                break;
            case kRelease:
                hat.getMotor(MOTOR_LEFT)->run(AdafruitDCMotor::kRelease);
                hat.getMotor(MOTOR_RIGHT)->run(AdafruitDCMotor::kRelease);
                return; //soll nicht schlafen
                break;
            case kLeft:
                hat.getMotor(MOTOR_LEFT)->run(AdafruitDCMotor::kBrake);
                hat.getMotor(MOTOR_RIGHT)->run(AdafruitDCMotor::kForward);
                break;
            case kRight:
                hat.getMotor(MOTOR_LEFT)->run(AdafruitDCMotor::kForward);
                hat.getMotor(MOTOR_RIGHT)->run(AdafruitDCMotor::kBrake);
                break;
            default:
                log::error("Undefined Command");
                return;
        }
    }
}