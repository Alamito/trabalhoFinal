#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABELA_ALIMENTOS "1000Shuffled.csv"
//#define TABELA_NUTRI "day1.csv"

//----------------- VARIAVEIS GLOBAIS ---------------------//
int rotacoesDir = 0;
int rotacoesEsq = 0;
int rotacoesDirEsq = 0;
int rotacoesEsqDir = 0;
int totalCalorias = 0;
int comp = 1;           // inicia com 1 por causa do while que na comparacao final nao soma 1 na variavel
//---------------------------------------------------------//

// estrutura do alimento que sera inserido na arvore
typedef struct {
  char nome[50];
  int caloria;
} Alimento;

// estrutura da arvore AVL
typedef struct no { 
  Alimento alimento;
  struct no *dir;
  struct no *esq;
  int altura;
} noAVL;

// funcao que cria novo no
noAVL *novoNo (Alimento a) {
    noAVL *novo = malloc(sizeof(noAVL));

    if (novo) {
      novo->alimento = a;
      novo->esq = NULL;
      novo->dir = NULL;
      novo->altura = 0;
    } else {
      printf("\nERRO AO ALOCAR O NOVO NO\n");
    }
    return novo;
}

// retorna o maior no
int maiorNo(int no1, int no2) {
    return (no1 > no2)? no1: no2; // operador ternario
}

// calcula a altura do no (necessario para tomar o FB do no)
int alturaNo (noAVL *no) {
    if (no == NULL) {
      return -1;
    } else {
      return no->altura;
    }
}

// calcula altura da arvore
int alturaAVL(noAVL *raiz) {
  if (raiz == NULL) {
    return -1;
  } else {
    int esq = alturaAVL(raiz->esq);
    int dir = alturaAVL(raiz->dir);
    if (esq > dir)
      return esq + 1;
    else
      return dir + 1;
  }
}

// calcula a quantidade de nos
int quantidade_nos(noAVL *raiz) {
  if (raiz == NULL)
    return 0;
  else
    return 1 + quantidade_nos(raiz->esq) + quantidade_nos(raiz->dir);
}

// calcula a quantidade de nos folhas
int quantidade_folhas(noAVL *raiz) {
  if (raiz == NULL)
    return 0;
  else if (raiz->esq == NULL && raiz->dir == NULL)
    return 1;
  else
    return quantidade_folhas(raiz->esq) + quantidade_folhas(raiz->dir);
}

// fator de balanceamento do no
int FB (noAVL *no) {
    if (no) {
      return (alturaNo(no->esq) - alturaNo(no->dir));
    } else {
      return 0;
    }
}

// rotacao para esquerda do no
noAVL *rotacaoEsq (noAVL *no, short flag) {
  noAVL *noProx, *noNull;
  if (flag == 0) {
    rotacoesEsq++;
  }

  noProx = no->dir;
  noNull = noProx->esq;

  noProx->esq = no;
  no->dir = noNull;

  no->altura = maiorNo(alturaNo(no->esq), alturaNo(no->dir)) + 1;
  noProx->altura = maiorNo(alturaNo(noProx->esq), alturaNo(noProx->dir)) + 1;

  return noProx;
}

// rotacao para direita do no
noAVL *rotacaoDir (noAVL *no, short flag) {
  noAVL *noProx, *noNull;
  if (flag == 0) {
    rotacoesDir++;
  }

  noProx = no->esq;
  noNull = noProx->dir;

  noProx->dir = no;
  no->esq = noNull;

  no->altura = maiorNo(alturaNo(no->esq), alturaNo(no->dir)) + 1;
  noProx->altura = maiorNo(alturaNo(noProx->esq), alturaNo(noProx->dir)) + 1;

  return noProx;
}

// rotacao dupla - direita esquerda
noAVL *rotacaoDirEsq (noAVL *no) {
  rotacoesDirEsq++;
  no->dir = rotacaoDir(no->dir, 1);
  return rotacaoEsq(no, 1);
}

// rotacao dupla - esquerda direita
noAVL *rotacaoEsqDir (noAVL *no) {
  rotacoesEsqDir++;
  no->esq = rotacaoEsq(no->esq, 1);
  return rotacaoDir(no, 1);
}

// chama as rotacoes para efetuar o balanceamento do no
noAVL *balanceamento(noAVL *raiz) {
    int fatorBal = FB(raiz);

    if (fatorBal < -1 && FB(raiz->dir) <= 0) { 
      raiz = rotacaoEsq(raiz, 0);
    } else if (fatorBal > 1 && FB(raiz->esq) >= 0) { 
      raiz = rotacaoDir(raiz, 0);
    } else if (fatorBal > 1 && FB(raiz->esq) < 0) {
      raiz = rotacaoEsqDir(raiz);
    } else if (fatorBal < -1 && FB(raiz->dir) > 0) {
      raiz = rotacaoDirEsq(raiz);
    }

    return raiz;
}

