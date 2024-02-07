#include "arvore.h"
#include <stdio.h>

int main() {
    int escolha, remove;
    RBTree* arvore = alocarArvore();
    Pessoa* pessoa = alocarPessoa();

    do{
        scanf("%d", &escolha);

        if (escolha == 1) {
            if(!(leArvore(&arvore)))
                printf("Erro ao inserir!\n\n\n\n");
        } else if (escolha == 2) {
            printf("Dados inOrder:\n"); 
            printInOrder(arvore->raiz);
        } else if (escolha == 3) {
            printf("Digite o nome e a idade da pessoa a ser removida:\n");
            scanf("%s %d", pessoa->nome, &pessoa->idade);
            No* noRemover = encontrarNo(arvore->raiz, pessoa);
            if (noRemover != NULL) {
                removerNo(arvore, noRemover);
                printf("Pessoa removida com sucesso!\n");
            } else {
                printf("Pessoa não encontrada na árvore!\n");
            }
        }
    }while(escolha != 0);

    printf("Dados inOrder:\n"); 
    printInOrder(arvore->raiz);
    desalocarArvore(&arvore);
    desalocarPessoa(&pessoa);
    return 0;
}