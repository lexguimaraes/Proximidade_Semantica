

typedef struct Palavra{
    char palavra[101];//101 pois tem uma palavra com 70+chars no arquivo, linha 65190 alguma coisa
    char categoria[16];
    double vetor[50];
}palavra;



palavra* palavra_cria(const char* categoria, const double* vetor,const char*palavra1,int size);

void palavra_libera(palavra* palavra1);

double cosin(palavra* palavra1,palavra* palavra2,int size);

double dist_euclid(palavra* palavra1,palavra* palavra2,int size);

double normavetor(palavra* palavra1,int size);

double multvetor(palavra* palavra1,palavra* palavra2,int size);

int norma_compara(palavra* palavra1,palavra* palavra2,int size);

int palavra_compara(palavra* palavra1,palavra* palavra2,int size);
