
#include "U8glib.h"
#include "SoftwareSerial.h"
#include <avr/sleep.h>


#define nextButton 7      //define the pin numbers for the buttons
#define previousButton 5
#define menuButton 4



SoftwareSerial bluetooth(2,3);    //Use software serial for bluetooth

const unsigned char PROGMEM ICON_BITMAP_1[] = 
{     //Android Logo
  0x00,0x00,
  0x00,0x00,
  0x04,0x40,
  0x07,0xc0,
  0x0f,0xe0,
  0x0b,0xa0,
  0x1f,0xf0,
  0x00,0x00,
  0x5f,0xf4,
  0x5f,0xf4,
  0x5f,0xf4,
  0x5f,0xf4,
  0x1f,0xf0,
  0x0c,0x60,
  0x0c,0x60,
  0x00,0x00
};

const unsigned char PROGMEM ICON_BITMAP_20[] = 
{    //chat
  0x00,0x00,
  0x00,0x00,
  0x3f,0xf8,
  0x40,0x04,
  0x40,0x04,
  0x40,0x04,
  0x40,0x04,
  0x49,0x24,
  0x40,0x04,
  0x40,0x04,
  0x40,0x04,
  0x3f,0xc8,
  0x00,0x28,
  0x00,0x18,
  0x00,0x08,
  0x00,0x00
}; 

const unsigned char PROGMEM ICON_BITMAP_65[] = 
{    //bar signal
  0x00,0x00,
  0x00,0x00,
  0x3f,0xe0,
  0x10,0x40,
  0x08,0x80,
  0x05,0x00,
  0x02,0x00,
  0x02,0x00,
  0x02,0x00,
  0x00,0x0c,
  0x00,0x6c,
  0x03,0x6c,
  0x1b,0x6c,
  0x1b,0x6c,
  0x00,0x00,
  0x00,0x00
};

int i,c=0,t=0;

boolean clockUp = true;
boolean menuShow = false;
boolean selectButtonbool = false;
boolean newMessage = false;
boolean newCall = false;
boolean newCallR = false;
boolean newNote = false;
boolean newBuzz = false;

byte hours = 0;
byte minutes = 0;
byte seconds = 0;
byte day = 0;
byte date = 0;
byte month = 0;
int year = 0;

int k,j,n1,n2,n3,n4,n5,n6,n7,n8,N1,N2,N3,N4,N5,N6,N7,N8 = 0;
float x1,x2,ans = 0.0;

char DateBuffer[30];
char* TimeStorage [6];
char* vout;

String monthw;
String number;
String numberR;
String NoteS;
String message;     //Variable for SMS messages
String blReceived;    //Storage for strings received from bluetooth

char msg[150];    //Char array for SMS
char numb[15];
char numbR[15];
char note[300];
String op[4]={"+","-","*","/"};

byte menuSelection = 0;   //Menu
byte menuSelections = 0;  //Settings
byte menuSelectiona = 0;  //Apps
byte len;         //Length of message
byte len1;
byte len2;
byte l;

static unsigned long lastTick = 0;

//U8GLIB_SSD1306_ADAFRUIT_128X64 u8g(U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST); // I2C / TWI
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);
//U8GLIB_SH1106_128X64_2X u8g(13, 11, 10, 9,8); // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);	// I2C / TWI 
void setup(void) {
  // flip screen, if required
  //u8g.setRot180();
  
pinMode(nextButton,INPUT);  //Set digital pins as inputs
pinMode(previousButton,INPUT);
pinMode(menuButton,INPUT);


  
Serial.begin(9600);
bluetooth.begin(9600);
  
digitalWrite(nextButton,HIGH);  //Enable internal pull up on all buttons
digitalWrite(previousButton,HIGH);
digitalWrite(menuButton,HIGH);

delay(500);

drawStartUp();                                                                                                                                   
delay(4000);
   
}//end of setup

