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
#define MAX_MONOMIOS 10
#define MEDIA(a, b) (((a)+(b))*0.5)

/*
 * Definição de um tipo de dado para guardar um monômerio completo de uma
 * função. Isto é, seja um monômero -12.5*x^3, esta estrutura deve conter
 * os valores da base e do expoente. 
 * */

// TODO: LIMPAR BUFFER QUANDO HOUVER DIGITO ERRADO

typedef struct monomio
{
	float base;
	int exp;
} monomio;

typedef struct tabela 
{
	float x;
	float f_x;
} tabela;

/* Definição dos protótipos de funções e procedimentos para dicotomia */

/* Procedimentos de alocação de memória */
void alloc_intervalo(float **i);
void alloc_monomio(monomio **f, int grau);

/* Procedimentos de leitura de valores */
void ler_grau(int *grau);
void ler_funcao(monomio **f, int grau);
void ler_intervalo(float **i, monomio **f, int grau);
void ler_condicao_parada(float *cond);

/* Procedimentos de escrita gráfica */
void inicio_grafico();
void escreve_funcao(monomio **f, int grau);

/* Procedimento de resolução de método */
void resolve_dicotomia(monomio **f, float **i, int grau, float cond);

/* Função para resolução da função f(x) */
float resolve_funcao(monomio **f, int grau, float x);

/* Procedimento para realizar todo o processo */

void run_dicotomia();

/* Definição dos protótipos de funções e procedimentos para LaGrange */

void alloc_tabela(tabela **t, int tam);
void alloc_l(float **l, int tam);
float resolve_lagrange(tabela **t, float **l, int tam, float valor);
void ler_tabela(tabela **t, int tam);
void ler_num_pontos(int *tam);
void ler_x_interpolar(float *valor);
void resultados_lagrange(float **l, int tam, float valor, float res);

void run_lagrange();

int main()
{
	int i;
	printf("ESCOLHA O MÉTODO QUE IRÁ UTILIZAR\n");
	printf("LAGRANGE [0] OU DICOTOMIA [1]: ");
	while(scanf("%d", &i) != 0)
	{
		fflush(stdin);
		switch(i)
		{
			case 0:
				system("clear");
				run_lagrange();
				break;
			case 1:
				system("clear");
				run_dicotomia();
				break;
			default:
				printf("ESCOLHA UM MÉTODO VÁLIDO.");
		}
		printf("ESCOLHA O MÉTODO QUE IRÁ UTILIZAR\n");
		printf("LAGRANGE [0] OU DICOTOMIA [1]: ");
	}
	
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
void alloc_monomio(monomio **f, int grau)
{
	*f = (monomio *)realloc(*f, sizeof(monomio) * (grau + 1));
	if(!*f)
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
		fflush(stdin);
	} while (*grau > MAX_MONOMIOS || *grau < LEN_INTERVALO);
}


/*
 * Procedimento para leitura da função que recebe o monômero e os dados
 * da função a fim de guardar as bases e os expoentes desejados.
 * */
void ler_funcao(monomio **f, int grau)
{
	float base = 0;
	for(int i = grau; i >= 0; i--)
	{
		inicio_grafico();
		printf("ESCREVA O VALOR DO TERMO DE %d GRAU\n", i);
		printf("Ex.: Para descrever 12*x^%d, entre 12.\nENTRADA: ", i);
		scanf("%f", &base);
		fflush(stdin);
		(*f + grau - i)->exp = i;
		(*f + grau - i)->base = base;
	}
}

/*
 * Procedimento que lê o intervalo a ser testado e verifica se o mesmo
 * obedece aos limites teóricos estabelecidos (seja [a,b], f(a)*f(b) < 0)
 * */
void ler_intervalo(float **i, monomio **f, int grau)
{
	char a = 0;
	do
	{	
		inicio_grafico();
		if(a != 0) printf("INTERVALO INVÁLIDO.\n");
		printf("ESCREVA O PRIMEIRO NÚMERO DO INTERVALO\nENTRADA: ");
		scanf("%f", (*i));
		fflush(stdin);
		printf("ESCREVA O SEGUNDO NÚMERO DO INTERVALO\nENTRADA: ");
		scanf("%f", (*i + 1));
		fflush(stdin);
		a = 1;
	} while(resolve_funcao(&*f, grau, *(*i)) * 
		resolve_funcao(&*f, grau, *(*i + 1)) >= 0);
}

/*
 * Procedimento para a leitura da condição de parada a ser obedecida nas
 * iterações do método.
 * */
void ler_condicao_parada(float *cond)
{
	inicio_grafico();
	printf("ESCREVA O VALOR DA CONDIÇÃO DE PARADA\n");
	printf("Ex.: Seja a condição u, as iterações param se |f(m)| <= u\n");
	printf("ENTRADA: ");
	scanf("%f", cond);
	fflush(stdin);
}

/*
 * Função que usa f, seu grau e um valor x para calcular o resultado
 * de f(x).
 * */
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

/*
 * Procedimento que imprime a função descrita no vetor de monômeros.
 * */
