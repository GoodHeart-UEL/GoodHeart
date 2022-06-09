#ifndef LAYER_H
#define LAYER_H
#include"Lista.h"

/*
Ponteiro que representa a estrutura "Layer".
A estrutura Layer possui uma Lista de neuronios, sua função de ativação e seu tipo (INPUT, HIDDEN ou OUTPUT).
*/
typedef void * Layer;

enum layer_type{ INPUT, HIDDEN, OUTPUT };

/*
Cria uma camada que pode ser adicionada a rede neural. Nessa função é feito o carregamento dos bias presentes nos arquivos h_bias.txt e o_bias.txt
    - parametros:
        char* activation_function -> Nome da função de ativação da camada. Opções: "relu" ou "sigmoid".
        int neurons -> Quantidade de neurônios que estarão presentes na camada.
        int layer_type -> Tipo da camada. Opções: INPUT, HIDDEN ou OUTPUT.

    - retorno: Retorna o ponteiro para a estrutura "Layer".
*/
Layer createLayer(char* activation_function, int neurons, int layer_type);

/*
Retorna a lista de neurônios da camada passada por parâmetro
    - parametros:
        Layer layer -> Ponteiro para a estrutura "Layer".
    
    - retorno: Retorna o ponteiro para a estrutura "Lista".
*/
Lista getNeurons(Layer layer);

/*
Retorna o tipo da camada passada por parâmetro
    - parametros:
        Layer layer -> Ponteiro para a estrutura "Layer".
    
    - retorno: Retorna um inteiro. INPUT, HIDDEN ou OUTPUT.
*/
int getLayerType(Layer layer);

/*
Aplica a função de ativação da camada passada por parâmetro e retorna o resultado dessa aplicação.
    - parametros:
        Layer layer -> Ponteiro para a estrutura "Layer".
        double value -> Valor a ser aplicado na função de ativação.
    
    - retorno: Retorna o valor calculado pela função de ativação.
*/
double activationFunction(Layer layer, double value);

#endif