void loop() {
  int k,j,n1,n2,n3,n4,n5,n6,n7,n8,N1,N2,N3,N4,N5,N6,N7,N8 = 0;
  float x1,x2,ans = 0.0;
  t++;
  if(t>160)
  {
    sleep();
  }
  if(digitalRead(menuButton)==LOW)
    t=0; 
if(bluetooth.available() > 0 /*&& bluetooth.find("(")*/)
{
  blReceived = bluetooth.readString();
  if(blReceived.startsWith("1"))    //1 is the preamble date
  {
   
    seconds++;
    blReceived.toCharArray(DateBuffer,blReceived.length()+1);
    vout = strtok(DateBuffer,",");
    for(int i=0;i<=6;i++)
    {
      //if(TimeStorage[i] = NULL)
      //{Serial.println("Exited Loop");
      //  break;}
      TimeStorage[i] = strtok(NULL,",");
      delay(10);
    }
    //int example = bluetooth.parseInt();
    day = atoi(TimeStorage[0] - 1);
    date = atoi(TimeStorage[1]);
    month = atoi(TimeStorage[2]);
    year = atoi(TimeStorage[3]);
    hours = atoi(TimeStorage[4]);
    minutes = atoi(TimeStorage[5]);
    seconds = atoi(TimeStorage[6]);
    blReceived="";
    t=0;
  }
  else if (blReceived.startsWith("2"))
  {
   
    seconds++;
    newMessage = true;
    message = blReceived;
    //sms();
    //delay(300000);
    blReceived="";
    t=0;
  }
  else if (blReceived.startsWith("3"))
  {
    
    seconds++;
    newCall = true;
    number = blReceived;
    //num();
    //delay(300000);
    blReceived="";
    t=0;
  }
  else if (blReceived.startsWith("4"))
  {
     
      newCallR = true;
      numberR = blReceived;
      t=0;
      u8g.firstPage();
   do
   {
      u8g.setFont(u8g_font_unifont);
      u8g.setPrintPos(0,20);
      u8g.print("Calling...");
      u8g.setPrintPos(0,40);
      u8g.print(numberR);
      len2 = numberR.length();

      number.toCharArray(numbR, len2+1);
      //Serial.println(len2);
      if(len2<15)
      {
         //Serial.println("If");
         for (int i = 0;(i-1)<len2;i++)
         {
            //Serial.println("Enter for loop");
            u8g.setPrintPos(i*8,30);
            u8g.print(numbR[i-15]);
            delay(10);
            if(numbR[i-16] == '\0')
            { 
               //Serial.println("Break!!");
               break;
               //delay(3000);
            }
         }
      }
  }
  while (u8g.nextPage());
  blReceived="";
  delay(15000);
  seconds=seconds+15;
  newCallR = false;
  }
  else if (blReceived.startsWith("5"))
  {
   
    seconds++;
    newNote = true;
    NoteS = blReceived;
    //notes();
    //delay(300000);
    blReceived="";
    t=0;
  }
  
  
  /*Serial.println(day-1);
  Serial.println(date);
  Serial.println(month);
  Serial.println(year);
  Serial.println(hours);
  Serial.println(minutes);
  Serial.println(seconds);*/
}     //end of if for datetime

  /*Serial.println(day-1);
  Serial.println(date);
  Serial.println(month);
  Serial.println(year);
  Serial.println(hours);
  Serial.println(minutes);
  Serial.println(seconds);*/

/*else if(bluetooth.available() > 0 && bluetooth.find("CMD")==true)
{
  Serial.println("ENTERED MESSAGE");
  message = bluetooth.readString();
  Serial.println(message);
  //delay(300000);
}
*/
  
if(digitalRead(previousButton)==LOW && digitalRead(nextButton)==LOW)  //button is activated
{
  seconds=seconds+4;    
  if(menuShow == false)    //To toggle the menu states
  {
     menuShow = true;
     delay(100);
     menu();  //Get back the menu selection number                                                            
     if(menuSelection == 4)
     {
        seconds=seconds+4;
        delay(100);
        menuSelection = 0;
        while(digitalRead(menuButton)== HIGH)
        {
           HA();
           delay(10);
        }
     }
     if(menuSelection == 3)
     {
        seconds=seconds+4; 
        settings();
        if(menuSelections == 2)
        {
           seconds=seconds+4;
           delay(100);
           menuSelections=0;
           delay(100);
           while(digitalRead(menuButton)== HIGH)
           {
              notice();
           }
           delay(100);
        }
        if(menuSelections == 3)
        {
           seconds=seconds+4;
           delay(100);
           menuSelections=0;
           while(digitalRead(menuButton)==HIGH)
           {
              torch();
              delay(10);
           }
        }
        if(menuSelections == 4)
        {
           seconds=seconds+4;
           delay(100);
           menuSelections=0;
           while(digitalRead(menuButton)==HIGH)
           {
             bluetooth.write("1");
             delay(1000);
             bluetooth.write("");
           }
        }
        bluetooth.write("");
        bluetooth.write("2");
        bluetooth.write("");
     }
     if(menuSelection == 2)
     {
        seconds=seconds+4;
        apps(); 
        if(menuSelectiona == 2)
        {  
           seconds=seconds+4;
           delay(100);
           menuSelectiona = 0;
           while(digitalRead(menuButton)==HIGH)
           {
              Calc();
           }
           bluetooth.write("");    
        }  
        
        if(menuSelectiona == 3)
        {
           seconds=seconds+4;
           delay(100);
           menuSelectiona = 0;
           while(digitalRead(menuButton)== HIGH)
           {sms();}
           delay(100);
        }
        if(menuSelectiona == 4)
        {
           seconds=seconds+4;
           delay(100);
           menuSelectiona = 0;
           while(digitalRead(menuButton)== HIGH)
           {call();}
           delay(100);
        }
     }
  }
  if(menuShow == true)
  {
     menuShow = false;
     delay(100);   
  }
}//end of if
advanceTime();
delay(30);
}//end of loop

