#ifndef ARVORE_H
#define ARVORE_H
#include <stdbool.h>

typedef struct pessoa {
    char nome[21];
    int idade;
} Pessoa;

typedef struct no{
    Pessoa* pessoa;
    int cor;
    struct no* pai;
    struct no* esq;
    struct no* dir;    
}No;

typedef struct rbtree {
    No* raiz;
}RBTree;


//Funções de alocação
RBTree* alocarArvore();
No* alocarNo();
Pessoa* alocarPessoa();

//Funções de desalocação
void desalocarArvore(RBTree**);
void desalocarNo(No**);
void desalocarPessoa(Pessoa**);

bool leArvore(RBTree**);
bool insercao(No**, Pessoa*);
bool arvoreRetira(No**, Pessoa);

int balanceamento (No**);
int balanceamentoEsquerda(No**);
int balanceamentoDireita(No**);
void rotacaoEsquerda (No**);
void rotacaoDireita (No**);
int compara(Pessoa*, Pessoa*);
int altura(No*);
int FB(No*);
bool paiNo (No**, No**, No**);
void printInOrder(No*);
void imprimeCor (char);

No* encontrarNo(No*, Pessoa*);
No* encontrarMenor(No*);
void substituirNo(RBTree*, No*, No*);
int balanceamentoRemocao(RBTree**, No**, int);
void removerNo(RBTree*, No*);

#endif