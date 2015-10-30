/*
 * Arduino USB HID adapter for the Palm Portable Keyboard
 *
 * Copyright (C) 2014 cy384
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 */

#include <SoftwareSerial.h>

// set to 3 for III hardware, or 5 for V hardware
#define PPK_VERSION 3

// set to 1 to enable debug mode, which notes to the arduino console at 9600
#define PPK_DEBUG 0

#if PPK_VERSION == 3
#define VCC_PIN       2
#define RX_PIN        8
#define RTS_PIN       4
#define DCD_PIN       5
#define GND_PIN       6
#endif

#if PPK_VERSION == 5
#define VCC_PIN       7
#define RX_PIN        8
#define RTS_PIN       5
#define DCD_PIN       4
#define GND_PIN       2
#endif

#define PULLDOWN_PIN  15
// set this to any unused pin
#define TX_PIN        11

#if (PPK_VERSION != 3) && (PPK_VERSION != 5)
#error
#error
#error    you did not set your ppk version!
#error    read the instructions or read the code!
#error
#error
#endif

// convenience masks
#define UPDOWN_MASK 0b10000000
#define X_MASK      0b00000111
#define Y_MASK      0b01111000
#define MAP_MASK    0b01111111

// wait this many milliseconds before making sure keyboard is still awake
#define TIMEOUT 500000

SoftwareSerial keyboard_serial(RX_PIN, TX_PIN, true); // RX, TX, inverted

char key_map[128] = { 0 };
char fn_key_map[128] = { 0 };

char last_byte = 0;
char key_byte = 0;

int fn_key_down = 0;

unsigned long last_comm = 0;

void config_keymap()
{
  // y0 row
  key_map[0b00000000] = '1';
  key_map[0b00000001] = '2';
  key_map[0b00000010] = '3';
  key_map[0b00000011] = 'z';
  key_map[0b00000100] = '4';
  key_map[0b00000101] = '5';
  key_map[0b00000110] = '6';
  key_map[0b00000111] = '7';

  // y1 row
  key_map[0b00001000] = KEY_LEFT_GUI; // "CMMD" or "Cmd"
  key_map[0b00001001] = 'q';
  key_map[0b00001010] = 'w';
  key_map[0b00001011] = 'e';
  key_map[0b00001100] = 'r';
  key_map[0b00001101] = 't';
  key_map[0b00001110] = 'y';
  key_map[0b00001111] = '`';

  // y2 row
  key_map[0b00010000] = 'x';
  key_map[0b00010001] = 'a';
  key_map[0b00010010] = 's';
  key_map[0b00010011] = 'd';
  key_map[0b00010100] = 'f';
  key_map[0b00010101] = 'g';
  key_map[0b00010110] = 'h';
  key_map[0b00010111] = ' '; // "Space 1"

  // y3 row
  key_map[0b00011000] = KEY_CAPS_LOCK;
  key_map[0b00011001] = KEY_TAB;
  key_map[0b00011010] = KEY_LEFT_CTRL;
  key_map[0b00011011] = 0;
  key_map[0b00011100] = 0;
  key_map[0b00011101] = 0;
  key_map[0b00011110] = 0;
  key_map[0b00011111] = 0;

  // y4 row
  key_map[0b00100000] = 0;
  key_map[0b00100001] = 0;
  key_map[0b00100010] = 0; // Fn key
  key_map[0b00100011] = KEY_LEFT_ALT;
  key_map[0b00100100] = 0;
  key_map[0b00100101] = 0;
  key_map[0b00100110] = 0;
  key_map[0b00100111] = 0;

  // y5 row
  key_map[0b00101000] = 0;
  key_map[0b00101001] = 0;
  key_map[0b00101010] = 0;
  key_map[0b00101011] = 0;
  key_map[0b00101100] = 'c';
  key_map[0b00101101] = 'v';
  key_map[0b00101110] = 'b';
  key_map[0b00101111] = 'n';

  // y6 row
  key_map[0b00110000] = '-';
  key_map[0b00110001] = '=';
  key_map[0b00110010] = KEY_BACKSPACE;
  key_map[0b00110011] = 0; // "Special Function One"
  key_map[0b00110100] = '8';
  key_map[0b00110101] = '9';
  key_map[0b00110110] = '0';
  key_map[0b00110111] = ' '; // "Space 2"

  // y7 row
  key_map[0b00111000] = '[';
  key_map[0b00111001] = ']';
  key_map[0b00111010] = '\\';
  key_map[0b00111011] = 0; // "Special Function Two"
  key_map[0b00111100] = 'u';
  key_map[0b00111101] = 'i';
  key_map[0b00111110] = 'o';
  key_map[0b00111111] = 'p';

  // y8 row
  key_map[0b01000000] = '\'';
  key_map[0b01000001] = KEY_RETURN;
  key_map[0b01000010] = 0; // "Special Function Three"
  key_map[0b01000011] = 0;
  key_map[0b01000100] = 'j';
  key_map[0b01000101] = 'k';
  key_map[0b01000110] = 'l';
  key_map[0b01000111] = ';';

  // y9 row
  key_map[0b01001000] = '/';
  key_map[0b01001001] = KEY_UP_ARROW;
  key_map[0b01001010] = 0; // "Special Function Four"
  key_map[0b01001011] = 0;
  key_map[0b01001100] = 'm';
  key_map[0b01001101] = ',';
  key_map[0b01001110] = '.';
  key_map[0b01001111] = 0; // "DONE" or "Done"

  // y10 row
  key_map[0b01010000] = KEY_DELETE;
  key_map[0b01010001] = KEY_LEFT_ARROW;
  key_map[0b01010010] = KEY_DOWN_ARROW;
  key_map[0b01010011] = KEY_RIGHT_ARROW;
  key_map[0b01010100] = 0;
  key_map[0b01010101] = 0;
  key_map[0b01010110] = 0;
  key_map[0b01010111] = 0;

  // y11 row
  key_map[0b01011000] = KEY_LEFT_SHIFT;
  key_map[0b01011001] = KEY_RIGHT_SHIFT;
  key_map[0b01011010] = 0;
  key_map[0b01011011] = 0;
  key_map[0b01011100] = 0;
  key_map[0b01011101] = 0;
  key_map[0b01011110] = 0;
  key_map[0b01011111] = 0;
}

