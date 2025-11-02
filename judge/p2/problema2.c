#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

long* recebeCanteiros(int size){
    long *lst = (long*) malloc(size * sizeof(long));
    for (int i = 0; i < size; i++) 
        scanf("%ld", &lst[i]);

    return lst;
}

void recalcMinBloco(long *lst, long *mins, int block_id, int block_size, int size){
    int start = block_id * block_size;
    int end = (block_id + 1) * block_size;
    
    // Garante que não ultrapassemos o fim do vetor lst
    if (end > size) end = size;
    
    long min_val = LONG_MAX; // começa com o maior valor possível
    for (int k = start; k < end; k++)
        min_val = MIN(min_val, lst[k]); 
    
    // Armazena o mínimo encontrado para este bloco
    if (start < size) // evita acessar mins[-1] se size for 0
        mins[block_id] = min_val;
}

void empurraLazy(long *lst, long *lazy, int block_id, int block_size, int size){
    // Se não há valor preguiçoso, não faz nada
    if (lazy[block_id] == 0) return;
    
    int start = block_id * block_size;
    int end = (block_id + 1) * block_size;
    if (end > size) end = size;
    
    // Aplica o valor preguiçoso a todos os elementos reais do bloco
    for (int k = start; k < end; k++)
        lst[k] += lazy[block_id];
    
    // Limpa o valor preguiçoso
    lazy[block_id] = 0;
}

void atualizaReforco(long *lst, long *mins, long *lazy, int L, int R, long val, int block_size, int size){
    // Garante que o intervalo é válido
    if (L > R || L >= size || R < 0) return;
    // Ajusta R para os limites do vetor
    if (R >= size) R = size - 1;
    
    int bloco_L = L / block_size;
    int bloco_R = R / block_size;
    
    // Empurra o valor preguiçoso dos blocos de borda ANTES de modificá-los
    empurraLazy(lst, lazy, bloco_L, block_size, size);
    if (bloco_L != bloco_R)
        empurraLazy(lst, lazy, bloco_R, block_size, size);
    
    // Caso 1: A atualização inteira ocorre dentro de um único bloco
    if (bloco_L == bloco_R) {
        for (int k = L; k <= R; k++)
            lst[k] += val;
        
        recalcMinBloco(lst, mins, bloco_L, block_size, size);
    } 
    // Caso 2: A atualização abrange múltiplos blocos
    else {
        // Borda Esquerda (do ponto L até o fim do bloco_L)
        for (int k = L; k < (bloco_L + 1) * block_size; k++) 
            lst[k] += val;
        
        recalcMinBloco(lst, mins, bloco_L, block_size, size);
        
        // Blocos do Meio (apenas marca como 'lazy')
        for (int j = bloco_L + 1; j < bloco_R; j++)
            lazy[j] += val;
        
        // Borda Direita (do início do bloco_R até o ponto R)
        for (int k = bloco_R * block_size; k <= R; k++)
            lst[k] += val;
        
        recalcMinBloco(lst, mins, bloco_R, block_size, size);
    }
}

long minimoGlobal(long *mins, long *lazy, int num_blocks) {
    long min_global = LONG_MAX;
    for (int j = 0; j < num_blocks; j++)
        min_global = MIN(min_global, mins[j] + lazy[j]);

    return min_global;
}

int encontraMinimo(long *lst, int size, int delta, int block_size){
    int num_blocks = (size + block_size - 1) / block_size;
    long *mins = (long*) malloc(num_blocks * sizeof(long));
    long *lazy = (long*) calloc(num_blocks, sizeof(long));
    long min_global = LONG_MIN;
    
    // Preenchemos o vetor de minimos com os mínimos parciais
    for (int i = 0; i < num_blocks; i++)
        recalcMinBloco(lst, mins, i, block_size, size);

    atualizaReforco(lst, mins, lazy, 0, 0, delta, block_size, size);

    for (int i = 0; i < size; i++) {
        
        long current_min = minimoGlobal(mins, lazy, num_blocks);
        
        min_global = MAX(min_global, current_min);

        // Já consultamos o último estado (A_N-1), não precisa calcular o próximo
        if (i == size - 1) break;

        int L_sub = (i + 1) - delta; // (n-K)
        if (L_sub < 0) L_sub = 0;
        int R_sub = i; // (n-1)
        int P_add = i + 1; // (n)

        atualizaReforco(lst, mins, lazy, L_sub, R_sub, -1, block_size, size);
        atualizaReforco(lst, mins, lazy, P_add, P_add, delta, block_size, size);
    }

    free(mins);
    free(lazy);

    return min_global;
}

int main(){
    // PRIMEIRA ENTRADA: Recebe N e K
    int qt_canteiros;
    long reforco;
    scanf("%d %ld", &qt_canteiros, &reforco);

    // SEGUNDA ENTRADA: Recebe a lista de canteiros
    long *lista_canteiros = recebeCanteiros(qt_canteiros);
    int block_size = sqrt(qt_canteiros); // tamanho dos blocos que vamos dividir o vetor

    // SAIDA
    int minimo = encontraMinimo(lista_canteiros, qt_canteiros, reforco, block_size);
    printf("%d", minimo);

    free(lista_canteiros);
    return 0;
}