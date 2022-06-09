#ifndef NEURON_H
#define NEURON_H
#include"Lista.h"

/*
Ponteiro que representa a estrutura "Neuron"
A estrutura Neuron corresponde aos neuronios presentes em cada camada da rede neural. 
A estrutura é composta por um valor, um bias (Carregado na criação das camadas) e uma lista de arestas.
*/
typedef void * Neuron;

typedef void * Edge;

/*
Cria um neuronio de bias com o valor passado por parametro.
    - parametros:
        double bias -> Valor do bias.

    - retorno: Ponteiro que representa a estrutura "Neuron"
*/
Neuron createNeuron(double bias);

/*
Seta o valor atual do neuronio passado por parametro.
    - parametros:
        Neuron neuron -> Neuronio a ser setado.
        double valor -> Valor a ser atribuido ao neuronio.
*/
void setNeuronValue(Neuron neuron, double value);

/*
Retorna o valor atual do neuronio passado por parametro.
    - parametros:
        Neuron neuron -> Neuronio a ser setado.
    
    - retorno: Retorno do valor atual do neuronio
*/
double getNeuronValue(Neuron neuron);

/*
Insere uma aresta na lista de arestas do neuronio passado por parametro.
    - parametros:
        Neuron neuron -> Neuronio a ser setado.
        Edge edge -> Aresta a ser adicionada na lista de arestas do neuronio
*/
void insertNeuronEdge(Neuron neuron, Edge edge);

/*
Retorna a lista de arestas do neuronio passado por parametro.
    - parametros:
        Neuron neuron -> Ponteiro da estrutura "Neuron".

    - retorno: Retorna a lista de arestas do neuronio.
*/
Lista getNeuronEdges(Neuron neuron);

/*
Retorna o valor do bias do neuronio passado por parametro.
    - parametros:
        Neuron neuron -> Ponteiro da estrutura "Neuron".
    
    - retorno: Valor do bias do neuronio passado por parametro
*/
double getNeuronBias(Neuron neuron);

#endif