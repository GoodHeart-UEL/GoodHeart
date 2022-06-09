#include"Edge.h"
#include<stdio.h>
#include<stdlib.h>

typedef struct elemento{
    double weight;
    Neuron neuron;
}EdgeImpl;

Edge createEdge(double weight, Neuron neuron){
    EdgeImpl* edge = malloc(sizeof(EdgeImpl));
    edge->weight = weight;
    edge->neuron = neuron;
    return edge;
}

double getEdgeWeight(Edge edge){
    EdgeImpl* e = edge;
    return e->weight;
}

Neuron getEdgeNeuron(Edge edge){
    EdgeImpl* e = edge;
    return e->neuron;
}

