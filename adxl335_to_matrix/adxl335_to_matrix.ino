#include <Digits.h>

//#include "Digits.h"

#define SER_OUTPUT_PIN     7
#define STORE_PIN          2
#define SHIFT_PIN          3
#define CLEAR_PIN          4
#define OUTPUT_ENABLE_PIN  6

#define OUTPUT_ENABLED     LOW
#define OUTPUT_DISABLED    HIGH

#define NUM_SAMPLES_FOR_AVERAGE  20

#define X_PIN              A0
#define Y_PIN              A1
#define Z_PIN              A2
#define SAMPLE_PIN         Y_PIN

#define Y_MINUS_NINETY     260 // analog input value at -90 degres (the adxl335 soc facing down)
#define Y_PLUS_NINETY      400 // analog input value at +90 degres (the adxl335 soc facing up) 
#define Y_ZERO_DEGREES     360 // analog input value at 0 degrees  (the adxl335 oriented on the X axis)
#define Y_EIGHTEEN_DEGREES  14

byte (*digits[]) [8] = { 
  ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TWENTY };

int power = 0;
int samples[NUM_SAMPLES_FOR_AVERAGE];
int rollingAverage = 0;
byte tick = 0;

int scaledDisplayValue = 0;

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


void writePixels(int power)
{
    int scale = 16;
    int numberOfPixels = power / scale;

    int rows = numberOfPixels / 8;
    int extraPixels = numberOfPixels % 8;
    
    for (int r = 0; r < rows; r++)
    {
        for(int c = 0; c < 8; c++)
        {
            writePixel(r,c);
        }
    }
    
    for (int r = rows; r<rows+1; r++)
    {
        for(int c = 0; c < extraPixels; c++)
        {
            writePixel(r, c);
        }
    }
    
}



void setup()
{

  pinMode(OUTPUT_ENABLE_PIN, OUTPUT);
  digitalWrite(OUTPUT_ENABLE_PIN, OUTPUT_DISABLED);

  pinMode(SER_OUTPUT_PIN, OUTPUT);
  pinMode(STORE_PIN, OUTPUT);
  pinMode(SHIFT_PIN, OUTPUT);
  pinMode(CLEAR_PIN, OUTPUT);

  pinMode(X_PIN, INPUT);

  reset();

  pinMode(SER_OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_ENABLE_PIN, OUTPUT_ENABLED);

  Serial.begin(9600);
}


void computeRollingAverage(byte tick, int power)
{
    samples[tick] = power;
  
    rollingAverage = 0;
    for ( int i = 0; i < NUM_SAMPLES_FOR_AVERAGE; i++ )
    {
        rollingAverage += samples[i] / NUM_SAMPLES_FOR_AVERAGE;
    }

    Serial.print("Average:  ");
    Serial.print(rollingAverage);
    Serial.print("  ");
}


void displayDigit(int digit)
{
    for (int row = 7; row >= 0; row--)
    {
        for (int col = 0; col <= 7; col++)
        {
            if(digits[digit][row][col] != 0)
            {
                writePixel(col, row);
            }
        }
    }
}


int scaleRollingAverage()
{
     scaledDisplayValue = max((rollingAverage - Y_MINUS_NINETY), 0);
     scaledDisplayValue = scaledDisplayValue/Y_EIGHTEEN_DEGREES;
     scaledDisplayValue = min( scaledDisplayValue, 10 );
     
     Serial.println(scaledDisplayValue);

     return 0;
}


void loop()
{
    //displayDigit(scaledDisplayValue);
    //displayDigit(scaledDisplayValue);
    //displayDigit(scaledDisplayValue);

    power = analogRead(X_PIN);
    Serial.print("Power:  ");
    Serial.print(power);
    Serial.print("  ");
  
    computeRollingAverage(tick, power);
    if (tick == NUM_SAMPLES_FOR_AVERAGE-1)
        tick = 0;
    else
        tick += 1;
      
    scaleRollingAverage();
    writePixels(power);
    writePixels(power);
    writePixels(power);
}

