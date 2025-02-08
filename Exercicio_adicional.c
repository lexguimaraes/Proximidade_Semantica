#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "palavra.h"
#include <float.h>

char categorias[20][16] = {"Veiculos","Brinquedos","Pessoas","Artigos","Simbolos","Animais","Classificadores",
                           "Verbos","Estados","Paises", "Cidades", "Pronomes", "Comidas","Bebidas","Casa",
                           "Roupas","Sons","Quantificadores","Adjetivos","Adverbios"};

typedef struct Node{
    palavra* palavra;
    struct Node* next;

}Node;

void limpar_lista_char(char** lista);

Node* insere(palavra* palavra,Node* head){
    Node* new = malloc(sizeof(Node));
    new->palavra = palavra;
    new->next = head;
    return new;
}

void imprimirlista(Node* head,int dimensao){
    for (Node* p = head; p!=NULL;p = p->next){
        printf("%s              %s        ",p->palavra->palavra, p->palavra->categoria);
        for(int i = 0;i<dimensao;i++){
            printf("%lf ",p->palavra->vetor[i]);
        }
        printf("\n");
    }

}

void exclui_lista(Node*head){
    Node* t;
    Node* p = head;
    while(p!= NULL){
        t = p->next;
        free(p->palavra);
        free(p);
        p = t;
    }

}


int palavra_na_lista(char** lista,char* palavra,int size){
    if (lista == NULL)return 0;
    for(int i = 0;i<size;i++){
        if (strcmp(lista[i],palavra)==0){
            return 1;
        }
    }
    return 0;
}

void imprimirlistaNorma(Node* head){
    for(Node* p = head;p!= NULL; p = p->next){
        printf("%s           %s                %lf\n",p->palavra->palavra, p->palavra->categoria, normavetor(p->palavra,50));
    }
}


void imprimirlistaDist(Node* head,char* palavra1, double(*dist)(palavra*,palavra*,int)){//IMPRIME LISTA E DISTANCIA DE ACORDO COM A FUNCAO DE PARAMETRO
    Node* temp = NULL;
    for(Node* p = head;p!= NULL;p = p->next){
        if (strcmp(p->palavra->palavra,palavra1)==0){
            temp = p;
            break;
        }
    }
    if(temp == NULL)return;
    for(Node* p = head;p!= NULL; p = p->next){
        printf("%s           %s                %lf\n",p->palavra->palavra, p->palavra->categoria, dist(temp->palavra,p->palavra,50));
    }
}


// -- criar uma lista de palavras a partir do arquivo em anexo,
// oriundo de http://nilc.icmc.usp.br/nilc/index.php/repositorio-de-word-embeddings-do-nilc, que contém  palavras e seus vetores.
Node* lerarquivo(Node* head){
    FILE* arquivo = fopen("cbow_s50.txt","r");
    if (arquivo == NULL)return NULL;
    int linhas, dimensao;
    fscanf(arquivo, "%d %d",&linhas, &dimensao);
    double tempvetor[dimensao];
    char temp[101];
    for(int i = 0;i<linhas;i++){
        fscanf(arquivo,"%s",temp);
        for(int j = 0; j < dimensao;j++){
            fscanf(arquivo, "%lf",&tempvetor[j]);
        }
        head = insere(palavra_cria(categorias[rand()%20],tempvetor,temp,dimensao),head);
    }
    fclose(arquivo);
    return head;
}

//-- retornar as palavras da lista ordenadas alfabeticamente
// -- retornar as palavras da lista ordenadas pela norma do vetor
void sort(Node* head,int (*comparacao)(palavra*,palavra*,int)){//ORDENAR PELA FUNCAOO
    palavra* temp;
    if(head == NULL)return;
    if(head->next == NULL)return;
    int swapped = 0;
    Node* p;
    Node* end = NULL;
    do{
        swapped = 0;
        p = head;
        while(p->next!=end){
            if(comparacao(p->palavra,p->next->palavra,50)>0){
                temp = p->palavra;
                p->palavra=p->next->palavra;
                p->next->palavra=temp;
                swapped = 1;
            }
            p = p->next;
        }
        end = p;
    }while(swapped);
}

