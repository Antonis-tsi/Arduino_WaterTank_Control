//ANTWNHS TSIGGERHS 2026
//ERGASIA EXAMINOY 2
//23/04/2024

#include <LiquidCrystal.h>//VIVLIOTHIKES GIA TO LCD,KEYPAD KAI I2C EPIKOINONIA
#include <Keypad.h>
#include <Wire.h>

#define EEPROM_ID 0xA0 >> 1//DEFINE TIS DIEYTIHINSEIS GIA EUKOLIA 
#define INLET 0x00
#define OUTLET 0x01
#define LOW_FLOATER 0x11
#define VOLUME 0x30
#define TEMP  0x31
#define HEATER 0x02
#define STIRRER 0x04

int cursor=11,i=0,target_temp=26,temp,position=16,digit[4];//DHLWSEIS METAVLITWN INT,UNSIGNED_INT KAI CHAR 
unsigned int desired_vol=0,vol=0;
char floater_value,Hbyte,Lbyte ;

const int rs =13,en=12,d4=8,d5=9,d6=10,d7=11;//DHLWSH KAI SYNDESH LCD
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

const byte ROWS = 4;//DHLWSH KAI SYNDESH KEYPAD
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A0, A1, A2, A3}; 
byte colPins[COLS] = {7, 6, 5,4};
Keypad customkeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);//KEYPAD


void setup() {//ARXH SETUP GIA EMFANISI MONO MIA FORA 
  
  lcd.begin(16,2);//DHLWSH KAI XEKINIMA TWN EPIKOINWNIWN
  Serial.begin(9600);
  Wire.begin(); 

  lcd.setCursor(0,0);//EMFANISI ARXIKOY MHNYMATOS STHN PRWTH KAI DEYTERH SEIRA ME TO EPITHETO AM KAI PROTROPI STON XRHSTH
  lcd.print("TSIGGERHS, 2026");
  lcd.setCursor(0,1);
  lcd.print("ENTER VOL: ????");
  lcd.setCursor(11,1);
  lcd.cursor();//RYTHMISI NA EMFANIZETAI O CURSORS OPOU THA FAINETAI POY THA GRAPSEI O XRHSTHS
  lcd.blink();  

}//TELOS SETUP

