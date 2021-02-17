#include <Joystick.h>

Joystick_ Joystick;

// put the max and min values from the analogRead in these arrays
// these are translated to a range of 0 - 1023
int axisLimits0[] = {0, 1023};
int axisLimits1[] = {345, 695};
int axisLimits2[] = {327, 678};
int axisLimits3[] = {342, 692};
int axisLimits4[] = {0, 1023};
int axisLimits5[] = {0, 1023};

// turn axes on or off by setting these variables
bool a0Used = true;
bool a1Used = false;
bool a2Used = false;
bool a3Used = false;
bool a4Used = false;
bool a5Used = false;

// setting mode prints the pin value and translated value to the serial monitor
// int setting = -1; // no printing to the serial monitor
// int setting = 2; // values 0 - 5, print the pin values to the serial monitor
int setting = -1;
int convVals[13] = {1023, 937, 852, 767, 682, 596, 511, 426, 341, 255, 170, 85, 0};
int rawVals[13] = {1023, 1018, 1004, 980, 888, 712, 534, 360, 181, 49, 24, 10, 0};

void setup() {
  if(a0Used) pinMode(A0, INPUT);
  if(a1Used) pinMode(A1, INPUT);
  if(a2Used) pinMode(A2, INPUT);
  if(a3Used) pinMode(A3, INPUT);
  if(a4Used) pinMode(A4, INPUT);
  if(a5Used) pinMode(A5, INPUT);
  Joystick.begin();
  if(setting >= 0) Serial.begin(9600);

  // Conversion table for linear pot
//  int convVals[] = 
//  int rawVals[] = 
  
}

void loop() {
  int value = 0;
  int pos = 0;

  if(a0Used){
    value = analogRead(A0);
    int converted_val = getConvertedValue(value);
    pos = translateValue(converted_val, axisLimits0[0], axisLimits0[1]);
    Joystick.setThrottle(pos);
    if(setting == 0) settingPrint(converted_val, pos);
  }
  
  if(a1Used){
    value = analogRead(A1);
    pos = translateValue(value, axisLimits1[0], axisLimits1[1]);
    Joystick.setRxAxis(pos);
    if(setting == 1) settingPrint(value, pos);
  }
  
  if(a2Used){
    value = analogRead(A2);
    pos = translateValue(value, axisLimits2[0], axisLimits2[1]);
    Joystick.setRyAxis(pos);
    if(setting == 2) settingPrint(value, pos);
  }
  
  if(a3Used){
    value = analogRead(A3);
    pos = translateValue(value, axisLimits3[0], axisLimits3[1]);
    Joystick.setRzAxis(pos);
    if(setting == 3) settingPrint(value, pos);
  }
  
  if(a4Used){
    value = analogRead(A4);
    pos = translateValue(value, axisLimits4[0], axisLimits4[1]);
    Joystick.setXAxis(pos);
    if(setting == 4) settingPrint(value, pos);
  }
  
  if(a5Used){
    value = analogRead(A5);
    pos = translateValue(value, axisLimits5[0], axisLimits5[1]);
    Joystick.setYAxis(pos);
    if(setting == 5) settingPrint(value, pos);
  }

  delay(5);
}

int getConvertedValue(int val) {
  int distance = 0;
  float linearConvFactor = 17.05;
  int low;
  int high;
  int low_idx;
  int high_idx;

  // Find known val range
  for (int i = 0; i < sizeof(rawVals); i++){
    if (rawVals[i] >= val){
      high = rawVals[i];
      high_idx = i;
    }
    if (rawVals[i] <= val){
      low = rawVals[i];
      low_idx = i;
      break;
    }
  }

  // Get position in range
  float position;
  if(high == low){
    position = 1;
  } else{
    int range_diff = high - low;
    position = (float(val) - float(low)) / float(range_diff);
  }

  // Convert to linear value
  int total_diff = convVals[high_idx] - convVals[low_idx];
  int lin_diff = round(float(total_diff) * position);
  int conv_pos = convVals[low_idx] + lin_diff;
  return conv_pos;
}

int translateValue(int v, int f1, int f2){
  // translates values to a 0 - 1023 range
  int result = 0;
  int start = 0;
  float range = 0;
  
  if(f1 < f2){
    start = f1;
    range = f2 - f1;
  }
  else{
    start = f2;
    range = f1 - f2;
  }
  
  result = (v - start) * (1023 / range);

  if(result < 0) result = 0;
  if(result > 1023) result = 1023;
  
  return result;
}

void settingPrint(int value, int pos){
  Serial.print(value); 
  Serial.print(" "); 
  Serial.println(pos);
}
