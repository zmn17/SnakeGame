#include <LiquidCrystal_PCF8574.h>
#include "Characters.h"
#include <stdlib.h>

// Pins definitions
#define VRx A0
#define VRy A1
#define SW 13

int score = 2;

// snake character - positioning, head and body
int head_pos = 0;
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

// array of snake parts
uint8_t* snake_parts[5] = {
  body,
  r_head,
  l_head,
  d_head,
  u_head
};

// array of foods
uint8_t* foods[] = {
  apple,
  banana,
  bread
};

// food coordinates
int rand_col;
int rand_row;

// timing for chaning foods
unsigned long previousMillis = 0;
long food_interval = 6000; // 6-seconds
int current_food_id = 0;

// tracking the count
// of joystick left and right, up and down state

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
  create_snake();

  // LCD display init
  lcd_initialization();

  Serial.println(": LCD initialized.");
}


void loop() {
  int switch_value = digitalRead(SW);
  int xValue = analogRead(VRx); 
  int yValue = analogRead(VRy);

  joystock_controller(xValue, yValue);

  // Update position based on current direction
  // default direction -> to the right
  if(default_movement_x){
    head_pos += x_dir;
  }else {
    row += y_dir;
  }

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
  set_foods_rand();
  tracking_score();

  // reset the game
  reset(switch_value);

  Serial.print("Score: ");
  Serial.println(score);

  delay(1000);  // Control the speed of movement
}

void joystock_controller(int &xValue, int &yValue){
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
}

void reset(int &switch_value){
   // Reset positions when the switch is pressed
  if (switch_value == LOW) {
    head_pos = 0;
    row = 0;
    x_dir = 1;  // Default movement to the right
    y_dir = 0;  // No vertical movement
    score = 0;
  }
}

void lcd_initialization(){
  lcd.setCursor(0, 0);
  lcd.setBacklight(255);
  lcd.home();
}

// Create custom characters
void create_snake(){
  for(int i = 0; i < 5; i++){
    lcd.createChar(i, snake_parts[i]);
  }
}

// create and set the foods on the grid
void set_foods_rand(){
  unsigned long curr_millis = millis();


  // check if the food update interval has passed
  if(curr_millis - previousMillis >= food_interval){
      previousMillis = curr_millis; // reset the timer

      // generate random food id and character
      int rand_id = rand() % 3 + 5;
      int rand_char = rand() & 3 + 1;

      // apple_id = 5;
      // banana_id = 6;
      // bread_id = 7;

      // 1. create characters : foods
      for(int i = 0; i < 3; i++){
        lcd.createChar(rand_id, foods[rand_char]);
      }
      
      // generate random positions for food on the grid
      rand_col = rand() % 19 + 1;
      rand_row = rand() % 4;

      // set cursor and write the food character
      lcd.setCursor(rand_col, rand_row);
      lcd.write(rand_id);

      // track the current food id
      current_food_id = rand_id;
    }

    // redraw the current food
    lcd.setCursor(rand_col, rand_row);
    lcd.write(current_food_id);

    food_interval = 6000;
}

void tracking_score(){
  if(head_pos == rand_col && row == rand_row){
      score++;
      food_interval = 100;
  }

  // Score
  lcd.setCursor(19, 0);
  lcd.print(score);
}
