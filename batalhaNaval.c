#include <stdio.h>
#include <stdbool.h>

// Constantes do jogo
#define TAM 10
#define NAVIO 3
#define TAM_NAVIO 3
#define HABILIDADE 5
#define TAM_HAB 5
#define CENTRO_HAB 2  // Índice do centro em uma matriz 5x5

// Protótipos otimizados
void inicializarTabuleiro(int tabuleiro[TAM][TAM]);
void imprimirTabuleiro(const int tabuleiro[TAM][TAM]);
bool estaDentroDosLimites(int linha, int coluna);
bool areaEstaLivre(const int tabuleiro[TAM][TAM], int linha, int coluna);
bool posicionarNavio(int tabuleiro[TAM][TAM], int tipo, int linha, int coluna);
void criarHabilidade(int habilidade[TAM_HAB][TAM_HAB], int tipo);
void aplicarHabilidade(int tabuleiro[TAM][TAM], const int habilidade[TAM_HAB][TAM_HAB], int centroLinha, int centroColuna);

// Estrutura para coordenadas de navios
typedef struct {
    int linha;
    int coluna;
    int tipo;
} CoordenadaNavio;

// Estrutura para coordenadas de habilidades
typedef struct {
    int linha;
    int coluna;
    int tipo;
} CoordenadaHabilidade;

int main() {
    int tabuleiro[TAM][TAM];
    inicializarTabuleiro(tabuleiro);

    // Dados de configuração
    CoordenadaNavio navios[] = {
        {1, 1, 0},  // Horizontal
        {5, 8, 1},  // Vertical
        {0, 7, 2},  // Diagonal ↘
        {3, 2, 3}   // Diagonal ↙
    };
    
    CoordenadaHabilidade habilidades[] = {
        {2, 2, 0},  // Cone
        {5, 5, 1},  // Cruz
        {7, 7, 2}   // Octaedro
    };
    
    const char* tiposNavio[] = {"horizontal", "vertical", "diagonal principal", "diagonal secundaria"};
    
    // Posiciona navios
    int numNavios = sizeof(navios) / sizeof(navios[0]);
    int erros = 0;
    
    for (int i = 0; i < numNavios; i++) {
        if (!posicionarNavio(tabuleiro, navios[i].tipo, navios[i].linha, navios[i].coluna)) {
            printf("Erro ao posicionar navio %s!\n", tiposNavio[navios[i].tipo]);
            erros++;
        }
    }
    
    if (erros == 0) {
        printf("Todos os navios posicionados com sucesso!\n\n");
    }

    // Aplica habilidades
    int numHabilidades = sizeof(habilidades) / sizeof(habilidades[0]);
    int matrizHabilidade[TAM_HAB][TAM_HAB];
    
    for (int i = 0; i < numHabilidades; i++) {
        criarHabilidade(matrizHabilidade, habilidades[i].tipo);
        aplicarHabilidade(tabuleiro, matrizHabilidade, habilidades[i].linha, habilidades[i].coluna);
    }

    // Exibe resultado final
    printf("Tabuleiro Batalha Naval (0 = água, 3 = navio, 5 = habilidade):\n\n");
    imprimirTabuleiro(tabuleiro);

    return 0;
}

// Inicializa o tabuleiro com água (0)
void inicializarTabuleiro(int tabuleiro[TAM][TAM]) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            tabuleiro[i][j] = 0;
        }
    }
}

