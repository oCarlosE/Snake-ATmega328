// Teste

#include <stdio.h>
#include <stdlib.h>
#include "nokia5110.h"
#include "game_logic.h"

void menu_inicial()
{   
    nokia_lcd_set_cursor(12, 0);
    nokia_lcd_write_string("START GAME", 1);
    nokia_lcd_set_cursor(28, 12);
    nokia_lcd_write_string("PRESS", 1);
    nokia_lcd_set_cursor(12, 24);
    nokia_lcd_write_string("ANY BUTTON", 1);
    nokia_lcd_render();

    while(testBit(PIND, PD0) && testBit(PIND, PD1) && testBit(PIND, PD2) && testBit(PIND, PD3)){
        
    }
    nokia_lcd_clear();
}

void fim_de_jogo(int pontos)
{
    char text[10];
    sprintf(text, "%d", pontos);
    nokia_lcd_clear();
    nokia_lcd_set_cursor(10, 6);
    nokia_lcd_write_string("FIM DE JOGO", 1);
    nokia_lcd_set_cursor(16, 24);
    nokia_lcd_write_string("PONTOS: ", 1);
    nokia_lcd_set_cursor(58, 24);
    nokia_lcd_write_string(text, 1);
    nokia_lcd_render();
}

void atualizar_pontos(int *pontos)
{
    (*pontos)++;
}

void gerar_comida(comida *apple)
{
    apple->x = 1+ rand() % 82;
    apple->y = 1+ rand() % 46;

    nokia_lcd_set_pixel(apple->x, apple->y, 1);
}

void print_cobra(cobra *head)
{   
    nokia_lcd_clear();
    nokia_lcd_drawrect(0,0,83,47);

    struct cobra *aux = head;

    while (aux != NULL)
    {   
        nokia_lcd_set_pixel(aux->x, aux->y, 1);
        aux = aux->proximo;
    }
}

int checar_colisao(comida *apple, cobra *head)
{
    if (head->x == 0 || head->y == 0 || head->x == 83 || head->y == 47)
    {
        return 1; // colisão com a parede
    }
    if (head->x == apple->x && head->y == apple->y)
    {
        return 2; // colisão com a comida
    }
    
    cobra *temp = head->proximo;
    while (temp != NULL)
    {
        if (head->x == temp->x && head->y == temp->y)
        {
            return 3; // colisão com o corpo
        }
        temp = temp->proximo;
    }
    return 0; // sem colisão
}

void iniciar_jogo(cobra *head,cobra* tail, comida *apple)
{
    
    nokia_lcd_clear();
    nokia_lcd_drawrect(0,0,83,47);

    head->anterior = NULL;
    head->proximo = tail;
    tail->anterior = head;
    tail->proximo = NULL;

    head->x = 20;
    head->y = 20;
    tail->x = 20;
    tail->y = 21;
    
    gerar_comida(apple);
}

void mover_cobra(int up, int down, int right, int left, cobra **head, cobra **tail, int *atualiza)
{
    if(*atualiza == 0){
        cobra* temp = (*tail)->anterior;
        temp->proximo = NULL;
        free(*tail);
        *tail = temp;
    }
    else
    {
        *atualiza = 0;
    }
    
    struct cobra *aux = calloc(1,sizeof(cobra));
    aux->proximo = *head;
    aux->x = (*head)->x;
    aux->y = (*head)->y;
    (*head)->anterior = aux;
    *head = aux;
    
    if(up){
        (*head)->y--;
    }
    else if (down){
        (*head)->y++;
    }
    else if (right){
        (*head)->x++;
    }
    else if (left){
        (*head)->x--;
    }
}

void print_comida(comida* apple){
    nokia_lcd_set_pixel(apple->x, apple->y, 1);
}
