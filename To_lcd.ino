//nummers
// the 8 arrays that form each segment of the custom numbers
// you can only program 0-7  8chars at a time 
byte LT[8] = 
{
  B00111,
  B01111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte UB[8] =
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte RT[8] =
{
  B11100,
  B11110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte LL[8] =
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01111,
  B00111
};
byte LB[8] =
{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};
byte LR[8] =
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11110,
  B11100
};
byte MB[8] =
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};
byte block[8] =
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

void init_create()
{
  // assignes each segment a write number
  lcd.createChar(0,LT);
  lcd.createChar(1,UB);
  lcd.createChar(2,RT);
  lcd.createChar(3,LL);
  lcd.createChar(4,LB);
  lcd.createChar(5,LR);
  lcd.createChar(6,MB);
  lcd.createChar(7,block);
  delay(500);
  lcd.backlight(); // Enable or Turn On the backlight
  lcd.begin();
  lcd.home(),
  lcd.clear();
  delay(200);
}

void custom0()
{ // uses segments to build the number 0
  lcd.setCursor(x,offsetrow); // set cursor to column 0, line 0 (first row)
  lcd.write(0);  // call each segment to create
  lcd.write(1);  // top half of the number
  lcd.write(2);
  lcd.setCursor(x, 1 + offsetrow); // set cursor to colum 0, line 1 (second row)
  lcd.write(3);  // call each segment to create
  lcd.write(4);  // bottom half of the number
  lcd.write(5);
}

void custom1()
{
  lcd.setCursor(x,+offsetrow);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(x,1+offsetrow);
  lcd.write(4);
  lcd.write(7);
  lcd.write(4);
}

void custom2()
{
  lcd.setCursor(x,offsetrow);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x, 1+ offsetrow);
  lcd.write(3);
  lcd.write(4);
  lcd.write(4);
}

void custom3()
{
  lcd.setCursor(x,offsetrow);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x, 1+ offsetrow);
  lcd.write(4);
  lcd.write(4);
  lcd.write(5); 
}

void custom4()
{
  lcd.setCursor(x,offsetrow);
  lcd.write(3);
  lcd.write(4);
  lcd.write(7);
  lcd.setCursor(x+2, 1+ offsetrow);
  lcd.write(7);
}

void custom5()
{
  lcd.setCursor(x,offsetrow);
  lcd.write(3);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(x, 1+ offsetrow);
  lcd.write(4);
  lcd.write(4);
  lcd.write(5);
}

void custom6()
{
  lcd.setCursor(x,offsetrow);
  lcd.write(0);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(x, 1+ offsetrow);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
}

void custom7()
{
  lcd.setCursor(x,offsetrow);
  lcd.write(1);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(x+2, 1+ offsetrow);
  lcd.write(7);
}

void custom8()
{
  lcd.setCursor(x,offsetrow);
  lcd.write(0);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x, 1+ offsetrow);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
}

void custom9()
{
  lcd.setCursor(x,offsetrow);
  lcd.write(0);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x+2, 1+ offsetrow);
  lcd.write(7);
}
void customdash()
{
  //dash
      lcd.setCursor(x,offsetrow);
      lcd.write(4);
      lcd.setCursor(x, 1 + offsetrow);
      lcd.write(4);
}

void clearnumber()
{ // clears the area the custom number is displayed in 
 lcd.setCursor(x,offsetrow);
 lcd.print("   ");
 lcd.setCursor(x,1+ offsetrow); 
 lcd.print("   ");
}

/*
x is column y is row set with offsetrow here for 20x4 offsetrow =1
 * 
 * 1  0 h
 * 2  3 h
 * 3  6 :
 * 5  9  m
 * 6  12 m
 */

