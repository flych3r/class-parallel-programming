# Correção: Parcial. Observações abaixo. 2,3 Pontos.
# Computação Paralela - Atividade 02

## Matheus Xavier Sampaio – 397710

### Questão 01

Suponha que duas matrizes 100 x 100 são multiplicadas usando 8 threads. Quantos produtos escalares, isto é, operações realizadas pelo laço for mais interno, cada thread deve calcular se as seguintes abordagens para paralelizar os dois laços for mais externos forem utilizadas:

    1. Apenas o laço for mais externo é paralelizado:

Apenas o laço externo é paralelizado, sendo assim, o numero de iterações a serem distribuídas é 100 (100 / 8, 100 % 8 = 12, 4).
Logo cada thread executa 1200 operações, restando assim 400 operações, sendo essas distribuídas entre 4 threads, 100 para cada .
*Correção: você só precisa ressaltar que ainda existem as iterações do laço mais interno.*

    2. A diretiva collapse(2) é aplicada ao laço mais externo:

Como ambos os laços são paralelizados, o numero de iterações a serem distribuídas é 100 * 100 (10000 / 8 = 1250).
Logo cada thread executa 1250 operações.
*Correção: você só precisa ressaltar que ainda existem as iterações do laço mais interno.*

    3. Ativando regiões paralelas aninhadas e OMP_NUM_THREADS=2,4.

Considerando que o primeiro laço é dividido em 2 threads (100 / 2 = 50), e destes, o segundo laço é dividido em 4 threads (100 / 4 = 25), esta divisão resulta em 25 * 50 operações (1250) para cada thread. O que muda é em relação ao collapse(2) é qual operação cada thread  realiza.

*Correção: você só precisa ressaltar que ainda existem as iterações do laço mais interno.*

### Questão 02

Suponha que o programa está em execução com 4 threads e o valor fornecido para max foi 20. Determine qual iteração será executada por qual thread se as seguintes diretivas forem utilizadas como estratégias de escalonamento.

    1. static,1, static,2 e static,3

```txt
static,1
==================================
thread, iterações
0     , [1, 5, 9, 13, 17]
1     , [2, 6, 10, 14, 18]
2     , [3, 7, 11, 15, 19]
3     , [4, 8, 12, 16, 20]
==================================
```
```txt
static,2
==================================
thread, iterações
0     , [1, 2, 9, 10, 17, 18]
1     , [3, 4, 11, 12, 19, 20]
2     , [5, 6, 13, 14]
3     , [7, 8, 15, 16]]
==================================
```
```txt
static,3
==================================
thread, iterações
0     , [1, 2, 3, 13, 14, 15]
1     , [4, 5, 6, 16, 17, 18]
2     , [7, 8, 9, 19, 20]
3     , [10, 11, 12]
==================================
```

    2. dynamic,1, dynamic,2 e dynamic,3

```txt
dynamic,1
==================================
thread, iterações
0     , [1, 6, 9, 13, 17]
1     , [4, 5, 7, 10, 14, 18]
2     , [3, 11, 15, 19]
3     , [2, 8, 12, 16, 20]
==================================
```
```txt
dynamic,2
==================================
thread, iterações
0     , [5, 6, 11, 12, 15, 16]
1     , [3, 4, 17, 18]
2     , [1, 2, 19, 20]
3     , [7, 8, 9, 10, 13, 14]
==================================
```
```txt
dynamic,3
==================================
thread, iterações
0     , [1, 2, 3]
1     , [7, 8, 9, 16, 17, 18]
2     , [4, 5, 6, 19, 20]
3     , [10, 11, 12, 13, 14, 15]
==================================
```
