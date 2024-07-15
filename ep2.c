#include <stdio.h>
#include <string.h>

typedef struct str{
    char* name;
    int GolsMarcados;
    int GolsSofridos;
    int Vitorias;
    int Defeaty;
    int Empaty;
    struct str* prox;
}NO;

int saveResult(NO* list){
    FILE* file = fopen("saida.txt","r");
    NO* p = list->prox;
    int i = 1;
    while(p){
        if(!fprintf(file,"%d %s %d \n",i, p->name, p->Vitorias*3+p->Empaty)){
            printf("Error: there is an error in the aswer file (allocation of the %dth value).\n",i);
            return 0;
        }
        p = p->prox;
        i++;
    }
    return 1;
}

NO* getNode(char* nome, NO* l){
    while(l)
        if(l->name == nome) return l;
        else l = l->prox;
    return NULL;
}

void inicializer(NO* novo, char* name){
    novo->name = name;
    novo->GolsMarcados-0;
    novo->GolsSofridos=0;
    novo->Vitorias=0;
    novo->Defeaty=0;
    novo->Empaty=0;
    novo->prox = NULL;
}

void insert(NO* l, NO* novo){
    if(getNode(novo, l) == NULL){
        NO* n = (NO*) malloc(sizeof(NO));
        inicializer(n,novo);
        l->prox = novo;
        l = l->prox;
    }
}

void showData(char* name, NO* list){
    if(getNode(name, list)){
        printf("===============================");
        printf("| Pontos Ganhos | %11.d |",list->Vitorias*3 + list->Empaty);
        printf("| Saldo de Gols | %11.d |",(list->GolsMarcados - list->GolsSofridos));
        printf("| Gols Avarege  | %11.2f |",(list->GolsMarcados+list->GolsSofridos)/(list->Vitorias+list->Defeaty+list->Empaty));
        printf("| Vitorias      | %11.d |",list->Vitorias);
        printf("| Derrotas      | %11.d |",list->Defeaty);
        printf("| Empates       | %11.d |",list->Empaty);
        printf("| Gols Marcados | %11.d |",list->GolsMarcados);
        printf("| Gols Sofridos | %11.d |",list->GolsSofridos);
        printf("===============================");
    }
}

void ranking(int m, NO* list){
    NO* p = list->prox;
    if(p){
        while(p && p->prox){
            int p1 = p->Vitorias*3 + p->Empaty;
            int p2 = p->prox->Vitorias*3 + p->prox->Empaty;
            NO* aux;
            if(p1 >= p2){
                if(p1 == p2){
                    int saldoGols1 = p->GolsMarcados - p->GolsSofridos;
                    int saldoGols2 = p->prox->GolsMarcados - p->prox->GolsSofridos;
                    if(saldoGols1 > saldoGols2){
                        aux = list;
                        list = list->prox;
                        list->prox = aux;
                    }
                }else{
                    aux = p;
                    list = p->prox;
                    p->prox = aux;
                }
            }
        }
    }
    if(!saveResult(list)){
        printf("Error: occured an error on the writing of the aswer file.");
        return -1;
    }
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Error: the number of elements passed by terminal is incorrect.");
        return -1;
    }
    
    int m = atoi(argv[1]);
    if(m<1){
        printf("Error: the number of matchs could not be zero or negative.");
        return -1;
    }

    FILE* file;
    file = fopen(argv[2],"r");
    if(!file){
        printf("Error: the file passed by parameter could not be open.");
        return -1;
    }

    NO* l = (NO*) malloc(sizeof(NO));
    inicializer(l,"");
    while(!feof(file)){
        for(int i = 0;i<m;i++){
            char* c1, c2;
            int n1,n2;
            NO* no1;
            NO* no2;

            fscanf(file, "%s,%s,%d,%d\n",c1,c2,n1,n2);

            if(getNode(c1,l) == NULL){
                no1 = (NO*) malloc(sizeof(NO));
                inicializer(no1,c1);
                insert(l,no1);
            }
            else no1 = getNode(c1,l);               
            
            if(getNode(c2,l) == NULL){
                no2 = (NO*) malloc(sizeof(NO));
                inicializer(l,no2);
                insert(l,no2);
            }
            else no2 = getNOde(c2,l);
        
            no2->GolsMarcados+=n2;
            no2->GolsSofridos+=n1;
            no1->GolsMarcados+=n1;
            no1->GolsSofridos+=n2;
            
            if(n1>=n2){
                if(n1==n2){
                    no1->Empaty++;
                    no2->Empaty++;
                }else{
                    no1->Vitorias++;
                    no2->Defeaty++;
                }
            }else{
                no1->Defeaty++;
                no2->Vitorias++;
            }
        }
    }
    ranking(m, file);
    return 0;
}