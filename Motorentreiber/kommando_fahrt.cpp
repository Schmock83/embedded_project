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

typedef std::pair<AdafruitController::ControllerCommand, double>  command_time_pair;

/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
    //std::cout << "Strg-C Programmende" << std::endl;
	
    //motoren releasen
    controller.drive(AdafruitController::kRelease);

    exit(signum);
}

// returned den entsprechenden AdafruitController::ControllerCommand fuer das Argument
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

// verarbeitet die argumente
std::vector<command_time_pair> parseArguments(int argc, char *argv[])
{
    std::vector<command_time_pair> commands;
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
            std::cout << "Command \'" << command << "\' not known\nClosing Program...\n";
            signalHandler(-1);
        }
        else    
        {
            commands.push_back(command_time_pair(getCommandForString(command), duration));
        }
    }

    return commands;
}

// ausgabe des gerade ausgeführten kommandos
void printCommand(AdafruitController::ControllerCommand cmd, double seconds)
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

    std::cout << "Driving \'" << commandString << "\' for " << seconds << " seconds\n";
}

int main(int argc, char* argv[])
{
    // Csignal für Abbruch über STRG-C
    signal(SIGINT, signalHandler);

    if(argc < 2)
    {
        std::cerr << "No Arguments given!\nUsage: " << argv[0] << " [direction-seconds]\n" << "Example: " << argv[0] << " Forward-1.5 Backward-2.3 ...\n";
        return -1;
    }

    auto commands = parseArguments(argc, argv); // kommandos in richtiges format zum späteren verarbeiten bringen

    if(controller.motorsAvailable())    // motoren erreichbar?
    {
        controller.setSpeed();

        for (auto pair : commands)
        {
            printCommand(pair.first, pair.second);

            controller.drive(pair.first);   //ausführen des eigendlichen kommandos

            std::this_thread::sleep_for(std::chrono::milliseconds( (int)(pair.second*1000) ));
        }
    }

    return 0;
}