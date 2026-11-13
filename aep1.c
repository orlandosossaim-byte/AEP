#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. DEFINIÇÃO DOS TIPOS DE DADOS (Devem ficar fora das funções)
typedef enum { 
    PLANEJADA, 
    REALIZADA, 
    CANCELADA 
} Situacao;

typedef struct {
    int codigo;
    char escola[50];
    char tema[50];
    char turma[20];
    int dia;
    int mes;
    int ano;
    char publico[50];
    char responsavel[50];
    int quantidadePrevista;
    int quantidadeParticipantes;
    Situacao situacao;
} Acao;

// 2. PROTÓTIPOS DAS FUNÇÕES (Avisa ao compilador que elas existem)
void cadastrar();
void listar();
void pesquisar();
void atualizar();
void registrarResultado();
void resumo();

// 3. FUNÇÃO PRINCIPAL (Apenas uma no programa inteiro)
int main(int argc, char *argv[]) {
    int opcao;
    
    do {
        printf("\n===== SISTEMA PSE EM ACAO =====\n");
        printf("1 - Cadastrar acao\n");
        printf("2 - Listar acoes\n");
        printf("3 - Pesquisar acao\n");
        printf("4 - Atualizar acao\n");
        printf("5 - Registrar resultado\n");
        printf("6 - Resumo geral\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: 
                cadastrar(); 
                break;
            case 2: 
                listar(); 
                break;
            case 3: 
                pesquisar(); 
                break;
            case 4: 
                atualizar(); 
                break;
            case 5: 
                registrarResultado(); 
                break;
            case 6: 
                resumo(); 
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while(opcao != 0);
    
    return 0;
}

// 4. IMPLEMENTAÇÃO DAS FUNÇÕES (Onde você vai escrever a lógica de cada uma)
void cadastrar() {
    printf("\n--- Funcionalidade: Cadastrar Acao ---\n");
    // Escreva o código de cadastro aqui
}

void listar() {
    printf("\n--- Funcionalidade: Listar Acoes ---\n");
    // Escreva o código de listagem aqui
}

void pesquisar() {
    printf("\n--- Funcionalidade: Pesquisar Acao ---\n");
    // Escreva o código de pesquisa aqui
}

void atualizar() {
    printf("\n--- Funcionalidade: Atualizar Acao ---\n");
    // Escreva o código de atualização aqui
}

void registrarResultado() {
    printf("\n--- Funcionalidade: Registrar Resultado ---\n");
    // Escreva o código de registro aqui
}

void resumo() {
    printf("\n--- Funcionalidade: Resumo Geral ---\n");
    // Escreva o código do resumo aqui
}