void loop() {//ARXH LOOP GIA SYNEXIS EPANALIPSI EKTOS AN DEN TO EPITREPSOUN OI ELEGXOI MAS

  char key = customkeypad.getKey();//DHLWSH METAVLITIS KEY GIA THN EISAGWGH XARAKTHRWN APO TO KEYPAD
  
  if (key && cursor<=14) { //ELEGXOS AN PATITHIKE KOUMPI KAI AN O CURSORAS EINAI STI THESI POU EPITHIMOUME DHLADH 14
    lcd.setCursor(cursor, 1);   //EMPAFNISI TOY ARITHMOU POY PATITHIKE STHN SWSTH THESI
    lcd.print(key);
    
    digit[i]= key - '0';//APOTHIKEUSI TWN PSIFIWN SE PINAKA
    
    Wire.beginTransmission(EEPROM_ID);//ENARJH METADOSHS I2C GIA NA EPIKINONISOUME KAI NA APOTHIKEUSOUME STHN MNHMH EEPROM
    Wire.write(position);
    Wire.write(key);
    Wire.endTransmission();//TELOS METADOSHS I2C
    
    i++;//AUJHSH METAVLITWN GIA NA DIATGIROUME TH THESI POU THELOUME KATHE FORA
    position++;
    cursor++;
  }else if(cursor == 15){//ELEGXOS WSTE AN PATITHIKAN 4 PSIFIA NA PROXWRHSEI TO PROGRAMMA
    desired_vol = digit[0]*1000 + digit[1]*100 + digit[2]*10 + digit[3];//YPOLOGISMOS TOU VOULUME POU PATHSE O XRHSTHS
    lcd.noCursor();//SVISIMO CURSORA AFOU DEN THA GRAPSEI KATI ALLO O XRSTHS

    delay(500);//MIKRO DELAY 0.5sec
    
    lcd.clear();//KATHARISMA OTHONIS KAI EKTIPOSI KAINOURGIOU MINIMATOS STH PRWTH KAI DEUTERI SEIRA
    lcd.setCursor(0,0);
    lcd.print("SAVING VOLUME");
    lcd.setCursor(0,1);
    lcd.print("TO EEPROM");
    
    delay(500);//MIKRO DELAY
    
    lcd.clear();//KATHARISMA OTHONIS KAI EKTIPOSI TOU VOLUME POY PATITHIKE APO TON XRHSTH
    lcd.setCursor(0,0);
    lcd.print("VOLUME  ");
    lcd.setCursor(9,0);
    lcd.print(desired_vol);
    
    delay(500);//MIKRO DELAY

    lcd.clear();//KATHARISMA LCD KAI EKTIPOSI MINIMATOS OPOU ENIMERONI TON XRHSH GIA TH KATASTASI THS DEXAMENIS
    lcd.setCursor(0,0);
    lcd.print("OUTLET ON");
    lcd.setCursor(0,1);
    lcd.print("INLET OFF");

    Serial.write(INLET);//EISAGWGH THS DIEUTHINSIS TIS VALVIDAS KAI KLEISIMO EKEINIS
    Serial.write(0x00);
    
    Serial.write(OUTLET);//EISAGWGH THS DIEUTHINSIS TIS VALVIDAS KAI ANOIGMA EKEINIS
    Serial.write(0x01);
    
    do{//LOOP KAI DIAVSMA SYNEXOS TIS TIMIS TOY LOW FLOATER WSTE NA KATALAVOUME POTE THA ADIASEI H DEJAMENI
      Serial.write(LOW_FLOATER);
      while(!Serial.available()){}
      floater_value = Serial.read();
    }while(floater_value == 1);
    
    Serial.write(OUTLET);//EISAGWGH THS DIEUTHINSIS TIS VALVIDAS KAI KLEISIMO EKEINIS
    Serial.write(0x00);
   
    delay(500);//MIKRO DELAY

    lcd.clear();//KATHARISMA LCD KAI EKTIPOSI MINIMATOS OPOU ENIMERONI TON XRHSH GIA TH KATASTASI THS DEXAMENIS
    lcd.setCursor(0,0);
    lcd.print("OUTLET OFF");
    lcd.setCursor(0,1);
    lcd.print("INLET ON");
    
    Serial.write(INLET);//EISAGWGH THS DIEUTHINSIS TIS VALVIDAS KAI ANOIGMA EKEINIS
    Serial.write(0x01);
    
    do{//LOOP KAI DIAVSMA SYNEXOS TIS TIMIS TOU ASITHITIRA VOLUME MEXRI NA FTASEI TIN EPITHMITI TIMI TOU XRHSTH
      Serial.write(VOLUME);
      while(!Serial.available()){}
      Hbyte=Serial.read();
      while(!Serial.available()){}
      Lbyte=Serial.read();
      vol =(Hbyte << 8) |(0x00ff&Lbyte);
    }while(vol <= desired_vol);

    Serial.write(INLET);//EISAGWGH THS DIEUTHINSIS TIS VALVIDAS KAI KLEISIMO EKEINIS
    Serial.write(0x00);
    
    delay(500);//MIKRO DELAY
    
    lcd.clear();//KATHARISMA LCD KAI EKTIPOSI MINIMATOS OPOU ENIMERONI TON XRHSH GIA TH KATASTASI THS DEXAMENIS
    lcd.setCursor(0,0);
    lcd.print("TEMP     ");
    lcd.setCursor(0,1);
    lcd.print("STIR/HEATER ON");   
 
    Serial.write(STIRRER);//EISAGWGH THS DIEUTHINSIS TOY STIRRER KAI ANOIGMA MESW SEIRIAKIS EPIKOINWNIAS
    Serial.write(0x01);
    
    Serial.write(HEATER);//EISAGWGH THS DIEUTHINSIS TOY HEATER KAI ANOIGMA MESW SEIRIAKIS EPIKOINWNIAS
    Serial.write(0x01);
    
    do{//DIAVASMA ASSTHITIRA THERMOKRASIAS KAI DIAKOPH PROGRAMMATOS MEXRI NA FTASEI TIN EPITHIMITI THERMOKRASIA ME DIARKIS ENIMEROSI THS THERMOKRASIAS
      Serial.write(TEMP);
      while(!Serial.available()){}
      Hbyte=Serial.read();
      while(!Serial.available()){}
      Lbyte=Serial.read();
      temp =(Hbyte << 8) |(0x00ff&Lbyte);  
      lcd.setCursor(11,0);
      lcd.print(temp);
    }while(temp < target_temp);
    
    Serial.write(STIRRER);//EISAGWGH THS DIEUTHINSIS TOY SIRRER KAI ANOIGMA MESW SEIRIAKIS EPIKOINWNIAS
    Serial.write(0x00);
    
    Serial.write(HEATER);
    Serial.write(0x00);//EISAGWGH THS DIEUTHINSIS TOY HEATER KAI KKLEISIMO MESW SEIRIAKIS EPIKOINWNIAS    

    delay(500);//MIKRO DELAY

    lcd.setCursor(11,0);//EKTYPWSI TIS THERMOKRASIAS KAI MHNYMA OTI TELEIWSE OLI DIADIKASIA 
    lcd.print(temp);
    lcd.setCursor(0,1);
    lcd.print("CYCLE COMPLETE");   
    
    
    cursor++;//AUJHSH TOU CURSOR GIA NA MHN JANA ARXISEI TO LOOP 
    

  }//TELOS ELEGXOY
 

}//TELOS LOOP
