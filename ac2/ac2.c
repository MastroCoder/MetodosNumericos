/*
 * TRABALHO - IMPLEMENTAÇÃO DOS MÉTODOS DOS TRAPÉZIOS E MÍNIMOS QUADRADOS
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

#define MIN_PONTOS 2
#define MEDIA(a, b) (((a) + (b)) * 0.5)

void alloc_vetor(float **v, int tam);
void ler_vetores_xy(float **x, float **y, int tam);
void ler_grau_ajuste(int *grau);
void resolve_reta(float **x, float **y, int tam);
void imprime_matriz(float **m, int linha, int coluna);
void imprime_vetor(float **v, int tam);
void resolve_parabola(float **x, float **y, int tam);
void eliminacao_gauss(float **matriz, int linha, int coluna);
void reorganiza_matriz(float **matriz, int linha, int coluna);
void obter_coefs(float **matriz, int linha, int coluna, int grau);
void run_mmq();

typedef struct monomio
{
	float base;
	int exp;
} monomio;
float mais_perto(float a);
void alloc_monomio(monomio **f, int grau);
void alloc_intervalo(float **i);
void ler_grau_equacao(int *grau);
void ler_funcao(monomio **f, int grau);
void ler_num_divisoes(float *num_div);
void escreve_funcao(monomio **f, int grau);
void ler_intervalo(float **i, monomio **f, int grau);
float resolve_funcao(monomio **f, int grau, float x);
void calcula_itr(monomio **funcao, int grau, float **i, float *num_div);
void run_trapezios();

int main()
{
	int i;
	printf("ESCOLHA O MÉTODO QUE IRÁ UTILIZAR\n");
	printf("MMQ [0] OU TRAPÉZIOS [1] (SAIR: 99): ");
	scanf("%d", &i);
	fflush(stdin);
	switch (i)
	{
	case 0:
		system("clear");
		run_mmq();
		break;
	case 1:
		system("clear");
		run_trapezios();
		break;
	case 99:
		return 0;
	default:
		printf("ESCOLHA UM MÉTODO VÁLIDO.");
	}
	return main();
}

void alloc_vetor(float **v, int tam)
{
	*v = (float *)realloc(*v, sizeof(float) * (tam));
	if (!*v)
	{
		printf("ERRO: Algo ocorreu com a alocação.\n");
		exit(1);
	}
}

void ler_vetores_xy(float **x, float **y, int tam)
{
	for (int i = 0; i < tam; i++)
	{
		printf("\nEscreva o valor de x[%d]: ", i);
		scanf("%f", (*x + i));
	}
	for (int i = 0; i < tam; i++)
	{
		printf("\nEscreva o valor de y[%d]: ", i);
		scanf("%f", (*y + i));
	}
}

void ler_grau_ajuste(int *grau)
{
	do
	{
		printf("\nEscreva o valor do grau para o ajuste: ");
		scanf("%d", grau);
	} while (*grau < 0 || *grau > 2);
}

float produto_vetores(float **v1, float **v2, int tam)
{
	float res = 0;
	for (int i = 0; i < tam; i++)
	{
		res += (*(*v1 + i) * *(*v2 + i));
	}
	return res;
}

void resolve_reta(float **x, float **y, int tam)
{
	float *u0 = NULL;
	alloc_vetor(&u0, tam);
	for (int i = 0; i < tam; i++)
	{
		*(u0 + i) = 1;
	}
	// printar y, u0 e u1;
	printf("VETORES\n\nu0 \t u1 \t y\n");
	for (int i = 0; i < tam; i++)
	{
		printf("%.2f \t %.2f \t %.2f\n", *(u0 + i), *(*x + i), *(*y + i));
	}

	float *matriz = NULL;
	alloc_vetor(&matriz, 6);
	*(matriz + 0 * 3 + 0) = produto_vetores(&u0, &u0, tam);
	*(matriz + 0 * 3 + 1) = produto_vetores(&u0, x, tam);
	*(matriz + 0 * 3 + 2) = produto_vetores(y, &u0, tam);
	*(matriz + 1 * 3 + 0) = produto_vetores(&u0, x, tam);
	*(matriz + 1 * 3 + 1) = produto_vetores(x, x, tam);
	*(matriz + 1 * 3 + 2) = produto_vetores(y, x, tam);

	printf("\nMATRIZ OBTIDA\n\n");
	imprime_matriz(&matriz, 2, 3);
	eliminacao_gauss(&matriz, 2, 3);
	printf("\nELIMINAÇÂO DE GAUSS\n\n");
	imprime_matriz(&matriz, 2, 3);
	obter_coefs(&matriz, 2, 3, 1);
	free(matriz);
	free(u0);
}

void resolve_parabola(float **x, float **y, int tam)
{
	float *u0 = NULL; 
	float *u2 = NULL;
	alloc_vetor(&u0, tam);
	alloc_vetor(&u2, tam);
	for (int i = 0; i < tam; i++)
	{
		*(u0 + i) = 1;
		*(u2 + i) = *(*x + i) * *(*x + i);
	}
	
	// printar y, u0, u1 e u2;
	printf("VETORES\n\nu0 \t u1 \t u2 \t y\n");
	for (int i = 0; i < tam; i++)
	{
		printf("%.2f \t %.2f \t %.2f \t %.2f\n", *(u0 + i), *(*x + i), *(u2 + i), *(*y + i));
	}
	float *matriz = NULL;
	alloc_vetor(&matriz, 12);

	*(matriz + 0 * 4 + 0) = produto_vetores(&u0, &u0, tam);
	*(matriz + 0 * 4 + 1) = produto_vetores(&u0, x, tam);
	*(matriz + 0 * 4 + 2) = produto_vetores(&u2, &u0, tam);
	*(matriz + 0 * 4 + 3) = produto_vetores(y, &u0, tam);

	*(matriz + 1 * 4 + 0) = produto_vetores(&u0, x, tam);
	*(matriz + 1 * 4 + 1) = produto_vetores(x, x, tam);
	*(matriz + 1 * 4 + 2) = produto_vetores(&u2, x, tam);
	*(matriz + 1 * 4 + 3) = produto_vetores(y, x, tam);

	*(matriz + 2 * 4 + 0) = produto_vetores(&u0, &u2, tam);
	*(matriz + 2 * 4 + 1) = produto_vetores(x, &u2, tam);
	*(matriz + 2 * 4 + 2) = produto_vetores(&u2, &u2, tam);
	*(matriz + 2 * 4 + 3) = produto_vetores(y, &u2, tam);

	printf("MATRIZ OBTIDA\n\n");
	imprime_matriz(&matriz, 3, 4);
	eliminacao_gauss(&matriz, 3, 4);
	printf("ELIMINAÇÂO DE GAUSS\n\n");
	imprime_matriz(&matriz, 3, 4);
	obter_coefs(&matriz, 3, 4, 2);
	free(matriz);
	free(u0);
	free(u2);
}

void imprime_matriz(float **m, int linha, int coluna)
{
	int l, c;
	for (l = 0; l < linha; l++)
	{
		for (c = 0; c < coluna; c++)
		{
			printf("%f \t", *(*m + l * coluna + c)); // conte�do de cada elemento
		}
		printf("\n");
	}
}

void reorganiza_matriz(float **matriz, int linha, int coluna)
{
	float a, b, temp;
	// Não é assim
	for (int i = 1; i < linha; i++)
	{
		a = *(*matriz + (i - 1) * coluna + 0);
		b = *(*matriz + i * coluna + 0);
		if (a > b)
		{
			// troca a linha toda
			for (int j = 0; j < coluna; j++)
			{
				temp = *(*matriz + (i - 1) * coluna + j);
				*(*matriz + (i - 1) * coluna + j) = *(*matriz + i * coluna + j);
				*(*matriz + i * coluna + j) = temp;
			}
		}
	}
}

void eliminacao_gauss(float **matriz, int linha, int coluna)
{
	float m;
	// reorganiza_matriz(&*matriz, linha, coluna);
	for (int i = 0; i < coluna; i++)
	{
		for (int j = i + 1; j < linha; j++)
		{
			// gera o m da linha
			m = *(*matriz + j * coluna + i) / *(*matriz + i * coluna + i);
			// multiplica a linha pelo m
			for (int k = 0; k < coluna; k++)
			{
				*(*matriz + j * coluna + k) = *(*matriz + j * coluna + k) - *(*matriz + i * coluna + k) * m;
			}
		}
	}
}

void obter_coefs(float **matriz, int linha, int coluna, int grau)
{
	float *coefs = NULL;
	alloc_vetor(&coefs, linha);
	for (int i = 0; i < linha; i++)
	{
		*(coefs + i) = 1;
	}
	if (grau == 1)
	{
		*(coefs + 1) = *(*matriz + 1 * coluna + 2) / *(*matriz + 1 * coluna + 1);
		*(coefs) = (*(*matriz + 0 * coluna + 2) - *(*matriz + 0 * coluna + 1) * *(coefs + 1)) / *(*matriz + 0 * coluna + 0);
		printf("a0 = %f \t a1 = %f\n", *(coefs), *(coefs + 1));
		printf("\np(x) = %.3f + %.3f*x\n", *(coefs), *(coefs + 1));
	}
	else
	{
		*(coefs + 2) = *(*matriz + 2 * coluna + 3) / *(*matriz + 2 * coluna + 2);
		*(coefs + 1) = (*(*matriz + 1 * coluna + 3) - *(*matriz + 1 * coluna + 2) * *(coefs + 2)) / *(*matriz + 1 * coluna + 1);
		*(coefs) = (*(*matriz + 0 * coluna + 3) - *(*matriz + 0 * coluna + 2) * *(coefs + 2) - *(*matriz + 0 * coluna + 1) * *(coefs + 1)) / *(*matriz + 0 * coluna + 0);
		printf("a0 = %f \t a1 = %f \t a2 = %f\n", *(coefs), *(coefs + 1), *(coefs + 2));
		printf("\np(x) = %.3f + %.3f*x + %.3f*x²\n", *(coefs), *(coefs + 1), *(coefs + 2));
	}
}

void run_mmq()
{
	float *x = NULL;
	float *y = NULL;
	int tam, grau;
	printf("MÉTODO DOS MINIMOS QUADRADOS\n");
	printf("Qual o tamanho da tabela: ");
	scanf("%d", &tam);
	alloc_vetor(&x, tam);
	alloc_vetor(&y, tam);
	ler_vetores_xy(&x, &y, tam);
	char a = 's';
	while(a == 's'){
		ler_grau_ajuste(&grau);
		if (grau == 1)
		{
			resolve_reta(&x, &y, tam);
		}
		else if (grau == 2)
		{
			resolve_parabola(&x, &y, tam);
		}
		fflush(stdin);
		printf("Deseja Calcular outro p(x)? (s/n): ");
		// Espaço para evitar de ler newline ao dar scanf em char
		scanf(" %c", &a);
	}
	free(x);
	free(y);
}

void ler_grau_equacao(int *grau)
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
	if (!*f)
	{
		printf("ERRO: Algo ocorreu com a alocação.\n");
		exit(1);
	}
}

void alloc_intervalo(float **i)
{
	*i = (float *)realloc(*i, sizeof(float) * LEN_INTERVALO);
	if (!*i)
	{
		printf("ERRO: Algo ocorreu com a alocação.\n");
		exit(1);
	}
}

void ler_funcao(monomio **f, int grau)
{
	float base = 0;
	for (int i = grau; i >= 0; i--)
	{
		if (i < grau)
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
	for (int i = 0; i < grau; i++)
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
	if (*(*i) > *(*i + 1))
	{
		float temp;
		temp = *(*i);
		*(*i) = *(*i + 1);
		*(*i + 1) = temp;
		printf("\nINTERVALO INVERTIDO: [%.2f, %.2f]\n", *(*i), *(*i + 1));
	}
}

void ler_num_divisoes(float *num_div)
{
	fflush(stdin);
	printf("ESCREVA O NÚMERO DE DIVISÕES DOS TRAPÉZIOS (n): ");
	scanf("%f", num_div);
}

float resolve_funcao(monomio **f, int grau, float x)
{
	float res_ant = 0, res = 0;
	for (int i = grau; i >= 0; i--)
	{
		res_ant = 0;
		res_ant += pow(x, (*f + i)->exp);
		res_ant *= (*f + i)->base;
		res += res_ant;
	}
	return res;
}

float mais_perto(float a){
	a = roundf(a * 100) / 100;
	return a;
}

void calcula_itr(monomio **funcao, int grau, float **i, float *num_div)
{
	float h = (*(*i + 1) - *(*i)) / *num_div;
	h = mais_perto(h);
	printf("\nh = (%.2f - %.2f)/(%.2f) = %f\n", *(*i + 1), *(*i), *num_div, h);
	float f_x1 = resolve_funcao(&*funcao, grau, *(*i));
	float f_x2 = resolve_funcao(&*funcao, grau, *(*i + 1));
	float itr = f_x1 + f_x2;
	printf("x 	f(x)\n");
	printf("%.2f 	%.2f\n", *(*i), f_x1);
	for (float step = mais_perto(*(*i) + h); step < *(*i + 1); step += h)
	{
		step = mais_perto(step);
		float f_xi = resolve_funcao(&*funcao, grau, step);
		itr += 2 * f_xi;
		printf("%.2f 	%.2f\n", step, f_xi);
	}
	printf("%.2f 	%.2f\n", *(*i + 1), f_x2);
	itr *= h / 2;
	printf("\nITR = %f\n", itr);
}

void run_trapezios()
{
	int grau;
	float num_div;
	float *intervalo = NULL;
	monomio *funcao = NULL;
	char a = 's';
	ler_grau_equacao(&grau);
	alloc_monomio(&funcao, grau);
	alloc_intervalo(&intervalo);
	ler_funcao(&funcao, grau);
	ler_intervalo(&intervalo, &funcao, grau);
	while(a == 's'){
		ler_num_divisoes(&num_div);
		calcula_itr(&funcao, grau, &intervalo, &num_div);
		fflush(stdin);
		printf("Gostaria de calcular com outro número de trapézios? (s/n): ");
		scanf(" %c", &a);
	}
	free(funcao);
	free(intervalo);
}