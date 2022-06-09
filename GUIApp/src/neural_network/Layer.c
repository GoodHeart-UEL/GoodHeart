#include"Layer.h"
#include"Neuron.h"
#include"Edge.h"
#include"Lista.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<string.h>

double relu(double value){
    return value < 0 ? 0 : value;
}

double sigmoid(double value){
    return 1/(1 + exp(-value));
}

double h_bias[] = {
    0.17553519,
    0.23510556,
    0.119718105,
    0.0
};

double o_bias[] = {0.18774033};

typedef struct elemento{
    Lista neurons;
    double (*activation_function)(double value);
    int type;
}LayerImpl;

Layer createLayer(char* activation_function, int neurons, int layer_type){
    LayerImpl* layer = malloc(sizeof(LayerImpl));
    layer->type = layer_type;
    layer->neurons = NULL;
    if(layer_type != INPUT){
        if(strcmp(activation_function, "relu") == 0){
            layer->activation_function = relu;
        }else if(strcmp(activation_function, "sigmoid") == 0){
            layer->activation_function = sigmoid;
        }
    }

    /*FILE* file = NULL;
    switch (layer_type){
        case HIDDEN:
            file = fopen("h_bias.txt","r");
            break;
        case OUTPUT:
            file = fopen("o_bias.txt","r");
            break;
        default:
            file = NULL;
            break;
    }

    char line[80];
    while (neurons>0){
        double bias=0;
        if(file!=NULL){
            fgets(line, 80, file);
            sscanf(line,"%lf", &bias);
        }
        layer->neurons = inserir(layer->neurons, createNeuron(bias));
        neurons--;
    }*/

    for(int i = 0;i<neurons;i++){
        double bias=0;
        if(layer_type == HIDDEN)
            bias = h_bias[i];
        else if(layer_type == OUTPUT)
            bias = o_bias[i];
        layer->neurons = inserir(layer->neurons, createNeuron(bias));
    }

    return layer;
}

Lista getNeurons(Layer layer){
    LayerImpl* l = layer;
    return l->neurons;
}

int getLayerType(Layer layer){
    LayerImpl* l = layer;
    return l->type;
}

double activationFunction(Layer layer, double value){
    LayerImpl* l = layer;
    return l->activation_function(value);
}
