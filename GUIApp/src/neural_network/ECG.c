#include"ECG.h"
#include<stdio.h>
#include<stdlib.h>
#include "json/cJSON.h"
#include "Lista.h"

typedef struct elemento{
    double P, Q, R, S, T, RR, QT, QRS;
}ECGImpl;

ECG createECG(double P, double Q, double R, double S, double T, double RR, double QT, double QRS){
    ECGImpl* ecg = malloc(sizeof(ECGImpl));
    ecg->P = P;
    ecg->Q = Q;
    ecg->R = R;
    ecg->S = S;
    ecg->T = T;
    ecg->RR = RR;
    ecg->QT = QT;
    ecg->QRS = QRS;
    return ecg;
}

double getPValue(ECG ecg){
    ECGImpl* e = ecg;
    return e->P;
}

double getQValue(ECG ecg){
    ECGImpl* e = ecg;
    return e->Q;
}

double getRValue(ECG ecg){
    ECGImpl* e = ecg;
    return e->R;
}

double getSValue(ECG ecg){
    ECGImpl* e = ecg;
    return e->S;
}

double getTValue(ECG ecg){
    ECGImpl* e = ecg;
    return e->T;
}

double getRRValue(ECG ecg){
    ECGImpl* e = ecg;
    return e->RR;
}

double getQTValue(ECG ecg){
    ECGImpl* e = ecg;
    return e->QT;
}

double getQRSValue(ECG ecg){
    ECGImpl* e = ecg;
    return e->QRS;
}

Lista JsonArrayToECG(char * str){
    cJSON *jsonArray = cJSON_Parse(str);
    if (jsonArray == NULL)
    {
        char *error_pointer = cJSON_GetErrorPtr();
        printf("Error at position %i (%p)\n", (int)error_pointer - (int)str, (void*)error_pointer);
        printf("Error before: %s\n", error_pointer);
    }
    cJSON *jsonItem = NULL;
    Lista list = NULL;
    int n = cJSON_GetArraySize(jsonArray);
    cJSON_ArrayForEach(jsonItem, jsonArray) {
        double P=-1, Q=-1, R=-1, S=-1, T=-1,  RR=-1, QT=-1, QRS=-1;
        cJSON *obj = cJSON_GetObjectItemCaseSensitive(jsonItem, "P");
        if (cJSON_IsNumber(obj)){
            P = obj->valuedouble;
        }
        obj = cJSON_GetObjectItemCaseSensitive(jsonItem, "Q");
        if (cJSON_IsNumber(obj)){
            Q = obj->valuedouble;
        }
        obj = cJSON_GetObjectItemCaseSensitive(jsonItem, "R");
        if (cJSON_IsNumber(obj)){
            R = obj->valuedouble;
        }
        obj = cJSON_GetObjectItemCaseSensitive(jsonItem, "S");
        if (cJSON_IsNumber(obj)){
            S = obj->valuedouble;
        }
        obj = cJSON_GetObjectItemCaseSensitive(jsonItem, "T");
        if (cJSON_IsNumber(obj)){
            T = obj->valuedouble;
        }
        obj = cJSON_GetObjectItemCaseSensitive(jsonItem, "RR");
        if (cJSON_IsNumber(obj)){
            RR = obj->valuedouble;
        }
        obj = cJSON_GetObjectItemCaseSensitive(jsonItem, "QT");
        if (cJSON_IsNumber(obj)){
            QT = obj->valuedouble;
        }
        obj = cJSON_GetObjectItemCaseSensitive(jsonItem, "QRS");
        if (cJSON_IsNumber(obj)){
            QRS = obj->valuedouble;
        }
        list = inserir(list, createECG(P, Q, R, S, T, RR, QT, QRS));
    }

    cJSON_Delete(jsonArray);
    return list;
}
