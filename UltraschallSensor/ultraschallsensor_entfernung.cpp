// Includes
#include <wiringPi.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

// Ultraschall-sensor (trigger und echo) Pins
#define GPIO_TRIGGER 1
#define GPIO_ECHO 5

// funktion die die distanz zum nächsten objekt vor dem buggy in cm zurueckgibt
double distance_cm(void) {
    // trigger signal schicken
    digitalWrite(GPIO_TRIGGER, HIGH);

    // 10ns warten, damit trigger das high registrieren kann
    std::this_thread::sleep_for(std::chrono::nanoseconds(10));

    // trigger ausschalten
    digitalWrite(GPIO_TRIGGER, LOW);

    auto startTime = std::chrono::high_resolution_clock::now();
    auto stopTime = std::chrono::high_resolution_clock::now();

    while (digitalRead(GPIO_ECHO) == LOW) {
        startTime = std::chrono::high_resolution_clock::now();
    }

    while (digitalRead(GPIO_ECHO) == HIGH) {
        stopTime = std::chrono::high_resolution_clock::now();
    }

    double elapsed_ms =
        std::chrono::duration<double, std::milli>(stopTime - startTime).count();

    // mit der Schallgeschwindigkeit (34.3 cm/ms) multiplizieren
    // durch 2 teilen, da hin und rueck-weg
    return (elapsed_ms * 34.3) / 2;
}

int main() {
    // wiringPi setuppen
    wiringPiSetup();

    // Richtung der GPIO - Pins festlegen(IN / OUT)
    pinMode(GPIO_TRIGGER, OUTPUT);
    pinMode(GPIO_ECHO, INPUT);

    while (true) {
        // distanz ausgeben
        std::cout << "Distance: " << std::fixed << std::setprecision(2)
                  << distance_cm() << "cm\n";
        // threat eine sekunde schlafen legen
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