void escreve_funcao(monomio **f, int grau)
{
	int i;
	printf("f(x) = ");
	for(int i = grau; i > 0; i--)
	{
		printf("(%6.3f)x^%d + ", (*f + i)->base, (*f + i)->exp);
	}
	printf("(%6.3f)x^%d\n", (*f + i)->base, (*f + i)->exp);
}

/*
 * Procedimento para calcular o valor mais próximo da raiz que está dentro do
 * intervalo a partir da função definida e do grau.
 *
 * CONDIÇÔES DE PARADA
 * |bk - ak| < u
 * |f(m)| <= u
 * */
void resolve_dicotomia(monomio **f, float **i, int grau, float cond){
	float media = 0;
	float check;
	//escreve_funcao(&*funcao, grau);
	printf("<============= RESULTADOS =============>\n");
	printf("|   a   |   m   |   b   | f(a) | f(m) | f(b) |\n");
	printf("|-------|-------|-------|------|------|------|\n");
	float *res = malloc(sizeof(float) * 3);
	do
	{
		media = MEDIA(*(*i), *(*i + 1));
		printf("|%7.3f|%7.3f|%7.3f|", *(*i), media, *(*i + 1));
		*(res) = resolve_funcao(&*f, grau, *(*i));
		*(res + 1) = resolve_funcao(&*f, grau, media);
		*(res + 2) = resolve_funcao(&*f, grau, *(*i + 1));
		printf("%6.3f|%6.3f|%6.3f|\n", *(res), *(res + 1), *(res + 2));
		printf("|-------|-------|-------|------|------|------|\n");
		if(*(res) * *(res + 1) < 0)
		{
			*(*i + 1) = media;
		}
		else if(*(res + 1) * *(res + 2) < 0)
		{
			*(*i) = media;
		}
		// Possivelmente errado
		check = fabsf(*(*i) - *(*i + 1));
	} while (fabsf(*(res + 1)) > cond && check >= cond);
	printf("RESULTADO MAIS PROXIMO: %6.3f\n", media);
	free(res);
}

void run_dicotomia()
{
	int grau;
	float cond, *intervalo = NULL;
	monomio *funcao = NULL;
	ler_grau(&grau);
	alloc_monomio(&funcao, grau);
	alloc_intervalo(&intervalo);
	ler_funcao(&funcao, grau);
	ler_intervalo(&intervalo, &funcao, grau);
	ler_condicao_parada(&cond);
	resolve_dicotomia(&funcao, &intervalo, grau, cond);
	free(funcao);
	free(intervalo);
}

void ler_num_pontos(int *tam)
{
	printf("ESCREVA O NÚMERO DE PONTOS DE SUA TABELA: ");
	scanf("%d", tam);
	fflush(stdin);
}

void ler_x_interpolar(float *valor)
{
	printf("ESCREVA O VALOR DE X PARA A INTERPOLAÇÃO: ");
	scanf("%f", valor);
	fflush(stdin);
}

void alloc_tabela(tabela **t, int tam)
{
	*t = (tabela *) realloc(*t, sizeof(tabela) * (tam));
	if(!*t)
	{
		printf("ERRO: Algo ocorreu com a alocação.\n");
		exit(1);
	}
}

void alloc_l(float **l, int tam)
{
	*l = (float *) realloc(*l, sizeof(float) * (tam));
	if(!*l)
	{
		printf("ERRO: Algo ocorreu com a alocação.\n");
		exit(1);
	}
}

void ler_tabela(tabela **t, int tam)
{
	printf("PREENCHIMENTO DE TABELA\n");
	for(int i = 0; i < tam; i++)
	{
		printf("\nENTRE x[%d]: ", i);
		scanf("%f", &(*t + i)->x);
		fflush(stdin);
	}
	for(int i = 0; i <  tam; i++)
	{
		printf("\nENTRE f(x[%d]): ", i);
		scanf("%f", &(*t + i)->f_x);
		fflush(stdin);
	}
}

float resolve_lagrange(tabela **t, float **l, int tam, float valor)
{
	float p_x = 0;
	for(int i = 0; i < tam; i++)
	{
		float l_i = 1;
		for(int j = 0; j < tam; j++)
		{
			if(i != j)
				l_i *= (valor - (*t + j)->x)/((*t + i)->x - (*t + j)->x);
		}
		*(*l + i) = l_i;
		p_x += l_i * (*t + i)->f_x;
	}
	return p_x;
}

void resultados_lagrange(float **l, int tam, float valor, float res)
{
	printf("RESULTADOS\n");
	for(int i = 0; i < tam; i++)
	{
		printf("Valor de L[%d]: %2.4f\n", i, *(*l + i));
	}
	printf("O valor de f(x) em %2.4f é %2.4f\n", valor, res);
}

void run_lagrange()
{
	tabela *t = NULL;
	float *l = NULL;
	float valor;
	int tam;
	
	ler_num_pontos(&tam);
	alloc_l(&l, tam);
	alloc_tabela(&t, tam);
	ler_tabela(&t, tam);
	ler_x_interpolar(&valor);
	float res = resolve_lagrange(&t, &l, tam, valor);
	resultados_lagrange(&l, tam, valor, res);
	free(t);
	free(l);
}
