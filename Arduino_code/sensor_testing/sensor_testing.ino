#include <CapacitiveSensor.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10 megohm between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50 kilohm - 50 megohm. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 * Best results are obtained if sensor foil and wire is covered with an insulator such as paper or plastic sheet
 */


CapacitiveSensor   cs_3_2 = CapacitiveSensor(3,2);        // 10 megohm resistor between pins 3 & 2, pin 2 is sensor pin, add wire, foil
CapacitiveSensor   cs_6_5 = CapacitiveSensor(6,5);        // 10 megohm resistor between pins 6 & 5, pin 5 is sensor pin, add wire, foil
CapacitiveSensor   cs_9_8 = CapacitiveSensor(9,8);        // 10 megohm resistor between pins 9 & 8, pin 8 is sensor pin, add wire, foil
CapacitiveSensor   cs_12_11 = CapacitiveSensor(12,11);        // 10 megohm resistor between pins 12 & 11, pin 11 is sensor pin, add wire, foil

void setup()                    
{

   cs_3_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(9600);

}

void loop()                    
{
    long start = millis();
    long total1 =  cs_3_2.capacitiveSensor(30);
    long total2 =  cs_6_5.capacitiveSensor(30);
    long total3 =  cs_9_8.capacitiveSensor(30);
    long total4 =  cs_12_11.capacitiveSensor(30);

    Serial.print(millis() - start);        // check on performance in milliseconds
    Serial.print("\t");                    // tab character for debug window spacing

    Serial.print(total1);                  // print sensor output 1
    Serial.print("\t");
    Serial.print(total2);                  // print sensor output 2
    Serial.print("\t");
    Serial.println(total3);                // print sensor output 3
    Serial.print("\t");
    Serial.println(total4);                // print sensor output 4


    delay(100);                             // arbitrary delay to limit data to serial port 
}