double dist_palavra(Node* head, double(*dist)(palavra*, palavra*,int),char* palavra1,palavra* palavra2){
    if(head == NULL)return -1;
    if(head->next == NULL)return -1;
    Node* temp1 = NULL;
    for(Node* p = head; p!=NULL;p = p->next){
        if (strcmp(p->palavra->palavra,palavra1)==0){
            temp1 = p;
            break;
        }
    }
    if (temp1==NULL)return -1;
    return dist(temp1->palavra,palavra2,50);



}
void sort_dist(Node* head,double (*dist)(palavra*,palavra*,int),char* palavra1){//ORDENAR PELA FUNCAOO
    palavra* temp;
    if(head == NULL)return;
    if(head->next == NULL)return;
    Node* temp1 = NULL;
    for(Node* p = head; p!=NULL;p = p->next){
        if (strcmp(p->palavra->palavra,palavra1)==0){
            temp1 = p;
            break;
        }
    }
    if (temp1==NULL)return;
    palavra* pivo = temp1->palavra;
    int swapped = 0;
    Node* end = NULL;
    Node* p;
    do{
        p = head;
        swapped = 0;
        while(p->next != end){
            double distp = dist(pivo,p->palavra,50);
            double disti = dist(pivo,p->next->palavra,50);
            if (distp>disti){
                temp = p->palavra;
                p->palavra = p->next->palavra;
                p->next->palavra=temp;
                swapped = 1;
            }
            p=p->next;
        }
        end = p;
    }while(swapped);

}

// -- retornar um vetor de números aleatórios, caso uma palavra não esteja na lista.
double* busca_palavra(Node*head, char*palavra){
    double* vetor = malloc(sizeof(double)*50);
    int cont = 0;
    for(Node* p = head; p!= NULL;p = p->next){
        if (strcmp(p->palavra->palavra,palavra)==0){
            for(int i = 0;i < 50; i++){
                vetor[i] = p->palavra->vetor[i];//achou
            }
            return vetor;
        }
        cont++;
    }
    for(int i = 0;i < 50; i++){
        vetor[i] = -1;
    }
    return vetor; //PRECISA DAR FREE FORA!!!!!!!!!!!!!!
}


//-- retornar as palavras que estejam a uma distância "d" de uma palavra "x", de acordo com as duas distâncias definidas
char** palavras_distancia(Node*head, double (*comparacao)(palavra*,palavra*,int), double distancia, char* palavra){//ORDENA DE ACORDO COM A FUNÇÃO DE PARAMETRO
    int cont = 0;                                                                   //dist_euclid para distancia euclidiana e cosin par distancia cosseno
    char** palavras;
    Node* temp = NULL;
    for(Node* p = head; p!=NULL;p = p->next){
        if (strcmp(p->palavra->palavra,palavra)==0){
            temp = p;
            break;
        }
    }
    if (temp == NULL)return NULL;
    for(Node* p = head;p!= NULL; p = p->next){
        if (fabs(comparacao(temp->palavra, p->palavra,50))<distancia){
            cont++;
        }
    }
    palavras = malloc(sizeof(char*)*(cont+1));//+1 para saber o finall
    cont = 0;
    for(Node* p = head;p!= NULL; p = p->next){
        if (  (fabs(comparacao(temp->palavra, p->palavra,50))<distancia) && (palavra_compara(temp->palavra,p->palavra,50)!=0)  ){
            palavras[cont] = malloc(sizeof(char)*51);
            strcpy(palavras[cont],p->palavra->palavra);
            cont++;
        }
    }
    palavras[cont] = malloc(sizeof(char)*8);
    strcpy(palavras[cont],"1f2i3m4");//(flag do fim)para poder printar sem saber o tamanho
    return palavras;//É PRECISO DAR FREE FORAAAAA!!!!!!!!!!!!!!!
}

