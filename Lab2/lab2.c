#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

int flagAz=0, flagR=0, flagV=0, flagAm =0;
int intpFlancNeg0=0, intpFlancNeg1=0, intpFlancNeg2=0;

int inicio=1, final=0;

int cont=0;
int count=0;
int random=6;
int ronda = 4;

int pulsacion=0;
int end_program=0;

int counter=0;
int flag_delay=0;
int i = 122;

int winRound=0;


int arrayG[10] = {1, 1, 0, 1};
int arrayI[10] = {};

ISR(TIMER0_OVF_vect){
  if(counter == i){
    counter=0;
    flag_delay=1;
  }else{
    counter = counter+1;
    //flag_delay=0;
  }
}

ISR (INT0_vect){ 
flagV = 1; 
}


ISR(INT1_vect){
flagAm= 1; 
}

ISR(PCINT0_vect){
  if (intpFlancNeg1 == 0) 
  {
    intpFlancNeg1 = intpFlancNeg1 + 1;
  }
  if (intpFlancNeg1 == 1)
  {
    flagR = 1; 
    intpFlancNeg1 = intpFlancNeg1 + 1;
  }
}
ISR(PCINT1_vect){
  if (intpFlancNeg0 == 0) 
  {
    intpFlancNeg0 = intpFlancNeg0 + 1;
  }
  if (intpFlancNeg0 == 1)
  {
    flagV = 1; 
    intpFlancNeg0 = intpFlancNeg0 + 1;
  }
}

void parpadeo_inicial(){ 
  if (flag_delay==1){
    if(PORTD==0b00000000){
      PORTD=0b00110011;
    }else{
      PORTD=0b00000000;
    }
    flag_delay=0;
  }

  if (flag_delay==1){
    if(PORTD==0b00000000){
      PORTD=0b00110011;
    }else{
      PORTD=0b00000000;
    }
    flag_delay=0;
  }
  if (flag_delay==1){
    if(PORTD==0b00000000){
      PORTD=0b00110011;
    }else{
      PORTD=0b00000000;
    }
    flag_delay=0;
  }
} 

void parpadeo_final(){ 

  if (flag_delay==1){
    if(PORTD==0b00000000){
      PORTD=0b00110011;
    }else{
      PORTD=0b00000000;
    }
    flag_delay=0;
  }

  if (flag_delay==1){
    if(PORTD==0b00000000){
      PORTD=0b00110011;
    }else{
      PORTD=0b00000000;
    }
    flag_delay=0;
  }
  if (flag_delay==1){
    if(PORTD==0b00000000){
      PORTD=0b00110011;
    }else{
      PORTD=0b00000000;
    }
    flag_delay=0;
  }

 
} 

void ledAz(){
  if (flag_delay==1){
    if(PORTD==0b00000000){
      PORTD=0b00000001;
    }else{
      PORTD=0b00000000;
    }
    flag_delay=0;
  }
}

void ledV(){
  if (flag_delay==1){
    if(PORTD==0b00000000){
      PORTD=0b00010000;
    }else{
      PORTD=0b00000000;
    }
    flag_delay=0;
  }
}

void ledR(){
  if (flag_delay==1){
    if(PORTD==0b00000000){
      PORTD=0b00000010;
    }else{
      PORTD=0b00000000;
    }
    flag_delay=0;
  }
}

void ledAm(){
  if (flag_delay==1){
    if(PORTD==0b00000000){
      PORTD=0b00100000;
    }else{
      PORTD=0b00000000;
    }
    flag_delay=0;
  }
}

void encender_leds(int *array_random){ 
  for (int i = 0; i < ronda; i++) {
    int random = array_random[i];
    if (random == 0){
      ledAz(); 
    }
    if (random == 1){ 
      ledR();
    }
    if (random == 2){ 
      ledV();
    }
    if (random == 3){ 
      ledAm();
    }
  }
}



int main(void)
{
  GIMSK = 0b11101000;
  DDRD=0b00110011;
  DDRB = 0x00; 
  DDRA = 0b00000000;
  PCMSK=0b00000111;
  //PORTB=0x00;
  sei();
  
  while (1) {
    parpadeo_inicial();
      inicio = 0;
      encender_leds(arrayG);
    while (inicio == 1){ 
        if (count < 4){
          count = count + 1;
        }else{
          count = 0;
        }
      if ((flagAz == 1) | (flagR == 1) | (flagV == 1) | (flagAm == 1)){
        flagAz = 0;
        flagR = 0;
        flagV = 0;
        flagAm = 0;
        inicio = 0;
      }
      if (inicio == 0)
      {
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
    }
    if (inicio == 1){
      parpadeo_inicial();
      inicio = 0;
      encender_leds(arrayG); 
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

      if (winRound == 1) 
      {
        
        arrayG[ronda] = cont;

        inicio = 1; 
        ronda = ronda +1; 
        pulsacion = 0; 
        winRound = 0; 
        
        if (flag_delay==1){
    if(PORTD==0b00000000){
      PORTD=0b00110011;
    }else{
      PORTD=0b00000000;
    }
    flag_delay=0;
  }
        
      }
    }
    if (cont < 3) 
    {
      cont = cont + 1;
    }
    else
    {
      cont = 0;
    }

   if (flagAz == 1){
      flagAz = 0;
      ledAz(); 
      arrayI[pulsacion] = 0;
      pulsacion = pulsacion + 1;
    }

    if (flagR == 1){
      flagR = 0;
      ledR(); 
      arrayI[pulsacion] = 1;
      pulsacion = pulsacion + 1;
    }

    if (flagV == 1){
      flagV = 0; 
      ledV(); 
      arrayI[pulsacion] = 2;
      pulsacion = pulsacion + 1;
      intpFlancNeg0 = 0;
    }

    if (flagAm == 1){
      flagAm = 0;
      ledAm(); 
      arrayI[pulsacion] = 3;
      pulsacion = pulsacion + 1;
      intpFlancNeg1 = 0;
    }
}
}
 