void config_fnkeymap()
{
  fn_key_map[0b01010001] = KEY_HOME; // left arrow
  fn_key_map[0b01010010] = KEY_PAGE_DOWN; // down arrow
  fn_key_map[0b01010011] = KEY_END; // right arrow
  fn_key_map[0b01001001] = KEY_PAGE_UP; // up arrow
  fn_key_map[0b00011001] = KEY_ESC; // tab key
}

void boot_keyboard()
{
  if (PPK_DEBUG)
  {
    // delay for a bit to allow for opening serial monitor etc.
    for (int i = 0; i < 15; delay(1000 + i++)) Serial.print(".");

    Serial.println("beginning keyboard boot sequence");
  }

  pinMode(VCC_PIN, OUTPUT);
  pinMode(GND_PIN, OUTPUT);
  pinMode(PULLDOWN_PIN, OUTPUT);

  pinMode(RX_PIN, INPUT);
  pinMode(DCD_PIN, INPUT);
  pinMode(RTS_PIN, INPUT);

  digitalWrite(VCC_PIN, LOW);
  digitalWrite(GND_PIN, LOW);
  digitalWrite(PULLDOWN_PIN, LOW);
  digitalWrite(VCC_PIN, HIGH);

  keyboard_serial.begin(9600);
  keyboard_serial.listen();

  if (PPK_DEBUG) Serial.print("waiting for keyboard response...");

  while(digitalRead(DCD_PIN) != HIGH) {;};

  if (PPK_DEBUG) Serial.println(" done");


  if (PPK_DEBUG) Serial.print("finishing handshake...");

  if (digitalRead(RTS_PIN) == LOW)
  {
    delay(10);
    pinMode(RTS_PIN, OUTPUT);
    digitalWrite(RTS_PIN, HIGH);
  }
  else
  {
    pinMode(RTS_PIN, OUTPUT);
    digitalWrite(RTS_PIN, HIGH);
    digitalWrite(RTS_PIN, LOW);
    delay(10);
    digitalWrite(RTS_PIN, HIGH);
  }

  delay(5);

  Serial.println(" done");

  if (PPK_DEBUG) Serial.print("waiting for keyboard serial ID...");

  while (keyboard_serial.available() < 2) {;};

  if (PPK_DEBUG) Serial.println(" done");

  int byte1 = keyboard_serial.read();
  int byte2 = keyboard_serial.read();

  if (!((byte1 == 0xFA) && (byte2 == 0xFD)))
  {
    if (PPK_DEBUG) Serial.println("got wrong bytes? giving up here");

    while (1) {;};
  }

  last_comm = millis();
}

void setup()  
{
  if (PPK_DEBUG)
  {
    Serial.begin(9600);
    Serial.print("compiled in debug mode with PPK_VERSION ");
    Serial.println(PPK_VERSION);
  }

  config_keymap();
  config_fnkeymap();

  boot_keyboard();

  Keyboard.begin();

  if (PPK_DEBUG) Serial.println("setup completed");
}

void loop()
{
  if (keyboard_serial.available())
  {
    for (int i = keyboard_serial.available(); i > 0; i--)
    {
      key_byte = keyboard_serial.read();

      int key_up = key_byte & UPDOWN_MASK;
      int key_x = 0 + (key_byte & X_MASK);
      int key_y = 0 + ((key_byte & Y_MASK) >> 3);

      char key_code = 0;

      if (fn_key_down && fn_key_map[key_byte & MAP_MASK])
      {
        key_code += fn_key_map[key_byte & MAP_MASK];
      }
      else
      {
        key_code += key_map[key_byte & MAP_MASK];
      }

      // keyboard duplicates the final key-up byte
      if (key_byte == last_byte)
      {
        Keyboard.releaseAll();
      }
      else
      {
        if (key_code != 0)
        {
          if (key_up)
          {
            if (PPK_DEBUG)
            {
              Serial.print("key released: ");
              Serial.println(key_code);
            }

            Keyboard.release(key_code);
          }
          else
          {
            if (PPK_DEBUG)
            {
              Serial.print("key pressed: ");
              Serial.println(key_code);
            }

            Keyboard.press(key_code);
          }
        }
        else
        {
          // special case the Fn key
          if ((key_byte & MAP_MASK) == 34)
          {
            if (PPK_DEBUG)
            {
              Serial.print("Fn key down: ");
              Serial.println(!key_up);
            }

            fn_key_down = !key_up;
          }
        }
      }

      last_byte = key_byte;
      last_comm = millis();
    }
  }
  else
  {
    // reboot if no recent comms, otherwise keyboard falls asleep
    if ((millis() - last_comm) > TIMEOUT)
    {
      if (PPK_DEBUG) Serial.println("rebooting keyboard for timeout");

      digitalWrite(VCC_PIN, LOW);
      boot_keyboard();
    }
  }
}


