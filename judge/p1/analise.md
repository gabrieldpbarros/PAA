# Código completo

```C
typedef struct {
    int nota; // Armazena a nota com menor distância em relação à anterior
    int dist; // Armazena a distância entre as duas notas
} notaDistancia;

void calculaDist(notaDistancia *menor_nota, int* lst, int index, int size, bool last){
    int distancia;

    // Encontra a distância e insere na matriz
    if (!last) {
        distancia = lst[index] - lst[index - 1];
        if (distancia < menor_nota->dist) {
            menor_nota->dist = distancia;
            menor_nota->nota = lst[index];
        }
    } else {
        distancia = size - (lst[index] - lst[0]); // calcula a maior distância entre a primeira e última nota
        if (distancia < menor_nota->dist) {
            menor_nota->dist = distancia;
            menor_nota->nota = lst[0];
        }
    } 
}

int* recebeListaNotas(int tam_acorde, int tam_escala, notaDistancia *menor_nota){
    int *lst = (int*) malloc(tam_acorde * sizeof(int));
    for (int i = 0; i < tam_acorde; i++) {
        scanf("%d", &lst[i]);

        // Insere as distâncias entre cada nota na matriz de distâncias
        if (i) calculaDist(menor_nota, lst, i, tam_escala, false);
    }
    // Insere a distância entre a primeira e a última nota
    calculaDist(menor_nota, lst, tam_acorde - 1, tam_escala, true);

    return lst;
}

void freeM(int **m, int size){
    for (int i = 0; i < size; i++)
        free(m[i]);
    free(m);
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

// Funções principais
int testaPossibilidades(int **blocos, int *lista_notas, int tam_escala, int tam_acorde, int tam_blocos, int nota_dist_min){
    int it = tam_blocos,
        falhas = 0;

    // Repete um número de vezes igual ao total de possibilidades de divisão
    while (it--) {
        // printf("%d\n\n", it); // DEBUG
        int *t_lst = (int*) malloc(tam_acorde * sizeof(int)); // Criamos uma lista que vai ser alterada a fim de acompanhar quais notas já foram inseridas em um bloco
        int k = 0, // Referencial da nota (geral) que vai ser inserida no bloco
            lst_index; // Referencial da nota que vai ser analisada na lista de notas do acorde

        copiaLista(t_lst, lista_notas, tam_acorde);
        // printf("\n\n-- %d --\n", nota_dist_min); // DEBUG
        
        if (falhas == 0) lst_index = binSearch(t_lst, nota_dist_min, 0, tam_acorde);
        else lst_index = linSearch(t_lst, nota_dist_min, tam_acorde);

        // For-loop principal            
        for (int i = 0; i < tam_acorde; i++) { // Máximo de divisões que devem ser feitas
            int retiradas = 0;
            for (int j = 0; j < tam_blocos; j++) { // Quantidade de notas que cabem em uma divisão
                int nota;
                // Correção caso cheguemos ao fim da escala
                if (nota_dist_min + k >= tam_escala) nota = (nota_dist_min + k) - tam_escala; 
                else nota = nota_dist_min + k;
                
                blocos[i][j] = nota;
                k++; // Vai para a próxima nota

                if (blocos[i][j] == t_lst[lst_index]) {
                    retiradas++;
                    if (retiradas > 1) break; // Sai do loop antes de atualizar o índice

                    lst_index++; // "Retiramos" a nota da lista de notas temporária e atualizamos o índice
                    if (lst_index == tam_acorde) lst_index = 0;
                }
            }

            // Ou duas notas estão no mesmo bloco, ou nenhuma nota foi inserida
            if (retiradas == 0 || retiradas > 1) {
                falhas++;
                break;
            }
        }
        // Atualiza o valor inicial
        if (nota_dist_min + 1 >= tam_escala) nota_dist_min = 0;
        else nota_dist_min++;
        if (falhas == 0) break;

        free(t_lst);
    }
    return falhas == tam_blocos;
}

// Retorna se é possível fazer a divisão ou não
bool divide(int tam_escala, int tam_acorde, int *lista_notas, int nota_dist_min){
    // Possibilidade de divisão
    int tam_blocos = tam_escala / tam_acorde;
    int **divisoes = (int**) malloc(tam_acorde * sizeof(int*));
    for (int i = 0; i < tam_acorde; i++)
        divisoes[i] = (int*) malloc(tam_blocos * sizeof(int));

    int final = testaPossibilidades(divisoes, lista_notas, tam_escala, tam_acorde, tam_blocos, nota_dist_min);
    freeM(divisoes, tam_acorde);
    return final == 1 ? false : true;
}

// Saída
void pExit(bool possivel){
    if (!possivel) printf("N\n");
    else printf("S\n");
}

int main(){
    int tam_escala, tam_acorde;
    scanf("%d %d", &tam_escala, &tam_acorde);

    notaDistancia menor_nota;
    menor_nota.dist = tam_escala;
    // Segunda linha de entrada (recebe a entrada e preenche a matriz de distâncias)
    int *lista_notas = recebeListaNotas(tam_acorde, tam_escala, &menor_nota);

    bool resultado = divide(tam_escala, tam_acorde, lista_notas, menor_nota.nota);
    pExit(resultado);

    free(lista_notas);

    return 0;
}
```

