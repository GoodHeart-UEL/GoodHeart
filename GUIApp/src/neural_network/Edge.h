#ifndef EDGE_H
#define EDGE_H

/*
Ponteiro que representa a estrutura "Edge"
A estrutura edge corresponde as arestas que fazem as ligações de cada neuronio.
A estrutura é composta por um peso e um neuronio de origem (camada anterior)
*/
typedef void * Edge;

typedef void * Neuron;

/*
Cria uma aresta de peso e neuronio de origem passados por parametro
    - parametros:
        double weight -> Peso da aresta. Carregado do arquivo de texto na etapa de adição da camada.
        Neuron neuron -> Neuronio de origem (camada anterior).

    - retorno: Retorna o ponteiro para estrutura da aresta.

*/
Edge createEdge(double weight, Neuron neuron);

/*
Função que retorna o peso da aresta passada por parametro.
    - parametros:
        Edge edge -> Ponteiro da estrutura da aresta.
    
    -retorno: O peso da aresta passada por parametro.
*/
double getEdgeWeight(Edge edge);


/*
Função que retorna o neuronio de origem da aresta passada por parametro.
    - parametros:
        Edge edge -> Ponteiro da estrutura da aresta.
    
    -retorno: O ponteiro do neuronio de origem associado a aresta passada por parametro.
*/
Neuron getEdgeNeuron(Edge edge);

#endif