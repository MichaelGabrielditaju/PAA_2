#include <stdio.h>

void troca(int *a, int *b);
void ordena5(int v[]);
void quicksort3p(int v[], int esq, int dir);
void imprime(int v[], int n);

void troca(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void ordena5(int v[]) {
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (v[i] > v[j]) troca(&v[i], &v[j]);
        }
    }
}

void quicksort3p(int v[], int esq, int dir) {
    if (dir - esq < 10) {  // caso base: insertion sort
        for (int i = esq + 1; i <= dir; i++) {
            int tmp = v[i];
            int j = i - 1;
            while (j >= esq && v[j] > tmp) {
                v[j + 1] = v[j];
                j--;
            }
            v[j + 1] = tmp;
        }
        return;
    }

    // 1. Selecionar 5 elementos equidistantes, evitando extremos
    int idx[5];
    int delta = (dir - esq + 1) / 6;  // dividir em 6 partes => 5 valores internos
    for (int i = 0; i < 5; i++) {
        idx[i] = esq + (i + 1) * delta;
    }

    //imprime(idx, 5);
    int candidatos[5];
    for (int i = 0; i < 5; i++) {
        candidatos[i] = v[idx[i]];
    }

    // 2. Ordenar os 5 candidatos e escolher os 3 do meio
    //imprime(candidatos, 5);
    ordena5(candidatos);
    //imprime(candidatos, 5);

    int p1 = candidatos[1];
    int p2 = candidatos[2];
    int p3 = candidatos[3];

    //printf("%d, %d, %d\n", p1, p2, p3);
    // 3. Particionar em 4 regiões
    int a = esq, b = esq, c = dir;
    int i = esq;

    while (i <= c) {
        if (v[i] < p1) {
            troca(&v[i], &v[a]);
            if (a < b) troca(&v[i], &v[b]);
            a++; b++; i++;
        } else if (v[i] < p2) {
            troca(&v[i], &v[b]);
            b++; i++;
        } else if (v[i] <= p3) {
            i++;
        } else {
            troca(&v[i], &v[c]);
            c--;
        }
    }

    // 4. Chamadas recursivas
    quicksort3p(v, esq, a - 1);     // < p1
    quicksort3p(v, a, b - 1);       // [p1, p2)
    quicksort3p(v, b, c);           // [p2, p3]
    quicksort3p(v, c + 1, dir);     // > p3
}

void imprime(int v[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

int main() {
    int v[] = {42, 87, 12, 95, 3, 61, 78, 20, 55, 1, 33, 99, 6, 70, 29, 8, 48, 83, 15, 66};

    int n = sizeof(v) / sizeof(v[0]);

    printf("Vetor original:\n");
    imprime(v, n);

    quicksort3p(v, 0, n - 1);

    printf("Vetor ordenado:\n");
    imprime(v, n);

    return 0;
}