void sleep()
{
   u8g.firstPage();
   do{}
   while(u8g.nextPage());
}
  
void advanceTime()
{
   if(millis()-lastTick>950)  //somehow
   {
      lastTick = millis();
      seconds++;
   }
   
   if (seconds > 59)
   {
      minutes++;
      seconds = 0;
   }
   
   if (minutes >59)
   {
      hours++;
      minutes = 0;
   }
   
   if (hours > 23)
   {
      hours = 0;
      minutes = 0;
      date+=1;
   }
   draw();                                                                                             
}

 
//*****------------------U8Glib Functions------------------*****
void HA()
{
  u8g.firstPage();  
  do {
        bluetooth.write("3");
        u8g.setFont(u8g_font_unifont);
        u8g.setPrintPos(0,10);
        u8g.print("other options");
        u8g.setFont(u8g_font_unifont);
        u8g.setPrintPos(0,30);
        u8g.print("In Development");
        bluetooth.write("");
  } while( u8g.nextPage() );
}



void drawStartUp()
{
  u8g.firstPage();
  do
  {
    //u8g_prepare();
    u8g.setColorIndex(0);
    u8g.drawBox(0,0,127,63);
    u8g.setColorIndex(1);
    u8g.setFont(u8g_font_7x14r);
    u8g.setPrintPos(5,10);
    u8g.print("Developed By:-");
    u8g.setFont(u8g_font_7x14r);
    u8g.setPrintPos(10,38);
    u8g.print("algerian inventor");
    u8g.setFont(u8g_font_7x14r);
    u8g.setPrintPos(5,62);
    u8g.print("V1.0");
  }
  while(u8g.nextPage());
}  //end of drawStartUp

