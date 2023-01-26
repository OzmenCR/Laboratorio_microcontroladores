#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  DDRB = 0xFF; //Configuracion del puerto

  //Parpadear
  while (1) {
    PORTB = 0b11110000; _delay_ms(2500);
    PORTB = 0b11111111; _delay_ms(2500);
  }

  // switch (state)
  // {
  // case off:
  //   /* code */
  //   break;

  // case start:
  //   /*code*/
  //   break;

  // case sequence:
  //   int counter = 4;
  //   while (counter < 15)
  //   {
  //     if (sequence =! user_sequence)
  //     {
        
  //     }
  //     /*code*/
  //     counter++;
  //   }

  // default:
  //   break;
  // }
}