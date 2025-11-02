# Projeto por indução

### **Hipótese**

Assumindo $A$ como o vetor de irrigadores, temos

$$
A = (x_1, x_2, x_3,..., x_N).
$$

Se $A_k$ é o vetor de irrigadores reforçados a partir do irrigador $K$, logo

$$
A_k = (..., x_{k-2}+K-2, x_{k-1}+K-1, x_k+K, ...),
$$

generalizado por

$$
A_k = A + R_k,
$$

em que $R_n$ é um vetor de reforço, cujo termo geral $r_i$ é dado por

$$
r_i =
\begin{cases}
K - (n - i), \ \hbox{se } n - K + 1 \le i \le n \\
0, \ \hbox{caso contrário}
\end{cases}
$$

Se conhecermos previamente $A_{n-1}$, podemos afirmar que 

$$
A_n = A_{n-1} + T_n,
$$

em que $T$ é uma vetor de transformação que descreve o avanço do reforço para um irrigador à frente, cujo termo geral $t_i$ é dado por

$$
t_i =  
\begin{cases}
K, \ \hbox{se } i = n \\
-1, \ \hbox{se } n - K \le i \le n - 1 \\
0 \ \hbox{caso contrário}
\end{cases}
$$

### **Caso base**

Para um $N$ qualquer, tomando $A_1$, temos que

$$
A_1 = A_0 + T_1
$$

Por definição, temos $A_0 = A$, pois $R_0 = 0$. Assim,

$$
A_1 = A + T_1 = A + (K, 0, ...) = (x_1 + K, x_2, ..., x_N).
$$

Portanto, $A_1 = (x_1 + K, x_2, ..., x_N)$, conforme definimos.

### **Passo indutivo**

Dado $A_n$, queremos provar que $A_n \Longrightarrow A_{n+1}$, ou seja

$$
A_{n-1} + T_n \Longrightarrow A_n + T_{n+1}.
$$

Desenvolvendo, temos

$$
A_{n+1}  = A_n + T_{n+1} \\
A_{n+1} = (A + R_n) + T_{n+1} \\
A_{n+1} = A + (R_n + T_{n+1}).
$$

Se pudermos encontrar $A_{n+1} = A + R_{n+1}$, significa que $A_{n+1}  = A_n + T_{n+1}$ está de acordo com nossa definição inicial.

Expandindo $R_n + T_{n+1}$,

$$
S = R_n + T_{n+1} = (..., x_{n-2} + K - 2, x_{n-1} + K - 1, x_n + K, ...) + (..., -1, K, ...).
$$

Primeira análise: $x_{n+1}$. Para esse termo, temos

$$
\begin{split}
T_{n+1}&: x_{n+1} = K \\
R_n&: x_{n+1} = 0
\end{split}
$$

Logo, $s_{n+1} = K$.

Segunda análise: $[x_{n-K+1}, x_n]$. Para esse intervalo, vamos avaliar os dois termos das extremidades e inferir o padrão para os termos intermediários.

- $x_{n-K+1}$:

$$
\begin{split}
T_{n+1}&: x_{n-K+1} = -1 \\
R_n&: x_{n-K+1} = K - [n - (n - K + 1)] = 1
\end{split}
$$

- $x_n$:

$$
\begin{split}
T_{n+1}&: x_n = -1 \\
R_n&: x_n = K - (n - n) = K
\end{split}
$$

Assim, o intervalo $[s_{n-K+1}, s_n] = [0, K - 1] = (0, 1, 2, ..., K - 1)$. Portanto, partindo de $[s_1, s_N]$, temos

$$
S = (..., 0, 1, 2, ..., K - 1, K, ...)
$$

Se observarmos a definição de $R_{n+1}$, percebemos que

$$
r_i =
\begin{cases}
K - (n + 1 - i), \ \hbox{se } n - K + 2 \le i \le n + 1 \\
0, \ \hbox{caso contrário}
\end{cases}
$$

Comparando com o vetor $S$, verificamos que $s_{n - K + 2} = 1, \ s_{n - K + 3} = 2, \ ..., \ s_{n+1} = K$, que descreve exatamente a definição de $R_{n+1}$. Portanto, $S = R_n + T_{n+1} = R_{n+1}$.

Retornando à equação inicial,

$$
A_{n+1} = A + (R_n + T_{n+1}) \\
A_{n+1} = A + R_{n + 1}, \ \hbox{que é verdadeiro conforme nossa definição.}
$$

Portanto, $A_n \Longrightarrow A_{n+1}$, logo provamos o passo indutivo.

## Conclusão

Considerando que podemos calcular $min(A_n)$ a partir de $A_{n-1}$, podemos concluir que essa verificação ocorre em tempo linear. Portanto, a complexidade do algoritmo é, pelo menos, $O(n)$, sendo necessário uma avaliação da transformação $T_n$ para inferir a complexidade final.