void lcdPrintNumber( int number)
{
  switch (number) {
    case 0:    
     custom0();
     x = x + 4; 
     break;
    case 1:   
      custom1();
       x =x + 4;
      break;
    case 2:   
      custom2();
       x =x + 4;
      break;
    case 3:    
      custom3();
       x =x + 4;
      break;
    case 4:    
     custom4();
      x =x + 4;
      break;
    case 5:   
      custom5();
      x =x + 4;
      break;
    case 6:   
      custom6();
      x =x + 4;
      break;
    case 7:    
      custom7();
       x =x + 4;
      break;
    case 8:   
      custom8();
      x =x + 4;
      break;
    case 9:    
      custom9();
      x =x+4;
      break;
    case 10:
      customdash();
      x =x + 2;
      break;  
        
  }
}

void bigfont_time()
{
  time_t t = now();
  int uur = hour(t);
  int minuten = minute(t);
  Serial.println("");
  Serial.println("tijd:");
  Serial.print(uur);
  Serial.print(":");
  Serial.print(minuten);
  int onesH = (uur%10);
  int tensH = ((uur/10)%10);
  int onesM = (minuten%10);
  int tensM = ((minuten/10)%10);
  x=0;
  lcdPrintNumber(tensH);
  lcdPrintNumber(onesH);  
  lcdPrintNumber(10);
  lcdPrintNumber(tensM);
  lcdPrintNumber(onesM);
  
}
//test big numbers 2 rows
void show_bigfont_show_all()
{
  x=0;
  lcd.clear();
  delay(2000);
  lcd.setCursor(x,0); 
  custom0();
  delay(2000);
  x=0;
  lcd.clear();
  custom1();
  delay(2000);
  x=0;
  lcd.clear();
  custom2();
  delay(2000);
  x=0;
  lcd.clear();
  custom3();
  delay(2000);
   x=0;
  lcd.clear();
  custom4();
  delay(2000);
   x=0;
  lcd.clear();
  custom5();
  delay(2000);
   x=0;
  lcd.clear();
  custom6();
  delay(2000);
   x=0;
  lcd.clear();
  custom7();
  delay(2000);
  x=0;
  lcd.clear();
  custom8();
  delay(2000);
   x=0;
  lcd.clear();
  custom9();
  delay(2000);
}
 //lcd
void clearline(int row)
{ if (row < 4)// row 0 to 3
   {lcd.setCursor(0,row);
               //01234567890123456789
      lcd.print("                    ");
      lcd.setCursor(0,row);
      delay(100);
   }
}
void clearall()
{ 
   //clear all faster than lcd.clear()
     for (int i = 0; i <= 3; i++)
      {lcd.setCursor(0,i);
     //01234567890123456789
      lcd.print("                    ");
      delay(100);
      }
      lcd.home();
}
//check wifi
void wificheck()
{ 
  if (WiFi.status() == WL_CONNECTED)
  {//connected show wificonnected on lcd
    lcd.setCursor(17,0);
    lcd.print("Wf ");
    if (bnowifi) 
    {//reload wheaterdata
      getWeatherData();
      bnowifi = false;
    }
  }
  else
  {//NOt connected show wificonnected on lcd
    lcd.setCursor(17,0);
    lcd.print("Wfx");
    initweatherstr();
    clearline(3);
    bnowifi = true;
  } 
}

//weathermessages scrolling on line 3
void scrollmessage (String message) //message always end with space for scroll
{
  if ( message.length() <= 1)
   { //no srolling when empty
     return ;}
  lcd.setCursor(scrollCursor, 3); // Cursor on row 3 and (scrolling from left end to right)
  lcd.print(message.substring(stringStart,stringEnd)); 
  if(stringStart == 0 && scrollCursor > 0)
  {scrollCursor--; // Moving cursor from 20 to 0
    stringEnd++; //  it will print out character from 0 to 20 the whole length of the screen
  }
  else if (stringStart == stringEnd){ // start all over again
    stringStart = stringEnd = 0;
    scrollCursor = screenWidth;
    counterscrollmessage++; //trigger scroll next string
  } 
  else if (stringEnd == message.length() && scrollCursor == 0) { // if reach to the end character
    stringStart++;
  } 
  else { 
    stringStart++;
    stringEnd++;
  }
}