void menu()
{  
   u8g.setColorIndex(1);
   //u8g.drawCircle(20,20,14);
   menuSelection = 1;
   while(digitalRead(menuButton) == HIGH)
   {
      if(digitalRead(nextButton) == LOW)
      {
         delay(20);
         menuSelection++;
         delay(50);
      }  
     
     if(digitalRead(previousButton) == LOW)
     {
        delay(20);
        menuSelection--;
        delay(50);
     }
     
     if(menuSelection > 4)
     {menuSelection = 1;}

     if(menuSelection < 1)
     {menuSelection = 4;}
     //draw picture here
     drawMenu();

     delay(50); //For debouncing
    
     //draw menus here
  }//end of while loop
}//end of menu

void drawMenu()
{
   u8g.firstPage();
   do
   {
      u8g.setFont(u8g_font_6x10r);
      u8g.setPrintPos(30,8);
      u8g.print("--- Menu ---");
      //u8g.setPrintPos(0,10);
      //u8g.print(menuSelection);
      u8g.drawFrame(0,(menuSelection*13)-3,128,13); //Box height is 13 pixels
      u8g.drawLine(0,10,128,10);

      //Draw menu options
      u8g.drawStr(3,20,"Return");
      u8g.drawStr(3,20+13,"Apps");
      u8g.drawStr(3,20+13+13,"Settings");
      u8g.drawStr(3,20+13+13+13,"other options");
      //u8g.drawStr(3,20+13+13+13,"More");   
   }
   while(u8g.nextPage());
}

void apps()
{
   u8g.setColorIndex(1);
   //u8g.drawCircle(20,20,14);
   menuSelectiona = 1;
   delay(200);
   while(digitalRead(menuButton) == HIGH)
   {
      if(digitalRead(nextButton) == LOW)
      {
         delay(20);
         menuSelectiona++;
         Serial.println("Up");
         delay(50);
      }
     
      if(digitalRead(previousButton) == LOW)
      {
         delay(20);
         menuSelectiona--;
         delay(50);
      }

      if(menuSelectiona > 4)
      {menuSelectiona = 1;}

      if(menuSelectiona < 1)
      {menuSelectiona = 4;}
      //draw picture here
    drawApps();
    delay(50); //For debouncing
  }//end of while loop
}

void notice()
{
   u8g.firstPage();
   do
   {
      u8g.setFont(u8g_font_6x10r);
      //u8g.print(NoteS);
      l = NoteS.length();
      NoteS.toCharArray(note, l+1);
      //Serial.println(len);
      if(l>0)
      {
        for(i=0;i<20;i++)
        {
           u8g.setPrintPos(i*6,8);
           u8g.print(note[i]);
           delay(10);
        }
      }
      if(l>20)
      {
        for(i=20;i<40;i++)
        {
           u8g.setPrintPos((i-20)*6,17);
           u8g.print(note[i]);
           delay(10);
        }
      }
      if(l>40)
      {
        for(i=40;i<60;i++)
        {
           u8g.setPrintPos((i-40)*6,26);
           u8g.print(note[i]);
           delay(10);
        }
      }
      if(l>60)
      {
        for(i=60;i<80;i++)
        {
           u8g.setPrintPos((i-60)*6,35);
           u8g.print(note[i]);
           delay(10);
        }
      }
      if(l>80)
      {
        for(i=80;i<100;i++)
        {
           u8g.setPrintPos((i-80)*6,43);
           u8g.print(note[i]);
           delay(10);
        }
      }
      if(l>100)
      {
        for(i=100;i<120;i++)
        {
           u8g.setPrintPos((i-100)*6,51);
           u8g.print(note[i]);
           delay(10);
        }
      }
      if(l>120)
      {
        for(i=120;i<140;i++)
        {
           u8g.setPrintPos((i-120)*6,59);
           u8g.print(note[i]);
           delay(10);
        }
      }
  }
  while (u8g.nextPage());
  delay(30);
  newNote = false;
}

void torch()
{
   u8g.firstPage();
   do{u8g.drawBox(0,0,127,63);}
   while(u8g.nextPage());
}

