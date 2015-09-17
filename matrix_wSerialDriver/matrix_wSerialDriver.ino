#include "Digits.h"

#define R 1

#define SER_OUTPUT_PIN     7
#define STORE_PIN          2
#define SHIFT_PIN          3
#define CLEAR_PIN          4
#define OUTPUT_ENABLE_PIN  6

#define X_PIN              19

void storageClockTick()
{
  digitalWrite(STORE_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(STORE_PIN, LOW);
}


void shiftClockTick()
{
  digitalWrite(SHIFT_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SHIFT_PIN, LOW);
}

void shift(byte value)
{
  if (value != LOW)
    digitalWrite(SER_OUTPUT_PIN, HIGH);

  shiftClockTick();
  digitalWrite(SER_OUTPUT_PIN, LOW);
}


void reset()
{
  digitalWrite(CLEAR_PIN, LOW);
  delay(1);
  digitalWrite(CLEAR_PIN, HIGH);
}


void doTestPattern()
{
  shift(LOW); // 2-H
  shift(HIGH);
  shift(HIGH);
  shift(HIGH);
  shift(HIGH);
  shift(HIGH);
  shift(HIGH); // 2-B
  shift(HIGH); // 2-A

  shift(HIGH); // 1-H
  shift(LOW);
  shift(LOW);
  shift(LOW);
  shift(LOW);
  shift(LOW);
  shift(LOW);
  shift(LOW); // 1-A

  digitalWrite(13, HIGH);
  storageClockTick();
  delay(1000);
  digitalWrite(13, LOW);

  while (true)
  {
    delay(2000);
  }
}


void writePixel(byte row, byte col)
{
  for (int c = 7; c >= 0; c--)
  {
    if ( col != c )
      shift(HIGH);
    else
      shift(LOW);
  }

  for ( int r = 0; r <= 7; r++)
  {
    if ( row != r )
      shift(LOW);
    else
      shift(HIGH);
  }

  storageClockTick();
}

byte (*digits[])[8] = { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TWENTY };

void setup()
{

  pinMode(OUTPUT_ENABLE_PIN, OUTPUT);
  digitalWrite(OUTPUT_ENABLE_PIN, HIGH);

  pinMode(SER_OUTPUT_PIN, OUTPUT);
  pinMode(STORE_PIN, OUTPUT);
  pinMode(SHIFT_PIN, OUTPUT);
  pinMode(CLEAR_PIN, OUTPUT);

  //pinMode(X_PIN, INPUT);

  reset();

//  digitalWrite(OUTPUT_ENABLE_PIN, LOW);
  pinMode(SER_OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_ENABLE_PIN, LOW);
  
  
  Serial.begin(9600);


}

int computeRollingAverage(int* rollingAverage)
{
  int average = 0;
  for ( int i = 0; i < 100; i++ )
  {
    average += rollingAverage[i] / 100;
  }

  //average /= 100;

  return average;
}

void loop()
{

  int power = (analogRead(X_PIN) * 1.33) / 102;
  randomSeed(analogRead(18));

  /*
  int digit = 5;
  int thresholdHigh = 310;
  int thresholdLow = 290;
  unsigned long time = 0;

  while(true)
  {
    delay(500);

    power = (analogRead(X_PIN));

    Serial.print("Power:  ");
    Serial.print(power);
    Serial.println("");

    if( power > thresholdHigh || power < thresholdLow )
    {
      digit = random(1,20);
      Serial.print("displaying digit:  ");
      Serial.println(digit);
    }
    else
      continue;

    time = millis();
    */
  //    while(millis() < time + 5000)

  //doTestPattern();

  byte digit = 0;
  unsigned long start = millis();
  bool on = false;

  while (true)
  {
    if (!on)
    {
        
    }
    
    if (millis() - start >= 1000){
      digit = random(0,9);
      start = millis();
    }
      
    
    for (int row = 7; row >= 0; row--)
    {
      for (int col = 0; col <= 7; col++)
      {
        if(digits[digit][row][col] != 0)
        {
            writePixel(col, row);
        } 
        /*
        if(twenty8x8image[row][col] != 0){
          writePixel(col, row);
          digitalWrite(13, HIGH);
        }*/
      }
    }
  }

  //}
}
