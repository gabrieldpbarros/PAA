#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Função auxiliar
/*
void printM(int **m, int size1, int size2){
    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++)
        printf("%d ", m[i][j]);
        printf("\n");
    }
}

void printL(int *l, int size){
    for (int i = 0; i < size; i++) printf("%d ", l[i]);
    printf("\n");
}
*/

int* recebeListaNotas(int tam_acorde){
    int *lst = (int*) malloc(tam_acorde * sizeof(int));
    for (int i = 0; i < tam_acorde; i++)
        scanf("%d", &lst[i]);

    return lst;
}

void copiaLista(int *l1, int *l2, int size){
    for (int i = 0; i < size; i++) l1[i] = l2[i];
}

int binSearch(int *lst, int x, int start, int end){ // Supomos previamente que é garantido que o valor está no vetor
    if (start <= end) {
        int idx = start + (end - start) / 2;

        if (lst[idx] == x) return idx;
        if (lst[idx] > x) return binSearch(lst, x, start, idx - 1);
        else return binSearch(lst, x, idx + 1, end);
    }
}

int linSearch(int *lst, int x, int size){
    for (int i = 0; i < size; i++)
        if (lst[i] >= x) return i;
}

// Saída
void pExit(int resto){
    if (resto) printf("N\n");
    else printf("S\n");
}

void testaPossibilidades(int **blocos, int *lista_notas, int tam_escala, int tam_acorde, int tam_blocos, int nota_dist_min){
    int it = tam_blocos,
        falhas = 0;

    // Repete um número de vezes igual ao total de possibilidades de divisão
    while (it--) {
        // printf("%d\n\n", it); // DEBUG
        int *t_lst = (int*) malloc(tam_acorde * sizeof(int)); // Criamos uma lista que vai ser alterada a fim de acompanhar quais notas já foram inseridas em um bloco
        int k = 0, // Referencial da nota (geral) que vai ser inserida no bloco
            lst_index; // Referencial da nota que vai ser analisada na lista de notas do acorde

        copiaLista(t_lst, lista_notas, tam_acorde);
        // printf("\nlista de notas do acorde = "); // DEBUG
        // printL(t_lst, tam_acorde); // DEBUG
        printf("\n\n-- %d --\n", nota_dist_min); // DEBUG
        
        // ESPECÍFICO DA PRIMEIRA ITERAÇÃO
        if (falhas == 0) lst_index = binSearch(t_lst, nota_dist_min, 0, tam_acorde);
        // RESTANTE DAS ITERAÇÕES
        else lst_index = linSearch(t_lst, nota_dist_min, tam_acorde);

        // For-loop principal            
        for (int i = 0; i < tam_acorde; i++) { // Máximo de divisões que devem ser feitas
            int retiradas = 0;
            for (int j = 0; j < tam_blocos; j++) { // Quantidade de notas que cabem em uma divisão
                // printf("LISTA NA ITERACAO i: %d, j: %d - ", i, j); // DEBUG
                // printL(t_lst, tam_acorde); // DEBUG
                int nota;
                // Correção caso cheguemos ao fim da escala
                if (nota_dist_min + k >= tam_escala) nota = (nota_dist_min + k) - tam_escala; 
                else nota = nota_dist_min + k;
                
                blocos[i][j] = nota;
                k++; // Vai para a próxima nota

                printf("NOTA ATUAL: %d\n", t_lst[lst_index]); // DEBUG
                if (blocos[i][j] == t_lst[lst_index]) {
                    retiradas++;
                    if (retiradas > 1) break; // Sai do loop antes de atualizar o índice

                    lst_index++; // "Retiramos" a nota da lista de notas temporária e atualizamos o índice
                    if (lst_index == tam_acorde) lst_index = 0;
                }
            }

            // printf("\nRETIRADAS NA %d ITERACAO = %d\n", i, retiradas); // DEBUG
            // Ou duas notas estão no mesmo bloco, ou nenhuma nota foi inserida
            if (retiradas == 0 || retiradas > 1) {
                falhas++;
                break;
            }
        }
        // Atualiza o valor inicial
        if (nota_dist_min + 1 >= tam_escala) nota_dist_min = 0;
        else nota_dist_min++;
        printf("\nfalhas = %d\n", falhas); // DEBUG
        printM(blocos, tam_acorde, tam_blocos); // DEBUG
        if (falhas == 0) break;

        free(t_lst);
    }
    // printf("falhas = %d\n", falhas); // DEBUG
    pExit(falhas == tam_blocos);
}

// Retorna se é possível fazer a divisão ou não
void divide(int tam_escala, int tam_acorde, int *lista_notas, int nota_dist_min){
    // Possibilidade de divisão
    int tam_blocos = tam_escala / tam_acorde;
    int **divisoes = (int**) malloc(tam_acorde * sizeof(int*));
    for (int i = 0; i < tam_acorde; i++)
        divisoes[i] = (int*) malloc(tam_blocos * sizeof(int));

    /*bool final = */testaPossibilidades(divisoes, lista_notas, tam_escala, tam_acorde, tam_blocos, nota_dist_min);

    //return final;
}

// Saída
// void pExit(bool resto){
//     if (resto == false) printf("N\n");
//     printf("S\n");
// }

int main(){
    // Primeira linha de entrada
    int tam_escala, tam_acorde;
    scanf("%d %d", &tam_escala, &tam_acorde);

    // Segunda linha de entrada
    int *lista_notas = recebeListaNotas(tam_acorde);

    int min;
    scanf("%d", &min);
    divide(tam_escala, tam_acorde, lista_notas, min);

    free(lista_notas);

    return 0;
}