// insere de acordo com o alfabeto em ordem crescente (A - Z)
noAVL *inserir(noAVL *raiz, Alimento a) {
    if (raiz == NULL) {
      return novoNo(a);
    } else {
      if (strcmp(a.nome, raiz->alimento.nome) < 0) { // se o novo alimento comecar com uma letra menor... insere a esquerda
        raiz->esq = inserir(raiz->esq, a);
      } else if (strcmp(a.nome, raiz->alimento.nome) > 0) { // se o novo alimento comecar com uma letra maior... insere a direita
        raiz->dir = inserir(raiz->dir, a);
      }
    }

    // recalcula a altura de todos os nos entre a raiz e o no inserido
    raiz->altura = maiorNo(alturaNo(raiz->esq), alturaNo(raiz->dir)) + 1;

    // faz o balanceamento da arvore
    raiz = balanceamento(raiz);
    
    return raiz;
}

// imprime a arvore indicando o nivel
void Desenha(noAVL *a, int nivel) { 
  int x;
  if (a != NULL) {
    for (x = 1; x <= nivel; x++) {
      printf("=");
    }
    printf("%s\n", a->alimento.nome);
    printf("%d\n", a->alimento.caloria);
    if (a->esq != NULL)
      Desenha(a->esq, (nivel + 1));
    if (a->dir != NULL)
      Desenha(a->dir, (nivel + 1));
  }
}

// leitura do alimento colocando na struct
Alimento ler_alimento(char *nomeAlimento, int calAlimento) {
  Alimento alimento;
  strcpy(alimento.nome, nomeAlimento);
  alimento.caloria = calAlimento;
  return alimento;
}

// realiza a leitura do arquivo com os alimentos e quantidade de calorias
noAVL *leituraArqCalorias(noAVL *raiz, char nomeArquivo[]) {
  char *texto[50], nomeAlimento[50], *arq;
  int iteracao = 0, caloria;
  FILE *open_arq;                                       // ponteiro para file

  open_arq = fopen(nomeArquivo, "r"); // abre o arquivo

  while (EOF != fscanf(open_arq, "%[^\n]\n", texto)) {  // enquanto nao chegar no final do arquivo faz a leitura ate a quebra de texto
    arq = strtok(texto, ";");                           // divide a string pelo delimitador ";" 
    while (arq != NULL) {                               // quando iteracao = 0 le o nome do alimento... iteracao = 1 le a caloria do alimento
      if (iteracao == 0) {                              // leitura do nome do alimento
        strcpy(nomeAlimento, arq);                      // copia o nome do alimento do arquivo para a variavel nomeAlimento   
        iteracao = 1;
      } else {                                          // leitura da caloria
        caloria = atoi(arq);                            // transforma em inteiro a quantidade de caloria do alimento
        raiz = inserir(raiz, ler_alimento(nomeAlimento, caloria));  // insere na arvore
        iteracao = 0;
      }
      arq = strtok(NULL, ";");                           // necessario para obter os proximos tokens do arquivo 
    }
  }
  fclose(open_arq);
  return raiz;
}

// leitura do arquivo com o alimento e a quantidade de gramas ingeridas
void leituraArqNutri(noAVL *raiz, char nomeArqIngerido[], char nomeSaida[], char nomeArquivo[]) {
  char *texto[50], nomeAlimento[50], *arq;
  short iteracao = 0;
  int porcao;
  FILE *open_arq, *arq_exit;                                // ponteiro para file

  open_arq = fopen(nomeArqIngerido, "r");                   // abre o arquivo
  arq_exit = fopen(nomeSaida, "w");

  informacoesTXT(raiz, arq_exit, 1, nomeArqIngerido, nomeArquivo);                        // chama funcao com parametro 1 para escrever o texto do titulo do arquivo

  while (EOF != fscanf(open_arq, "%[^\n]\n", texto)) {      // enquanto nao chegar no final do arquivo faz a leitura ate a quebra de texto
    arq = strtok(texto, ";");                               // divide a string pelo delimitador ";"
    while (arq != NULL) {                                   // quando iteracao = 0 le o nome do alimento... iteracao = 1 le a caloria do alimento
      if (iteracao == 0) {                                  // leitura do nome do alimento;
        strcpy(nomeAlimento, arq);                          // copia o nome do alimento do arquivo para a variavel nomeAlimento
        iteracao = 1;
      } else {                                              // leitura da porcao ingerida
        porcao = atoi(arq);                                 // transforma em inteiro                
        iteracao = 0;
        if (!(procuraAVL(raiz, nomeAlimento, porcao, arq_exit))) { // procura na ABP o nome do alimento... se achar retorna true... se nao achar retorna false
          printf("Alimento %s nao encontrado!\n", nomeAlimento);
        }
      }
      arq = strtok(NULL, ";");                              // necessario para obter os proximos tokens do arquivo       
    }
  }
  informacoesTXT(raiz, arq_exit, 2, nomeArqIngerido, nomeArquivo);                        // funcao com parametro 2 para escrever as estatisticas no arquivo de saida
  fclose(open_arq);
  fclose(arq_exit);
}

