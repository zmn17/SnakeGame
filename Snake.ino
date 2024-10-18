#include <LiquidCrystal_PCF8574.h>
#include "Characters.h"

// Pins definitions
#define VRx A0
#define VRy A1
#define SW 13

int head_pos = 0;
// int body_pos = 1;
int row = 0;

int body_id = 0;
// All the head custom characters ID
int r_head_id = 1;
int l_head_id = 2;
int d_head_id = 3;
int u_head_id = 4;

// this is the default head characters
int head_id = 1;

int x_dir = 1;  // Default direction for X-axis (moving right)
int y_dir = 1;  // Default direction for Y-axis (moving up)

// flag: to track the movement of the character
// default: x-axis
bool default_movement_x = true;

// LCD library
LiquidCrystal_PCF8574 lcd(0x27);

void setup() {
  Serial.begin(115200);
  // Joystick and switch setup
  pinMode(SW, INPUT_PULLUP);

  // I2C -> Display
  Wire.begin();
  lcd.begin(20, 4);

  // Create custom characters
  lcd.createChar(body_id, body);
  lcd.createChar(r_head_id, r_head);
  lcd.createChar(l_head_id, l_head);
  lcd.createChar(u_head_id, u_head);
  lcd.createChar(d_head_id, d_head);

  // Set initial LCD cursor
  lcd.setCursor(0, 0);
  lcd.setBacklight(255);
  lcd.home();

  Serial.println(": LCD initialized.");
}

void loop() {
  int switch_value = digitalRead(SW);
  int xValue = analogRead(VRx); 
  int yValue = analogRead(VRy);

  // Add dead zone for joystick (to avoid jitter)
  int deadZone = 30;

  // Joystick Y-axis (Vertical movement)
  if (yValue > 530 + deadZone) {
    // if use moves down, the default direction is set to false
    // so, the characters moves on y-axis
    default_movement_x = false;
    y_dir = 1;  // Move down
    // chaning the head
    head_id = d_head_id;

  } else if (yValue < 530 - deadZone) {
    // if use moves down, the default direction is set to false
    // so, the characters moves on y-axis
    default_movement_x = false;
    y_dir = -1;  // Move up
    // chaning the head 
    head_id = u_head_id;
  } 

  // // Joystick X-axis (Horizontal movement)
  if (xValue > 530 + deadZone) {
    // if user moves to the right, the default direction is set to true
    // so, the character moves on x-axis
    default_movement_x = true;
    x_dir = 1;  // Move right
    head_id = r_head_id;
  } else if (xValue < 530 - deadZone) {
    // if user moves to the right, the default direction is set to true
    // so, the character moves on x-axis
    default_movement_x = true;
    x_dir = -1;  // Move left
    head_id = l_head_id;
  }

  // Reset positions when the switch is pressed
  if (switch_value == LOW) {
    head_pos = 0;
    row = 0;
    x_dir = 1;  // Default movement to the right
    y_dir = 0;  // No vertical movement
  }

  // Update position based on current direction
  // default direction -> to the right
  if(default_movement_x){
    head_pos += x_dir;
  }else {
    row += y_dir;
  }
  // head_pos += x_dir;
  // row += y_dir;

  // Wrap around when reaching the edges of the screen
  if (head_pos >= 19) {  // Right edge
    head_pos = 0;
  } else if (head_pos < 0) {  // Left edge
    head_pos = 19;
  }
  
  if (row >= 4) {  // Bottom row
    row = 0;
  } else if (row < 0) {  // Top row
    row = 3;
  }

  // Display head and body at the new positions
  lcd.clear();
// For vertical movement (up/down)
if (!default_movement_x) {
    lcd.setCursor(head_pos, row);      // Set head position
    lcd.write(head_id);                // Write head
    if(y_dir == 1){
      lcd.setCursor(head_pos, row - 1);  // Set body position directly below head
    }else {
      lcd.setCursor(head_pos, row + 1);
    }
    lcd.write(body_id);                // Write body
}
// For horizontal movement (left/right)
else {
    lcd.setCursor(head_pos, row);      // Set body position first
    lcd.write(body_id);                // Write body
    lcd.setCursor(head_pos + x_dir, row);  // Set head position based on direction
    lcd.write(head_id);                // Write head
}
  delay(1000);  // Control the speed of movement
}
