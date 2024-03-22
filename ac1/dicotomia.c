/*
 * TRABALHO AC1 - IMPLEMENTAÇÃO DO MÉTODO DA DICOTOMIA
 *
 * ALUNOS:
 * 210186 - Felipe Mastromauro Corrêa
 * 190276 - Caio Gabriel Machado Vales
 * 210369 - Eduardo Martimiano
 * */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define LEN_INTERVALO 2
#define MAX_MONOMEROS 10
#define MEDIA(a, b) (((a)+(b))*0.5)

/*
 * Definição de um tipo de dado para guardar um monômerio completo de uma
 * função. Isto é, seja um monômero -12.5*x^3, esta estrutura deve conter
 * os valores da base e do expoente. 
 * */

// TODO: LIMPAR BUFFER QUANDO HOUVER DIGITO ERRADO

typedef struct monomero
{
	float base;
	int exp;
} monomero;

/* Definição dos protótipos de funções */

void inicio_grafico();
void ler_grau(int *grau);
void alloc_intervalo(float **i);
void alloc_monomero(monomero **m, int grau);
void ler_funcao(monomero **m, int grau);
void ler_intervalo(float **intervalo, monomero **m, int grau);
void ler_condicao_parada(float *valor_condicao);
float resolve_funcao(monomero **funcao, int grau, float x);
void resolve_dicotomia(monomero **funcao, float **intervalo, int grau, float valor_condicao);

int main()
{
	int grau;
	float parada, *intervalo = NULL;
	monomero *funcao = NULL;
	ler_grau(&grau);
	alloc_monomero(&funcao, grau);
	alloc_intervalo(&intervalo);
	ler_funcao(&funcao, grau);
	ler_intervalo(&intervalo, &funcao, grau);
	ler_condicao_parada(&parada);
	resolve_dicotomia(&funcao, &intervalo, grau, parada);
	free(funcao);
	free(intervalo);
	return 0;
}

/*
 * Marcador gráfico cosmético para melhor visualização do programa.
 * */

void inicio_grafico()
{
	system("clear"); // TROCAR POR system('cls') NO WINDOWS
	printf("<==================== SOLUCIONADOR DE EQUAÇÕES ====================>\n");
	printf("<====================== MÉTODO DA DICOTOMIA ======================>\n\n");
}

/*
 * Função de alocação dos monômeros necessários de acordo com o grau solicitado.
 * */
void alloc_monomero(monomero **m, int grau)
{
	*m = (monomero *)realloc(*m, sizeof(monomero) * (grau + 1));
	if(!*m)
	{
		printf("ERRO: Algo ocorreu com a alocação.\n");
		exit(1);
	}
}

/*
 * Funcão de alocação do intervalo 
 * */
void alloc_intervalo(float **i)
{
	*i = (float *)realloc(*i, sizeof(float) * LEN_INTERVALO);
	if(!*i)
	{
		printf("ERRO: Algo ocorreu com a alocação.\n");
		exit(1);
	}
}

/*
 * Função para leitura do grau da função, que é mantido entre 2 e 10;
 * */
void ler_grau(int *grau)
{	
	do 
	{
		inicio_grafico();
		printf("ESCREVA O VALOR DO GRAU DA EQUACAO (max. 10, min. 2)\nENTRADA: ");
		scanf("%d", grau);
	} while (*grau > MAX_MONOMEROS || *grau < LEN_INTERVALO);
}


/*
 * Procedimento para leitura da função que recebe o monômero e os dados
 * da função a fim de guardar as bases e os expoentes desejados.
 * */
void ler_funcao(monomero **m, int grau)
{
	float base = 0;
	for(int i = grau; i >= 0; i--)
	{
		inicio_grafico();
		printf("ESCREVA O VALOR DO TERMO DE %d GRAU\n", i);
		printf("Ex.: Para descrever 12*x^%d, entre 12.\nENTRADA: ", i);
		scanf("%f", &base);
		(*m + grau - i)->exp = i;
		(*m + grau - i)->base = base;
	}
}

/*
 * Procedimento que lê o intervalo a ser testado e verifica se o mesmo
 * obedece aos limites teóricos estabelecidos (seja [a,b], f(a)*f(b) < 0)
 * */
void ler_intervalo(float **intervalo, monomero **m, int grau)
{
	do
	{	
		inicio_grafico();
		printf("ESCREVA O PRIMEIRO NÚMERO DO INTERVALO\nENTRADA: ");
		scanf("%f", (*intervalo));
		printf("ESCREVA O SEGUNDO NÚMERO DO INTERVALO\nENTRADA: ");
		scanf("%f", (*intervalo + 1));
	} while(resolve_funcao(&*m, grau, *(*intervalo)) * 
		resolve_funcao(&*m, grau, *(*intervalo + 1)) >= 0);
}

/*
 * Procedimento para a leitura da condição de parada a ser obedecida nas
 * iterações do método.
 * */
void ler_condicao_parada(float *valor_condicao)
{
	inicio_grafico();
	printf("ESCREVA O VALOR DA CONDIÇÃO DE PARADA\n");
	printf("Ex.: Seja a condição u, as iterações param se |f(m)| <= u\n");
	printf("ENTRADA: ");
	scanf("%f", valor_condicao);
}


/*
 * Função que usa f, seu grau e um valor x para calcular o resultado
 * de f(x).
 * */
float resolve_funcao(monomero **funcao, int grau, float x)
{ 
	float r = 0, r_monomero = 0;
	for(int i = grau; i >= 0; i--) 
	{
		r_monomero = 0;
		r_monomero += pow(x, (*funcao + i)->exp);
		r_monomero *= (*funcao + i)->base;
		r += r_monomero;
	}
	return r;
}

/*
 * Procedimento para calcular o valor mais próximo da raiz que está dentro do
 * intervalo a partir da função definida e do grau.
 *
 * CONDIÇÔES DE PARADA
 * |bk - ak| < u
 * |f(m)| <= u
 * */
void resolve_dicotomia(monomero **funcao, float **intervalo, int grau, float valor_condicao){
	float media = 0;
	float check;
	printf("<============= RESULTADOS =============>\n");
	printf("|   a   |   m   |   b   | f(a) | f(m) | f(b) |\n");
	printf("|-------|-------|-------|------|------|------|\n");
	float *resultados = malloc(sizeof(float) * 3);
	do
	{
		media = MEDIA(*(*intervalo), *(*intervalo + 1));
		printf("|%7.3f|%7.3f|%7.3f|", *(*intervalo), media, *(*intervalo + 1));
		*(resultados) = resolve_funcao(&*funcao, grau, *(*intervalo));
		*(resultados + 1) = resolve_funcao(&*funcao, grau, media);
		*(resultados + 2) = resolve_funcao(&*funcao, grau, *(*intervalo + 1));
		printf("%6.3f|%6.3f|%6.3f|\n", *(resultados), *(resultados + 1), *(resultados + 2));
		printf("|-------|-------|-------|------|------|------|\n");
		if(*(resultados) * *(resultados + 1) < 0)
		{
			*(*intervalo + 1) = media;
		}
		else if(*(resultados + 1) * *(resultados + 2) < 0)
		{
			*(*intervalo) = media;
		}
		check = fabsf(*(*intervalo) - *(*intervalo + 1));
	} while (fabsf(*(resultados + 1)) > valor_condicao && check >= valor_condicao);
	printf("RESULTADO MAIS PROXIMO: %6.3f\n", media);

}

