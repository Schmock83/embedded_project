///
/// Project main
///

/// Includes
#include <signal.h>

#include "source/adafruitController.h"
#include "source/util.h"

AdafruitController controller;

/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
    std::cout << "Strg-C Programmende" << std::endl;

    //motoren releasen
    controller.drive(AdafruitController::kRelease);

    exit(signum);
}

int main(void)
{
    // Csignal für Abbruch über STRG-C
    signal(SIGINT, signalHandler);

    

    if(controller.motorsAvailable())
    {
        controller.setSpeed();

        //rechteck fahren
        controller.drive(AdafruitController::kForward);
        std::this_thread::sleep_for(1s);

        controller.drive(AdafruitController::kRight);
        std::this_thread::sleep_for(0.65s);

        controller.drive(AdafruitController::kForward);
        std::this_thread::sleep_for(1s);

        controller.drive(AdafruitController::kRight);
        std::this_thread::sleep_for(0.65s);

        controller.drive(AdafruitController::kBrake);

        controller.drive(AdafruitController::kForward);
        std::this_thread::sleep_for(1s);

        controller.drive(AdafruitController::kRight);
        std::this_thread::sleep_for(0.65s);

        controller.drive(AdafruitController::kForward);
        std::this_thread::sleep_for(1s);

        controller.drive(AdafruitController::kRight);
        std::this_thread::sleep_for(0.65s);
        
    }

    return 0;
}
