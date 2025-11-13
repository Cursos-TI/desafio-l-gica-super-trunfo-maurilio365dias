
#include <stdio.h>
#include <string.h>   
#include <stdlib.h>   
// Definicao de constantes para o tamanho maximo das strings 
#define MAX_STRING_LEN 50
#define NUM_ATRIBUTOS 5

// Definição da Estrutura de Dados (Struct em C)

struct Carta {
    char estado[MAX_STRING_LEN];
    char codigo[MAX_STRING_LEN];
    char nome_cidade[MAX_STRING_LEN];
    int populacao;
    float area; // em km²
    float pib;  // em bilhões de R$
    int num_pontos_turisticos;
    float densidade_populacional;
    float pib_per_capita;
};

// Enumeração manual 
typedef enum {
    POPULACAO = 1,
    AREA = 2,
    PIB = 3,
    DENSIDADE_POPULACIONAL = 4,
    PIB_PER_CAPITA = 5
} AtributoID;


void calcular_atributos_derivados(struct Carta* carta);
float obter_valor_atributo(struct Carta carta, AtributoID attr_id);
const char* obter_nome_atributo(AtributoID attr_id);
void exibir_menu_atributos(int atributos_disponiveis[], int num_disponiveis);
int escolher_atributo(int atributos_disponiveis[], int num_disponiveis, const char* prompt_msg);
void comparar_rodada(struct Carta c1, struct Carta c2, AtributoID attr1_id, AtributoID attr2_id);

int main() {
    // Definicao das cartas pré-cadastradas
    struct Carta carta1;
    strcpy(carta1.estado, "SP");
    strcpy(carta1.codigo, "SP01");
    strcpy(carta1.nome_cidade, "Sao Paulo");
    carta1.populacao = 12396372;
    carta1.area = 1521.11f;
    carta1.pib = 763.8f;
    carta1.num_pontos_turisticos = 25;

    struct Carta carta2;
    strcpy(carta2.estado, "RJ");
    strcpy(carta2.codigo, "RJ01");
    strcpy(carta2.nome_cidade, "Rio de Janeiro");
    carta2.populacao = 6748000;
    carta2.area = 1200.27f;
    carta2.pib = 354.2f;
    carta2.num_pontos_turisticos = 30;

    // Calculo dos atributos derivados
    calcular_atributos_derivados(&carta1);
    calcular_atributos_derivados(&carta2);

    printf("\n");
    printf("  Bem-vindo ao Super Trunfo \n");
    printf("\n");
    printf("Cartas em jogo: 1. %s | 2. %s\n\n", carta1.nome_cidade, carta2.nome_cidade);

    // Gerenciamento dos atributos disponiveis em um array C
    int atributos_disponiveis[NUM_ATRIBUTOS] = {
        POPULACAO, AREA, PIB, DENSIDADE_POPULACIONAL, PIB_PER_CAPITA
    };
    int num_disponiveis = NUM_ATRIBUTOS;

    // 1. Escolha do primeiro atributo
    int attr1_int = escolher_atributo(atributos_disponiveis, num_disponiveis, "Escolha o PRIMEIRO atributo para comparacao:");
    AtributoID attr1 = (AtributoID)attr1_int;

    // Remove o atributo escolhido da lista 
    for (int i = 0; i < num_disponiveis; i++) {
        if (atributos_disponiveis[i] == attr1_int) {
            // Move os elementos restantes para preencher o espaco
            for (int j = i; j < num_disponiveis - 1; j++) {
                atributos_disponiveis[j] = atributos_disponiveis[j + 1];
            }
            num_disponiveis--;
            break;
        }
    }

    // 2. Escolha do segundo atributo
    int attr2_int = escolher_atributo(atributos_disponiveis, num_disponiveis, "Escolha o SEGUNDO atributo:");
    AtributoID attr2 = (AtributoID)attr2_int;

    // 3. Execucao da logica de comparacao
    comparar_rodada(carta1, carta2, attr1, attr2);

    return 0;
}

// Implementação das Funções

void calcular_atributos_derivados(struct Carta* carta) {
    carta->densidade_populacional = (float)carta->populacao / carta->area;
    double pib_em_reais = (double)carta->pib * 1000000000.0;
    carta->pib_per_capita = (float)(pib_em_reais / carta->populacao);
}

