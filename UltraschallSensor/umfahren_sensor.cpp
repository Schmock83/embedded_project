#include <iostream>
#include <signal.h>

#include "source/adafruitController.h"

AdafruitController controller; // controller zum steuern der Motoren

/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
    #if LOG_OUTPUT
        std::cout << "Strg-C Programmende" << std::endl;
    #endif

    // motoren releasen
    controller.drive(AdafruitController::kRelease);

    exit(signum);
}

int main(int argc, char *argv[])
{
    // Csignal für Abbruch über STRG-C
    signal(SIGINT, signalHandler);

    // falls kein commandline-argument angegeben wurde
    if (argc < 2)
    {
        std::cerr << "No Arguments given!\nUsage: " << argv[0] << " [direction-seconds]\n"
                  << "Example: " << argv[0] << " Forward-1.5 Backward-2.3 ...\n";
        return -1;
    }

    //Richtung der GPIO - Pins festlegen(IN / OUT)
    wiringPiSetup();

    // pinModes setzen
    pinMode(GPIO_TRIGGER, OUTPUT);
    pinMode(GPIO_ECHO, INPUT);
    pinMode(BREMSLICHT, OUTPUT);

    // commandline-Argumente in richtiges/brauchbares format bringen
    auto commands = controller.parseArguments(argc, argv, signalHandler);

    // ueperpruefen ob Motorhat & Motoren benutzbar sind
    if (controller.motorsAvailable())
    {
        // geschwindigkeit einstellen -> default=100
        controller.setSpeed();
        
        controller.driveThreadFunc(commands);
    }

    return 0;
}