// -- retornar as palavras que estejam na mesma categoria.
char** palavras_categorias(Node*head, const char* categoria){
    int cont = 0;
    for(Node* p = head;p!= NULL; p = p->next){
        if(strcmp(p->palavra->categoria,categoria)==0){
            cont++;
        }
    }
    char** palavras = malloc(sizeof(char*)*(cont+1));//mais 1 para sentila do fim
    cont = 0;
    for(Node* p = head; p!= NULL; p = p->next){
        if(strcmp(p->palavra->categoria,categoria)==0){
            palavras[cont] = malloc(sizeof(char)*51);
            strcpy(palavras[cont],p->palavra->palavra);
            cont++;
        }
    }
    palavras[cont] = malloc(sizeof(char)*8);
    strcpy(palavras[cont],"1f2i3m4");//(sentinela do fim)printar sem ter tam
    return palavras;//FREE FORA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void imprimir_Agrupamento(char** agrupamento){
    if (agrupamento== NULL){
        printf("agrupamento n existe");
        return;
    }
    int i = 0;
    while(strcmp(agrupamento[i],"1f2i3m4")!=0){
        printf("%s\n",agrupamento[i]);
        i++;
    }
}


char** agrupar_palavras(Node* head, int k, char* palavra1,double (*comparacao)(palavra*,palavra*,int)){
    Node* temp;
    int encon = 0;
    for(Node* p = head;p!=NULL; p = p->next){
        if (strcmp(p->palavra->palavra,palavra1)==0){
            temp = p;
            encon = 1;
            break;
        }
    }
    if (encon){
        char** agrupamento = malloc(sizeof(char*)*(k+1));
        if (agrupamento == NULL){
            return NULL;
        }
        int achados = 0;
        Node* temp2;
        double min_dist;
        double dist;
        /*if (temp!=head){
            min_dist= comparacao(temp->palavra,head->palavra,50);
            temp2 = head;
        }
        else{
            min_dist = comparacao(temp->palavra,temp->next->palavra,50);
            temp2 = temp->next;
        }*/
        while(achados<k){
            min_dist = DBL_MAX;
            for(Node* p = head;p!=NULL; p = p->next){
                if (p == temp)continue;
                if (palavra_na_lista(agrupamento,p->palavra->palavra,achados)== 0){
                    dist = comparacao(temp->palavra,p->palavra,50);
                    if (fabs(dist)<min_dist){
                            temp2 = p;
                            min_dist = dist;
                    }
                }
            }
            agrupamento[achados] = malloc(sizeof(char)*51);
            strcpy(agrupamento[achados],temp2->palavra->palavra);
            //printf("%s     dist:  %lf\n",agrupamento[achados],comparacao(temp->palavra,temp2->palavra,50));
            achados++;
        }
        agrupamento[achados] = malloc(sizeof(char)*8);
        strcpy(agrupamento[achados],"1f2i3m4");
        return agrupamento;
    }
    return NULL;
}
//-- retornar a categoria prevalecente dos "k" vizinhos mais próximos de uma palavra "x"
char* categoria_vizinhos(Node* head, int k , char* palavra1, double (*comparacao)(palavra*,palavra*,int),char categorias1[20][16]){
    Node* temp;
    int encon = 0;
    int contcat[20] = {0};
    for(Node* p = head;p!=NULL; p = p->next){
        if (strcmp(p->palavra->palavra,palavra1)==0){
            temp = p;
            encon = 1;
            break;
        }
    }
    if (encon){
        char** agrupamento = malloc(sizeof(char*)*k);
        if (agrupamento == NULL)return NULL;
        int achados = 0;
        Node* temp2;
        double min_dist;
        double dist;
        while(achados<k){
            min_dist = DBL_MAX;
            for(Node* p = head;p!=NULL; p = p->next){
                if (p == temp)continue;
                if (palavra_na_lista(agrupamento,p->palavra->palavra,achados)== 0){
                    dist = comparacao(temp->palavra,p->palavra,50);
                    if (fabs(dist)<min_dist){
                        temp2 = p;
                        min_dist = dist;
                    }
                }
            }
            agrupamento[achados] = malloc(sizeof(char)*51);
            strcpy(agrupamento[achados],temp2->palavra->palavra);
            for(int i = 0;i<20;i++){
                if(strcmp(temp2->palavra->categoria,categorias1[i])==0){
                    //printf("CATEGORIA: %s\n",categorias1[i]);
                    contcat[i]++;
                    break;
                }
            }
            achados++;
        }
        int maior = 0;
        int indice;
        for(int i = 0; i < 20;i++){
            if (contcat[i]>maior){
                maior = contcat[i];
                indice = i;
            }
        }
        for(int i = 0;i<achados-1;i++){
            free(agrupamento[i]);
        }
        free(agrupamento);


        return categorias1[indice];
    }
    return NULL;
}
// -- retornar "m"  agrupamentos de "k" vizinhos mais próximos, onde cada agrupamento começa com um elemento aleatório.
char*** m_agrupamentos(Node* head,int k,double (*comparacao)(palavra*,palavra*,int),int m){
    char*** m_agrup = malloc(sizeof(char**)*m);
    if(m_agrup == NULL)return NULL;
    Node* p = head;
    if (p==NULL){
        free(m_agrup);
        return NULL;
    }
    for(int i = 0;i<m;i++){
        p = head;
        for(int j = 0;j<rand()%929606;j++){//deveria %929606 para o arquivo inteiro, meu computador não processa isso, então vou deixar outros valores temporários
            p = p->next;
            if(p == NULL)break;
        }
        if(p!= NULL){
            m_agrup[i] = agrupar_palavras(head, k,p->palavra->palavra,comparacao);
            if (m_agrup[i]==NULL) {
                printf("ERROR 404");
                continue;
            }
            /*printf("Agrupamento: %d  \nPalavra: %s\n",i+1,p->palavra->palavra);
            //imprimirlistaDist(head,p->palavra->palavra,comparacao);
            imprimir_Agrupamento(m_agrup[i]);
            printf("\n");*/
        }
    }
    return m_agrup;//FREE FORAAAAAAAAAAAAAAA!!!!!!!!!!!
}

