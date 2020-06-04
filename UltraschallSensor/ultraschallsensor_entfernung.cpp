#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <wiringPi.h>

#define GPIO_TRIGGER 1
#define GPIO_ECHO 5

double distance_cm(void)
{
    digitalWrite(GPIO_TRIGGER, HIGH);

    std::this_thread::sleep_for(std::chrono::nanoseconds(10));

    digitalWrite(GPIO_TRIGGER, LOW);

    auto startTime = std::chrono::high_resolution_clock::now();
    auto stopTime = std::chrono::high_resolution_clock::now();

    while(digitalRead(GPIO_ECHO) == LOW)
    {
        startTime = std::chrono::high_resolution_clock::now();
    }

    while (digitalRead(GPIO_ECHO) == HIGH)
    {
        stopTime = std::chrono::high_resolution_clock::now();
    }

    double elapsed_ms = std::chrono::duration<double, std::milli>(stopTime - startTime).count();

    // mit der Schallgeschwindigkeit (34.3 cm/ms) multiplizieren
    // durch 2 teilen, da hin und rueck-weg
    return (elapsed_ms * 34.3) / 2;
}

int main()
{

    //Richtung der GPIO - Pins festlegen(IN / OUT)
    wiringPiSetup();

    pinMode(GPIO_TRIGGER, OUTPUT);
    pinMode(GPIO_ECHO, INPUT);

    while(true)
    {
        std::cout << "Distance: " << std::fixed << std::setprecision(2) << distance_cm() << "cm\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}