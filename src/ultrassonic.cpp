#include "ultrassonic.hpp"

namespace Ultrassonic {

    void setup() {
        pinMode(trig, OUTPUT);
        pinMode(echo, INPUT);
        long t = pulseIn(echo, HIGH);
        int distance = t / 29 / 2;
    }

    int measureWaterLevel() {
        digitalWrite(trig, LOW);
        delayMicroseconds(4);
        digitalWrite(trig, HIGH);
        delayMicroseconds(10);
        digitalWrite(trig, LOW);
        long t = pulseIn(echo, HIGH);
        int distance = t / 29 / 2;

        return (MaxLevel - distance);
    }

}
