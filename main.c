/* USB Keyboard Firmware code for generic Teensy Keyboards
 * Copyright (c) 2012 Fredrik Atmer, Bathroom Epiphanies Inc
 * http://www.bathroomepiphanies.com
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include "lib/usb_keyboard_debug.h"
#include "lib/print.h"
#include "lib/keycode.h"
#include "lib/avr_extra.h"
#include  KEYBOARD_MODEL_FILE

struct {uint8_t is_modifier; uint8_t value;}  layout[] = KEYBOARD_LAYOUT;
struct {bool is_pressed;     uint8_t bounce;} key[NUMBER_OF_KEYS];
uint8_t queue[7] = {255, 255, 255, 255, 255, 255, 255};
uint8_t mod_keys = 0;

void scan_keys(void);
void send(void);
void key_press(uint8_t k);
void key_release(uint8_t k);
void update_leds(void);
void init(void);
void poll_timer_enable(void);
void poll_timer_disable(void);

int main(void) {
  init();
  poll_timer_enable();
  for(;;);
}


ISR(TIMER0_COMPA_vect) {
  poll_timer_disable();
  
  LOAD_KEY_REGISTERS;
  for(int j=0, k=0; j<9; j++) {
    for(int i=0; i<8; i++, k++) {
      key[k].bounce |= READ_KEY;  // Read keys
      SHIFT_KEY_DATA;
    }
  }

  for(uint8_t k = 0; k<9*8; k++) {
    if(key[k].bounce == 0b01111111 && !key[k].is_pressed)
      key_press(k);
    if(key[k].bounce == 0b10000000 &&  key[k].is_pressed)
      key_release(k);
    
    key[k].bounce <<= 1;
  }

//  for(uint8_t k=0; k<72; k++)
//    if(key[k].is_pressed)
//      print("1");
//    else
//      print("0");
//  print("\n");

  update_leds();
  poll_timer_enable();
}


void send(void) {
  uint8_t i;
  for(i = 0; i < 6; i++)
    keyboard_keys[i] = queue[i] != 255 ? layout[queue[i]].value : 0;
  keyboard_modifier_keys = mod_keys;
  usb_keyboard_send();
}
void key_press(uint8_t k) {
  uint8_t i;
  key[k].is_pressed = true;
  if(layout[k].is_modifier)
    mod_keys |= layout[k].value;
  else {
    for(i = 5; i > 0; i--)
      queue[i] = queue[i-1];
    queue[0] = k;
  }
  send();
}
void key_release(uint8_t k) {
  uint8_t i;
  key[k].is_pressed = false;
  if(layout[k].is_modifier)
    mod_keys &= ~layout[k].value;
  else {
    for(i = 0; i < 6; i++)
      if(queue[i]==k)
        break;
    for(i = i; i < 6; i++)
      queue[i] = queue[i+1];
  }
  send();
}


// 2 = scroll lock, 1 = caps lock, 0 = num lock.
void update_leds() {
  /*
    LED data  D0 // Data pin for serial LED data
    Blank     D2 // Disable LED output, active high
    Latch     D3 // Latch LED data to driver
    Clock     D4 // Clock signal
    Load      D5 // Load key data into shift registers, active low
    Key data  D7 // Data pin for serial key values
   */
}


void init(void) {
  usb_init();
  while(!usb_configured());
  CPU_PRESCALE(0);
  MCUCR |= 0x80; MCUCR |= 0x80;
  DDRD  = 0b01111111;
  PORTD = 0b11111111;
  TCCR0A |=      // Timer0 control register A: timer mode
    (1<<WGM01);  // Set CTC, clear timer on compare
  TCCR0B |=      // Timer0 control register B: step frequency
    (1<<CS00) |  // Prescaler 1024, frequency 15.6kHz (Combined with next line)
    (1<<CS02);   // Prescaler 256, frequency 62.5kHz (This line alone)
  OCR0A = 16;    // Set Timer0 comparison to 16 (the number of steps)
  mod_keys = 0;
  for(uint8_t k = 0; k < NUMBER_OF_KEYS; k++)
    key[k].bounce = key[k].is_pressed = false;
  sei();
}
void poll_timer_enable(void) {
  TIMSK0 |=      // Timer interrupt mask register 0
    (1<<OCIE0A); // Enable timer interrupt on compare match with OCR0A
}
void poll_timer_disable(void) {
  TIMSK0 &=       // Timer interrupt mask register 0
    ~(1<<OCIE0A); // Disable timer interrupt on compare match with OCR0A
}
