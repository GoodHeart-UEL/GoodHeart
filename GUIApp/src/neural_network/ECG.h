#ifndef ECG_H
#define ECG_H
#include "Lista.h"
/*
Ponteiro que representa a estrutura de um eletrocardiograma (ECG).
A estrutura possui os valores dos pontos fiduciais P-QRS-T e dos intervalos RR, QT e QRS.
*/
typedef void * ECG;


/*
Cria um ECG e retorna o ponteiro da estrutura "ECG".
    - parametros:
        double P -> Valor da onda P em mV
        double Q -> Valor da onda Q em mV
        double R -> Valor da onda R em mV
        double S -> Valor da onda S em mV
        double T -> Valor da onda T em mV
        double RR -> Valor do intervalo RR em segundos
        double QT -> Valor do intervalo QT em segundos
        double QRS -> Valor do intervalo QRS em segundos
    
    - retorno: Retorna o ponteiro da estrutura "ECG"
*/
ECG createECG(double P, double Q, double R, double S, double T, double RR, double QT, double QRS);

/*
Retorna o valor da onda P do ECG passado por parametro.
    - parametros:
        ECG ecg -> Ponteiro para estrutura do ECG.
    
    - retorno: Valor da onda P do ECG.
*/
double getPValue(ECG ecg);

/*
Retorna o valor da onda Q do ECG passado por parametro.
    - parametros:
        ECG ecg -> Ponteiro para estrutura do ECG.
    
    - retorno: Valor da onda Q do ECG.
*/
double getQValue(ECG ecg);

/*
Retorna o valor da onda R do ECG passado por parametro.
    - parametros:
        ECG ecg -> Ponteiro para estrutura do ECG.
    
    - retorno: Valor da onda R do ECG.
*/
double getRValue(ECG ecg);

/*
Retorna o valor da onda S do ECG passado por parametro.
    - parametros:
        ECG ecg -> Ponteiro para estrutura do ECG.
    
    - retorno: Valor da onda S do ECG.
*/
double getSValue(ECG ecg);

/*
Retorna o valor da onda T do ECG passado por parametro.
    - parametros:
        ECG ecg -> Ponteiro para estrutura do ECG.
    
    - retorno: Valor da onda T do ECG.
*/
double getTValue(ECG ecg);

/*
Retorna o valor do intervalo RR do ECG passado por parametro.
    - parametros:
        ECG ecg -> Ponteiro para estrutura do ECG.
    
    - retorno: Valor do intervalo RR do ECG.
*/
double getRRValue(ECG ecg);

/*
Retorna o valor do intervalo QT do ECG passado por parametro.
    - parametros:
        ECG ecg -> Ponteiro para estrutura do ECG.
    
    - retorno: Valor do intervalo QT do ECG.
*/
double getQTValue(ECG ecg);

/*
Retorna o valor do intervalo QRS do ECG passado por parametro.
    - parametros:
        ECG ecg -> Ponteiro para estrutura do ECG.
    
    - retorno: Valor do intervalo QRS do ECG.
*/
double getQRSValue(ECG ecg);

Lista JsonArrayToECG(char * str);

#endif
