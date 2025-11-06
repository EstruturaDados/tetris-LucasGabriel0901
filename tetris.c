#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Constantes Globais ---
// Define o tamanho máximo da nossa fila
#define TAMANHO_FILA 5

// --- Definição das Estruturas ---

/**
 * @brief Define a estrutura de uma Peca.
 * Possui um nome (char) e um id único (int).
 */
typedef struct {
    char nome;
    int id;
} Peca;

/**
 * @brief Define a estrutura da Fila Circular (FIFO).
 * Armazena um array de Pecas, índices de início e fim, e um contador.
 */
typedef struct {
    Peca itens[TAMANHO_FILA];
    int inicio;    // Índice do primeiro elemento
    int fim;       // Índice do último elemento
    int contador;  // Número de elementos atualmente na fila
} FilaCircular;

// --- Variável Global ---
// Contador global para garantir que cada peça tenha um ID único
int proximoId = 0;

// --- Protótipos das Funções (Declarações) ---

// Funções da Fila
void inicializarFila(FilaCircular *f);
int filaEstaCheia(FilaCircular *f);
int filaEstaVazia(FilaCircular *f);
int enqueue(FilaCircular *f, Peca peca); // Enfileirar (Inserir no fim)
int dequeue(FilaCircular *f, Peca *pecaRemovida); // Desenfileirar (Remover do início)

// Funções do Jogo
Peca gerarPeca();
void exibirPeca(Peca p);
void exibirEstado(FilaCircular *f);
void exibirMenu();

