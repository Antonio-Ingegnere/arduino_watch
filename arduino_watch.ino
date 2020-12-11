#include <SPI.h>
#include "epd2in7b.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED 1
#define UNCOLORED 0

#define LEN(arr) ((int)(sizeof(arr) / sizeof(arr)[0]))
#define LEN_COLUMNS(arr) ((int)(sizeof(arr[0]) / sizeof(arr)[0][0]))

/////////////////////////////////////////////////
//BL constants
/////////////////////////////////////////////////
const int MINUTES_PER_HOUR = 60;
const int HOURS_PER_DAY = 3;
const int HOURS_AM_PM = 12;

volatile byte _minutes;
volatile byte _hours;

/////////////////////////////////////////////////
//  Screen/State map:
//  Screen: 0000 - main screen - watch
//  Screen: 0001 - main settings screen
//        State:
//          0000 - set time highlighted
//          0001 - set date highlighted
//          0010 - set alarm highlighted
//          0011 - back highlighted
//  Screen: 0010 - set time settings screen
//        State:
//          0000 - hours highlighted
//          0001 - minutes highlighted
//          0010 - yes/no highlighted
/////////////////////////////////////////////////
volatile byte _currentScreen;
volatile byte _currentState;

/////////////////////////////////////////////////
//Screen constants
/////////////////////////////////////////////////
const byte MAIN_SCREEN = 0x00;
const byte SETTINGS_SCREEN = 0x01;
const byte SET_TIME_SCREEN = 0x02;
const byte SET_DATE_SCREEN = 0x03;

/////////////////////////////////////////////////
//Commands constants
/////////////////////////////////////////////////
const byte COMMAND_UP = 0x00;
const byte COMMAND_DOWN = 0x01;
const byte COMMAND_OK = 0x02;

void setup()
{
  Serial.begin(115200);

  // // ShowMainMenu();

  // Epd epd;

  // if (epd.Init() != 0)
  // {
  //   Serial.print("e-Paper init failed\r\n");
  //   return;
  // }

  // epd.ClearFrame();
  // epd.DisplayFrame();

  //Initial setup
  _currentScreen = MAIN_SCREEN;
  _currentState = 0;
}

void setup_old()
{
  _minutes = 0;
  _hours = 0;

  Serial.begin(115200);
  Epd epd;

  if (epd.Init() != 0)
  {
    Serial.print("e-Paper init failed\r\n");
    return;
  }

  epd.ClearFrame();

  unsigned char image[1600];
  Paint paint(image, 176, 24); //width should be the multiple of 8

  paint.Clear(UNCOLORED);
  // paint.DrawStringAt(0, 0, "e-Paper Demo", &Font16, COLORED);
  // epd.TransmitPartialBlack(paint.GetImage(), 16, 32, paint.GetWidth(), paint.GetHeight());

  // paint.Clear(COLORED);
  // paint.DrawStringAt(2, 2, "Hello world!", &Font20, UNCOLORED);
  // epd.TransmitPartialRed(paint.GetImage(), 0, 64, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(64);
  paint.SetHeight(64);

  // paint.Clear(UNCOLORED);
  // paint.DrawRectangle(0, 0, 40, 50, COLORED);
  // paint.DrawLine(0, 0, 40, 50, COLORED);
  // paint.DrawLine(40, 0, 0, 50, COLORED);
  // epd.TransmitPartialBlack(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

  // paint.Clear(UNCOLORED);
  // paint.DrawCircle(32, 32, 30, COLORED);
  // epd.TransmitPartialBlack(paint.GetImage(), 90, 120, paint.GetWidth(), paint.GetHeight());

  // paint.Clear(UNCOLORED);
  // paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  // epd.TransmitPartialRed(paint.GetImage(), 10, 200, paint.GetWidth(), paint.GetHeight());

  // paint.Clear(UNCOLORED);
  // paint.DrawFilledCircle(32, 32, 30, COLORED);
  // epd.TransmitPartialRed(paint.GetImage(), 90, 190, paint.GetWidth(), paint.GetHeight());

  // epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);
  epd.DisplayFrame(IMAGE_REDDIT, NULL);

  epd.DisplayFrame();

  //   unsigned char image[1024];
  //   Paint paint(image, 264, 176);

  //   paint.SetWidth(24);
  //   paint.SetHeight(264);

  //   paint.Clear(UNCOLORED);

  //   paint.SetRotate(ROTATE_270);
  //   /*
  //   paint.DrawStringAt(0,0, "Antonio Ingegnere", &Font16, COLORED);
  // */

  //   paint.Clear(UNCOLORED);
  //   paint.DrawStringAt(0, 0, "Antonio Ingegnere", &Font16, COLORED);

  //   // paint.DrawLine(0, 10, 200, 200, COLORED);

  //   epd.TransmitPartialBlack(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

  //   // epd.TransmitPartialBlack(paint.GetImage(), 85, 120, paint.GetWidth(), paint.GetHeight());

  //   //attachInterrupt(digitalPinToInterrupt(2), rtcTick, RISING);

  //   epd.DisplayFrame();

  //Sleep(5000);

  // epd.RefreshPartial(10,10, 50, 50);
}

