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
	// Beenden Sie hier bitte alle Verbindung zu den Sensoren etc.
	
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

        
    }

    return 0;
}
