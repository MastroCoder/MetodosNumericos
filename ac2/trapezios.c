#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define LEN_INTERVALO 2
#define MAX_MONOMIOS 10

#define MIN_PONTOS 2
#define MEDIA(a, b) (((a)+(b))*0.5)

typedef struct monomio
{
	float base;
	int exp;
} monomio;

void alloc_monomio(monomio **f, int grau);
void alloc_intervalo(float **i);
void ler_grau(int *grau);
void ler_funcao(monomio **f, int grau);
void ler_num_divisoes(float *num_div);
void escreve_funcao(monomio **f, int grau);
void ler_intervalo(float **i, monomio **f, int grau);
float resolve_funcao(monomio **f, int grau, float x);
void calcula_itr(monomio **funcao, int grau, float **i, float *num_div);
void run_trapezios();

int main(){
    run_trapezios();
    return 0;
}

void ler_grau(int *grau)
{	
	do 
	{
		printf("ESCREVA O VALOR DO GRAU DA EQUACAO (max. 10, min. 2)\nENTRADA: ");
		scanf("%d", grau);
		fflush(stdin);
	} while (*grau > MAX_MONOMIOS || *grau < LEN_INTERVALO);
}

void alloc_monomio(monomio **f, int grau)
{
	*f = (monomio *)realloc(*f, sizeof(monomio) * (grau + 1));
	if(!*f)
	{
		printf("ERRO: Algo ocorreu com a alocação.\n");
		exit(1);
	}
}

void alloc_intervalo(float **i)
{
       	*i = (float *)realloc(*i, sizeof(float) * LEN_INTERVALO);
	if(!*i)
	{
		printf("ERRO: Algo ocorreu com a alocação.\n");
		exit(1);
	}
}

void ler_funcao(monomio **f, int grau)
{
	float base = 0;
	for(int i = grau; i >= 0; i--)
	{
		if(i < grau)
		{
			escreve_funcao(&*f, grau - i - 1);
		}
		printf("ESCREVA O VALOR DO TERMO DE %d GRAU\n", i);
		printf("Ex.: Para descrever 12*x^%d, entre 12.\nENTRADA: ", i);
		scanf("%f", &base);
		fflush(stdin);
		(*f + grau - i)->exp = i;
		(*f + grau - i)->base = base;
	}
}

void escreve_funcao(monomio **f, int grau)
{
	printf("\n<=================== FUNÇÃO DESCRITA =====================>\n");
	printf("\nf(x) = ");
	for(int i = 0; i < grau; i++)
	{
		printf("(%6.3f)x^%d + ", (*f + i)->base, (*f + i)->exp);
	}
	printf("(%6.3f)x^%d\n", (*f + grau)->base, (*f + grau)->exp);
	printf("\n<=========================================================>\n");
}

void ler_intervalo(float **i, monomio **f, int grau)
{
    escreve_funcao(&*f, grau);
    printf("ESCREVA O PRIMEIRO NÚMERO DO INTERVALO\nENTRADA: ");
    scanf("%f", (*i));
    fflush(stdin);
    printf("ESCREVA O SEGUNDO NÚMERO DO INTERVALO\nENTRADA: ");
    scanf("%f", (*i + 1));
    fflush(stdin);
    if(*(*i) > *(*i + 1)){
        float temp;
        temp = *(*i);
        *(*i) = *(*i + 1);
        *(*i + 1) = temp;
    } 
}

void ler_num_divisoes(float *num_div){
    fflush(stdin);
    printf("ESCREVA O NÚMERO DE DIVISÕES DOS TRAPÉZIOS (n): ");
    scanf("%f", num_div);
}

float resolve_funcao(monomio **f, int grau, float x)
{ 
	float res_ant = 0, res = 0;
	for(int i = grau; i >= 0; i--) 
	{
		res_ant = 0;
		res_ant += pow(x, (*f + i)->exp);
		res_ant *= (*f + i)->base;
		res += res_ant;
	}
	return res;
}


void calcula_itr(monomio **funcao, int grau, float **i, float *num_div){
    float h = (*(*i + 1) - *(*i)) / *num_div;
	float f_x1 = resolve_funcao(&*funcao, grau, *(*i));
	float f_x2 = resolve_funcao(&*funcao, grau, *(*i + 1));
	float itr = f_x1 + f_x2;
	printf("x 	f(x)\n");
	printf("%.2f 	%.2f\n", *(*i), f_x1);
	for(float step = *(*i) + h; step < *(*i + 1); step += h){
		float f_xi = resolve_funcao(&*funcao, grau, step);
		itr += 2 * f_xi;
		printf("%.2f 	%.2f\n", step, f_xi);
	}
	printf("%.2f 	%.2f\n", *(*i + 1), f_x2);
	itr *= h/2;
	printf("ITR = %f\n", itr);
    // calculo dos trapezios

}

void run_trapezios(){
    int grau;
    float num_div;
	float *intervalo = NULL;
    monomio *funcao = NULL;
    ler_grau(&grau);
    alloc_monomio(&funcao, grau);
	alloc_intervalo(&intervalo);
	ler_funcao(&funcao, grau);
	ler_intervalo(&intervalo, &funcao, grau);
    ler_num_divisoes(&num_div);
    calcula_itr(&funcao, grau, &intervalo, &num_div);
    free(funcao);
	free(intervalo);
}