void showMyInformationCallback()
{
}

void rtcTick()
{
  Serial.println("Interrupt has occured");

  ++_minutes;

  if (_minutes == MINUTES_PER_HOUR)
  {
    ++_hours;
    _minutes = 0;
  }

  if (_hours == HOURS_PER_DAY)
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

void setupInterruption()
{
}

//Menu fucntionality
#pragma region Menu

const int INITIAL_SHIFT = 40;

void ShowMainMenu()
{
  char menu_options[4][10] = {
      "Set time",
      "Set date",
      "Set alarm",
      "Back"};

  //TODO: replace with regular initialization for EPD

  // Epd* dpd = new Epd();

  // Epd* epd = new Epd();// = getEpd();

  Epd epd;

  if (epd.Init() != 0)
  {
    Serial.print("e-Paper init failed\r\n");
    return;
  }

  //TODO: replace paint to get always ready one
  unsigned char image[1600];
  Paint paint(image, 16, 174);

  epd.ClearFrame();

  paint.SetRotate(ROTATE_270);

  int rows_count = LEN(menu_options);

  for (int i = 0; i < rows_count; ++i)
  {
    paint.Clear(UNCOLORED);
    paint.DrawStringAt(0, 0, menu_options[i], &Font16, COLORED);
    epd.TransmitPartialBlack(paint.GetImage(), INITIAL_SHIFT + 15 * (i + 1), 10, paint.GetWidth(), paint.GetHeight());
  }
  //Set pointer at the beginning
  paint.Clear(UNCOLORED);
  paint.SetHeight(10);
  paint.DrawStringAt(0, 0, ">", &Font16, COLORED);
  epd.TransmitPartialBlack(paint.GetImage(), INITIAL_SHIFT + 15, 190, paint.GetWidth(), paint.GetHeight());

  epd.DisplayFrame();

  //   drawMenuPointer(&dpd, &paint, _currentState);

  // delete epd;
}

void drawMenuPointer(byte state)
{
  Epd epd;

  if (epd.Init() != 0)
  {
    Serial.print("e-Paper init failed\r\n");
    return;
  }

  unsigned char image[1600];
  Paint paint(image, 16, 16);

  epd.ClearFrame();

  paint.SetRotate(ROTATE_270);

  paint.Clear(UNCOLORED);
  // paint.SetHeight(10);
  // paint.SetWidth(16);
  paint.DrawStringAt(0, 0, ">", &Font16, COLORED);
  epd.TransmitPartialBlack(paint.GetImage(), INITIAL_SHIFT + 15 * (state + 1), 184, paint.GetWidth(), paint.GetHeight());
  // epd.TransmitPartialBlack(paint.GetImage(), 5, 35, paint.GetWidth(), paint.GetHeight());
  epd.RefreshPartial(42, 190, 72, 16);
}

#pragma endregion Menu

//Generally there are 3 command:
//  0001 - enter
//  0010 - press up
//  0100 - press down
//The rest business logic is build based on these command and current state
void userInteraction(byte command)
{
  if (_currentScreen == MAIN_SCREEN)
  {
    switch (command)
    {
    case COMMAND_OK:
      _currentScreen = SETTINGS_SCREEN;
      ShowMainMenu();
      break;

    default:
      break;
    }
  }
  else if (_currentScreen == SETTINGS_SCREEN)
  {
    switch (command)
    {
    case COMMAND_DOWN:
      ++_currentState;

      if (_currentState > 3)
        _currentState = 0;

      drawMenuPointer(_currentState);
      break;

    default:
      break;
    }
  }
}

void loop()
{

  //digitalWrite(13, HIGH);
  //delay(1000); // Wait for 1000 millisecond(s)
  //digitalWrite(13, LOW);
  //delay(1000); // Wait for 1000 millisecond(s)
  if (Serial.available() > 0)
  {
    int inByte = Serial.read();
    Serial.print("Byte in - ");
    Serial.println(inByte, DEC);
  }
  emulateUserBehavior();
}

void emulateUserBehavior()
{
  const int SLEEP_TIME = 3000;
  delay(SLEEP_TIME);
  userInteraction(COMMAND_OK);
  delay(SLEEP_TIME);
  userInteraction(COMMAND_DOWN);
  delay(SLEEP_TIME);
  userInteraction(COMMAND_DOWN);
  delay(SLEEP_TIME);
  userInteraction(COMMAND_DOWN);
  delay(SLEEP_TIME);
  userInteraction(COMMAND_DOWN);
  delay(SLEEP_TIME);
  userInteraction(COMMAND_DOWN);
  delay(SLEEP_TIME);
  userInteraction(COMMAND_DOWN);
  delay(SLEEP_TIME);
  userInteraction(COMMAND_DOWN);
}