// Imprime o tabuleiro de forma otimizada
void imprimirTabuleiro(const int tabuleiro[TAM][TAM]) {
    // Cabeçalho
    printf("   ");
    for (int j = 0; j < TAM; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    
    // Corpo do tabuleiro
    for (int i = 0; i < TAM; i++) {
        printf("%2d ", i);
        for (int j = 0; j < TAM; j++) {
            printf("%2d ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

// Verifica se uma coordenada está dentro dos limites
bool estaDentroDosLimites(int linha, int coluna) {
    return (linha >= 0 && linha < TAM && coluna >= 0 && coluna < TAM);
}

// Verifica se a área ao redor está livre (otimizada)
bool areaEstaLivre(const int tabuleiro[TAM][TAM], int linha, int coluna) {
    // Verifica todas as células adjacentes (incluindo diagonais)
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int novaLinha = linha + i;
            int novaColuna = coluna + j;
            
            if (estaDentroDosLimites(novaLinha, novaColuna) && 
                tabuleiro[novaLinha][novaColuna] == NAVIO) {
                return false;
            }
        }
    }
    return true;
}

// Função unificada para posicionar navios (mais eficiente)
bool posicionarNavio(int tabuleiro[TAM][TAM], int tipo, int linha, int coluna) {
    int deltaLinha = 0, deltaColuna = 0;
    
    // Define a direção com base no tipo
    switch (tipo) {
        case 0: deltaColuna = 1; break; // Horizontal
        case 1: deltaLinha = 1; break;  // Vertical
        case 2: deltaLinha = 1; deltaColuna = 1; break; // Diagonal ↘
        case 3: deltaLinha = 1; deltaColuna = -1; break; // Diagonal ↙
        default: return false;
    }
    
    // Verifica limites
    int fimLinha = linha + (TAM_NAVIO - 1) * deltaLinha;
    int fimColuna = coluna + (TAM_NAVIO - 1) * deltaColuna;
    
    if (!estaDentroDosLimites(fimLinha, fimColuna) || 
        (tipo == 3 && fimColuna < 0)) {
        return false;
    }
    
    // Verifica se a área está livre
    for (int i = 0; i < TAM_NAVIO; i++) {
        int currentLinha = linha + i * deltaLinha;
        int currentColuna = coluna + i * deltaColuna;
        
        if (!estaDentroDosLimites(currentLinha, currentColuna) || 
            !areaEstaLivre(tabuleiro, currentLinha, currentColuna)) {
            return false;
        }
    }
    
    // Posiciona o navio
    for (int i = 0; i < TAM_NAVIO; i++) {
        int currentLinha = linha + i * deltaLinha;
        int currentColuna = coluna + i * deltaColuna;
        tabuleiro[currentLinha][currentColuna] = NAVIO;
    }
    
    return true;
}

// Função unificada para criar habilidades
void criarHabilidade(int habilidade[TAM_HAB][TAM_HAB], int tipo) {
    // Inicializa com zeros
    for (int i = 0; i < TAM_HAB; i++) {
        for (int j = 0; j < TAM_HAB; j++) {
            habilidade[i][j] = 0;
        }
    }
    
    // Preenche com base no tipo
    switch (tipo) {
        case 0: // Cone
            for (int i = 0; i < TAM_HAB; i++) {
                int largura = i + 1;
                int inicio = CENTRO_HAB - i/2;
                
                for (int j = inicio; j < inicio + largura && j < TAM_HAB; j++) {
                    if (j >= 0) habilidade[i][j] = 1;
                }
            }
            break;
            
        case 1: // Cruz
            for (int i = 0; i < TAM_HAB; i++) {
                habilidade[i][CENTRO_HAB] = 1;
                habilidade[CENTRO_HAB][i] = 1;
            }
            break;
            
        case 2: // Octaedro
            for (int i = 0; i < TAM_HAB; i++) {
                int distancia = (i <= CENTRO_HAB) ? i : TAM_HAB - 1 - i;
                int inicio = CENTRO_HAB - distancia;
                int fim = CENTRO_HAB + distancia;
                
                for (int j = inicio; j <= fim; j++) {
                    if (j >= 0 && j < TAM_HAB) {
                        habilidade[i][j] = 1;
                    }
                }
            }
            break;
    }
}

// Aplica habilidade ao tabuleiro (otimizada)
void aplicarHabilidade(int tabuleiro[TAM][TAM], const int habilidade[TAM_HAB][TAM_HAB], int centroLinha, int centroColuna) {
    int offset = TAM_HAB / 2;
    
    for (int i = 0; i < TAM_HAB; i++) {
        int linhaTab = centroLinha - offset + i;
        
        for (int j = 0; j < TAM_HAB; j++) {
            int colunaTab = centroColuna - offset + j;
            
            if (estaDentroDosLimites(linhaTab, colunaTab) && 
                habilidade[i][j] == 1 && 
                tabuleiro[linhaTab][colunaTab] == 0) {
                tabuleiro[linhaTab][colunaTab] = HABILIDADE;
            }
        }
    }
}