# Análise das funções individuais

### **calculaDist**

```C
int distancia;

if (!last) {                                <- # = 1
    distancia=lst[index]-lst[index-1];      <- # = 1
    if (distancia < menor_nota->dist) {     <- # = 1
        menor_nota->dist=distancia;         <- # = 1
        menor_nota->nota=lst[index];        <- # = 1
    }
} else {                                    <- # = 1
    distancia=size-(lst[index]-lst[0]);     <- # = 1 
    if (distancia < menor_nota->dist) {     <- # = 1 
        menor_nota->dist=distancia;         <- # = 1
        menor_nota->nota=lst[0];            <- # = 1
    }
} 
```
Custo máximo = 7 (if não é cumprido, logo entra no else) $\Longrightarrow C(n) = O(1)$

### **recebeListaNotas**

```C
int *lst=malloc(tam_acorde*sizeof(int));    <- # = 1
for (int i=0; i < tam_acorde; i++) {        <- # = i
    scanf("%d", &lst[i]);                  <- # = i * 1

    if (i) calculaDist(menor_nota, lst, i, tam_escala, false);                      <- # = i * Custo da função
}
calculaDist(menor_nota, lst, tam_acorde - 1, tam_escala, true);                          <- # = 7

return lst;                                 <- # = 1
```
Encontrando o custo do laço, temos

$$
C(n) = \sum_{i=0}^{n-1} 1 + 7 = 8(n - 1) = 8n - 8.
$$

Somando a quantidade de repetições das outras linhas do algoritmo,

$$
C(n) = 1 + (8n - 1) + 7 + 1 = 8n + 1.
$$

Como o resto da função tem custo constante, logo $C(n) = O(n)$.

### **freeM**

```C
for (int i=0; i < size; i++)               <- # = i
    free(m[i]);                            <- # = i * 1
free(m);                                   <- # = 1
```

De forma semelhante à função anterior, temos

$$
C(n) = \sum_{i=0}^{n-1} 1 = n - 1,
$$

que nos dá o custo $C(n) = O(n)$.

### **copiaLista**

```C
for (int i=0; i < size; i++)               <- # = i
    l1[i] = l2[i];                         <- # = i * 1
```

Temos

$$
C(n) = \sum_{i=0}^{n-1} 1 = n - 1.
$$

Logo, $C(n) = O(n)$.

### **binSearch**

```C
if (start<=end) {
    int idx=start+(end-start)/2;

    if (lst[idx]==x) return idx;
    if (lst[idx] > x) return binSearch(lst, x, start, idx-1);
    else return binSearch(lst, x, idx+1, end);
}
```

Sabemos que o custo da busca binária é $O(log n)$

### **linSearch**

```C
for (int i=0; i < size; i++)
    if (lst[i]>=x) return i;
```

Sabemos que o custo da busca linear é $O(n)$

## **testaPossibilidades**
```C
int it=tam_blocos,
    falhas=0;

// Repete um número de vezes igual ao total de possibilidades de divisão
while (it--) {
    int *t_lst = malloc(tam_acorde * sizeof(int)); // Criamos uma lista que vai ser alterada a fim de acompanhar quais notas já foram inseridas em um bloco
    int k=0,
        lst_index;

    copiaLista(t_lst, lista_notas, tam_acorde);
        
    if (falhas==0) lst_index = binSearch(t_lst, nota_dist_min, 0, tam_acorde);
    else lst_index=linSearch(t_lst, nota_dist_min, tam_acorde);
           
    for (int i=0; i < tam_acorde; i++) { // Máximo de divisões que devem ser feitas
        int retiradas=0;
        for (int j=0; j < tam_blocos; j++) { // Quantidade de notas que cabem em uma divisão
            int nota;
            // Correção caso cheguemos ao fim da escala
            if (nota_dist_min+k>=tam_escala) nota=(nota_dist_min+k)-tam_escala; 
            else nota=nota_dist_min+k;
                
            blocos[i][j]=nota;
            k++; // Vai para a próxima nota

            if (blocos[i][j]==t_lst[lst_index]) {
                retiradas++;
                if (retiradas > 1) break; // Sai do loop antes de atualizar o índice

                lst_index++; 
                if (lst_index==tam_acorde) lst_index=0;
                }
            }


        if (retiradas==0 || retiradas > 1) {
            falhas++;
            break;
        }
    }
    if (nota_dist_min+1 >= tam_escala) nota_dist_min=0;
    else nota_dist_min++;
    if (falhas==0) break;

    free(t_lst);
}
return falhas==tam_blocos;
```