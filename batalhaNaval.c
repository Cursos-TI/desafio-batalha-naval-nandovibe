#include <stdio.h>

#define TAM 10      // Tamanho fixo do tabuleiro
#define NAVIO 3     // Valor que representa navio
#define TAM_NAVIO 3 // Tamanho fixo dos navios

// Função para inicializar o tabuleiro com água (0)
void inicializarTabuleiro(int tabuleiro[TAM][TAM]) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            tabuleiro[i][j] = 0;
        }
    }
}

// Função para imprimir o tabuleiro
void imprimirTabuleiro(int tabuleiro[TAM][TAM]) {
    printf("   ");
    for (int j = 0; j < TAM; j++) {
        printf("%2d ", j);
    }
    printf("\n");

    for (int i = 0; i < TAM; i++) {
        printf("%2d ", i);
        for (int j = 0; j < TAM; j++) {
            printf("%2d ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

// Função para posicionar navio horizontal
int posicionarNavioHorizontal(int tabuleiro[TAM][TAM], int linha, int coluna) {
    if (coluna + TAM_NAVIO > TAM) return 0; // fora dos limites
    for (int j = 0; j < TAM_NAVIO; j++) {
        if (tabuleiro[linha][coluna + j] == NAVIO) return 0; // sobreposição
    }
    for (int j = 0; j < TAM_NAVIO; j++) {
        tabuleiro[linha][coluna + j] = NAVIO;
    }
    return 1;
}

// Função para posicionar navio vertical
int posicionarNavioVertical(int tabuleiro[TAM][TAM], int linha, int coluna) {
    if (linha + TAM_NAVIO > TAM) return 0; // fora dos limites
    for (int i = 0; i < TAM_NAVIO; i++) {
        if (tabuleiro[linha + i][coluna] == NAVIO) return 0; // sobreposição
    }
    for (int i = 0; i < TAM_NAVIO; i++) {
        tabuleiro[linha + i][coluna] = NAVIO;
    }
    return 1;
}

int main() {
    int tabuleiro[TAM][TAM];
    inicializarTabuleiro(tabuleiro);

    // Coordenadas iniciais dos navios (pode alterar para testar)
    int linhaNavioH = 2, colunaNavioH = 4; // horizontal
    int linhaNavioV = 5, colunaNavioV = 7; // vertical

    // Posiciona os navios
    if (!posicionarNavioHorizontal(tabuleiro, linhaNavioH, colunaNavioH)) {
        printf("Erro ao posicionar navio horizontal!\n");
    }
    if (!posicionarNavioVertical(tabuleiro, linhaNavioV, colunaNavioV)) {
        printf("Erro ao posicionar navio vertical!\n");
    }

    // Exibe tabuleiro
    printf("\nTabuleiro Batalha Naval (0 = água, 3 = navio):\n\n");
    imprimirTabuleiro(tabuleiro);

    return 0;
}