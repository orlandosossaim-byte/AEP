#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ACOES 100
#define TAM_NOME 50
#define TAM_DATA 11

typedef enum {
    PLANEJADA = 1,
    REALIZADA = 2,
    CANCELADA = 3
} Situacao;

typedef struct {
    int codigo;
    char escola[TAM_NOME];
    char tema[TAM_NOME];
    char data[TAM_DATA];
    char publico[TAM_NOME];
    char responsavel[TAM_NOME];
    int quantidadePrevista;
    int quantidadeParticipantes;
    Situacao situacao;
} Acao;

Acao acoes[MAX_ACOES];
int totalAcoes = 0;

void limparLinha(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void lerTexto(const char *mensagem, char destino[], int tamanho) {
    size_t n;
    do {
        printf("%s", mensagem);
        if (fgets(destino, tamanho, stdin) == NULL) {
            destino[0] = '\0';
            return;
        }
        n = strlen(destino);
        if (n > 0 && destino[n - 1] == '\n')
            destino[n - 1] = '\0';
        else
            limparLinha();

        if (destino[0] == '\0')
            printf("O campo nao pode ficar vazio.\n");

    } while (destino[0] == '\0');
}

int lerInteiro(const char *mensagem, int minimo, int maximo) {
    char linha[80], extra;
    int valor;

    for (;;) {
        printf("%s", mensagem);

        if (fgets(linha, sizeof(linha), stdin) == NULL)
            return minimo;

        if (sscanf(linha, " %d %c", &valor, &extra) == 1 &&
            valor >= minimo && valor <= maximo)
            return valor;

        printf("Entrada invalida. Informe um numero entre %d e %d.\n",
               minimo, maximo);
    }
}

int compararTexto(const char *a, const char *b) {
    while (*a && *b) {
        int ca = tolower((unsigned char)*a++);
        int cb = tolower((unsigned char)*b++);

        if (ca != cb)
            return ca - cb;
    }

    return tolower((unsigned char)*a) -
           tolower((unsigned char)*b);
}

int contemTextoIgnorandoMaiusculas(
    const char *texto, const char *termo) {

    size_t i, j;
    size_t n = strlen(texto);
    size_t m = strlen(termo);

    if (m == 0)
        return 1;

    if (m > n)
        return 0;

    for (i = 0; i + m <= n; i++) {
        for (j = 0; j < m; j++) {
            if (tolower((unsigned char)texto[i + j]) !=
                tolower((unsigned char)termo[j]))
                break;
        }

        if (j == m)
            return 1;
    }

    return 0;
}

int buscarIndiceCodigo(int codigo) {
    int i;

    for (i = 0; i < totalAcoes; i++) {
        if (acoes[i].codigo == codigo)
            return i;
    }

    return -1;
}

int dataValida(const char *data) {
    int dia, mes, ano;
    char extra;
    int diasMes;

    if (sscanf(data, "%2d/%2d/%4d%c",
               &dia, &mes, &ano, &extra) != 3)
        return 0;

    if (ano < 2020 || mes < 1 || mes > 12)
        return 0;

    diasMes = (mes == 4 || mes == 6 ||
               mes == 9 || mes == 11) ? 30 : 31;

    if (mes == 2) {
        int bissexto = (ano % 400 == 0) ||
                       (ano % 4 == 0 && ano % 100 != 0);

        diasMes = bissexto ? 29 : 28;
    }

    return dia >= 1 && dia <= diasMes;
}

void lerData(char destino[]) {
    do {
        lerTexto("Data prevista (DD/MM/AAAA): ",
                 destino, TAM_DATA);

        if (!dataValida(destino))
            printf("Data invalida. Use DD/MM/AAAA e uma data existente.\n");

    } while (!dataValida(destino));
}

const char *nomeSituacao(Situacao s) {
    switch (s) {
        case PLANEJADA:
            return "Planejada";

        case REALIZADA:
            return "Realizada";

        case CANCELADA:
            return "Cancelada";

        default:
            return "Indefinida";
    }
}

void exibirAcao(const Acao *a) {
    printf("\nCodigo: %d\nEscola: %s\nTema: %s\nData: %s\n",
           a->codigo, a->escola, a->tema, a->data);

    printf("Publico-alvo: %s\nResponsavel: %s\n",
           a->publico, a->responsavel);

    printf("Participantes previstos: %d\nParticipantes presentes: %d\nSituacao: %s\n",
           a->quantidadePrevista,
           a->quantidadeParticipantes,
           nomeSituacao(a->situacao));
}

void cadastrar(void) {
    Acao nova;

    if (totalAcoes >= MAX_ACOES) {
        printf("Limite de armazenamento atingido (%d acoes).\n",
               MAX_ACOES);
        return;
    }

    printf("\n--- CADASTRAR ACAO ---\n");

    nova.codigo = lerInteiro("Codigo (1 a 999999): ", 1, 999999);

    if (buscarIndiceCodigo(nova.codigo) != -1) {
        printf("Ja existe uma acao com esse codigo. Cadastro cancelado.\n");
        return;
    }

    lerTexto("Escola: ", nova.escola, sizeof(nova.escola));
    lerTexto("Tema: ", nova.tema, sizeof(nova.tema));
    lerData(nova.data);
    lerTexto("Publico-alvo: ", nova.publico, sizeof(nova.publico));
    lerTexto("Responsavel: ", nova.responsavel, sizeof(nova.responsavel));

    nova.quantidadePrevista =
        lerInteiro("Quantidade prevista (0 a 100000): ", 0, 100000);

    nova.quantidadeParticipantes = 0;
    nova.situacao = PLANEJADA;

    acoes[totalAcoes++] = nova;

    printf("Acao cadastrada com sucesso! Situacao inicial: Planejada.\n");
}

void listar(void) {
    int i;

    printf("\n--- ACOES CADASTRADAS (%d/%d) ---\n",
           totalAcoes, MAX_ACOES);

    if (totalAcoes == 0) {
        printf("Nenhuma acao cadastrada.\n");
        return;
    }

    for (i = 0; i < totalAcoes; i++) {
        printf("\n[%d de %d]", i + 1, totalAcoes);
        exibirAcao(&acoes[i]);
    }
}

void pesquisar(void) {
    int opcao, codigo, i, encontrados = 0;
    char termo[TAM_NOME];

    printf("\n--- PESQUISAR ACOES ---\n");
    printf("1 - Por codigo\n2 - Por escola\n3 - Por tema\n");

    opcao = lerInteiro("Escolha: ", 1, 3);

    if (opcao == 1) {
        codigo = lerInteiro("Codigo: ", 1, 999999);
        i = buscarIndiceCodigo(codigo);

        if (i == -1)
            printf("Nenhuma acao encontrada com esse codigo.\n");
        else
            exibirAcao(&acoes[i]);

        return;
    }

    lerTexto(opcao == 2 ?
             "Nome da escola (ou parte): " :
             "Tema (ou parte): ",
             termo, sizeof(termo));

    for (i = 0; i < totalAcoes; i++) {
        const char *campo =
            (opcao == 2) ? acoes[i].escola : acoes[i].tema;

        if (contemTextoIgnorandoMaiusculas(campo, termo)) {
            exibirAcao(&acoes[i]);
            encontrados++;
        }
    }

    if (!encontrados)
        printf("Nenhuma acao encontrada para o termo informado.\n");
    else
        printf("\nTotal encontrado: %d\n", encontrados);
}

void atualizar(void) {
    int codigo, i, opcao;

    printf("\n--- ATUALIZAR ACAO ---\n");

    if (totalAcoes == 0) {
        printf("Nao ha registros para atualizar.\n");
        return;
    }

    codigo = lerInteiro("Codigo da acao: ", 1, 999999);
    i = buscarIndiceCodigo(codigo);

    if (i == -1) {
        printf("Registro inexistente.\n");
        return;
    }

    printf("1 - Escola\n2 - Tema\n3 - Data prevista\n");
    printf("4 - Publico-alvo\n5 - Responsavel\n");
    printf("6 - Quantidade prevista\n");

    opcao = lerInteiro("Qual informacao deseja alterar? ", 1, 6);

    switch (opcao) {
        case 1:
            lerTexto("Nova escola: ", acoes[i].escola,
                     sizeof(acoes[i].escola));
            break;

        case 2:
            lerTexto("Novo tema: ", acoes[i].tema,
                     sizeof(acoes[i].tema));
            break;

        case 3:
            lerData(acoes[i].data);
            break;

        case 4:
            lerTexto("Novo publico-alvo: ", acoes[i].publico,
                     sizeof(acoes[i].publico));
            break;

        case 5:
            lerTexto("Novo responsavel: ", acoes[i].responsavel,
                     sizeof(acoes[i].responsavel));
            break;

        case 6:
            acoes[i].quantidadePrevista =
                lerInteiro("Nova quantidade prevista: ",
                           acoes[i].quantidadeParticipantes,
                           100000);
            break;
    }

    printf("Informacao atualizada com sucesso.\n");
}

void registrarResultado(void) {
    int codigo, i, opcao, presentes;

    printf("\n--- REGISTRAR RESULTADO ---\n");

    if (totalAcoes == 0) {
        printf("Nao ha registros cadastrados.\n");
        return;
    }

    codigo = lerInteiro("Codigo da acao: ", 1, 999999);
    i = buscarIndiceCodigo(codigo);

    if (i == -1) {
        printf("Registro inexistente.\n");
        return;
    }

    printf("1 - Realizada\n2 - Cancelada\n3 - Voltar sem alterar\n");

    opcao = lerInteiro("Nova situacao: ", 1, 3);

    if (opcao == 3) {
        printf("Operacao cancelada pelo usuario.\n");
        return;
    }

    if (opcao == 2) {
        acoes[i].situacao = CANCELADA;
        acoes[i].quantidadeParticipantes = 0;

        printf("Acao marcada como cancelada.\n");
        return;
    }

    presentes = lerInteiro(
        "Quantidade efetiva de participantes: ", 0, 100000);

    acoes[i].situacao = REALIZADA;
    acoes[i].quantidadeParticipantes = presentes;

    printf("Resultado registrado. Acao marcada como realizada.\n");
}

void resumo(void) {
    int i;
    int planejadas = 0, realizadas = 0, canceladas = 0;

    long totalPrevistoRealizadas = 0;
    long totalPresentes = 0;

    double percentual = 0.0;

    for (i = 0; i < totalAcoes; i++) {
        if (acoes[i].situacao == PLANEJADA)
            planejadas++;

        else if (acoes[i].situacao == REALIZADA) {
            realizadas++;
            totalPrevistoRealizadas += acoes[i].quantidadePrevista;
            totalPresentes += acoes[i].quantidadeParticipantes;
        }

        else if (acoes[i].situacao == CANCELADA)
            canceladas++;
    }

    if (totalPrevistoRealizadas > 0)
        percentual =
            (double)totalPresentes * 100.0 /
            (double)totalPrevistoRealizadas;

    printf("\n--- RESUMO GERAL ---\n");
    printf("Total de acoes: %d\n", totalAcoes);
    printf("Planejadas: %d\n", planejadas);
    printf("Realizadas: %d\n", realizadas);
    printf("Canceladas: %d\n", canceladas);

    printf("Participantes previstos nas acoes realizadas: %ld\n",
           totalPrevistoRealizadas);

    printf("Participantes registrados: %ld\n", totalPresentes);

    if (totalPrevistoRealizadas == 0)
        printf("Percentual de participacao: nao calculavel (sem previsao nas acoes realizadas).\n");
    else
        printf("Percentual de participacao: %.2f%%\n", percentual);
}

int main(void) {
    int opcao;

    do {
        printf("\n========== PSE EM ACAO ==========\n");
        printf("1 - Cadastrar acao\n");
        printf("2 - Listar acoes\n");
        printf("3 - Pesquisar acao\n");
        printf("4 - Atualizar acao\n");
        printf("5 - Registrar resultado\n");
        printf("6 - Resumo geral\n");
        printf("0 - Sair\n");

        opcao = lerInteiro("Escolha uma opcao: ", 0, 6);

        switch (opcao) {
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
                printf("Encerrando o PSE em Acao. Ate logo!\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}
