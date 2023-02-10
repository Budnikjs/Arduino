#include "RTClib.h"

#include "GyverTM1637.h"

#define CLK 4
#define DIO 5

GyverTM1637 disp(CLK, DIO);

RTC_DS3231 rtc;
//TM1637Display display = TM1637Display(CLK, DIO);

void setup() {
  Serial.begin(9600);
  delay(1000);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  disp.brightness(7);
  disp.clear();
}

void loop() {
   
  DateTime now = rtc.now();

  int displaytime = (now.hour() * 100) + now.minute();

//  Serial.println(displaytime);

int8_t TimeDisp[4];
   
   TimeDisp[0] = now.hour() / 10;  // Десятки часов
   TimeDisp[1] = now.hour() % 10;  // Единицы часов
   TimeDisp[2] = now.minute() / 10;// Десятки минут
   TimeDisp[3] = now.minute() % 10;// Единицы минут


Serial.print(now.hour()); 
Serial.print(":"); 
Serial.println(now.minute());
  
//  disp.point(1);
  
 

// byte hell[4] = {6, 6, 6, 6};
 // disp.display(hell);
disp.display(TimeDisp);
  disp.point(1);
  delay(500);
  disp.point(0);
  delay(500);
}
