// Includes
#include <signal.h>
#include <vector>
#include <math.h>
#include <string.h>
#include <iostream>

#include "source/adafruitController.h"

//globale Controller Variable zum kontrollieren des Buggys
AdafruitController controller;

//typedefinition damit uebersichtlicher
typedef std::pair<AdafruitController::ControllerCommand, double> command_time_pair;

/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
    std::cout << "Strg-C Programmende" << std::endl;

    //motoren releasen
    controller.drive(AdafruitController::kRelease);

    exit(signum);
}

// returned den entsprechenden AdafruitController::ControllerCommand fuer das Argument
AdafruitController::ControllerCommand getCommandForString(const char *command)
{
    if (strcmp(command, "forward") == 0 || strcmp(command, "Forward") == 0)
    {
        return AdafruitController::kForward;
    }
    else if (strcmp(command, "backward") == 0 || strcmp(command, "Backward") == 0)
    {
        return AdafruitController::kBackward;
    }
    else if (strcmp(command, "right") == 0 || strcmp(command, "Right") == 0)
    {
        return AdafruitController::kRight;
    }
    else if (strcmp(command, "left") == 0 || strcmp(command, "Left") == 0)
    {
        return AdafruitController::kLeft;
    }
    else if (strcmp(command, "brake") == 0 || strcmp(command, "Brake") == 0)
    {
        return AdafruitController::kBrake;
    }
    else if (strcmp(command, "release") == 0 || strcmp(command, "Release") == 0)
    {
        return AdafruitController::kRelease;
    }
    else //invalides kommando bzw. Fahrrichtung
    {
        return AdafruitController::kInvalidCommand;
    }
}

// verarbeitet die argumente
std::vector<command_time_pair> parseArguments(int argc, char *argv[])
{
    std::vector<command_time_pair> commands;

    for (int i = 1; i < argc; i++)
    {
        //filtert kommando und zeit aus jedem kommando
        char *command = strtok(argv[i], "-");
        //erste zeichenfolge vor dem '-' ist das kommando
        double duration = atof(strtok(nullptr, "-"));
        //letzte zeichenfolge nach dem '-' ist die Zeit(dauer)

        AdafruitController::ControllerCommand cmd;
        
        //ueberpruefen ob jedes kommando gueiltig ist
        if ((cmd = getCommandForString(command)) == AdafruitController::kInvalidCommand)
        {
            //falls nicht, programm stoppen
            std::cerr << "Command \'" << command << "\' not known\nClosing Program...\n";
            signalHandler(-1);
        }
        else
        {
            //falls gueltig -> dem vektor hinzufuegen um spaeter ausgefuehrt zu werden
            commands.push_back(command_time_pair(getCommandForString(command), duration));
        }
    }

    //vektor zurueckgeben mit allen (Kommando, Zeit-dauer)-Paaren
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

int main(int argc, char *argv[])
{
    // Csignal für Abbruch über STRG-C
    signal(SIGINT, signalHandler);

    if (argc < 2)
    {
        std::cerr << "No Arguments given!\nUsage: " << argv[0] << " [direction-seconds]\n"
                  << "Example: " << argv[0] << " Forward-1.5 Backward-2.3 ...\n";
        return -1;
    }

    auto commands = parseArguments(argc, argv); // kommandos in richtiges format zum späteren verarbeiten bringen

    //ueberpruefen ob motoren bzw. Motorhat erreichbar sind bzw. ist
    if (controller.motorsAvailable())
    {
        //speed des controller setzten (default=100)
        controller.setSpeed();

        for (auto pair : commands)
        {
            printCommand(pair.first, pair.second);

            controller.drive(pair.first); //ausfuehren des aktuellen kommandos

            std::this_thread::sleep_for(std::chrono::milliseconds((int)(pair.second * 1000))); //legt threat schlafen für aktuelle Zeit-dauer
        }
    }

    return 0;
}
