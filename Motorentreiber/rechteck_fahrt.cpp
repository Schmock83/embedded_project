// Includes
#include <signal.h>

#include "source/adafruitController.h"
#include "source/util.h"

AdafruitController controller;

/// Interrupt Routine for STRG-C
void signalHandler(int signum) {
    std::cout << "Strg-C Programmende" << std::endl;

    // motoren releasen
    controller.drive(AdafruitController::kRelease);

    exit(signum);
}

int main(void) {
    // Csignal für Abbruch über STRG-C
    signal(SIGINT, signalHandler);

    // ueberpruefen ob motoren bzw. Motorhat erreichbar sind bzw. ist
    if (controller.motorsAvailable()) {
        // speed des motorhats einstellen (default=100)
        controller.setSpeed();

        // rechteck fahren
        controller.drive(AdafruitController::kForward);
        std::this_thread::sleep_for(1s);

        // nach rechts drehen um 90 Grad
        controller.turnRight(90);

        controller.drive(AdafruitController::kForward);
        std::this_thread::sleep_for(1s);

        // nach rechts drehen um 90 Grad
        controller.turnRight(90);

        controller.drive(AdafruitController::kForward);
        std::this_thread::sleep_for(1s);

        // nach rechts drehen um 90 Grad
        controller.turnRight(90);

        controller.drive(AdafruitController::kForward);
        std::this_thread::sleep_for(1s);

        // nach rechts drehen um 90 Grad
        controller.turnRight(90);
    }

    return 0;
}
