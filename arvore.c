#include "arvore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ANSI_BOLD           "\x1b[1m"  // coloca o texto em negrito
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_BLACK    "\x1b[30m"
#define ANSI_RESET          "\x1b[0m"
#define ANSI_BG_C_WHITE     "\x1b[47m"
#define BOLD_RED(string) ANSI_COLOR_RED ANSI_BOLD string ANSI_RESET
#define BOLD_BLACK_BG_WHITE(string) ANSI_COLOR_BLACK ANSI_BOLD  ANSI_BG_C_WHITE string ANSI_RESET

//Funções de alocação

RBTree* alocarArvore() {
    RBTree* arvore = (RBTree*)malloc(sizeof(RBTree));
    arvore->raiz = NULL;
    return arvore;
}

No* alocarNo(Pessoa* pessoa) {
    No* novo = (No*)malloc(sizeof(No));
    novo->pessoa = alocarPessoa();
    strcpy(novo->pessoa->nome, pessoa->nome);
    novo->pessoa->idade = pessoa->idade;
    novo->cor = 0;
    novo->pai = NULL;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

Pessoa* alocarPessoa() {
    Pessoa* pessoa = (Pessoa*)malloc(sizeof(Pessoa));
    pessoa->idade = 0;
    return pessoa;
}

//Funções de desalocação
void desalocarArvore(RBTree** arvore) {
    desalocarNo(&(*arvore)->raiz);
    free(*arvore);
}

void desalocarNo(No** no) {
    if (*no == NULL)
        return;
    desalocarNo(&(*no)->esq);
    desalocarNo(&(*no)->dir);
    desalocarPessoa(&(*no)->pessoa);
    free(*no);
}

void desalocarPessoa(Pessoa** pessoa) {
    free(*pessoa);
    *pessoa = NULL;
}

bool leArvore(RBTree** arvore) {
    int quantidade;
    scanf("%d", &quantidade);
    Pessoa* pessoa = (Pessoa*)malloc(sizeof(Pessoa));

    for (int i = 0; i < quantidade; i++) {
        scanf("%s %d", pessoa->nome, &pessoa->idade);
        if (!(insercao(&(*arvore)->raiz, pessoa))) {
            return false;
        } else {
            paiNo(&(*arvore)->raiz, &(*arvore)->raiz->esq, &(*arvore)->raiz->dir);
        }
    }
    desalocarPessoa(&pessoa);
    return true;
}

bool insercao(No** no, Pessoa* pessoa) {
    if (*no == NULL) {
        *no = alocarNo(pessoa);
        if ((*no)->pai != NULL) {
            balanceamento(no); // Correção na chamada da função balanceamento
        }
        return true;
    }
    if (compara((*no)->pessoa, pessoa) < 0) {
        if (insercao(&((*no)->esq), pessoa)) {
            balanceamentoEsquerda(no); // Correção na chamada da função balanceamentoEsquerda
            return true;
        }
    } else if (compara((*no)->pessoa, pessoa) > 0) {
        if (insercao(&((*no)->dir), pessoa)) {
            balanceamentoDireita(no); // Correção na chamada da função balanceamentoDireita
            return true;
        }
    }
    return false;
}


int balanceamento(No** no) {
    if (*no == NULL || (*no)->pai == NULL || (*no)->pai->pai == NULL) // Verifica se o nó, o pai e o avô são válidos
        return 0;

    if ((*no)->cor == 1) {
        // Caso 1: Pai e tio são vermelhos
        if (((*no)->pai->pai->esq->cor == 1) && ((*no)->pai->pai->dir->cor == 1)) {
            (*no)->pai->cor = 0;
            (*no)->pai->pai->esq->cor = 0;
            (*no)->pai->pai->dir->cor = 0;
            (*no)->pai->pai->cor = 1;
            balanceamento(&(*no)->pai->pai); // Recursivamente balanceia o avô
            return 1;
        } else {
            // Caso 2: Pai vermelho, tio preto, e nó é filho à esquerda de seu pai que é filho à direita do avô
            if ((*no)->pai->dir == *no && (*no)->pai->pai->esq == (*no)->pai) {
                rotacaoEsquerda(&(*no)->pai);
                *no = (*no)->esq;
            }
            // Caso 3: Pai vermelho, tio preto, e nó é filho à direita de seu pai que é filho à esquerda do avô
            else if ((*no)->pai->esq == *no && (*no)->pai->pai->esq == (*no)->pai) {
                rotacaoDireita(&(*no)->pai->pai);
                (*no)->pai->cor = 0;
                (*no)->pai->pai->cor = 1;
            }
            return 1;
        }
    }
    (*no)->cor = 0; // O nó raiz deve ser preto
    return 0;
}

int balanceamentoEsquerda (No** no) {
    if (*no == NULL || (*no)->pai == NULL) // Verifica se o nó e o pai são válidos
        return 0;

    if ((*no)->cor == 1 && (*no)->esq != NULL && (*no)->esq->cor == 1) {
        // Caso 1: Pai e tio são vermelhos
        if (((*no)->pai->pai->esq->cor == 1) && ((*no)->pai->pai->dir->cor == 1)) {
            (*no)->pai->cor = 0;
            (*no)->pai->pai->esq->cor = 0;
            (*no)->pai->pai->dir->cor = 0;
            (*no)->pai->pai->cor = 1;
            balanceamentoEsquerda(no); // Correção na chamada recursiva da função balanceamentoEsquerda
            return 1;
        } else {
            // Caso 2: Pai vermelho, tio preto, e nó é filho à direita de seu pai que é filho à esquerda do avô
            if ((*no)->pai->dir == *no && (*no)->pai->pai->esq == (*no)->pai) {
                rotacaoEsquerda(&(*no)->pai);
                *no = (*no)->esq;
            }
            // Caso 3: Pai vermelho, tio preto, e nó é filho à esquerda de seu pai que é filho à esquerda do avô
            else if ((*no)->pai->esq == *no && (*no)->pai->pai->esq == (*no)->pai) {
                rotacaoDireita(&(*no)->pai->pai);
                (*no)->pai->cor = 0;
                (*no)->pai->pai->cor = 1;
            }
            return 1;
        }
    }
    (*no)->cor = 0; // O nó raiz deve ser preto
    return 0;
}

int balanceamentoDireita(No** no) {
    if (*no == NULL || (*no)->pai == NULL) // Verifica se o nó e o pai são válidos
        return 0;

    if ((*no)->cor == 1 && (*no)->dir != NULL && (*no)->dir->cor == 1) {
        // Caso 1: Pai e tio são vermelhos
        if (((*no)->pai->pai->esq->cor == 1) && ((*no)->pai->pai->dir->cor == 1)) {
            (*no)->pai->cor = 0;
            (*no)->pai->pai->esq->cor = 0;
            (*no)->pai->pai->dir->cor = 0;
            (*no)->pai->pai->cor = 1;
            balanceamentoDireita(no); // Correção na chamada recursiva da função balanceamentoDireita
            return 1;
        } else {
            // Caso 2: Pai vermelho, tio preto, e nó é filho à esquerda de seu pai que é filho à direita do avô
            if ((*no)->pai->esq == *no && (*no)->pai->pai->dir == (*no)->pai) {
                rotacaoDireita(&(*no)->pai);
                *no = (*no)->dir;
            }
            // Caso 3: Pai vermelho, tio preto, e nó é filho à direita de seu pai que é filho à direita do avô
            else if ((*no)->pai->dir == *no && (*no)->pai->pai->dir == (*no)->pai) {
                rotacaoEsquerda(&(*no)->pai->pai);
                (*no)->pai->cor = 0;
                (*no)->pai->pai->cor = 1;
            }
            return 1;
        }
    }
    return 0;
}

void rotacaoDireita(No** no) {
    No* temp = (*no)->esq;
    (*no)->esq = temp->dir;
    if ((*no)->esq != NULL) {
        (*no)->esq->pai = (*no);
    }
    temp->dir = *no;
    temp->pai = (*no)->pai;
    (*no)->pai = temp;
    *no = temp;
}

void rotacaoEsquerda(No** no) {
    No* temp = (*no)->dir;
    (*no)->dir = temp->esq;
    if ((*no)->dir != NULL) {
        (*no)->dir->pai = (*no);
    }
    temp->esq = *no;
    temp->pai = (*no)->pai;
    (*no)->pai = temp;
    *no = temp;
}

int compara(Pessoa* pessoa1, Pessoa* pessoa2) {
    if ((pessoa1->idade > pessoa2->idade) && (strcmp(pessoa1->nome, pessoa2->nome) != 0)) {
        return -1;
    } else if ((pessoa1->idade < pessoa2->idade) && (strcmp(pessoa1->nome, pessoa2->nome) != 0)){
        return 1;
    } else if ((pessoa1->idade == pessoa2->idade) && (strcmp(pessoa1->nome, pessoa2->nome) != 0)) {
        if (strcmp(pessoa1->nome, pessoa2->nome) > 0) {
            return -1;
        } else if (strcmp(pessoa1->nome, pessoa2->nome) < 0){
            return -1;
        }
    }
    return 0;
}

int FB(No *no) {
    return altura(no->esq) - altura(no->dir);
}

int altura(No *pRaiz) {
    int iEsq, iDir;
    if (pRaiz == NULL)
        return 0;
    iEsq = altura(pRaiz->esq);
    iDir = altura(pRaiz->dir);
    if (iEsq > iDir)
        return iEsq + 1;
    else
        return iDir + 1;
}

void printInOrder(No* no) {
    if (no == NULL) {
        return;
    }
    printInOrder(no->esq);
    printf("Nome: %s\n", no->pessoa->nome);
    printf("Idade: %d\n", no->pessoa->idade);
    imprimeCor(no->cor); //Imprimir a cor deu ruim
    printInOrder(no->dir);
}

void imprimeCor (char cor) {
     if (cor == 1) {
        printf("Cor: ");
        printf(BOLD_RED("Vermelho"));
        printf("!\n");
    } else if(cor == 0){
        printf("Cor: ");
        printf(BOLD_BLACK_BG_WHITE("Preto"));
        printf("!\n");
    }
}

bool paiNo (No** pai, No** fEsq, No** fDir) {
    if (pai == NULL || fEsq == NULL || fDir == NULL || *pai == NULL || *fEsq == NULL || *fDir == NULL) {
        return false; // Verifica se os ponteiros são válidos
    }

    if (((*pai)->esq == NULL) && ((*pai)->dir == NULL)){
        return false;
    } else {
        (*fEsq)->pai = (*pai);
        (*fDir)->pai = (*pai);
    }
    return true;
}

//Funções de retirar pessoas da árvore

No* encontrarNo(No* raiz, Pessoa* pessoa) {
    if (raiz == NULL)
        return NULL;

    int comp = compara(raiz->pessoa, pessoa);
    if (comp == 0)
        return raiz;
    else if (comp < 0)
        return encontrarNo(raiz->esq, pessoa);
    else
        return encontrarNo(raiz->dir, pessoa);
}

No* encontrarMenor(No* no) {
    No* atual = no;
    while (atual && atual->esq != NULL) {
        atual = atual->esq;
    }
    return atual;
}

void substituirNo(RBTree* arvore, No* alvo, No* substituto) {
    if (alvo->pai == NULL) {
        arvore->raiz = substituto;
    } else if (alvo == alvo->pai->esq) {
        alvo->pai->esq = substituto;
    } else {
        alvo->pai->dir = substituto;
    }
    if (substituto != NULL) {
        substituto->pai = alvo->pai;
    }
}

int balanceamentoRemocao(RBTree** arvore, No** no, int fb) {
    No* pai = NULL;
    No* irmao = NULL;

    while (*no != (*arvore)->raiz && (*no)->cor == 0) {
        pai = (*no)->pai;
        if (*no == pai->esq) {
            irmao = pai->dir;
            if (irmao->cor == 1) {
                irmao->cor = 0;
                pai->cor = 1;
                rotacaoEsquerda(&pai);
                irmao = pai->dir;
            }
            if (irmao->esq->cor == 0 && irmao->dir->cor == 0) {
                irmao->cor = 1;
                *no = pai;
            } else {
                if (irmao->dir->cor == 0) {
                    irmao->esq->cor = 0;
                    irmao->cor = 1;
                    rotacaoDireita(&irmao);
                    irmao = pai->dir;
                }
                irmao->cor = pai->cor;
                pai->cor = 0;
                irmao->dir->cor = 0;
                rotacaoEsquerda(&pai);
                *no = (*arvore)->raiz;
            }
        } else {
            irmao = pai->esq;
            if (irmao->cor == 1) {
                irmao->cor = 0;
                pai->cor = 1;
                rotacaoDireita(&pai);
                irmao = pai->esq;
            }
            if (irmao->esq->cor == 0 && irmao->dir->cor == 0) {
                irmao->cor = 1;
                *no = pai;
            } else {
                if (irmao->esq->cor == 0) {
                    irmao->dir->cor = 0;
                    irmao->cor = 1;
                    rotacaoEsquerda(&irmao);
                    irmao = pai->esq;
                }
                irmao->cor = pai->cor;
                pai->cor = 0;
                irmao->esq->cor = 0;
                rotacaoDireita(&pai);
                *no = (*arvore)->raiz;
            }
        }
    }

    if (*no != NULL)
        (*no)->cor = 0; // Corrige a cor do nó raiz
    return 0;
}

void removerNo(RBTree* arvore, No* no) {
    No* temp = no;
    No* filho;
    char temp_cor = temp->cor;

    if (no->esq == NULL) {
        filho = no->dir;
        substituirNo(arvore, no, no->dir);
    } else if (no->dir == NULL) {
        filho = no->esq;
        substituirNo(arvore, no, no->esq);
    } else {
        temp = encontrarMenor(no->dir);
        temp_cor = temp->cor;
        filho = temp->dir;
        if (temp->pai == no) {
            if (filho != NULL) {
                filho->pai = temp;
            }
        } else {
            substituirNo(arvore, temp, temp->dir);
            temp->dir = no->dir;
            temp->dir->pai = temp;
        }
        substituirNo(arvore, no, temp);
        temp->esq = no->esq;
        temp->esq->pai = temp;
        temp->cor = no->cor;
    }
    free(no);
    if (temp_cor == 0) {
        balanceamentoRemocao(&arvore, &filho, 0);
    }
}