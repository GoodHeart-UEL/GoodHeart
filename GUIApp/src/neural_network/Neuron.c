#include"Neuron.h"
#include"Edge.h"
#include"Lista.h"
#include<stdio.h>
#include<stdlib.h>

typedef struct elemento{
    double value;
    double bias;
    Lista edges;
}NeuronImpl;

Neuron createNeuron(double bias){
    NeuronImpl* neuron = malloc(sizeof(NeuronImpl));
    neuron->bias = bias;
    neuron->edges = NULL;
    return neuron;
}

void setNeuronValue(Neuron neuron, double value){
    NeuronImpl* n = neuron;
    n->value = value;
}

double getNeuronValue(Neuron neuron){
    NeuronImpl* n = neuron;
    return n->value;
}

void insertNeuronEdge(Neuron neuron, Edge edge){
    NeuronImpl* n = neuron;
    n->edges = inserir(n->edges, edge);
}

Lista getNeuronEdges(Neuron neuron){
    NeuronImpl* n = neuron;
    return n->edges;
}

double getNeuronBias(Neuron neuron){
    NeuronImpl* n = neuron;
    return n->bias;
}

