#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2, 3);
// Connect HC-06      Arduino Uno
//     Pin 1/TXD          Pin 7
//     Pin 2/RXD          Pin 8

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
// SDA     A4
// SCL     A5

// DATA
char serieData;
String data;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    bluetooth.begin(9600);
    bluetooth.print("Encendido!");
    lcd.init();
    lcd.backlight();
}

void loop()
{
    // put your main code here, to run repeatedly:
    if (bluetooth.available())
    {
        serieData = bluetooth.read();
        data += serieData;
        lcd.setCursor(0, 0);
        lcd.print(data);

        clean(serieData);
        changeMode(data);
    }
}

void clean(char clean)
{
    if (serieData == '@')
    {
        data = "";
        lcd.print("cleaning...");
        delay(500);
        lcd.clear();
    }
}

void changeMode(String mode)
{
    if (mode == "Mode")
    {
        lcd.clear();
        data = "";
        lcd.setCursor(0, 0);
        lcd.print("Change Mode");
        lcd.setCursor(0, 1);
        lcd.print("1) Calculator");

        if (mode == "Calculator")
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Chenging mode to:");
            lcd.setCursor(0, 1);
            lcd.print("---Calculator---");
            data = "";
            calculator(serieData);
        }
        mode = "";
    }
}

void calculator(int i)
{
    i += serieData;
    if (i == 0)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter a correct number");
    }
}