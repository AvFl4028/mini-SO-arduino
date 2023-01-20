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
    String commandsTxt[] = {"ls", "mkdir", "neofetch"};

  // INT
    int modeData = 0;                           // Verifica si esta conectado el bluetooth
    int y;                                  // Parte de la funcion de selector de menu
    int x;
    int len = sizeof(modes) / sizeof(modes[0]); // Tamaño del array de modes[]
    int commandLen = sizeof(commandsTxt) / sizeof(commandsTxt[0]);

void setup() {
  bluetooth.begin(9600);
  bluetooth.println("Connected");
  Serial.begin(9600);

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
    modeData = 1;
    serialData = bluetooth.read();
    clean(serialData);
  }
  mode();
}

//NOTE - Funcion de modo
void mode(){
  if (modeData == 1)
  {
    menu();
  }else if(modeData == 0){
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
  /*+
    lcd.setCursor(0,0);
    lcd.print(serialData);

    lcd.setCursor(0, 1);
    lcd.print(len);
*/
  delay(200);
}

void select(){
  switch (serialData)
  {
  case '1':
    y--;
    serialData = '0';
    break;
  case '2':
    y++;
    serialData = '0';
    break;
  case '3':
    x = 1;
    serialData = '0';
    break;
  case '4':
    x = 0;
    serialData = '0';
    break;
  default:
    break;
  }
  if (y > len - 1 || y < 0)
  {
    y = 0;
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
      commands();
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

}

//NOTE - Funcion de comandos
//TODO - Cambiar los comandos a una lista

void commands(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("--Command Mode--");
  delay(500);
  lcd.clear();

  String file;
  String string;
  
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
//REVIEW - NO SE HA PROBADO
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