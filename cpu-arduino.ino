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
  // CHAR
    char serialData;
  // STRING
    String info;
    String commandsTxt[] = {
      "led 1",
      "led 2",
      "SO",
      "uptime",
      "temperature", 
      "print txt",
      "Motor"
      };

    String ledsText[] = {
      "On",
      "Off"
      };

  //Pins
    #define tempRead A0
    #define ledOne 4
    #define ledTwo 5
    #define servo_motor 6
  // BOOl
    bool modeData = false;  // Verifica si esta conectado el bluetooth
    bool commandActivate = false;
    bool commandSystem = false;
    bool modeMotor = false;
    bool ledOneActivate = false;
    bool ledTwoActivate = false;
  //INT
    int y;  // Parte de la funcion de selector de menu
    int x;  // Parte de la funcion de submenu
    int uptime;
    int commandsSelect;
    int commandLen = sizeof(commandsTxt) / sizeof(commandsTxt[0]);
    int ledMenu = 0;
//Function that only execute only one tine
void setup() {
  //Start the bluetooth and Serial Port
  bluetooth.begin(9600);
  Serial.begin(9600);

  //initialization of digital Pins
  initPins();

  //Start the LCD with IC2
  lcd.init();
  lcd.backlight();
  beginMessage();

  //Start the servo-motor
  motor.attach(servo_motor);
}

//Loop for arduino
void loop() {
  if (bluetooth.available())
  {
    uptime = millis() / 1000;
    modeData = true;
    serialData = bluetooth.read();
    clean(serialData);
  }

  mode();
}

void initPins(){
  pinMode(ledOne, OUTPUT);
  pinMode(ledTwo, OUTPUT);
}

void beginMessage(){
  lcd.setCursor(0, 0);
  lcd.print("Booting AROPSY.");
  delay(500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Booting AROPSY..");
  delay(500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Booting AROPSY.");
  delay(500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Booting AROPSY..");
  delay(500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Booting AROPSY.");
  delay(500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Booting AROPSY..");
  delay(500);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Loading...");
  lcd.setCursor(0, 1);
  lcd.print("----------------");

  delay(2000);
  lcd.clear();
}

//NOTE - Funcion de modo
void mode(){
  if (modeData)
  {
    menu();
  }else if(modeData == false){
    lcd.setCursor(0, 0);
    lcd.print("Esperando");
    lcd.setCursor(0, 1);
    lcd.print("Conexion");
  }
}

//NOTE - Funcion de menu
void menu(){
  lcd.clear();
  select();
  selectMenu();
  subMenu();
  delay(150);
}

void select(){
  if (commandActivate)
  {
    switch (serialData)
    {
    case '4': // Left
      serialData = '0';
      x = 0;
      commandActivate = false;
      break;
    default:
      break;
    }
  }
  //Move in y axis in the main mode
  else{
    switch (serialData)
    {
    case '1': // Up
      y--;
      serialData = '0';
      break;
    case '2': // Down
      y++;
      serialData = '0';
      break;
    case '3': // Right
      x = 1;
      commandsSelect = 0;
      commandActivate = true;
      serialData = '0';
      break;
    case '4': // Left
      x = 0;
      serialData = '0';
      break;
    default:
      break;
    }
  }

  //For y axis
  if (y > commandLen - 1)
  {
    y--;
  }else if (y < 0)
  {
    y = 0;
  }
  
  //For y axis in commands mode
  if (commandsSelect > commandLen - 1)
  {
    commandsSelect--;
  }
  else if (commandsSelect < 0)
  {
    commandsSelect = 0;
  }
}

void selectMenu()
{
  if (y == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print(">" + commandsTxt[0]);
    lcd.setCursor(1, 1);
    lcd.print(commandsTxt[1]);
  }
  else if (y == 1)
  {
    lcd.setCursor(1, 0);
    lcd.print(commandsTxt[0]);
    lcd.setCursor(0, 1);
    lcd.print(">" + commandsTxt[1]);
  }
  else if (y == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">" + commandsTxt[2]);
    lcd.setCursor(1, 1);
    lcd.print(commandsTxt[3]);
  }
  else if (y == 3)
  {
    lcd.setCursor(1, 0);
    lcd.print(commandsTxt[2]);
    lcd.setCursor(0, 1);
    lcd.print(">" + commandsTxt[3]);
  }
  else if (y == 4)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">" + commandsTxt[4]);
    lcd.setCursor(1, 1);
    lcd.print(commandsTxt[5]);
  }
  else if (y == 5)
  {
    lcd.setCursor(1, 0);
    lcd.print(commandsTxt[4]);
    lcd.setCursor(0, 1);
    lcd.print(">" + commandsTxt[5]);
  }
  else if (y == 6)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">" + commandsTxt[6]);
  }
}

