
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fogefoge.h"
#include "mapa.h"
#include "ui.h"

MAPA m;
POSICAO heroi;
int tempilula = 0;

int praondeofantasmavai(int xatual, int yatual, int* xdestino, int* ydestino) {
    int opcoes[4][2] = {
        {xatual, yatual+1}, // direita
        {xatual+1, yatual},  // baixo
        {xatual, yatual-1}, // esquerda
        {xatual-1, yatual}, // cima
    };

    srand(time(0));

    for(int i = 0; i < 10; i++) {
        int posicao = rand() % 4;

        if(podeandar(&m, FANTASMA, opcoes[posicao][0], opcoes[posicao][1])) {
            *xdestino = opcoes[posicao][0];
            *ydestino = opcoes[posicao][1];
            return 1;
        }
    }

    return 0;
      
}

void fantasmas(){
    MAPA copia;

    copiamapa(&copia, &m);

    for(int i = 0; i < copia.linhas; i++) {
        for(int j = 0; j < copia.colunas; j++) {
            if(copia.matriz[i][j] == FANTASMA) {
                int xdestino;
                int ydestino;

                int encontrou = praondeofantasmavai(i, j, &xdestino, &ydestino);
                
                if(encontrou) {
                    andanomapa(&m, i, j, xdestino, ydestino);
                }
            }
        }
    }

    liberamapa(&copia);
}

int acabou() {
    POSICAO pos;

    int fogefogenomapa = encontramapa(&m, &pos, HEROI);
    return !fogefogenomapa;
}

int ehdirecao(char direcao) {
    return direcao == CIMA || 
        direcao == ESQUERDA || 
        direcao == BAIXO || 
        direcao == DIREITA;
}

void move(char direcao){
    
    if(!ehdirecao(direcao)) 
        return;

    int proximox = heroi.x;
    int proximoy = heroi.y;

    switch(direcao) {
        case ESQUERDA:
            // move to left
            proximoy--;          
            break;
        case CIMA:
            // move to up
            proximox--;          
            break;
        case BAIXO:
            // move to down
            proximox++;          
            break;
        case DIREITA:
            // move to right
            proximoy++;      
            break;
    }

   if(!podeandar(&m, HEROI, proximox, proximoy)) return;

   if(ehpersonagem(&m, PILULA, proximox, proximoy)) {
       tempilula = 1;
   }

    andanomapa(&m, heroi.x, heroi.y, proximox, proximoy);
    heroi.x = proximox;
    heroi.y = proximoy;
}

void explodepilula() {

    if(!tempilula) return;

    explodepilula2(heroi.x, heroi.y, 0, 1, 3);
    explodepilula2(heroi.x, heroi.y, 0, -1, 3);
    explodepilula2(heroi.x, heroi.y, 1, 0, 3);
    explodepilula2(heroi.x, heroi.y, -1, 0, 3);

    tempilula = 0;
}

void explodepilula2(int x, int y, int somax, int somay, int qtd) {

    if(qtd == 0) return;

    int novox = x + somax;
    int novoy = y + somay;

    if(!ehvalida(&m, novox, novoy)) return;
    if(ehparede(&m, novox, novoy)) return;

    m.matriz[novox][novoy] = VAZIO;
    explodepilula2(novox, novoy, somax, somay, qtd-1);
}

int main() {

    lemapa(&m);
    encontramapa(&m, &heroi, HEROI);

    do{
        printf("Tem pilula? %s\n", (tempilula ? "Sim" : "Não"));

        imprimirmapa(&m);

        char comando;
        scanf(" %c", &comando);

        move(comando);
        if(comando == BOMBA) explodepilula();

        fantasmas();

    }while(!acabou());   

    liberamapa(&m);   
}


// alocando espaço para as linhas,
// que guardam ponteiro de inteiro.
// matriz = malloc(sizeof(int*) * linhas);

// agora, para cada linha, alocamos
// for(int i = 0; i < linhas; i++) {
//     matriz[i] = malloc(sizeof(int) * (colunas+1));
// }
