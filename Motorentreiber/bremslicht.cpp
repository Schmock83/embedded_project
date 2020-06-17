#include <wiringPi.h>
#include <signal.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>
#include "source/adafruitController.h"

#define GPIO_TRIGGER 1
#define GPIO_ECHO 5
#define BREMSLICHT 26
#define MINIMAL_DISTANCE_CM 15

#define INTERVALL_MS \
    50 // Intervall in dem geprueft werden soll ob hinderniss vor buggy

AdafruitController controller;

/// Interrupt Routine for STRG-C
void signalHandler(int signum)
{
    std::cout << "Strg-C Programmende" << std::endl;

    // motoren releasen
    controller.drive(AdafruitController::kRelease);

    exit(signum);
}

double distance_cm(void)
{
    digitalWrite(GPIO_TRIGGER, HIGH);

    std::this_thread::sleep_for(std::chrono::nanoseconds(10));

    digitalWrite(GPIO_TRIGGER, LOW);

    auto startTime = std::chrono::high_resolution_clock::now();
    auto stopTime = std::chrono::high_resolution_clock::now();

    while (digitalRead(GPIO_ECHO) == LOW)
    {
        startTime = std::chrono::high_resolution_clock::now();
    }

    while (digitalRead(GPIO_ECHO) == HIGH)
    {
        stopTime = std::chrono::high_resolution_clock::now();
    }

    double elapsed_ms =
        std::chrono::duration<double, std::milli>(stopTime - startTime).count();

    // mit der Schallgeschwindigkeit (34.3 cm/ms) multiplizieren
    // durch 2 teilen, da hin und rueck-weg
    double distance = (elapsed_ms * 34.3) / 2;

    if (distance <= MINIMAL_DISTANCE_CM)
    {
        // bremslicht anschalten
        digitalWrite(BREMSLICHT, HIGH);

        // buggy anhalten
        controller.drive(AdafruitController::kBrake);

        std::cout << "Buggy wurde angehalten.\n";
    }
    else
    {
        // bremslicht ausschalten
        digitalWrite(BREMSLICHT, LOW);
    }

    return distance;
}

int main()
{
    // Csignal für Abbruch über STRG-C
    signal(SIGINT, signalHandler);

    // Richtung der GPIO - Pins festlegen(IN / OUT)
    wiringPiSetup();

    // wiringPi pin-modes festlegen
    pinMode(GPIO_TRIGGER, OUTPUT);
    pinMode(GPIO_ECHO, INPUT);
    pinMode(BREMSLICHT, OUTPUT);

    if (controller.motorsAvailable())
    {
        controller.setSpeed();

        // geradeaus fahren
        controller.drive(AdafruitController::kForward);

        // schleife, die periodisch testet, ob Hinderniss vor Buggy ist
        double distance = -1;
        do
        {
            // distance neu berechnen
            distance = distance_cm();
            std::cout << "Distance: " << std::fixed << std::setprecision(2)
                      << distance << "cm\n";
            // threat schlafen legen für INTERVALL ms
            std::this_thread::sleep_for(
                std::chrono::milliseconds(INTERVALL_MS));
        } while (distance > MINIMAL_DISTANCE_CM);
    }

    return 0;
}
