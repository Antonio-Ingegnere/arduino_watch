#include <EEPROM.h> //Is it necessary to use EEPROM?
#include <GxEPD.h>
#include <GxGDEW027C44/GxGDEW027C44.h>

//TODO: Check that I need all the fonts - remove if no.
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>


#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

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

GxIO_Class io(SPI, /*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9); // arbitrary selection of 8, 9 selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=*/ 9, /*BUSY=*/ 7); // default selection of (9), 7

void setup()
{
  Serial.begin(115200);
  //pinMode(13, OUTPUT);
  
  _minutes = 0;
  _hours = 0;

  display.init(115200); // enable diagnostic output on Serial

  Serial.println("setup done");

  display.setRotation(1);
  
  display.drawPaged(showMyInformationCallback);


  
  //attachInterrupt(digitalPinToInterrupt(2), rtcTick, RISING);
}

void showMyInformationCallback()
{
  const char* name = "FreeMonoBold9pt7b";
  const GFXfont* f = &FreeMonoBold9pt7b;
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();
  display.println("Antonio");
  
  display.setTextColor(GxEPD_RED);

  display.println("Il Ingegnere");
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
