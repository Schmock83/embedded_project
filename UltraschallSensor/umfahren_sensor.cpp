#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <wiringPi.h>
#include <signal.h>
#include <string.h>


#include "source/adafruitController.h"


#define GPIO_TRIGGER 1
#define GPIO_ECHO 5
#define BREMSLICHT 26
#define MINIMAL_DISTANCE_CM 15
#define INTERVALL_MS 50 // Intervall in dem geprueft werden soll ob hinderniss vor buggy

#define BACKUP_INTERVALL 10 // wie viele cm buggy nach hinten fahren soll wenn rechts(45grad) und links(45grad) hindernisse sind

#define LOG_OUTPUT 0 // togglen der hilfs-ausgaben


AdafruitController controller; // controller zum steuern der Motoren

bool readyToDrive = false; // flag ob buggy fahren kann
bool threadFinished = false; // flag zum signalisieren dass Thread (und damit auch das Fahren des Buggys) zu ende ist

typedef std::pair<AdafruitController::ControllerCommand, double> command_time_pair; //datentyp zum speichern von (FahrRichtung, Zeit)



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

// wandelt string (Forward, Backward,...) in tatsaechlichen AdafruitController::Command um (kForward, kBackward -> benoetigt fuer controller.drive)
AdafruitController::ControllerCommand getCommandForString(const char *command)
{
    #if LOG_OUTPUT
        std::cout << "getCommandForString(" << command << "): ";
    #endif
    
    if (strcmp(command, "forward") == 0 || strcmp(command, "Forward") == 0)
    {
        #if LOG_OUTPUT
            std::cout << "forward\n";
        #endif
        return AdafruitController::kForward;
    }
    else if (strcmp(command, "backward") == 0 || strcmp(command, "Backward") == 0)
    {
        #if LOG_OUTPUT
                std::cout << "backward\n";
        #endif
        return AdafruitController::kBackward;
    }
    else if (strcmp(command, "right") == 0 || strcmp(command, "Right") == 0)
    {
        #if LOG_OUTPUT
                std::cout << "right\n";
        #endif
        return AdafruitController::kRight;
    }
    else if (strcmp(command, "left") == 0 || strcmp(command, "Left") == 0)
    {
        #if LOG_OUTPUT
                std::cout << "left\n";
        #endif
        return AdafruitController::kLeft;
    }
    else
    {
        #if LOG_OUTPUT
            std::cout << "invalid\n";
        #endif
        return AdafruitController::kInvalidCommand;
    }
}

// liefert distanz in cm zu dem Object vor dem Buggy
void checkFrontalDistance(void)
{
    // trigger ausloesen
    digitalWrite(GPIO_TRIGGER, HIGH);

    std::this_thread::sleep_for(std::chrono::nanoseconds(10));

    digitalWrite(GPIO_TRIGGER, LOW);

    auto startTime = std::chrono::high_resolution_clock::now();
    auto stopTime = std::chrono::high_resolution_clock::now();

    while (digitalRead(GPIO_ECHO) == LOW)
    {
        startTime = std::chrono::high_resolution_clock::now();
    }

    // abwarten bis echo zurueck kommt
    while (digitalRead(GPIO_ECHO) == HIGH)
    {
        stopTime = std::chrono::high_resolution_clock::now();
    }

    double elapsed_ms = std::chrono::duration<double, std::milli>(stopTime - startTime).count();

    // mit der Schallgeschwindigkeit (34.3 cm/ms) multiplizieren
    // durch 2 teilen, da hin und rueck-weg
    double distance = (elapsed_ms * 34.3) / 2;

    // ueberpruefen ob hinderniss in minimaler naehe ist
    if (distance <= MINIMAL_DISTANCE_CM)
    {
        // bremslicht einschalten
        digitalWrite(BREMSLICHT, HIGH);
        // thread signalisieren, dass er fahrt unterbrechen soll
        readyToDrive = false;
    }   
    else
    {
        // bremslicht ausschalten
        digitalWrite(BREMSLICHT, LOW);
        // thread signalisieren, dass er weiter fahren kann
        readyToDrive = true;
    }

    #if LOG_OUTPUT
        std::cout << "Distance: " << std::fixed << std::setprecision(2) << distance << "cm\n";
    #endif
}

// hilfsfunktion zum ausgeben der richtung und fahrtzeit
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

