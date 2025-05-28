// Multi-Layer Dijkstra com estimativa r[v] (heurística reversa) baseada no artigo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NOS 200
#define MAX_ARESTAS 1000
#define MAX_PILHA 10
#define INFINITO 999999
#define K_MAX 5
#define MAX_COMPRIMENTO 20

typedef enum
{
    ETHERNET,
    STS_24C,
    STS_3C7V,
    WDM_1310,
    WDM_1550
} Tecnologia;

const char *nomes_tecnologia[] = {
    "ETHERNET", "STS_24C", "STS_3C7V", "WDM_1310", "WDM_1550"};

typedef struct
{
    int id;
    Tecnologia pilha[MAX_PILHA];
    int tamanho_pilha;
} No;

typedef struct
{
    No origem;
    No destino;
    int peso;
    int capacidade;
} Aresta;

Aresta arestas[MAX_ARESTAS];
int total_arestas = 0;

int estimativa_reversa[MAX_NOS * K_MAX];

bool mesma_pilha(No a, No b)
{
    if (a.id != b.id || a.tamanho_pilha != b.tamanho_pilha)
        return false;
    for (int i = 0; i < a.tamanho_pilha; i++)
        if (a.pilha[i] != b.pilha[i])
            return false;
    return true;
}

No criar_no(int id, Tecnologia *pilha, int tamanho)
{
    No n;
    n.id = id;
    n.tamanho_pilha = tamanho;
    for (int i = 0; i < tamanho; i++)
        n.pilha[i] = pilha[i];
    return n;
}

void imprimir_no(No n)
{
    printf("Dispositivo %d [", n.id);
    for (int i = 0; i < n.tamanho_pilha; i++)
    {
        printf("%s", nomes_tecnologia[n.pilha[i]]);
        if (i < n.tamanho_pilha - 1)
            printf("->");
    }
    printf("]");
}

void adicionar_aresta(No de, No para, int peso, int capacidade)
{
    arestas[total_arestas++] = (Aresta){de, para, peso, capacidade};
}

void calcular_estimativas(No destino, int banda_minima)
{
    for (int i = 0; i < MAX_NOS * K_MAX; i++)
        estimativa_reversa[i] = INFINITO;
    for (int i = 0; i < total_arestas; i++)
    {
        if (arestas[i].capacidade >= banda_minima && mesma_pilha(arestas[i].destino, destino))
        {
            estimativa_reversa[i] = arestas[i].peso;
        }
    }
}

bool caminho_valido(int caminho[], int tam, int novo_id)
{
    for (int i = 0; i < tam; i++)
    {
        if (caminho[i] == novo_id)
            return false;
    }
    return true;
}

void dijkstra(No origem, No destino, int banda_minima)
{
    typedef struct
    {
        No no;
        int dist;
        int prioridade;
        int anterior;
        int caminho[MAX_COMPRIMENTO];
        int tam_caminho;
    } Entrada;

    Entrada fila[MAX_NOS * K_MAX];
    int tam_fila = 0;
    int melhor_custo = INFINITO;
    int melhor_idx = -1;

    calcular_estimativas(destino, banda_minima);

    fila[0] = (Entrada){origem, 0, 0, -1, {origem.id}, 1};
    tam_fila = 1;

    while (tam_fila > 0)
    {
        int melhor = 0;
        for (int i = 1; i < tam_fila; i++)
        {
            if (fila[i].prioridade < fila[melhor].prioridade)
                melhor = i;
        }

        Entrada atual = fila[melhor];
        fila[melhor] = fila[--tam_fila];

        if (mesma_pilha(atual.no, destino))
        {
            if (atual.dist < melhor_custo)
            {
                melhor_custo = atual.dist;
                melhor_idx = tam_fila;
                fila[tam_fila++] = atual;
            }
            continue;
        }

        for (int i = 0; i < total_arestas; i++)
        {
            if (!mesma_pilha(arestas[i].origem, atual.no))
                continue;
            if (arestas[i].capacidade < banda_minima)
                continue;

            No prox = arestas[i].destino;
            int novo_dist = atual.dist + arestas[i].peso;
            if (novo_dist >= melhor_custo)
                continue;

            if (!caminho_valido(atual.caminho, atual.tam_caminho, prox.id))
                continue;

            Entrada novo;
            novo.no = prox;
            novo.dist = novo_dist;
            novo.prioridade = novo_dist + estimativa_reversa[i];
            novo.anterior = melhor;
            memcpy(novo.caminho, atual.caminho, sizeof(int) * atual.tam_caminho);
            novo.caminho[atual.tam_caminho] = prox.id;
            novo.tam_caminho = atual.tam_caminho + 1;

            fila[tam_fila++] = novo;
        }
    }

    if (melhor_idx != -1)
    {
        printf("\nCaminho encontrado! Custo: %d\n", melhor_custo);
        for (int i = 0; i < fila[melhor_idx].tam_caminho; i++)
        {
            printf("%d", fila[melhor_idx].caminho[i]);
            if (i < fila[melhor_idx].tam_caminho - 1)
                printf(" -> ");
        }
        printf("\n");
    }
    else
    {
        printf("Nenhum caminho viável encontrado.\n");
    }
}

// Análise de complexidade
void analisar_complexidade()
{
    printf("Complexidade estimada: O((V + E) * log V)\n");
}

// Função principal
int main()
{
    Tecnologia eth[] = {ETHERNET};
    Tecnologia eth_sts[] = {ETHERNET, STS_24C};
    Tecnologia sts_wdm[] = {STS_24C, WDM_1310};
    Tecnologia sts3c7v[] = {STS_3C7V};

    // Criando nós com IDs únicos e tecnologias corretas
    No A1 = criar_no(0, eth, 1);
    No B1 = criar_no(1, eth_sts, 2);
    No B2 = criar_no(2, eth_sts, 2);
    No B3 = criar_no(3, sts3c7v, 1);
    No D1 = criar_no(4, eth, 1);
    No D2 = criar_no(5, eth_sts, 2);
    No D3 = criar_no(6, sts3c7v, 1);
    No E1 = criar_no(7, eth_sts, 2);
    No E2 = criar_no(8, sts3c7v, 1);
    No F1 = criar_no(9, eth, 1);
    No F2 = criar_no(10, eth_sts, 2);
    No F3 = criar_no(11, sts3c7v, 1);
    No C1 = criar_no(12, eth, 1);

    adicionar_aresta(A1, B1, 1, 5);
    adicionar_aresta(B1, B2, 1, 5);
    adicionar_aresta(B2, D2, 2, 10);
    adicionar_aresta(B2, E1, 2, 10);
    adicionar_aresta(D2, D1, 1, 5);
    adicionar_aresta(D2, E1, 2, 10);
    adicionar_aresta(D1, D3, 1, 5);
    adicionar_aresta(E1, F2, 2, 10);
    adicionar_aresta(B3, D3, 3, 15);
    adicionar_aresta(B3, E2, 3, 15);
    adicionar_aresta(D3, E2, 3, 15);
    adicionar_aresta(E2, F3, 3, 15);
    adicionar_aresta(F3, F1, 1, 5);
    adicionar_aresta(F1, C1, 1, 5);

    dijkstra(A1, F3, 5);

    analisar_complexidade();
    return 0;
}
