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

void AdafruitController::driveThreadFunc(std::vector<command_time_pair> commands)
{

    // flag, ob buggy gestoppt ist oder faehrt -> wichtig zum zaehlen der fahrtzeit
    bool stopped = true;

    // jedes fahrt-komando ausfuehren
    for (auto pair : commands)
    {
        #if LOG_OUTPUT
            printCommand(pair.first, pair.second);
        #endif

        // zaehlen der eigendlichen fahrtzeit
        for (int i = 0; i < pair.second * 1000;)
        {
            // wenn buggy noch nicht gestoppt ist und nicht bereit ist zum fahren (weil hinderniss im weg) -> bremsen und 'stopped'-flag setzten
            if (checkFrontalDistance())
            {
                stopped = true;
                drive(AdafruitController::kBrake);
                evadeObstacle();
            }
            // wenn buggy fahren darf
            else
            {
                // fahrtzeit addieren (zeit in der buggy tatsaechlich fahren kann)
                i += INTERVALL_MS;
                // wenn noch gestoppt ist -> 'stopped'-flag zuruecksetzten und weiter fahren in die vorherige richtung
                if (stopped)
                {
                    stopped = false;
                    drive(pair.first);
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(INTERVALL_MS));
        }

        // setzten des flags, damit jede iteration der schleife wie der erste funktionsaufruf ablaeuft
        stopped = true;
    }
}

// ausweichen/umfahren des hindernisses
void AdafruitController::evadeObstacle(void)
{
    unsigned int distanceToObstacle = MINIMAL_DISTANCE_CM; // distanz die buggy fahren muss bis es bei dem Hinderniss ist

    while (true)
    {
        // nach links drehen und schauen ob hinderniss im weg:
        turnLeft(45);
        // pruefen ob hinderniss im weg
        if (!checkFrontalDistance())
        {
            std::cout << "evading left\n";
            //TODO Fahren bis zum hinderniss, dass wieder 45Grad drehen u nd gleichen abstand fahren
            driveDistance(AdafruitController::kForward, distanceToObstacle * 2.5);
            turnRight(80);
            driveDistance(AdafruitController::kForward, distanceToObstacle * 2.5);
            turnLeft(35);
            return;
        }
        // 90 = 45 (um wieder in der urspruenglichen fahrtrichtung zu sein - umkehrung von turnLeft) + 45 (um nach rechts zu drehen fuer 45 Grad)
        turnRight(90);
        // pruefen ob hinderniss im weg
        if (!checkFrontalDistance())
        {
            std::cout << "evading right\n";
            //TODO Fahren bis zum hinderniss, dass wieder 45Grad drehen u nd gleichen abstand fahren
            driveDistance(AdafruitController::kForward, distanceToObstacle * 2.5);
            turnLeft(95);
            driveDistance(AdafruitController::kForward, distanceToObstacle * 2.5);
            turnRight(30);
            return;
        }
        // zurueck in urspruengliche fahrtrichtung
        turnLeft(45);
        // rueckwaerts fahren fuer bestimmte distanz - auf distanceToObstacle addieren
        //driveDistance(AdafruitController::kBackward, BACKUP_INTERVALL);
        int speed = getSpeed();
        if(speed == -1)
        {
            return;
        }
        drive(AdafruitController::kBackward);
        std::this_thread::sleep_for(std::chrono::milliseconds(
            (int)((BACKUP_INTERVALL / (speed * DISTANCE_CM_SPEED_PER_SECOND))*1000)
            ));
        distanceToObstacle += BACKUP_INTERVALL;
    }
}

void AdafruitController::driveDistance(AdafruitController::ControllerCommand cmd, double cm)
{
    int speed = -1;
    bool stopped = true;
    
    if (isValid() && (speed = getSpeed()) != -1)
    {
        //drive(cmd);

        #if LOG_OUTPUT
            printCommand(cmd, (int)((cm / (speed * DISTANCE_CM_SPEED_PER_SECOND))));
        #endif

        // zaehlen der eigendlichen fahrtzeit
        for (int i = 0; i < (int)((cm / (speed * DISTANCE_CM_SPEED_PER_SECOND)) * 1000);)
        {
            // wenn buggy noch nicht gestoppt ist und nicht bereit ist zum fahren (weil hinderniss im weg) -> bremsen und 'stopped'-flag setzten
            if (checkFrontalDistance())
            {
                stopped = true;
                drive(AdafruitController::kBrake);
                evadeObstacle();
            }
            // wenn buggy fahren darf
            else
            {
                // fahrtzeit addieren (zeit in der buggy tatsaechlich fahren kann)
                i += INTERVALL_MS;
                // wenn noch gestoppt ist -> 'stopped'-flag zuruecksetzten und weiter fahren in die vorherige richtung
                if (stopped)
                {
                    stopped = false;
                    drive(cmd);
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(INTERVALL_MS));
        }
        drive(AdafruitController::kBrake);
    }else
    {
        std::cerr << "driveDistance: Motorhat not valid or could not get the proper speed\n";
    }
    
}

// liefert distanz in cm zu dem Object vor dem Buggy
bool AdafruitController::checkFrontalDistance(void)
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

    #if LOG_OUTPUT
        std::cout << "Distance: " << std::fixed << std::setprecision(2) << distance << "cm\n";
    #endif

    // ueberpruefen ob hinderniss in minimaler naehe ist
    if (distance <= MINIMAL_DISTANCE_CM)
    {
        // bremslicht einschalten
        digitalWrite(BREMSLICHT, HIGH);

        return true;
    }
    else
    {
        // bremslicht ausschalten
        digitalWrite(BREMSLICHT, LOW);

        return false;
    }
}

// verarbeitet die command-line argumente in brauchbare(ausfuehrbare) anweisungen fuer den AdafruitController
std::vector<command_time_pair> AdafruitController::parseArguments(int argc, char *argv[], void (*signalHandler)(int))
{
    std::vector<command_time_pair> commands;

    for (int i = 1; i < argc; i++)
    {
        // richtung und dauer aus den commandline-Argumenten rausfiltern - 'Richtung-Dauer' -> Richtung(AdafruitController::Command); Dauer(double - sekunden)
        char *command = strtok(argv[i], "-");
        double duration = atof(strtok(nullptr, "-"));

        AdafruitController::ControllerCommand cmd;

        #if LOG_OUTPUT
            std::cout << argv[i] << ": " << command << "~" << duration << std::endl;
        #endif

        // programm stoppen bei fehlerhaftem argument
        if ((cmd = getCommandForString(command)) == AdafruitController::kInvalidCommand)
        {
            std::cout << "Command \'" << command << "\' not known\n";
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

// hilfsfunktion zum ausgeben der richtung und fahrtzeit
void AdafruitController::printCommand(AdafruitController::ControllerCommand cmd, double seconds)
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

// wandelt string (Forward, Backward,...) in tatsaechlichen AdafruitController::Command um (kForward, kBackward -> benoetigt fuer controller.drive)
AdafruitController::ControllerCommand AdafruitController::getCommandForString(const char *command)
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