void drawApps()
{
   u8g.firstPage();
   do
   {
      u8g.setFont(u8g_font_6x10r);
      u8g.setPrintPos(5,8);
      u8g.print("--- Applications ---");
      //u8g.setPrintPos(0,10);
      //u8g.print(menuSelection);
      u8g.drawFrame(0,(menuSelectiona*13)-3,128,13);  //Box height is 13 pixels
      u8g.drawLine(0,10,128,10);

      //Draw menu options
      u8g.drawStr(3,20,"Return");
      u8g.drawStr(3,20+13,"CALC");
      u8g.drawStr(3,20+13+13,"SMS");
      u8g.drawStr(3,20+13+13+13,"CALLS");
      //u8g.drawStr(3,20+13+13+13,"More");
   }  
   while(u8g.nextPage());
}

void settings()
{
   u8g.setColorIndex(1);
   //u8g.drawCircle(20,20,14);
   menuSelections = 1;
   delay(200);
   while(digitalRead(menuButton) == HIGH)
   {  
      if(digitalRead(nextButton) == LOW)
      {
         delay(20);
         menuSelections++;
         Serial.println("Up");
         delay(50);
      }
     
      if(digitalRead(previousButton) == LOW)
      {
         delay(20);
         menuSelections--;
         delay(50);
      }
     
      if(menuSelections > 4)
      {menuSelections = 1;}

      if(menuSelections < 1)
      {menuSelections = 4;}
      //draw picture here
      drawSettings();

      delay(50); //For debouncing
   }//end of while loop
}

void sms()
{
   u8g.firstPage();
   do
   {
      u8g.setFont(u8g_font_unifont);
      u8g.setPrintPos(0,10);
      u8g.print(message);
      len = message.length();

      message.toCharArray(msg, len+1);
      //Serial.println(len);
      if(len>16)
      {
         //Serial.println("If");
         for (int i = 0;(i-1)<len;i++)
         {
            //Serial.println("Enter for loop");
            u8g.setPrintPos(i*8,30);
            u8g.print(msg[i+16]);
            delay(10);
            if(msg[i+17] == '\0')
            { 
               //Serial.println("Break!!");
               break;
               //delay(3000);
            }
         }
      }
      
      if(len>32)
      {
         //Serial.println("If");
         for (int i = 0;(i-1)<len;i++)
         {
            //Serial.println("Enter for loop");
            u8g.setPrintPos(i*8,50);
            u8g.print(msg[i+32]);
            delay(10);
            if(msg[i+33] == '\0')
            { 
               //Serial.println("Break!!");
               break;
               //delay(3000);
            }
         }
      }
  }
  while (u8g.nextPage());
  delay(30);
  newMessage = false;
}

void call()
{
   u8g.firstPage();
   do
   {
      u8g.setFont(u8g_font_unifont);
      u8g.setPrintPos(0,10);
      u8g.print(number);
      len1 = number.length();

      number.toCharArray(numb, len1+1);
      //Serial.println(len1);
      if(len1<15)
      {
         //Serial.println("If");
         for (int i = 0;(i-1)<len1;i++)
         {
            //Serial.println("Enter for loop");
            u8g.setPrintPos(i*8,30);
            u8g.print(numb[i-15]);
            delay(10);
            if(numb[i-16] == '\0')
            { 
               //Serial.println("Break!!");
               break;
               //delay(3000);
            }
         }
      }
  }
  while (u8g.nextPage());
  delay(30);
  newCall = false;
}

void drawSettings()
{
   u8g.firstPage();
   do
   {
      u8g.setFont(u8g_font_6x10r);
      u8g.setPrintPos(15,8);
      u8g.print("--- Settings ---");
      //u8g.setPrintPos(0,10);
      //u8g.print(menuSelection);
      u8g.drawFrame(0,(menuSelections*13)-3,128,13);  //Box height is 13 pixels
      u8g.drawLine(0,10,128,10);

      //Draw menu options
      u8g.drawStr(3,20,"Return");
      u8g.drawStr(3,20+13,"Notes");
      u8g.drawStr(3,20+13+13,"Torch");
      u8g.drawStr(3,20+13+13+13,"Find My Phone");
  }
  while(u8g.nextPage());
}

