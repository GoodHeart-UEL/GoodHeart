#include"Layer.h"
#include"NeuralNetwork.h"
#include"Neuron.h"
#include"Edge.h"
#include"ECG.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct elemento{
    Layer input_layer;
    Layer hidden_layer;
    Layer output_layer;
}NeuralNetworkImpl;

double h_weights[] = {
    0.62483275,
    -2.17886,
    0.72318494,
    0.78624016,
    1.1654408,
    -0.13635767,
    -0.08786797,
    -0.50736105,
    -1.0216297,
    1.2133216,
    0.57142025,
    -1.3304504,
    -0.60613847,
    0.6734291,
    -0.35304195,
    1.3840201,
    0.77570444,
    -2.0710368,
    1.0285048,
    0.99156916,
    1.396082,
    0.11226737,
    -0.25664058,
    -0.08537648,
    0.4062479,
    0.21855634,
    -0.70404017,
    0.2317028,
    -0.48990095,
    0.017290175,
    0.52245456,
    -0.05953443
};

double o_weights[] = {
    -1.9082618,
    1.1022542,
    -1.245951,
    0.013958693
};

NeuralNetwork createNeuralNetwork(){
    return malloc(sizeof(NeuralNetworkImpl));
}


void addLayer(NeuralNetwork neuralNetwork, Layer layer){
    NeuralNetworkImpl* nn = neuralNetwork;
    //FILE* weights;
    Lista neurons;
    //char line[80];
    int i;
    switch (getLayerType(layer)){
        case INPUT:
            nn->input_layer = layer;
            break;
        case HIDDEN:
            if(nn->input_layer == NULL){
                printf("Não foi possivel encontrar a camada de entrada");
                exit(0);
            }
            //weights = fopen("h_weights.txt", "r");
            i=0;
            neurons = getNeurons(layer);
            while(neurons != NULL){
                Neuron neuron = get(neurons);
                Lista inputNeurons = getNeurons(nn->input_layer);
                while(inputNeurons != NULL){
                    Neuron inputNeuron = get(inputNeurons);
                    double weight;
                    //fgets(line, 80, weights);
                    //sscanf(line, "%lf", &weight);
                    weight = h_weights[i];
                    Edge e = createEdge(weight, inputNeuron);
                    insertNeuronEdge(neuron, e);
                    inputNeurons = getProx(inputNeurons);
                    i++;
                }
                neurons = getProx(neurons);
            }
            nn->hidden_layer = layer;
            break;
        case OUTPUT:
            if(nn->hidden_layer == NULL){
                printf("Não foi possivel encontrar a camada oculta");
                exit(0);
            }
            //weights = fopen("o_weights.txt", "r");
            i=0;
            neurons = getNeurons(layer);
            while(neurons != NULL){
                Neuron neuron = get(neurons);
                Lista hiddenNeurons = getNeurons(nn->hidden_layer);
                while(hiddenNeurons != NULL){
                    Neuron hiddenNeuron = get(hiddenNeurons);
                    double weight;
                    //fgets(line, 80, weights);
                    //sscanf(line, "%lf", &weight);
                    weight = o_weights[i];
                    Edge e = createEdge(weight, hiddenNeuron);
                    insertNeuronEdge(neuron, e);
                    hiddenNeurons = getProx(hiddenNeurons);
                    i++;
                }
                neurons = getProx(neurons);
            }
            nn->output_layer = layer;
            break;
        default:
            break;
    }
}


double executeNeuralNetwork(NeuralNetwork neuralNetwork, ECG ecg){
    NeuralNetworkImpl* nn = neuralNetwork;

    Lista inputNeuronsList = getNeurons(nn->input_layer);
    Neuron inputNeuron = get(inputNeuronsList);
    setNeuronValue(inputNeuron, getPValue(ecg));

    inputNeuronsList = getProx(inputNeuronsList);
    inputNeuron = get(inputNeuronsList);
    setNeuronValue(inputNeuron, getQValue(ecg));

    inputNeuronsList = getProx(inputNeuronsList);
    inputNeuron = get(inputNeuronsList);
    setNeuronValue(inputNeuron, getRValue(ecg));

    inputNeuronsList = getProx(inputNeuronsList);
    inputNeuron = get(inputNeuronsList);
    setNeuronValue(inputNeuron, getSValue(ecg));

    inputNeuronsList = getProx(inputNeuronsList);
    inputNeuron = get(inputNeuronsList);
    setNeuronValue(inputNeuron, getTValue(ecg));

    inputNeuronsList = getProx(inputNeuronsList);
    inputNeuron = get(inputNeuronsList);
    setNeuronValue(inputNeuron, getRRValue(ecg));

    inputNeuronsList = getProx(inputNeuronsList);
    inputNeuron = get(inputNeuronsList);
    setNeuronValue(inputNeuron, getQTValue(ecg));

    inputNeuronsList = getProx(inputNeuronsList);
    inputNeuron = get(inputNeuronsList);
    setNeuronValue(inputNeuron, getQRSValue(ecg));

    Lista hiddenNeuronsList = getNeurons(nn->hidden_layer);
    while (hiddenNeuronsList != NULL){
        Neuron hiddenNeuron = get(hiddenNeuronsList);
        Lista edgesList = getNeuronEdges(hiddenNeuron);
        double value = 0;
        while (edgesList != NULL){

            Edge edge = get(edgesList);
            double inputValue = getNeuronValue(getEdgeNeuron(edge));
            double weight = getEdgeWeight(edge);
            value+= inputValue * weight;

            edgesList = getProx(edgesList);
        }

        value+=getNeuronBias(hiddenNeuron);
        setNeuronValue(hiddenNeuron, activationFunction(nn->hidden_layer,value));

        hiddenNeuronsList = getProx(hiddenNeuronsList);
    }

    Lista outputNeuronsList = getNeurons(nn->output_layer);
    while (outputNeuronsList != NULL){
        Neuron outputNeuron = get(outputNeuronsList);
        Lista edgesList = getNeuronEdges(outputNeuron);
        double value = 0;
        while (edgesList != NULL){

            Edge edge = get(edgesList);
            double hiddenValue = getNeuronValue(getEdgeNeuron(edge));
            double weight = getEdgeWeight(edge);
            value+= hiddenValue * weight;

            edgesList = getProx(edgesList);
        }

        value+=getNeuronBias(outputNeuron);
        setNeuronValue(outputNeuron, activationFunction(nn->output_layer,value));

        outputNeuronsList = getProx(outputNeuronsList);
    }

    double result = getNeuronValue(get(getNeurons(nn->output_layer)));

    return result;
}

