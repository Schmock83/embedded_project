///
/// Project main
///

/// Includes
#include <signal.h>
#include <vector>
#include <math.h>
#include <string.h>
#include <iostream>

#include "source/adafruitController.h"

AdafruitController controller;

/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
    //std::cout << "Strg-C Programmende" << std::endl;
	
    //motoren releasen
    controller.drive(AdafruitController::kRelease);

    exit(signum);
}

AdafruitController::ControllerCommand getCommandForString(const char *command)
{
    //std::cout << "getCommandForString(" << command << "): ";
    if (strcmp(command, "forward") == 0 || strcmp(command, "Forward") == 0)
    {
        //std::cout << "forward\n";
        return AdafruitController::kForward;
    }
    else if (strcmp(command, "backward") == 0 || strcmp(command, "Backward") == 0)
    {
        //std::cout << "backward\n";
        return AdafruitController::kBackward;
    }
    else if (strcmp(command, "right") == 0 || strcmp(command, "Right") == 0)
    {
        //std::cout << "right\n";
        return AdafruitController::kRight;
    }
    else if (strcmp(command, "left") == 0 || strcmp(command, "Left") == 0)
    {
        //std::cout << "left\n";
        return AdafruitController::kLeft;
    }
    else
    {
        //std::cout << "invalid\n";
        return AdafruitController::kInvalidCommand;
    }   
}

std::vector<std::pair<AdafruitController::ControllerCommand, double>> parseArguments(int argc, char *argv[])
{
    std::vector<std::pair<AdafruitController::ControllerCommand, double>> commands;
    //commands.resize(argc-1);

    for (int i = 1; i < argc; i++)
    {
        char *command = strtok(argv[i], "-");
        double duration = atof(strtok(nullptr, "-"));
        AdafruitController::ControllerCommand cmd;
        //std::cout << argv[i] << ": " << command << "~" << seconds << std::endl;

        if((cmd = getCommandForString(command)) == AdafruitController::kInvalidCommand)
        {
            //stop programm
            std::cout << "Kommando \'" << command << "\' wurde nicht erkannt\nProgramm wird abgebrochen\n";
            signalHandler(-1);
        }
        else    
        {
            commands.push_back(std::pair<AdafruitController::ControllerCommand, double>(getCommandForString(command), duration));
        }
    }

    return commands;
}

void printCommand(AdafruitController::ControllerCommand cmd, int seconds, int milliseconds)
{
    std::string commandString = "";
    switch (cmd)
    {
        case AdafruitController::kForward:
            commandString = "Forward";
            break;
        case AdafruitController::kBackward:
            commandString = "Backward";
            break;
        case AdafruitController::kLeft:
            commandString = "Left";
            break;
        case AdafruitController::kRight:
            commandString = "Right";
            break;
    }

    std::cout << "Driving \'" << commandString << "\' for " << seconds << " seconds and " << milliseconds << " milliseconds\n";
}

int main(int argc, char* argv[])
{
    // Csignal für Abbruch über STRG-C
    signal(SIGINT, signalHandler);

    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " [direction-seconds]\n" << "Example: " << argv[0] << "Forward-1.5 Backward-2.3 ...\n";
    }

    auto commands = parseArguments(argc, argv);

    if(controller.motorsAvailable())
    {
        controller.setSpeed();

        for (auto pair : commands)
        {
            //dauer in sekunden und millisekunden aufteilen
            double seconds = 0, milliseconds = 0;
            milliseconds = modf(pair.second, &seconds) * 1000;

            printCommand(pair.first, (int)seconds, (int)milliseconds);

            controller.drive(pair.first);

            std::this_thread::sleep_for(std::chrono::seconds((int)seconds));
            std::this_thread::sleep_for(std::chrono::milliseconds((int)milliseconds));
        }
    }

    return 0;
}