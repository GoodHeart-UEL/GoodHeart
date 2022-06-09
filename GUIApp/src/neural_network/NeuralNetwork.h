#ifndef NNETWORK_H
#define NNETWORK_H

/*
Ponteiro que representa a estrutura "NeuralNetwork"
A estrutura "NeuralNetwork" possui como atributos suas 3 camadas:
    Layer input_layer
    Layer hidden_layer
    Layer output_layer

*/
typedef void * NeuralNetwork;
typedef void * ECG;
typedef void * Layer;

/*
Cria uma "instância de rede neural"
    - retorno: Ponteiro da estrutura "NeuralNetwork".
*/
NeuralNetwork createNeuralNetwork();

/*
Adiciona uma camada (representado por Layer) na rede neural passada como parametro. Nesse processo é feito o carregamento dos pesos que estão predefinidos
nos arquivos h_weights.txt e o_weights.txt
    - parametros:
        NeuralNetwork neuralNetwork -> Ponteiro que representa a estrutura "NeuralNetwork".
        Layer layer -> Ponteiro que representa a estrutura "Layer".
*/
void addLayer(NeuralNetwork neuralNetwork, Layer layer);

/*
Executa a rede neural passada por parametro e utiliza como input o ECG passado como segundo parametro. Nessa função é feito todo o processamento
da rede neural. Desde a inserção dos valores do ECG na camada de entrada até o calculo de todos os valores da camada oculta e de saída.
    - parametros:
        NeuralNetwork neuralNetwork -> Ponteiro que representa a estrutura "NeuralNetwork".
        ECG ecg -> Ponteiro que representa a estrutura "ECG".
    
    - retorno: Valor calculado na camada de saída da rede neural depois da execução.
*/
double executeNeuralNetwork(NeuralNetwork neuralNetwork, ECG ecg);

#endif