// ausweichen/umfahren des hindernisses
void evadeObstacle(void)
{
    unsigned int distanceToObstacle = MINIMAL_DISTANCE_CM; // distanz die buggy fahren muss bis es bei dem Hinderniss ist
    
    while(true)
    {
        // nach links drehen und schauen ob hinderniss im weg:
        turnLeft(45);
        // pruefen ob hinderniss im weg
        checkFrontalDistance();
        if(readyToDrive)
        {
            //TODO Fahren bis zum hinderniss, dass wieder 45Grad drehen u nd gleichen abstand fahren
            controller.driveDistance(AdafruitController::kForward, distanceToObstacle);
            turnRight(90);
            controller.driveDistance(AdafruitController::kForward, distanceToObstacle);
            turnLeft(45);
            return;
        }
        // 90 = 45 (um wieder in der urspruenglichen fahrtrichtung zu sein - umkehrung von turnLeft) + 45 (um nach rechts zu drehen fuer 45 Grad)
        turnRight(90);
        // pruefen ob hinderniss im weg
        checkFrontalDistance();
        if(readyToDrive)
        {
            //TODO Fahren bis zum hinderniss, dass wieder 45Grad drehen u nd gleichen abstand fahren
            controller.driveDistance(AdafruitController::kForward, distanceToObstacle);
            turnLeft(90);
            controller.driveDistance(AdafruitController::kForward, distanceToObstacle);
            turnRight(45);
            return;
        }
        // zurueck in urspruengliche fahrtrichtung
        turnLeft(45);
        // rueckwaerts fahren fuer bestimmte distanz - auf distanceToObstacle addieren
        controller.driveDistance(AdafruitController::kBackward, BACKUP_INTERVALL);
        distanceToObstacle += BACKUP_INTERVALL;
    }
}

void driveThreadFunc(std::vector <command_time_pair> commands)
{
    // flag, ob buggy gestoppt ist oder faehrt -> wichtig zum zaehlen der fahrtzeit
    bool stopped = true;

    // jedes fahrt-komando ausfuehren
    for(auto pair : commands)
    {
        #if LOG_OUTPUT
            printCommand(pair.first, pair.second);
        #endif
        
        // zaehlen der eigendlichen fahrtzeit
        for(int i=0; i < pair.second * 1000; )
        {
            // wenn buggy noch nicht gestoppt ist und nicht bereit ist zum fahren (weil hinderniss im weg) -> bremsen und 'stopped'-flag setzten
            if(!readyToDrive && !stopped)
            {
                stopped = true;
                controller.drive(AdafruitController::kBrake);
                evadeObstacle();
            }
            // wenn buggy fahren darf
            else if(readyToDrive)
            {
                // fahrtzeit addieren (zeit in der buggy tatsaechlich fahren kann)
                i += INTERVALL_MS;
                // wenn noch gestoppt ist -> 'stopped'-flag zuruecksetzten und weiter fahren in die vorherige richtung
                if (stopped)
                {
                    stopped = false;
                    controller.drive(pair.first);
                }   
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(INTERVALL_MS));
        }

        // setzten des flags, damit jede iteration der schleife wie der erste funktionsaufruf ablaeuft
        stopped = true;

    }

    // flag setzten um zu signalisieren, dass thread fertig ist
    threadFinished = true;
}

// verarbeitet die command-line argumente in brauchbare(ausfuehrbare) anweisungen fuer den AdafruitController
std::vector<command_time_pair> parseArguments(int argc, char *argv[])
{
    std::vector<command_time_pair> commands;

    for (int i = 1; i < argc; i++)
    {
        // richtung und dauer aus den commandline-Argumenten rausfiltern - 'Richtung-Dauer' -> Richtung(AdafruitController::Command); Dauer(double - sekunden)
        char *command = strtok(argv[i], "-");
        double duration = atof(strtok(nullptr, "-"));

        AdafruitController::ControllerCommand cmd;

        #if LOG_OUTPUT
            std::cout << argv[i] << ": " << command << "~" << seconds << std::endl;
        #endif

        // programm stoppen bei fehlerhaftem argument
        if ((cmd = getCommandForString(command)) == AdafruitController::kInvalidCommand)
        {
            std::cout << "Command \'" << command << "\' not known\nClosing Program...\n";
            signalHandler(-1);
        }
        else
        {
            // bei gueltiger fahrtrichtung das Paar(Richtung, Dauer) in vektor speichern
            commands.push_back(command_time_pair(getCommandForString(command), duration));
        }
    }

    return commands;
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
    auto commands = parseArguments(argc, argv);

    // ueperpruefen ob Motorhat & Motoren benutzbar sind
    if (controller.motorsAvailable())
    {
        // geschwindigkeit einstellen -> default=100
        controller.setSpeed();

        std::thread driveThread(driveThreadFunc, commands);

        // solange thread (und damit die fahrt des buggies) noch nicht zu ende ist -> ueperprufen ob hinderniss im weg
        while (!threadFinished)
        {
            checkFrontalDistance();
            std::this_thread::sleep_for(std::chrono::milliseconds(INTERVALL_MS));
        }
        
        // warten bis thread zuende
        driveThread.join();
    }

    return 0;
}