void CalcPrintdata()
{
  u8g.setPrintPos(50,25);
  u8g.print(n3);
  u8g.setPrintPos(40,25);
  u8g.print(n4);
  u8g.setPrintPos(30,25);
  u8g.print(n5);
  u8g.setPrintPos(20,25);
  u8g.print(n6);
  u8g.setPrintPos(60,25);
  u8g.print(".");
  u8g.setPrintPos(70,25);
  u8g.print(n7);
  u8g.setPrintPos(80,25);
  u8g.print(n8);
  u8g.setPrintPos(3,37);
  u8g.print(op[k]);
  u8g.setPrintPos(50,37);
  u8g.print(N3);
  u8g.setPrintPos(40,37);
  u8g.print(N4);
  u8g.setPrintPos(30,37);
  u8g.print(N5);
  u8g.setPrintPos(20,37);
  u8g.print(N6);
  u8g.setPrintPos(60,37);
  u8g.print(".");
  u8g.setPrintPos(70,37);
  u8g.print(N7);
  u8g.setPrintPos(80,37);
  u8g.print(N8);
  u8g.setPrintPos(5,46);
  u8g.print("---------------");      
  u8g.setPrintPos(5,54);
  u8g.print(ans);
}

void Calc()
{
   u8g.firstPage();
   do{
        u8g.setFont(u8g_font_unifont);
        u8g.setPrintPos(0,10);
        u8g.print("Calculator:-");
        if(digitalRead(previousButton) == LOW)
        {
          j++;
          delay(300);
          if(j>12)
            j = 0;
        }
        if(digitalRead(nextButton)==LOW && digitalRead(previousButton)==LOW)
        {
          j=13;
          delay(300);
        }
        if(j==0)
        {
          CalcPrintdata();
          if(digitalRead(nextButton) == LOW)
          {
            n6++;
            delay(300);
          }
          if(n6>9)
            n6=0;
        }
        if(j==1)
        {
          CalcPrintdata();
          if(digitalRead(nextButton) == LOW)
          {
            n5++;
            delay(300);
          }
          if(n5>9)
            n5=0;
        }
        if(j==2)
        {
          CalcPrintdata();
          if(digitalRead(nextButton) == LOW)
          {
            n4++;
            delay(300);
          }
          if(n4>9)
            n4=0;
        }
        if(j==3)
        {
          CalcPrintdata();
          if(digitalRead(nextButton) == LOW)
          {
            n3++;
            delay(300);
          }
          if(n3>9)
            n3=0;
        }
        if(j==4)
        {
          CalcPrintdata();
          if(digitalRead(nextButton) == LOW)
          {
            n7++;
            delay(300);
          }
          if(n7>9)
            n7=0;
        }
        if(j==5)
        {
          CalcPrintdata();
          if(digitalRead(nextButton) == LOW)
          {
            n8++;
            delay(300);
          }
          if(n8>9)
            n8=0;
        }
        if(j==6)
        {
          CalcPrintdata();
          if(digitalRead(nextButton) == LOW)
          {
            k++;
            delay(300);
          }
          if(k>3)
            k=0;
        }
        if(j==7)
        {
          CalcPrintdata();
          if(digitalRead(nextButton) == LOW)
          {
            N6++;
            delay(300);
          }
          if(N6>9)
            N6=0;
        }
        if(j==8)
        {
          CalcPrintdata();
          if(digitalRead(nextButton) == LOW)
          {
            N5++;
            delay(300);
          }
          if(N5>9)
            N5=0;
        }
        if(j==9)
        {
          CalcPrintdata();
          if(digitalRead(nextButton) == LOW)
          {
            N4++;
            delay(300);
          }
          if(N4>9)
            N4=0;
        }
        if(j==10)
        {
          CalcPrintdata();
          if(digitalRead(nextButton) == LOW)
          {
            N3++;
            delay(300);
          }
          if(N3>9)
            N3=0;
        }
        if(j==11)
        {
          CalcPrintdata();
          if(digitalRead(nextButton) == LOW)
          {
            N7++;
            delay(300);
          }
          if(N7>9)
            N7=0;
        }
        if(j==12)
        {
          CalcPrintdata();
          if(digitalRead(nextButton) == LOW)
          {
            N8++;
            delay(300);
          }
          if(N8>9)
            N8=0;
        }
        if(j==13)
        {
          CalcPrintdata();
          x1 = n6*1000+n5*100+n4*10+n3+n7*0.1+n8*0.01;
          x2 = N6*1000+N5*100+N4*10+N3+N7*0.1+N8*0.01;
          if(k==0)
          {
            ans = x1 + x2;
          }
          if(k==1)
          {
            ans = x1 - x2;
          }
          if(k==2)
          {
            ans = x1 * x2;
          }
          if(k==3)
          {
            ans = x1 / x2;
          }
        }
     }
     while(u8g.nextPage());
}

