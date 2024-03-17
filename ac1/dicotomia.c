/*
 * TRABALHO AC1 - IMPLEMENTAÇÃO DO MÉTODO DA DICOTOMIA
 *
 * ALUNOS:
 * 210186 - Felipe Mastromauro Corrêa
 * */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
 * Definição de um tipo de dado para guardar um monômerio completo de uma
 * função. Isto é, seja um monômero -12.5*x^3, esta estrutura deve conter
 * os três valores 
 * */

typedef struct monomero
{
	float base;
	float x; // Talvez seja interessante a remoção
	int exp;
} monomero;

/* Definição dos protótipos de funções */

void inicio_grafico();
void ler_grau(int *grau);
void ler_funcao(monomero *m, int grau);
void ler_intervalo(float *intervalo, monomero *m);
void ler_condicao_parada(float *valor_condicao);
float *alloc_intervalo();
float media(float a, float b);
monomero *alloc_monomero(int grau);
void resolve_funcao(monomero *funcao, int grau, float *intervalo, float x);

/* TODO: PASSAR POR REFERÊNCIA (malloc vira realloc com cast) */
int main()
{
	int grau;
	float parada;
	ler_grau(&grau);
	monomero *funcao = alloc_monomero(grau);
	float *intervalo = alloc_intervalo();
	ler_funcao(funcao, grau);
	ler_intervalo(intervalo, funcao);
	ler_condicao_parada(&parada);
	resolve_funcao(funcao, grau, intervalo, 0);
	free(funcao);
	free(intervalo);
	return 0;
}

void inicio_grafico()
{
	system("clear"); // TROCAR POR system('cls') NO WINDOWS
	printf("<===== SOLUCIONADOR DE EQUAÇÕES =====>\n");
	printf("<===== MÉTODO DA DICOTOMIA  =====>\n\n");
}

monomero *alloc_monomero(int grau)
{
	monomero *m = malloc(sizeof(monomero) * (grau + 1));
	if(m) return m;
	return NULL;
}

float *alloc_intervalo()
{
	float *intervalo = malloc(sizeof(float) * 2);
	if(intervalo) return intervalo;
	return NULL;
}

float media(float a, float b) // pode ser feito como macro
{
	return (a + b) * 0.5;
}

void ler_grau(int *grau)
{	
	do 
	{
		inicio_grafico();
		printf("ESCREVA O VALOR DO GRAU DA EQUACAO (max. 10, min. 2)\nENTRADA: ");
		scanf("%d", grau);
	} while (*grau > 10 || *grau <= 1);
}

void ler_funcao(monomero *m, int grau)
{
	float base = 0;
	for(int i = grau; i >= 0; i--)
	{
		printf("ESCREVA O VALOR DO TERMO DE %d GRAU\n", i);
		printf("Ex.: Para descrever 12*x^%d, entre 12.\nENTRADA: ", grau);
		scanf("%f", &base);
		(m + grau - i)->exp = i;
		(m + grau - i)->base = base;
	}
}

void ler_intervalo(float *intervalo, monomero *m)
{
	// TODO: do-while para checar se f(a)*f(b) > 0
	printf("ESCREVA O PRIMEIRO NÚMERO DO INTERVALO\nENTRADA: ");
	scanf("%f", (intervalo));
	printf("ESCREVA O SEGUNDO NÚMERO DO INTERVALO\nENTRADA: ");
	scanf("%f", (intervalo + 1));
}

void ler_condicao_parada(float *valor_condicao)
{
	inicio_grafico();
	printf("ESCREVA O VALOR DA CONDIÇÃO DE PARADA\n");
	printf("Ex.: Seja a condição u, as iterações param se |f(m)| <= u\n");
	printf("ENTRADA: ");
	scanf("%f", valor_condicao);
}



void resolve_funcao(monomero *funcao, int grau, float *intervalo, float x)
{ 
	printf("chegou no solve\n");
	for(int i = grau; i >= 0; i--) printf("%2.2f\n", (funcao + i)->base);
	for(int i = 0; i < 2; i++) printf("%2.2f\n", *(intervalo + i));
}
