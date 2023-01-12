//!SECTION LIBRARIES
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2, 3);
// Connect HC-06      Arduino Uno
//     Pin 1/TXD          Pin 2
//     Pin 2/RXD          Pin 3

LiquidCrystal_I2C lcd(0x27, 16, 2); 
// set the LCD address to 0x27 for a 16 chars and 2 line display
// SDA     A4
// SCL     A5

Servo motor;
// Pin 6

// DATA
char serialData;
String info;

void setup() {
  bluetooth.begin(9600);
  bluetooth.println("Connected");
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
  motor.attach(6);
}

void loop() {
  if (bluetooth.available())
  {
    lcd.clear();
    serialData = bluetooth.read();
    info += serialData;
    mode();
  }
    lcd.setCursor(0, 0);
    lcd.print("Esperando");
    lcd.setCursor(0, 1);
    lcd.print("Conexion");
    delay(200);
}

//NOTE - Funcion de menu
void mode(){
    lcd.clear();
    info = "";
    lcd.setCursor(0,0);
    lcd.print("Select mode:");
    lcd.setCursor(0,1);
    lcd.print("1)Commands   2)Motor");
    if (info == "Command")
    {
      lcd.clear();
      lcd.print("--Command Mode--");
      delay(1000);
      lcd.clear();
      }
}

//NOTE - Funcion de limpiar pantalla
void clean(char data)
{
  if (data == '@')
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cleanning...");    
    delay(500);
    lcd.clear();
    info = "";
  }

}

//NOTE - Funcion de comandos
void commands(String string){
  
  String file;
  
  if (string == "ls")
  {
    if (file == "")
    {
      lcd.clear();
      lcd.print("no hay archivos");
      info = "";
    }else {
      lcd.clear();
      lcd.print(file);
      info = "";
    }
  }else if(string == "mkdir")
  {
  
    lcd.clear();
    lcd.print("File name: ");
    lcd.setCursor(0,1);
    lcd.print(info);
    file = info;
    delay(100);
    info = "";
  
  }else if(string == "neofetch")
  {
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SO: Test");  
  
  }else{
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Command error");
  
  }
}

//NOTE - Funcion de controlar servomotor
void Motor(char i){
  int angle = map(i, 0, 9, 0, 180);
  if (i < 180)
  {
    lcd.clear();
    lcd.print("Angle error");
  }else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Valor: " + i);

    lcd.setCursor(0, 0);
    lcd.print("Angulo: " + angle);

    motor.write(angle);
  }
}
