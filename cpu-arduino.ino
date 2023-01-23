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
    String modes[] = {"Commands", "Motor", "Info"};
    String commandsTxt[] = {"ls", "mkdir", "neofetch", "test"};

  // BOOl
    bool modeData = false;                           // Verifica si esta conectado el bluetooth
    bool modeCommandActivate = false;
    bool modeAbout = false;  
  //INT
    int y;                                  // Parte de la funcion de selector de menu
    int x;
    int commandsSelect;
    int commandsSelectX;
    int len = sizeof(modes) / sizeof(modes[0]); // Tamaño del array de modes[]
    int commandLen = sizeof(commandsTxt) / sizeof(commandsTxt[0]);

//Function that only execute only one tine
void setup() {
  //Start the bluetooth and Serial Port
  bluetooth.begin(9600);
  bluetooth.println("Connected");
  Serial.begin(9600);
  //Start the LCD with IC2
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
  //Start the servo-motor
  motor.attach(6);
}

//Loop for arduino
void loop() {
  if (bluetooth.available())
  {
    modeData = true;
    serialData = bluetooth.read();
    clean(serialData);
  }
  if (bluetooth.available() == false)
  {
    modeData = false;
  }
  
  mode();
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
  Serial.println(modeData);
}

//NOTE - Funcion de menu
void menu(){
  lcd.clear();

  select();
  selectMenu();
  subMenu();
  commands();
  infoAbout();
/**
    lcd.setCursor(0,0);
    lcd.print(serialData);

    lcd.setCursor(0, 1);
    lcd.print(len);
*/
  delay(200);
}

void select(){
  if (modeCommandActivate)
  {
    switch (serialData)
    {
    case '1': // Up
      commandsSelect--;
      serialData = '0';
      break;
    case '2': // Down
      commandsSelect++;
      serialData = '0';
      break;
    case '3': // Right
      commandsSelect = 0;
      serialData = '0';
      break;
    case '4': // Left
      x--;
      serialData = '0';
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
      serialData = '0';
      break;
    case '4': // Left
      x--;
      serialData = '0';
      break;
    default:
      break;
    }
  }

  //For y axis
  if (y > len - 1)
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
    lcd.setCursor(0,0);
    lcd.print(">" + modes[0]);
    lcd.setCursor(1, 1);
    lcd.print(modes[1]);
  }
  if (y == 1)
  {
    lcd.setCursor(1, 0);
    lcd.print(modes[0]);
    lcd.setCursor(0, 1);
    lcd.print(">" + modes[1]);
  }
  if (y == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">" + modes[2]);
  }
}

void subMenu(){
  if (x == 1)
  {
    lcd.clear();
    if (y == 0)
    {
      modeCommandActivate = true;
    }
    if (y == 2)
    {
      modeAbout = true;
    }
    
  }else if (modeCommandActivate && x == 2)
  {
    lcd.clear();
  }
  else{
    modeCommandActivate = false;
    modeAbout = false;
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

}

//NOTE - Funcion de comandos

void commands(){
  if(modeCommandActivate == true){
    if(commandsSelect == 0)
    {
      lcd.setCursor(0,0);
      lcd.print(">" + commandsTxt[0]);
      lcd.setCursor(1,1);
      lcd.print(commandsTxt[1]);
    }
    else if(commandsSelect == 1)
    {
      lcd.setCursor(1, 0);
      lcd.print(commandsTxt[0]);
      lcd.setCursor(0, 1);
      lcd.print(">" + commandsTxt[1]);
    }
    else if (commandsSelect == 2)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(">" + commandsTxt[2]);
      lcd.setCursor(1,1);
      lcd.print(commandsTxt[3]);
    }
    else if (commandsSelect == 3)
    {
      lcd.setCursor(1, 0);
      lcd.print(commandsTxt[2]);
      lcd.setCursor(0, 1);
      lcd.print(">" + commandsTxt[3]);
    }
    
  }else{
    return;
  }
}

void infoAbout(){
  if(modeAbout)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("-SO: Arduino OS-");
    lcd.setCursor(0, 1);
    lcd.print("-Based in C++ --");
    delay(500);
  }
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