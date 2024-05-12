#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define LEN_INTERVALO 2
#define MAX_MONOMIOS 10

#define MIN_PONTOS 2
#define MEDIA(a, b) (((a)+(b))*0.5)

void alloc_vetor(float **v, int tam);
void ler_vetores_xy(float **x, float **y, int tam);
void ler_grau(int *grau);
void resolve_reta(float **x, float **y, int tam);
void imprime(float **m, int linha, int coluna);
void resolve_parabola(float **x, float **y, int tam);
void eliminacao_gauss(float **matriz, int linha, int coluna);
void obter_coefs(float **matriz, int linha, int coluna, int grau);
void run_mmq();

int main(){
    run_mmq();
    return 0;
}

void alloc_vetor(float **v, int tam){
    *v = (float *)realloc(*v, sizeof(float) * (tam));
	if(!*v)
	{
		printf("ERRO: Algo ocorreu com a alocação.\n");
		exit(1);
	}
}

void ler_vetores_xy(float **x, float **y, int tam){
    for(int i = 0; i < tam; i++){
        printf("\nEscreva o valor de x[%d]: ", i);
        scanf("%f", (*x + i));
    }
    for(int i = 0; i < tam; i++){
        printf("Escreva o valor de y[%d]: ", i);
        scanf("%f", (*y + i));
    }
}

void ler_grau(int *grau){
    do{
        printf("\nEscreva o valor do grau para o ajuste: ");
        scanf("%d", grau);
    }while(*grau < 0 || *grau > 2);
}

float produto_vetores(float **v1, float **v2, int tam){
    float res = 0;
    for(int i = 0; i < tam; i++){
        res += (*(*v1 + i) * *(*v2 + i));
    }
    return res;
}

void resolve_reta(float **x, float **y, int tam){
    float *u0 = NULL;
    alloc_vetor(&u0, tam);
    for(int i = 0; i < tam; i++){
        *(u0 + i) = 1;
    }
    // printar y, u0 e u1;

    float *matriz = NULL;
    alloc_vetor(&matriz, 6);
    //coisa feia
    *(matriz+0*3+0) = produto_vetores(&u0, &u0, tam);
    *(matriz+0*3+1) = produto_vetores(&u0, x, tam);
    *(matriz+0*3+2) = produto_vetores(y, &u0, tam);
    *(matriz+1*3+0) = produto_vetores(&u0, x, tam);
    *(matriz+1*3+1) = produto_vetores(x, x, tam);
    *(matriz+1*3+2) = produto_vetores(y, x, tam);
    imprime(&matriz, 2, 3);
    eliminacao_gauss(&matriz, 2, 3);
    imprime(&matriz, 2, 3);
    obter_coefs(&matriz, 2, 3, 1);
}

// não vai usar provavelmente
void imprime(float **m, int linha, int coluna)
{
	int l, c;
	for(l=0;l<linha;l++)
	{
	   for(c=0;c<coluna;c++)
	   {
	        printf("%f \t", *(*m+l*coluna+c));		// conte�do de cada elemento
	   }
	   printf("\n");
	}
}

void eliminacao_gauss(float **matriz, int linha, int coluna){
    float m;
    for(int i = 0; i < coluna; i++){
        for(int j = i + 1; j < linha; j++){
            // gera o m da linha
            m = *(*matriz+j*coluna+i) / *(*matriz+i*coluna+i);
            //multiplica a linha pelo m
            for(int k = 0; k < coluna; k++){
                *(*matriz+j*coluna+k) = *(*matriz+j*coluna+k) - *(*matriz+(j-1)*coluna+k)*m;
            }
        }
    }
}

void obter_coefs(float **matriz, int linha, int coluna, int grau){
    float *coefs = NULL;
    alloc_vetor(&coefs, linha);
    for(int i = 0; i < linha; i++){
        *(coefs + i) = 1;
    }
    if(grau == 1){
        *(coefs + 1) = *(*matriz+1*coluna+2)/ *(*matriz+1*coluna+1);
        *(coefs) = (*(*matriz+0*coluna+2) - *(*matriz+0*coluna+1)* *(coefs + 1)) / *(*matriz+0*coluna+0);
        printf("a0 = %f \t a1 = %f", *(coefs), *(coefs + 1));
    }
    else{
        //
    }
    
}

void run_mmq(){
    float *x = NULL;
    float *y = NULL;
    int tam, grau;
    printf("Qual o tamanho da tabela: ");
    scanf("%d", &tam);
    alloc_vetor(&x, tam);
    alloc_vetor(&y, tam);
    ler_vetores_xy(&x, &y, tam);
    ler_grau(&grau);
    if(grau == 1){
        resolve_reta(&x, &y, tam);
    }
    /*else if(grau == 2){
        resolve_parabola(&x, &y, tam);
    }*/
    free(x);
    free(y);
}