void limpar_lista_char(char** lista) {
    int i = 0;
    while (strcmp(lista[i], "1f2i3m4") != 0) {
        free(lista[i]);
        i++;
    }
    free(lista[i]);
    free(lista);
}



int main(int argc, char** argv){
    //srand(time(NULL));
    //Node* head = NULL;
    //head = lerarquivo(head);
    //sort_dist(head,cosin,"de");
    //sort(head,norma_compara);
    //imprimirlistaNorma(head);
    //imprimirlista(head,50);
    //imprimirlistaDist(head,"de",cosin);
    //printf("//////////////////////\n");
    //imprimirlista(head,50);
    //imprimirlistaDist(head,"de", cosin);
    //char** teste3 = palavras_distancia(head,dist_euclid,1.06,"alemão");
    //imprimir_Agrupamento(teste3);
    //imprimir_Agrupamento(teste3);
    //imprimirlistaDist(head,"de",dist_euclid);
    //char* teste = categoria_vizinhos(head, 5,"de", dist_euclid,categorias);
    //printf("%s",teste);
    //char** teste1 = agrupar_palavras(head,5,"alemão",cosin);
    //imprimir_Agrupamento(teste1);
    //limpar_lista_char(teste1);
    //char** teste11= agrupar_palavras(head,5,"um",cosin);

    //int cont = 0;
    //printf("/////////////////\n////////////////////////////\n");
    //printf("%s\n",teste);
    /*char** testecat = palavras_categorias(head, "Verbos");
    while(strcmp(testecat[cont],"1f2i3m4")!=0){
        printf("%s  \n",testecat[cont]);
        cont++;
    }*/

    /*int m = 10;
    char*** teste2 = m_agrupamentos(head,3,cosin,m);
    if (teste2!=NULL){
        for(int i = 0;i<m;i++){
            if (teste2[i]== NULL)continue;
            limpar_lista_char(teste2[i]);
        }
        free(teste2);
    }*/
    //exclui_lista(head);
    return 0;
}
