#include <stdio.h>

#define TAM 10          // Tamanho fixo do tabuleiro
#define NAVIO 3         // Valor que representa navio
#define TAM_NAVIO 3     // Tamanho fixo dos navios

// Protótipos de funções
void inicializarTabuleiro(int tabuleiro[TAM][TAM]);
void imprimirTabuleiro(int tabuleiro[TAM][TAM]);
int dentroDosLimites(int linha, int coluna);
int areaLivre(int tabuleiro[TAM][TAM], int linha, int coluna);
int posicionarNavioHorizontal(int tabuleiro[TAM][TAM], int linha, int coluna);
int posicionarNavioVertical(int tabuleiro[TAM][TAM], int linha, int coluna);
int posicionarNavioDiagonalPrincipal(int tabuleiro[TAM][TAM], int linha, int coluna);
int posicionarNavioDiagonalSecundaria(int tabuleiro[TAM][TAM], int linha, int coluna);

int main() {
    int tabuleiro[TAM][TAM];
    inicializarTabuleiro(tabuleiro);

    // Coordenadas otimizadas para evitar conflitos
    int coordenadasNavios[4][3] = {
        {1, 1, 0},  // Horizontal: linha, coluna, tipo (0 = horizontal)
        {5, 8, 1},  // Vertical: linha, coluna, tipo (1 = vertical)
        {0, 7, 2},  // Diagonal Principal: linha, coluna, tipo (2 = diagonal ↘)
        {3, 2, 3}   // Diagonal Secundária: linha, coluna, tipo (3 = diagonal ↙)
    };

    // Posiciona os navios com validação usando loop otimizado
    const char* tiposNavio[] = {"horizontal", "vertical", "diagonal principal", "diagonal secundaria"};
    int erros = 0;
    
    for (int i = 0; i < 4; i++) {
        int sucesso = 0;
        switch (coordenadasNavios[i][2]) {
            case 0:
                sucesso = posicionarNavioHorizontal(tabuleiro, coordenadasNavios[i][0], coordenadasNavios[i][1]);
                break;
            case 1:
                sucesso = posicionarNavioVertical(tabuleiro, coordenadasNavios[i][0], coordenadasNavios[i][1]);
                break;
            case 2:
                sucesso = posicionarNavioDiagonalPrincipal(tabuleiro, coordenadasNavios[i][0], coordenadasNavios[i][1]);
                break;
            case 3:
                sucesso = posicionarNavioDiagonalSecundaria(tabuleiro, coordenadasNavios[i][0], coordenadasNavios[i][1]);
                break;
        }
        
        if (!sucesso) {
            printf("Erro ao posicionar navio %s!\n", tiposNavio[coordenadasNavios[i][2]]);
            erros++;
        }
    }

    if (erros == 0) {
        printf("\nTodos os navios posicionados com sucesso!\n");
    }

    // Exibe tabuleiro
    printf("\nTabuleiro Batalha Naval (0 = água, 3 = navio):\n\n");
    imprimirTabuleiro(tabuleiro);

    return 0;
}

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
    // Cabeçalho com índices das colunas
    printf("   ");
    for (int j = 0; j < TAM; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    
    // Linhas do tabuleiro com índices
    for (int i = 0; i < TAM; i++) {
        printf("%2d ", i);
        for (int j = 0; j < TAM; j++) {
            printf("%2d ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

// Função para verificar se uma célula está dentro dos limites do tabuleiro
int dentroDosLimites(int linha, int coluna) {
    return (linha >= 0 && linha < TAM && coluna >= 0 && coluna < TAM);
}

// Função para verificar se a área ao redor está livre (distância mínima de 1 célula de água)
int areaLivre(int tabuleiro[TAM][TAM], int linha, int coluna) {
    // Verifica todas as células adjacentes (incluindo diagonais)
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int novaLinha = linha + i;
            int novaColuna = coluna + j;
            
            if (dentroDosLimites(novaLinha, novaColuna) && tabuleiro[novaLinha][novaColuna] == NAVIO) {
                return 0; // Encontrou um navio próximo
            }
        }
    }
    return 1; // Área livre
}

// Função para posicionar navio horizontal
int posicionarNavioHorizontal(int tabuleiro[TAM][TAM], int linha, int coluna) {
    // Verifica limites
    if (coluna + TAM_NAVIO > TAM) return 0;
    
    // Verifica se todas as células e suas adjacências estão livres
    for (int j = 0; j < TAM_NAVIO; j++) {
        if (!dentroDosLimites(linha, coluna + j) || !areaLivre(tabuleiro, linha, coluna + j)) {
            return 0;
        }
    }
    
    // Posiciona o navio
    for (int j = 0; j < TAM_NAVIO; j++) {
        tabuleiro[linha][coluna + j] = NAVIO;
    }
    return 1;
}

// Função para posicionar navio vertical
int posicionarNavioVertical(int tabuleiro[TAM][TAM], int linha, int coluna) {
    // Verifica limites
    if (linha + TAM_NAVIO > TAM) return 0;
    
    // Verifica se todas as células e suas adjacências estão livres
    for (int i = 0; i < TAM_NAVIO; i++) {
        if (!dentroDosLimites(linha + i, coluna) || !areaLivre(tabuleiro, linha + i, coluna)) {
            return 0;
        }
    }
    
    // Posiciona o navio
    for (int i = 0; i < TAM_NAVIO; i++) {
        tabuleiro[linha + i][coluna] = NAVIO;
    }
    return 1;
}

// Função para posicionar navio diagonal ↘
int posicionarNavioDiagonalPrincipal(int tabuleiro[TAM][TAM], int linha, int coluna) {
    // Verifica limites
    if (linha + TAM_NAVIO > TAM || coluna + TAM_NAVIO > TAM) return 0;
    
    // Verifica se todas as células e suas adjacências estão livres
    for (int i = 0; i < TAM_NAVIO; i++) {
        if (!dentroDosLimites(linha + i, coluna + i) || !areaLivre(tabuleiro, linha + i, coluna + i)) {
            return 0;
        }
    }
    
    // Posiciona o navio
    for (int i = 0; i < TAM_NAVIO; i++) {
        tabuleiro[linha + i][coluna + i] = NAVIO;
    }
    return 1;
}

// Função para posicionar navio diagonal ↙
int posicionarNavioDiagonalSecundaria(int tabuleiro[TAM][TAM], int linha, int coluna) {
    // Verifica se o navio cabe no tabuleiro
    if (linha + TAM_NAVIO > TAM || coluna - (TAM_NAVIO - 1) < 0) return 0;
    
    // Verifica se todas as células e suas adjacências estão livres
    for (int i = 0; i < TAM_NAVIO; i++) {
        if (!dentroDosLimites(linha + i, coluna - i) || !areaLivre(tabuleiro, linha + i, coluna - i)) {
            return 0;
        }
    }
    
    // Posiciona o navio
    for (int i = 0; i < TAM_NAVIO; i++) {
        tabuleiro[linha + i][coluna - i] = NAVIO;
    }
    return 1;
}