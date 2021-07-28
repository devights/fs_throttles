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
bool a1Used = true;
bool a2Used = true;
bool a3Used = true;
bool a4Used = true;
bool a5Used = true;

// setting mode prints the pin value and translated value to the serial monitor
int setting = false;
int convVals[13] = {1023, 937, 852, 767, 682, 596, 511, 426, 341, 255, 170, 85, 0};
int rawVals[13] = {1023, 1018, 1004, 980, 888, 712, 534, 360, 181, 49, 24, 10, 0};


// Gear state
bool gear_down = false;
bool gear_up = false;

int flap_state[2] = {0, 0};

void setup() {
  if (a0Used) pinMode(A0, INPUT);
  if (a1Used) pinMode(A1, INPUT);
  if (a2Used) pinMode(A2, INPUT);
  if (a3Used) pinMode(A3, INPUT);
  if (a4Used) pinMode(A4, INPUT);
  if (a5Used) pinMode(A5, INPUT);

  // Init gear lever position
  int gear_value = analogRead(A3);
  if (gear_value >= 200) {
    // Gear lever up
    gear_down = false;
    gear_up = true;
  } else {
    // Gear lever dn
    gear_down = true;
    gear_up = false;
  }

  Joystick.begin();
//  Serial.begin(9600);
//  while (!Serial) {
//    // do nothing
//  };

  // Conversion table for linear pot
  //  int convVals[] =
  //  int rawVals[] =

}

void loop() {
  int value = 0;
  int pos = 0;

  if (a0Used) {
    value = analogRead(A0);
    int converted_val = getConvertedValue(value);
    pos = translateValue(converted_val, axisLimits0[0], axisLimits0[1]);
    Joystick.setThrottle(pos);
    if (setting) settingPrint(0, converted_val);
  }

  if (a1Used) {
    value = analogRead(A1);
    int converted_val = getConvertedValue(value);
    pos = translateValue(converted_val, axisLimits0[0], axisLimits0[1]);
    Joystick.setRxAxis(pos);
    if (setting) settingPrint(1, converted_val);
  }

  if (a2Used) {
    value = analogRead(A2);
    int converted_val = getConvertedValue(value);
    pos = translateValue(converted_val, axisLimits0[0], axisLimits0[1]);
    Joystick.setRyAxis(pos);
    if (setting) settingPrint(2, converted_val);
  }

  if (a3Used) {
    value = analogRead(A3);
    if (value <= 200) {
      // Gear lever up
      if (gear_down) {
        Joystick.pressButton(1);
        Serial.print("Retract");
        gear_down = false;
        gear_up = true;
      } else {
        Joystick.releaseButton(1);
      }
    } else if (value > 200) {
      // Gear lever down
      if (gear_up) {
        Joystick.pressButton(2);
        Serial.print("Extend");
        gear_down = true;
        gear_up = false;
      } else {
        Joystick.releaseButton(2);
      }
    }
  }

  handle_flap();

  delay(5);
}

void handle_flap() {
  bool dn = get_pressed(analogRead(A5));
  bool up = get_pressed(analogRead(A4));
  if (dn != flap_state[0]) {
    Serial.println("dn");
    Joystick.setButton(3, dn);
    Serial.print(dn);
    Serial.println(" 2 pressed");
    flap_state[0] = dn;
  }
  if (up != flap_state[1]) {
    Serial.println("up");
    Joystick.setButton(4, up);
    Serial.print(up);
    Serial.println(" 2 pressed");
    flap_state[1] = up;
  }

}
bool get_pressed(int val) {
  if (val >= 200) {
    return true;
  } else {
    return false;
  }
}
int getConvertedValue(int val) {
  int distance = 0;
  float linearConvFactor = 17.05;
  int low;
  int high;
  int low_idx;
  int high_idx;

  // Find known val range
  for (int i = 0; i < sizeof(rawVals); i++) {
    if (rawVals[i] >= val) {
      high = rawVals[i];
      high_idx = i;
    }
    if (rawVals[i] <= val) {
      low = rawVals[i];
      low_idx = i;
      break;
    }
  }

  // Get position in range
  float position;
  if (high == low) {
    position = 1;
  } else {
    int range_diff = high - low;
    position = (float(val) - float(low)) / float(range_diff);
  }

  // Convert to linear value
  int total_diff = convVals[high_idx] - convVals[low_idx];
  int lin_diff = round(float(total_diff) * position);
  int conv_pos = convVals[low_idx] + lin_diff;
  return conv_pos;
}

int translateValue(int v, int f1, int f2) {
  // translates values to a 0 - 1023 range
  int result = 0;
  int start = 0;
  float range = 0;

  if (f1 < f2) {
    start = f1;
    range = f2 - f1;
  }
  else {
    start = f2;
    range = f1 - f2;
  }

  result = (v - start) * (1023 / range);

  if (result < 0) result = 0;
  if (result > 1023) result = 1023;

  return result;
}

void settingPrint(int value, int pos) {
//  Serial.print(value);
//  Serial.print(" ");
//  Serial.println(pos);
}
