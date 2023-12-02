#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__
#include <stdio.h>
#include <avr/io.h>
#define toggleBit(valor, bit) (valor ^= (1 << bit))
#define testBit(valor, bit) (valor & (1 << bit))
#define setBit(valor, bit) (valor |= (1 << bit))
#define clearBit(valor, bit) (valor &= ~(1 << bit))

typedef struct comida
{   
    uint8_t x;
    uint8_t y;
    
}comida;

typedef struct cobra
{
    uint8_t x;
    uint8_t y;
    
    struct cobra * anterior;
    struct cobra * proximo;
}cobra;

void gerar_comida(comida* apple);

void atualizar_pontos(int* pontos);

void print_cobra(cobra* head);

int checar_colisao(comida* apple, cobra* head);

void menu_inicial();

void iniciar_jogo(cobra *head, cobra* tail,comida *apple);

void mover_cobra(int up, int down, int right, int left, cobra **head, cobra **tail, int* atualiza);

void print_comida(comida* apple);

void fim_de_jogo(int pontos);

#endif