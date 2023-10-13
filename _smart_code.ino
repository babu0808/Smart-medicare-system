#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);  //RS,EN,D4,D5,D6,D7
int count, Alarm_hour[20];
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
int v;
int DONEPEZIL = 7;
int RAZADYNE = 6;
int msg;
int call = 0;
int w = 10;
int x = 10;
int hours;
int minutes;
int seconds;
int ir1 = A0;
int ir2 = A1;
int voice = 3;
int voicecall;
void tablets();
int d = 0, r = 0;
void setup() {
  // put your setup code here, to run once:
  while (!Serial)
    ;  // for Leonardo/Micro/Zero
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SMART MEDICARE SYSTEM");
  delay(1000);
  pinMode(DONEPEZIL, OUTPUT);  //4
  pinMode(RAZADYNE, OUTPUT);   //5
  pinMode(ir1, INPUT_PULLUP);
  pinMode(ir2, INPUT_PULLUP);
  pinMode(voice, OUTPUT);

  lcd.clear();
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print(now.day());
  lcd.print('/');
  lcd.print(now.month());
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.setCursor(0, 1);
  hours = now.hour();
  lcd.print(hours);
  lcd.print(':');
  minutes = now.minute();
  lcd.print(minutes);
  lcd.print(':');
  lcd.print(now.second());
  lcd.print(" ");
  seconds = now.second();
  if ((hours == 22 && minutes >= 0) && (hours == 22 && minutes <= 59)) {

    lcd.setCursor(7, 0);
    lcd.print(' ');
    lcd.setCursor(8, 0);
    lcd.print("plz take ");
    lcd.setCursor(9, 1);
    lcd.print("RAZADYNE");
    digitalWrite(RAZADYNE, HIGH);   //4
    digitalWrite(DONEPEZIL, HIGH);  //4
                                    // lcd.print(" ");

    if (v == 0) {
      digitalWrite(voice, LOW);
    } else {
      digitalWrite(voice, HIGH);
    }
    if (call == 0) {
      Serial.println("ATD9705192524;");
      delay(1000);
    } else {
      Serial.print("ATH");
    }
    tablets();
    if (d == 1) {
      digitalWrite(4, LOW);                                 //WRONG TAB ANNOUNCEMNT
      gsm0_msg("8897037974", "WARNING:Took Wrong Tablet");  //patien
      delay(1000);
      d = 0;
    } else if (r == 0) {
      // digitalWrite(5, LOW); //THANK YOU ANNOUNCEMNT
    }
  }

  else if ((hours == 20 && minutes >= 2) && (hours == 21 && minutes <= 59)) {
    lcd.setCursor(7, 0);
    lcd.print(' ');
    lcd.setCursor(8, 0);
    lcd.print("plz take ");
    lcd.setCursor(9, 1);
    lcd.print("DONEPEZIL");
    digitalWrite(DONEPEZIL, LOW);  //4
    digitalWrite(RAZADYNE, LOW);   //4

    if (v == 0) {
      digitalWrite(voice, LOW);
    } else {
      digitalWrite(voice, HIGH);
    }
    if (call == 0) {
      Serial.println("ATD9705192524;");
      delay(1000);
    } else {
      Serial.print("ATH");
    }
    tablets();
    if (d == 1) {
      digitalWrite(4, LOW);                                 //WRONG TAB ANNOUNCEMNT
      gsm0_msg("8897037974", "WARNING:Took Wrong Tablet");  //patien
      delay(1000);
    } else if (r == 0) {
      // digitalWrite(5, LOW); //THANK YOU ANNOUNCEMNT
    }
  } else {
    digitalWrite(DONEPEZIL, HIGH);
    digitalWrite(RAZADYNE, LOW);
    digitalWrite(voice, HIGH);
  }
}



void tablets() {
  if (digitalRead(ir1) == LOW)  //DONEPEZIL
  {

    digitalWrite(voice, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" DONEPEZIL ");
    Serial.print("ATH");
    gsm0_msg("9705192524", "HAREESH has taken DONEPEZIL tablet successfully at");
    delay(1000);

    call = 1;
    v = 1;
    w--;
    d = 1;
    lcd.setCursor(0, 0);
    lcd.print("REMAINING:");
    lcd.print(w);
    delay(2000);
  } else if (digitalRead(ir2) == LOW)  //RAZADYNE
  {
    digitalWrite(voice, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" RAZADYNE ");
    Serial.print("ATH");

    gsm0_msg("9705192524", "HAREESH has taken DONEPEZIL tablet successfully at");
    delay(1000);
    v = 1;
    x--;
    r = 1;

    call = 1;
    lcd.setCursor(0, 0);
    lcd.print("REMAINING:");
    lcd.print(x);
    delay(2000);
  }
}
void gsm_init() {
  Serial.println("AT\r");
  delay(1000);
  Serial.println("AT+CMGF=1\r");
  delay(1000);
}
void gsm0_msg(char *num, char *msg) {
  gsm_init();
  Serial.print("AT+CMGS=\"");
  Serial.print(num);
  Serial.println("\"\r");
  delay(1000);
  Serial.println(msg);  // The SMS text you want to send
  delay(1000);
  Serial.print(hours);  // The SMS text you want to send
  delay(1000);
  Serial.print(':');  // The SMS text you want to send
  delay(1000);
  Serial.print(minutes);  // The SMS text you want to send
  delay(1000);
  Serial.print(':');  // The SMS text you want to send
  delay(1000);
  Serial.println(seconds);  // The SMS text you want to send
  delay(1000);
  Serial.println("THANK YOU");  // The SMS text you want to send
  delay(1000);
  Serial.println((char)26);  // ASCII code of CTRL+Z
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("THANK YOU");
  delay(1000);
}
