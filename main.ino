#include <EEPROM.h>

/////////////////////////////////////////////////
//Address information section
/////////////////////////////////////////////////
/*
5th - hours
6th - minutes
*/
const int HOURS_ADDR = 5;
const int MINUTES_ADDR = 6;  

/////////////////////////////////////////////////
//BL constants
/////////////////////////////////////////////////
const int MINUTES_PER_HOUR = 60;
const int HOURS_PER_DAY = 3;
const int HOURS_AM_PM = 12;

volatile byte _minutes;
volatile byte _hours;

void setup()
{
  Serial.begin(9600);
  //pinMode(13, OUTPUT);
  
  _minutes = 0;
  _hours = 0;
  
  attachInterrupt(digitalPinToInterrupt(2), rtcTick, RISING);
}

void rtcTick()
{
  Serial.println("Interrupt has occured");
  
  ++_minutes;
  
  if(_minutes == MINUTES_PER_HOUR)
  {
    ++_hours;
    _minutes = 0;
  }
  
  if(_hours == HOURS_PER_DAY)
  {
    _hours = 0;
  }
  
  showTime();
}

void showTime()
{
  char strFinal[10];
  char strMins[5];
  
  itoa(_hours, strFinal, DEC);
  strcat(strFinal, " : ");

  itoa(_minutes, strMins, DEC);
  strcat(strFinal, strMins);
  
  Serial.println(strFinal);
}

void readInformation()
{  
  _hours = EEPROM.read(HOURS_ADDR);
  _minutes = EEPROM.read(MINUTES_ADDR);
}

void writeInformation()
{
  EEPROM.write(HOURS_ADDR, _hours);
  EEPROM.write(MINUTES_ADDR, _minutes);
}

void setupInterruption()
{
}

void loop()
{

  //digitalWrite(13, HIGH);
  //delay(1000); // Wait for 1000 millisecond(s)
  //digitalWrite(13, LOW);
  //delay(1000); // Wait for 1000 millisecond(s)
  
  
  
}