// --- Função Principal (main) ---
int main() {
    // Inicializa o gerador de números aleatórios (para a função gerarPeca)
    srand(time(NULL));

    FilaCircular fila;
    int opcao;

    // 1. Prepara a estrutura da fila
    inicializarFila(&fila);

    // 2. Preenche a fila com as 5 peças iniciais, como pedido
    printf("Inicializando o jogo... Gerando 5 pecas iniciais:\n");
    for (int i = 0; i < TAMANHO_FILA; i++) {
        Peca novaPeca = gerarPeca();
        enqueue(&fila, novaPeca);
    }

    // 3. Loop principal do jogo
    do {
        // Mostra o estado atual e as opções
        exibirEstado(&fila);
        exibirMenu();
        
        // Lê a opção do usuário
        if (scanf("%d", &opcao) != 1) {
            // Limpa o buffer de entrada se o usuário digitar algo não numérico
            while (getchar() != '\n');
            opcao = -1; // Força uma opção inválida
        }

        printf("\n"); // Adiciona espaço para legibilidade

        switch (opcao) {
            case 1:
                // Ação: Jogar peça (Dequeue)
                { // Bloco usado para declarar variável local 'pecaJogada'
                    Peca pecaJogada;
                    if (dequeue(&fila, &pecaJogada)) {
                        printf("Acao: Peca jogada ");
                        exibirPeca(pecaJogada);
                        printf(" (removida do jogo).\n");
                    } else {
                        printf("Acao: Fila esta vazia! Nao e possivel jogar.\a\n");
                    }
                }
                break;
            
            case 2:
                // Ação: Inserir nova peça (Enqueue)
                {
                    Peca novaPeca = gerarPeca();
                    if (enqueue(&fila, novaPeca)) {
                        printf("Acao: Nova peca inserida no final da fila ");
                        exibirPeca(novaPeca);
                        printf(".\n");
                    } else {
                        printf("Acao: Fila esta cheia! Nao e possivel inserir.\a\n");
                    }
                }
                break;
            
            case 0:
                printf("Encerrando o Tetris Stack. Ate logo!\n");
                break;
            
            default:
                printf("Opcao invalida! Por favor, escolha 0, 1 ou 2.\a\n");
                break;
        }
        
        if (opcao != 0) {
             printf("\nPressione Enter para continuar...");
             while (getchar() != '\n'); // Limpa buffer
             getchar(); // Espera pelo Enter
             // system("cls"); // Descomente para limpar a tela (Windows)
             // system("clear"); // Descomente para limpar a tela (Linux/Mac)
        }

    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções ---

// --- Funções da Fila ---

/**
 * @brief Inicializa a fila circular.
 * O 'inicio' é 0, 'fim' é -1 (indicando estar vazia) e 'contador' é 0.
 */
void inicializarFila(FilaCircular *f) {
    f->inicio = 0;
    f->fim = -1;
    f->contador = 0;
}

/**
 * @brief Verifica se a fila está cheia.
 * @return 1 (true) se cheia, 0 (false) caso contrário.
 */
int filaEstaCheia(FilaCircular *f) {
    return f->contador == TAMANHO_FILA;
}

/**
 * @brief Verifica se a fila está vazia.
 * @return 1 (true) se vazia, 0 (false) caso contrário.
 */
int filaEstaVazia(FilaCircular *f) {
    return f->contador == 0;
}

/**
 * @brief Adiciona (enfileira) uma peça no fim da fila.
 * @return 1 (true) se sucesso, 0 (false) se a fila estava cheia.
 */
int enqueue(FilaCircular *f, Peca peca) {
    if (filaEstaCheia(f)) {
        return 0; // Falha, fila cheia
    }
    
    // Calcula a nova posição final de forma circular
    // (Ex: se fim=4 e TAMANHO_FILA=5, (4+1) % 5 = 0. Volta ao início)
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    
    // Adiciona a peça na posição calculada
    f->itens[f->fim] = peca;
    
    // Incrementa o número de itens na fila
    f->contador++;
    
    return 1; // Sucesso
}

/**
 * @brief Remove (desenfileira) uma peça do início da fila.
 * @param pecaRemovida Ponteiro para onde a peça removida será armazenada.
 * @return 1 (true) se sucesso, 0 (false) se a fila estava vazia.
 */
int dequeue(FilaCircular *f, Peca *pecaRemovida) {
    if (filaEstaVazia(f)) {
        return 0; // Falha, fila vazia
    }
    
    // Pega a peça que está no início da fila
    *pecaRemovida = f->itens[f->inicio];
    
    // Move o início para a próxima posição de forma circular
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    
    // Decrementa o número de itens na fila
    f->contador--;
    
    return 1; // Sucesso
}

// --- Funções do Jogo ---

/**
 * @brief Gera uma nova peça com tipo aleatório ('I', 'O', 'T', 'L')
 * e um ID sequencial único.
 */
Peca gerarPeca() {
    Peca p;
    char tipos[] = "IOTL";
    
    // Escolhe um tipo aleatório (0, 1, 2 ou 3)
    p.nome = tipos[rand() % 4]; 
    
    // Atribui o ID global e incrementa para o próximo
    p.id = proximoId++;         
    
    return p;
}

/**
 * @brief Exibe uma única peça formatada. Ex: [I 0]
 */
void exibirPeca(Peca p) {
    printf("[%c %d]", p.nome, p.id);
}

/**
 * @brief Exibe o estado atual completo da Fila.
 */
void exibirEstado(FilaCircular *f) {
    printf("\n=====================================\n");
    printf("      ESTADO ATUAL (Nivel Basico)\n");
    printf("=====================================\n\n");

    printf("Fila de pecas (%d/%d): \n", f->contador, TAMANHO_FILA);
    if (filaEstaVazia(f)) {
        printf("[Fila Vazia]\n");
    } else {
        // Para exibir na ordem correta, começamos do 'inicio'
        // e andamos 'contador' vezes
        int indiceAtual = f->inicio;
        for (int i = 0; i < f->contador; i++) {
            exibirPeca(f->itens[indiceAtual]);
            printf(" ");
            
            // Avança o índice de forma circular
            indiceAtual = (indiceAtual + 1) % TAMANHO_FILA;
        }
        printf("\n");
    }
    printf("\n-------------------------------------\n");
}

/**
 * @brief Exibe o menu de opções para o jogador.
 */
void exibirMenu() {
    printf("Opcoes de acao:\n");
    printf("Codigo\tAcao\n");
    printf("1\tJogar peca (dequeue)\n");
    printf("2\tInserir nova peca (enqueue)\n");
    printf("0\tSair\n");
    printf("\nOpcao escolhida: ");
}