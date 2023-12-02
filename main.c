#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "nokia5110.h"
#include "game_logic.h"

//Timer 2 interrompe 61/s
//61/15 ~= 0.25 s
#define COUNTER_TIME 15

uint32_t seed = 0; 
uint8_t contador = 0;
int pontos = 0;

struct cobra *head = NULL;
struct cobra *tail = NULL;
struct comida apple;

volatile int up = 0, down = 0, left = 0;
volatile int right = 1;

int atualiza = 0;
int running = 0;
int pause = 0;

void define(){
    DDRD &= ~(1 << PD0); // UP
    PORTD |= (1 << PD0);
    DDRD &= ~(1 << PD1); // RIGHT
    PORTD |= (1 << PD1);
    DDRD &= ~(1 << PD2); // LEFT
    PORTD |= (1 << PD2);
    DDRD &= ~(1 << PD3); // DOWN
    PORTD |= (1 << PD3);
    DDRB &= ~(1 << PB0); // Interromper
    PORTB |= (1 << PB0);
}

ISR(TIMER2_OVF_vect){
    if (contador == COUNTER_TIME)
    {
        if (running && !pause)
        {
            mover_cobra(up, down, right, left, &head, &tail, &atualiza);
        }
        contador = 0;
    }
    else
    {
        contador++;
    }
    seed++;
}

void Iniciar_Timer()
{
    cli();
    TCCR2A = 0x00;
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
    TIMSK2 = (1 << TOIE2);
    sei();
}

int main(void)
{
    Iniciar_Timer();
    define();
    nokia_lcd_init();
    menu_inicial();

    srand(seed);

    if (head == NULL || tail == NULL)
    {
        head = calloc(1, sizeof(cobra));
        tail = calloc(1, sizeof(cobra));
    }

    iniciar_jogo(head, tail, &apple);

    print_cobra(head);
    
    running = 1;
    while (1)
    {
        if (running)
        {   
            if (!testBit(PINB, PB0))
            {
                pause = !pause;
                while(!testBit(PINB,PB0)){}
            }
            else if (!testBit(PIND,PD0) && !down)
            {
                left = right = 0;
                up = 1;
            }
            else if (!testBit(PIND,PD1) && !left)
            {
                down = up = 0;
                right = 1;
            }
            else if (!testBit(PIND,PD2) && !right)
            {
                down = up = 0;
                left = 1;
            }
            else if (!testBit(PIND,PD3) && !up)
            {
                left = right = 0;
                down = 1;
            }

            switch (checar_colisao(&apple, head))
            {
            case 0:
                print_cobra(head);
                print_comida(&apple);
                nokia_lcd_render();
                break;
            case 1:
                running = 0;
                fim_de_jogo(pontos);
                break;
            case 2:
                pontos++;
                gerar_comida(&apple);
                atualiza = 1;
                break;
            case 3:
                running = 0;
                fim_de_jogo(pontos);
                break;
            }
        }
    }
}
