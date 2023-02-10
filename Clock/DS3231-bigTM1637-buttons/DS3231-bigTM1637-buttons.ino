/*budnikAndrei*/
//для Витиных практикантов
/*недостатки схемы - за сутки отстают примерно на 6 минут (почему?), 
(вопрос - как исправить? какие строчки кода закоментить, какие добавить?),
у нашей микросхемы часов реального времени DS3231 шесть выводов,
а задействованы 4, почему?
Разобраться как запрограммировать DS3231, установить дуту и время?
можно ли это сделать в данной программе?
Объяснить как все работает*/
 
//устанавливаем эти библиотеки (Ctrl+Shift+i) - менеджер библиотек
#include "RTClib.h"
#include "GyverTM1637.h"

#define CLK 4
#define DIO 5

GyverTM1637 disp(CLK, DIO);

// Кнопки установки времени
#define buttonHour 6  // Часы
#define buttonMin  7  // Минуты
#define buttonSet  8  // Установка
#define buttonBright   9  // Кнопка яркости

byte Bri=1; //переменная для усановки яркости

RTC_DS3231 rtc;

void setup() {
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);

  /*будем использовать монитор порта (Ctrl+Shift+i) 
  на случай отладки, если возникнут проблемы с LED-дисплеем или плохим контактом где либо*/
  Serial.begin(9600); 
  delay(1000);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));  стартовые настройки DS-3231, задаются ранее в другой программе
  }
// яркость у нашего LED-дисплея меняется от 0 до 7 
  disp.brightness(5); //(мы устанавливаем яркость на 5 (ее интенсивность)
  disp.clear();
}

void loop() {
   
  DateTime now = rtc.now();

  int displaytime = (now.hour() * 100) + now.minute();



int8_t TimeDisp[4];
   
   TimeDisp[0] = now.hour() / 10;  // Десятки часов
   TimeDisp[1] = now.hour() % 10;  // Единицы часов
   TimeDisp[2] = now.minute() / 10;// Десятки минут
   TimeDisp[3] = now.minute() % 10;// Единицы минут

//вывод данных на "Монитор порта"
Serial.print(now.hour()); 
Serial.print(":"); 
Serial.println(now.minute());

//установка минут
if (digitalRead(buttonMin) == LOW && digitalRead(buttonSet) == LOW)
{  
    TimeDisp[3]++;
    if (TimeDisp[3] > 9) 
    {
      TimeDisp[3] = 0; 
      TimeDisp[2]++;
    }
    if (TimeDisp[2] > 5) 
      TimeDisp[2] = 0;
}

//установка часов
if (digitalRead(buttonHour) == LOW && digitalRead(buttonSet) == LOW)
{
    TimeDisp[1]++;
    if (TimeDisp[1] > 9) 
    {
        TimeDisp[1] = 0;
        TimeDisp[0]++;
        if (TimeDisp[0] > 2)
          TimeDisp[0] = 0;
     }
  if (TimeDisp[0]*10 + TimeDisp[1] > 24) 
    {
      TimeDisp[1] = 0; 
      TimeDisp[0] = 0;
    }
}

//установка яркости
if (digitalRead(buttonBright) == LOW && digitalRead(buttonSet) == LOW)
{ 
  Bri++;
  if (Bri>7)
    {Bri=1;}
}


int ho = (TimeDisp[0]*10 +  TimeDisp[1]);
int mi = (TimeDisp[2]*10 +  TimeDisp[3]);


rtc.adjust(DateTime(now.year(), now.month(), now.day(), ho,  mi, now.second()));

Serial.print("Brithness is ");
Serial.println(Bri);
Serial.print(now.year()); Serial.print("/");
Serial.print(now.month()); Serial.print("/");
Serial.println(now.day());
Serial.print(now.hour()); Serial.print(":");
Serial.print(now.minute()); Serial.print(":");
Serial.println(now.second());
//Serial.println(now.year(), now.month(), now.day(), now.hour(),  now.minute(), now.second());

//ваывод данных на дисплей (point - точки-разделитель часов и минут)
disp.brightness(Bri);
disp.display(TimeDisp);
//disp.point(now.second() % 2 ? POINT_ON : POINT_OFF);
  disp.point(1);
  delay(500);
  disp.point(0);
  delay(500);
}
