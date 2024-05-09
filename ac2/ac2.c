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


/*
 * PARA METODO MINIMOS QUADRADOS
 * */
typedef struct tabela
{
	float x;
	float f_x;
} tabela;

void alloc_tabela(tabela **t, int tam);

int main()
{
	char option;
	printf("ENTRE (0) PARA DESCREVER UMA RETA E (1) PARA UMA PARABOLA: ");
	scanf(" %c", &option);
	if(option == '0'){
		printf("RETA\n");
	}
	else if(option == '1'){
		printf("PARABOLA\n");
	}
	return 0;
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

/*
 * Procedimento para ler o número de pontos da tabela.
 * */
void ler_num_pontos(int *tam)
{
	char a = 0;
	do
	{
		if(a == 1)
		{
			printf("INSIRA UM VALOR VÁLIDO (NUM > 1).\n");
		}
		a = 1;
		printf("ESCREVA O NÚMERO DE PONTOS DE SUA TABELA: ");
		scanf("%d", tam);
		fflush(stdin);
	}while(*tam < 2);
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

void escreve_tabela(tabela **t, int tam)
{
	system("clear");
	inicio_grafico_lagrange();
	printf("<======================= TABELA ========================>\n");
	printf("|   x   |  f(x)  |\n");
	printf("|-------|--------|\n");
	for(int i = 0; i < tam; i++)
	{
		printf("|%7.3f|%8.3f|\n", (*t + i)->x, (*t + i)->f_x);
		printf("|-------|--------|\n");
	}
}

void aloca(int **p, int tam)
{
	if((*p = (int *)realloc(*p, tam*sizeof(int))) == NULL){
		print("xabu e silva");
	}
}

void alloc_vetor(float **v, int tam)
{
	
	if((*v = (float *)realloc(*v, tam*sizeof(float))) == NULL)
		print("xabu e silva");
}

void criacao_vetores(char option, int tam)
{
	if(option == '0')
	{
		float *u0 = NULL;
		float *u1 = NULL;
		float *y = NULL;
	}
}