void draw() 
{
   u8g.firstPage();
   do
   {
     if(t<160)
     {
      //u8g.drawLine(0,15,128,15);
      u8g.setFont(u8g_font_7x14r);
      u8g.drawStr(3,14,"V1.0");
      u8g.setFont(u8g_font_fur17r);  
      String strDate = String("");
      char time[10];      //9
      if (hours < 10)
         strDate += "0";
      strDate += hours;
      strDate += ":";
      if (minutes < 10)
         strDate += "0";
      strDate += minutes;
      strDate += ":";
      if (seconds < 10)
         strDate += "0";
      strDate += seconds;
      strDate.toCharArray(time,10);     //9
      time[12] = 0x00;            //9

      u8g.drawStr(20,40,time);
  
     switch (month)
     {
       case 01:
          monthw = "January";
          break;
       case 02:
          monthw = "February";
          break;
       case 03:
          monthw = "March";
          break;
       case 04:
          monthw = "April";
          break;
       case 05:
          monthw = "May";
          break;
       case 06:
          monthw = "June";
          break;
       case 07:
          monthw = "July";
          break;
       case 8:
          monthw = "August";
          break;
       case 9:
          monthw = "September";
          break;
       case 10:
          monthw = "October";
          break;
       case 11:
          monthw = "November";
          break;
       case 12:
          monthw = "December";
          break;
    } 
    u8g.setFont(u8g_font_6x10r);  //Smaller font for date
    //u8g.setFont(u8g_font_7x14r);
    u8g.setPrintPos(0,55);
    u8g.print(date);
    u8g.setPrintPos(15,55);
    u8g.print("/");
    /*switch (date)
    {
  case 1:
    u8g.print("st");
    break;
  case 2:
    u8g.print("nd");
    break;
  case 3:
    u8g.print("rd");
    break;
  default:
    u8g.print("th");
    break;
  }
  */
   u8g.setPrintPos(27,55);
   u8g.print(month); //formerly monthw
   u8g.setPrintPos(35,55);
   u8g.print("/");
   u8g.setPrintPos(43,55);
   u8g.print(year);

   //u8g.drawBitmapP(110,1,1,8,IMG_indicator_msg);
   u8g.drawBitmapP( 110, 1, 2, 16, ICON_BITMAP_65);
   if(newNote == true)
   {
      u8g.drawBitmapP(88,45,2,16,ICON_BITMAP_20);
   }
   if(newMessage == true)
   {
      u8g.drawBitmapP(110,45,2,16,ICON_BITMAP_1);
   }
   if(newCall == true)
   {
      u8g.drawBitmapP(110,45,2,16,ICON_BITMAP_1);
   }
   }
   }
 while(u8g.nextPage());
}//end of draw
  