float obter_valor_atributo(struct Carta carta, AtributoID attr_id) {
    switch (attr_id) {
        case POPULACAO: return (float)carta.populacao;
        case AREA: return carta.area;
        case PIB: return carta.pib;
        case DENSIDADE_POPULACIONAL: return carta.densidade_populacional;
        case PIB_PER_CAPITA: return carta.pib_per_capita;
        default: return 0.0f;
    }
}

const char* obter_nome_atributo(AtributoID attr_id) {
    switch (attr_id) {
        case POPULACAO: return "Populacao";
        case AREA: return "Area (km^2)";
        case PIB: return "PIB (bi R$)";
        case DENSIDADE_POPULACIONAL: return "Densidade Populacional";
        case PIB_PER_CAPITA: return "PIB per capita (R$)";
        default: return "Desconhecido";
    }
}

void exibir_menu_atributos(int atributos_disponiveis[], int num_disponiveis) {
    printf("Opcoes de atributos:\n");
    for (int i = 0; i < num_disponiveis; i++) {
        printf("%d. %s\n", atributos_disponiveis[i], obter_nome_atributo((AtributoID)atributos_disponiveis[i]));
    }
}

int escolher_atributo(int atributos_disponiveis[], int num_disponiveis, const char* prompt_msg) {
    int escolha;
    int entrada_valida = 0;

    while (!entrada_valida) {
        exibir_menu_atributos(atributos_disponiveis, num_disponiveis);
        printf("%s (Digite o numero): ", prompt_msg);
        
        if (scanf("%d", &escolha) != 1) {
            printf("Entrada invalida. Digite um numero.\n");
            // Limpa o buffer de entrada do C
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        for (int i = 0; i < num_disponiveis; i++) {
            if (atributos_disponiveis[i] == escolha) {
                entrada_valida = 1;
                break;
            }
        }

        if (!entrada_valida) {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }
    return escolha;
}

// Retorna 1 se C1 vence, 0 caso contrario (C2 vence ou empata)
int comparar_atributo_individual_score(float val1, float val2, AtributoID attr_id) {
    int is_densidade = (attr_id == DENSIDADE_POPULACIONAL);
    int c1_vence = is_densidade ? (val1 < val2) : (val1 > val2);
    
    if (c1_vence) {
        return 1;
    } else {
        return 0;
    }
}

void comparar_rodada(struct Carta c1, struct Carta c2, AtributoID attr1_id, AtributoID attr2_id) {
    printf("\n--- Resultado---\n");

    float val1_attr1 = obter_valor_atributo(c1, attr1_id);
    float val2_attr1 = obter_valor_atributo(c2, attr1_id);
    float val1_attr2 = obter_valor_atributo(c1, attr2_id);
    float val2_attr2 = obter_valor_atributo(c2, attr2_id);

    // Calcular pontuacoes individuais e somar
    int pontos_c1 = 0;
    pontos_c1 += comparar_atributo_individual_score(val1_attr1, val2_attr1, attr1_id);
    pontos_c1 += comparar_atributo_individual_score(val1_attr2, val2_attr2, attr2_id);
    
    int pontos_c2 = 2 - pontos_c1; // Se o total e 2 pontos

    // Exibicao formatada usando printf
    printf("Atributo                      %s              %s\n", c1.nome_cidade, c2.nome_cidade);
    printf("\n");
    printf("%-30s%-20.2f%-20.2f\n", obter_nome_atributo(attr1_id), val1_attr1, val2_attr1);
    printf("%-30s%-20.2f%-20.2f\n", obter_nome_atributo(attr2_id), val1_attr2, val2_attr2);
    printf("\n");
    printf("%-30s%-20d%-20d\n", "Pontuacao Total:", pontos_c1, pontos_c2);

    // Declaracao do vencedor ou tratamento de empates
    if (pontos_c1 > pontos_c2) {
        printf("\nResultado Final: Carta 1 (%s) venceu a rodada!\n", c1.nome_cidade);
    } else if (pontos_c2 > pontos_c1) {
        printf("\nResultado Final: Carta 2 (%s) venceu a rodada!\n", c2.nome_cidade);
    } else {
        printf("\nResultado Final: Empate na soma total dos pontos!\n");
    }
    printf("\n");
}