// procura na arvore o alimento encontrado na funcao leituraArqNutri
int procuraAVL(noAVL *raiz, char *nomeAlimento, int porcao, FILE *arq_exit) {
  while (raiz != NULL) {
    comp++;
    if (strcmp(nomeAlimento, raiz->alimento.nome) ==  0) {        // se o nome do alimento for igual ao nome do alimento do nodo
      exitAlimentoCaloria(nomeAlimento, raiz->alimento.caloria, porcao,  arq_exit); // escreve no arquivo de saida a estatistica do alimento encontrado
      return 1;                                                   // se achou vai retornar verdadeiro
      comp++;
    } else if (strcmp(nomeAlimento, raiz->alimento.nome) > 0) {   // se a letra do nome do alimento for maior q a letra do alimento do nodo vai para a direita
      raiz = raiz->dir; 
      comp++;
    } else {                                                      // se a letra do nome do alimento for menor q a letra do alimento do nodo vai para a esquerda
      raiz = raiz->esq;
    }
  }
  return 0; // se nao achou vai retornar falso
}

// escreve no arquivo de saida as informacoes do alimento encontrado
void exitAlimentoCaloria(char *nomeAlimento, int porcaoNutri, int caloriaAlimento, FILE *arq_exit) {
  int quantidadeCalorias;
  FILE *open_arq;

  quantidadeCalorias = (caloriaAlimento * porcaoNutri) / 100; // regra de 3 para calculo das calorias ingeridas
  totalCalorias += quantidadeCalorias;

  fprintf(arq_exit, "%dg de ", caloriaAlimento);
  fputs(nomeAlimento, arq_exit);
  fprintf(arq_exit, " (%d calorias por 100g)", porcaoNutri);
  fputs(" = ", arq_exit);
  fprintf(arq_exit, "%d calorias", caloriaAlimento, quantidadeCalorias);
  fputs("\n", arq_exit);
}

// escreve informacoes de titulo e de estatisticas de comparacoes, altura, nos folhas, etc
void informacoesTXT(noAVL *raiz, FILE *arq_exit, short flag, char nomeArqIngerido[], char nomeArquivo[]) {
  if (flag == 1) {
    fputs("Calorias calculada para ", arq_exit);
    fputs(nomeArqIngerido, arq_exit);
    fputs(" usando a tabela ", arq_exit);
    fputs(nomeArquivo, arq_exit);
    fputs("\n\n", arq_exit);
  } else if (flag == 2) {
    fputs("\n", arq_exit);
    fputs("Total de ", arq_exit);
    fprintf(arq_exit, "%d calorias consumidas no dia.\n\n", totalCalorias); // totalCalorias eh uma variavel global
    fputs("======== ESTATÍSTICAS AVL ============\n", arq_exit);
    fprintf(arq_exit, "Numero de Nodos: %d\n", quantidade_nos(raiz));
    fprintf(arq_exit, "Numero de Nodos folhas: %d\n", quantidade_folhas(raiz));
    fprintf(arq_exit, "Altura: %d\n", alturaAVL(raiz));
    fprintf(arq_exit, "Rotações à direita: %d\n", rotacoesDir);
    fprintf(arq_exit, "Rotações à esquerda: %d\n", rotacoesEsq);
    fprintf(arq_exit, "Rotações direita-esquerda: %d\n", rotacoesDirEsq);
    fprintf(arq_exit, "Rotações esquerda-direita: %d\n", rotacoesEsqDir);
    fprintf(arq_exit, "Comparações: %d\n\n", comp);
  }
}

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  //----- PASSAGEM DE PARAMETROS PELA main -----//
  char nomeArqCaloria[50];
  char nomeArqIngerido[50];
  char nomeOutput[50];
  strcpy(nomeArqCaloria, argv[1]);
  strcpy(nomeArqIngerido, argv[2]);
  strcpy(nomeOutput, argv[3]);
  //--------------------------------------------//

  clock_t timeExecution = clock(); // inicia a contagem de tempo
  noAVL *raiz = NULL;

  if (argc != 4) {  // sao necessarios 4 parametros para o programa funcionar como deveria
    printf("Numero incorreto de parametros!\n");
    printf("Insira: <.exe> <arq_entrada_caloria> <arq_entrada_ingerido> <arq_saida>");
  } else {
    raiz = leituraArqCalorias(raiz, nomeArqCaloria);
    printf("%.1f ms - Tempo de armazenar na arvore \n", (double)(clock() - timeExecution));
    leituraArqNutri(raiz, nomeArqIngerido, nomeOutput, nomeArqCaloria);

    // Desenha(raiz, 1);
    printf("%.1f ms - Tempo de armazenar, procurar e escrever no arquivo \n", (double)(clock() - timeExecution));
    system("pause");
    system("cls");
  }
  return 0;
}