#ifndef LISTD_H
#define LISTD_H

/**
 * Neste header file estão declaradas as funções necessárias para a implementação de
 * uma lista encadeada dinâmica, bem como "getters" para obter as informações de cada
 * posição desta e também funções que auxiliam a utilização da mesma.
*/

/**
 * Uma "Lista" é uma estrutura que possui um ponteiro para um elemento a ser armazenado
 * e um ponteiro para outra "Lista", sendo esta a próxima posição da lista.
*/
typedef void * Lista;

/**
 * Um "Elem" é um void pointer (void*) que refere-se a uma informação a ser
 * armazenada dentro de uma posição da lista.
*/
typedef void * Elem;

/**
 * Recebe o item da lista "L" e um elemento a ser 
 * adicionado "e". Insere este e retorna a primeira posição da lista.
*/
Lista inserir(Lista L,Elem e);

/**
 * Recebe a primeira posição da lista "L" e um elemento a ser removido. 
 * Percorre a lista, desaloca o nó que possua este elemento e retorna a primeira
 * posição da lista. 
*/
Lista remover(Lista L,Elem elemento);

/**
 * Recebe a primeira posição da lista "L" e um elemento a ser removido. 
 * Percorre a lista, remove o nó que possua este elemento sem desalocar e retorna a primeira
 * posição da lista. 
*/
Lista removerSemFree(Lista L,Elem elemento);

/**
 * Recebe uma posição da lista e retorna a próxima.
 * A posição "L" não deve ser NULL.
*/
Lista getProx(Lista L);

/**
 * Recebe um nó da lista e retorna o elemento interior a ele.
 * "L" não deve ser NULL.
*/
Elem get(Lista L);

/**
 * Recebe a primeira posição de uma lista e retorna um inteiro
 * que representa o número de elementos presentes nela.
 * Retorna zero caso "L" seja NULL.
*/
int getListaSize(Lista lista);

/**
 * Desaloca todas as posições da lista.
*/
void finalizar(Lista L);

#endif