void subMenu(){
  //Seleccion del primer menu
  if (x == 1)
  {
    lcd.clear();
    //Seleccion de las opciones del menu
    if (y == 0)
    {
      ledOneFunction();
    }

    if (y == 1)
    {
      ledTwoFunction();
    }

    if (y == 2)
    {
      systemOperativeCommand();
    }

    if (y == 3)
    {
      uptimeFunction();
    }

    if (y == 4)
    {
      willBeAdded();
    }

    if (y == 5)
    {
      willBeAdded();
    }

    if (y == 6)
    {
      willBeAdded();
    }
  }
  return;
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
return;
}

//NOTE - Funcion de comandos

void systemOperativeCommand(){
  lcd.setCursor(0, 0);
  lcd.print("---SO: AROPSY---");
  lcd.setCursor(0, 1);
  lcd.print("-Based in C++ --");
  return;
}
//NOTE - Funcion de controlar servomotor
//REVIEW - NO SE HA PROBADO
void Motor(){
  char i;
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

void ledOneFunction(){
  switch(serialData){
    case '1':
      commandsSelect = 0;
      serialData = 0;
      break;
    case '2':
      commandsSelect = 1;
      serialData = 0;
     break;
    case '3':
      if(commandsSelect == 0){
        ledOneActivate = true;
      }else{
        ledOneActivate = false;
      }
      break;
  }

  if(commandsSelect == 0){
    lcd.setCursor(0,0);
    lcd.print(">" + ledsText[0]);
    lcd.setCursor(1,1);
    lcd.print(ledsText[1]);
  }else{
    lcd.setCursor(1,0);
    lcd.print(ledsText[0]);
    lcd.setCursor(0,1);
    lcd.print(">" + ledsText[1]);
  }

  if(ledOneActivate){
    digitalWrite(ledOne, HIGH);
  }else{
    digitalWrite(ledOne, LOW);
  }
}

void ledTwoFunction(){
  lcd.clear();
  switch(serialData){
    case '1':
      commandsSelect = 0;
      serialData = '0';
      break;
    case '2':
      commandsSelect = 1;
      serialData = '0';
      break;
    case '3':
      if(commandsSelect == 0){
        ledTwoActivate = true;
      }else{
        ledTwoActivate = false;
      }
      break;
  }

  if(commandsSelect == 0){
    lcd.setCursor(0,0);
    lcd.print(">" + ledsText[0]);
    lcd.setCursor(1,1);
    lcd.print(ledsText[1]);
  }else if(commandsSelect == 1){
    lcd.setCursor(1,0);
    lcd.print(ledsText[0]);
    lcd.setCursor(0,1);
    lcd.print(">" + ledsText[1]);
  }

  if(ledTwoActivate){
    digitalWrite(ledTwo, HIGH);
  }else{
    digitalWrite(ledTwo, LOW);
  }
}

void uptimeFunction(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("-----Uptime-----");
  lcd.setCursor(0,1);
  lcd.print(uptime);
  lcd.setCursor(9,1);
  lcd.print("seconds");
}

void tempFunction(){

}

void printTextFunction(){

}

void willBeAdded(){
  lcd.setCursor(0,0);
  lcd.print("This function is");
  lcd.setCursor(1,1);
  lcd.print("in progress :D");
}
