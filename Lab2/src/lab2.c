#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

int flag0=0, flag1=0, flag2=0, flag3 =0;
int intpFlancNeg0=0, intpFlancNeg1=0, intpFlancNeg2=0, inicio=0, final=0; 
int ronda = 4;

int cont=0;
int count=0;
int random=6;

int pulsacion=0;
int end_program=0;

int counter=0;
int flag_delay=0;
int i = 122;

int winRound=0;

int arrayG[10] = {1, 1, 0, 1};
int arrayI[10] = {};

void parpadeo_inicial(){
  int i;
  for (i = 0; i < 2; ++i){
    PORTB = 0x00; _delay_ms(1000);
    PORTB = 0b00001000; _delay_ms(1000);
    PORTB = 0x00; _delay_ms(1000);
    PORTB = 0b00000100; _delay_ms(1000);
    PORTB = 0x00; _delay_ms(1000);
    PORTB = 0b00000010; _delay_ms(1000);
    PORTB = 0x00; _delay_ms(1000);
    PORTB = 0b00000001; _delay_ms(1000);
    PORTB = 0x00; _delay_ms(1000);
    PORTB = 0b00000001; _delay_ms(1000);
    PORTB = 0x00; _delay_ms(1000);
    PORTB = 0b00000010; _delay_ms(1000);
    PORTB = 0x00; _delay_ms(1000);
    PORTB = 0b00000100; _delay_ms(1000);
    PORTB = 0x00; _delay_ms(1000);
    PORTB = 0b00001000; _delay_ms(1000);
  }
}
void parpadeo_final(){
  int i;
  for (i = 0; i < 10; ++i){
    PORTB = 0x00; _delay_ms(1000);
    PORTB = 0b00001111; _delay_ms(1000);
  }
}

ISR(PCINT13_vect){
  if (intpFlancNeg1 == 0) 
  {
    intpFlancNeg1 = intpFlancNeg1 + 1;
  }
  if (intpFlancNeg1 == 1)
  {
    flag3 = 1; 
    intpFlancNeg1 = intpFlancNeg1 + 1;
  }
}
ISR(PCINT14_vect){
  if (intpFlancNeg1 == 0) 
  {
    intpFlancNeg1 = intpFlancNeg1 + 1;
  }
  if (intpFlancNeg1 == 1)
  {
    flag2 = 1; 
    intpFlancNeg1 = intpFlancNeg1 + 1;
  }
}
ISR(PCINT15_vect){
  if (intpFlancNeg1 == 0) 
  {
    intpFlancNeg1 = intpFlancNeg1 + 1;
  }
  if (intpFlancNeg1 == 1)
  {
    flag1 = 1; 
    intpFlancNeg1 = intpFlancNeg1 + 1;
  }
}
ISR(PCINT16_vect){
  if (intpFlancNeg1 == 0) 
  {
    intpFlancNeg1 = intpFlancNeg1 + 1;
  }
  if (intpFlancNeg1 == 1)
  {
    flag0 = 1; 
    intpFlancNeg1 = intpFlancNeg1 + 1;
  }
}

void led0(){
    PORTB = 0b00000001; _delay_ms(1000);
    PORTB = 0x00;
}
void led1(){
    PORTB = 0b00000010; _delay_ms(1000);
    PORTB = 0x00;
}
void led2(){
    PORTB = 0b00000100; _delay_ms(1000);
    PORTB = 0x00;
}
void led3(){
    PORTB = 0b00001000; _delay_ms(1000);
    PORTB = 0x00;
}

void encender_leds(int *array_random){ 
  for (int i = 0; i < ronda; i++) {
    int random = array_random[i];
    if (random == 0){
      led0(); 
    }
    if (random == 1){ 
      led1();
    }
    if (random == 2){ 
      led2();
    }
    if (random == 3){ 
      led3();
    }
  }
}
int main(void)
{
  //int inicio = 0;
  //int final = 0;
  DDRB = 0b11111111; //Configuracion del puerto
  GIMSK = 0b11111111;
  PCMSK2 |= (1 << PCINT17);
  //Parpadear
  while (1) {
    parpadeo_inicial();
    if (inicio == 0){
      if ((flag0 == 1) | (flag1 == 1) | (flag2 == 1) | (flag3 == 1)){
        parpadeo_inicial();
        inicio = 1;
        PORTB = 0b00001111; _delay_ms(10000);
      }
    } else{
        if (flag0){
          led0();
        }
        if (flag1){
          led1();
        }
        if (flag2){
          led2();
        }
        if (flag3){
          led3();
        }
        encender_leds(arrayG);

        for (int i = 0; i < 4; i++)
        {
          arrayG[i] = count; 
          count = (count + 1);

          if (count > 3)
          {
            count = 0;
          }
        }
    }

    if (pulsacion == ronda) {
      winRound = 1; 

      for (int i = 0; i < pulsacion; i++) {
        if (arrayG[i]!= arrayI[i]) {
          winRound = 0;
          final = 1;
          end_program = 1;
        }
      }

while (final == 1)
      {
        if (end_program == 1)
        {
          end_program = 0;
          parpadeo_final();
        }
      }
    
  }
parpadeo_final();
}
}
