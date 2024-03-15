#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Functions
void recebeParametros(float *p, char **argv, int tam);
float calculaDelta(float *p);
void calculaRaizes(float *p, float *r, float d);
void mostraRaizes(float *r, float delta);
void alocaFloat(float **p, int tam);

int main(int argc, char **argv)
{
    float *coefs = NULL;
    float *raizes = NULL;
    float delta;
    alocaFloat(&coefs, argc - 1);
    alocaFloat(&raizes, 2);
    recebeParametros(coefs, argv, argc - 1);
    printf("COEFICIENTES: %4.2f %4.2f %4.2f\n", *coefs, *(coefs + 1), *(coefs + 2));
    delta = calculaDelta(coefs);
    if (delta < 0)
    {
        printf("DELTA = %4.2f\nRaizes nao reais.\n", delta);
        return -1;
    }
    calculaRaizes(coefs, raizes, delta);
    mostraRaizes(raizes, delta);
    return 0;
}

void alocaFloat(float **p, int tam)
{
    if ((*p = (float *)realloc(*p, tam * sizeof(float))) == NULL)
    {
        printf("Erro na alocacao!");
        exit(1);
    }
    printf("Espaco alocado com sucesso!\n");
}

void recebeParametros(float *p, char **argv, int tam)
{
    for(int i = tam; i > 0; i--) *(p + (i - 1)) = atof(*(argv + i));
}

float calculaDelta(float *p)
{
    return pow(*(p + 1), 2) - (4 * *(p) * *(p + 2));
}

void calculaRaizes(float *p, float *r, float d)
{
    *r = ((-1) * (*(p + 1)) + sqrt(d)) / (*p * 2);
    *(r + 1) = ((-1) * *(p + 1) - sqrt(d)) / (2 * *(p));
}

void mostraRaizes(float *r, float delta)
{
    printf("DELTA = %4.2f\n", delta);
    printf("\nRAIZES:\nX1: %4.2f\nX2: %4.2f\n", *r, *(r + 1));
}