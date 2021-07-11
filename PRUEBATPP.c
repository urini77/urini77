#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Peli{
    int ID;
    char nombre[60];
};

struct Datos7D{
    int ID;
    int Dia;
    int Vis;
    float Val;
};

struct RankingTot{
    int ID;
    int VisT;
};

int countlines(){
    int lines = 0, ch = 0;
    FILE *ultdias;
    ultdias = fopen("datos_7dias.txt","r");
    while(!feof(ultdias)){
        ch = fgetc(ultdias);
        if(ch == '\n'){
            lines++;
        }
    }
    fclose(ultdias);
    return(lines);
}

void GRABAR_TOP10(struct RankingTot RankingT[], struct Peli Pel[]){
    FILE *top10;
    int j = 0;
    top10 = fopen("top10.txt","w");

    for(int i = 0 ; i < 10 ; i++){
        j = 0;
        while(RankingT[i].ID != Pel[j].ID){
            j++;
        }
        fprintf(top10, "ID: %d | Nombre: %s | Visualizaciones Totales: %d\n",RankingT[i].ID,Pel[j].nombre,RankingT[i].VisT);
    }
    fclose(top10);
}

void RANKING(struct Peli Pel[], struct Datos7D DatosUltDias[], int size){
    int i, j;
    struct RankingTot RankingT[100];

    for(i = 0; i < 100 ; i++){
        RankingT[i].VisT = 0;
    }

    for(i = 0 ; i < 100 ; i++){
        RankingT[i].ID = Pel[i].ID;
        for(j = 0 ; j < size ; j++){
            if(Pel[i].ID == DatosUltDias[j].ID){
                RankingT[i].VisT = RankingT[i].VisT + DatosUltDias[j].Vis;
            }
        }
    }
    int swapID, swapVisT;
    for(i = 0 ; i < 100 ; i++){
        for(j = 0 ; j < 100 - i ; j++){
            if(RankingT[j].VisT < RankingT[j+1].VisT){
                swapVisT = RankingT[j].VisT;
                RankingT[j].VisT = RankingT[j+1].VisT;
                RankingT[j+1].VisT = swapVisT;
                swapID = RankingT[j].ID;
                RankingT[j].ID = RankingT[j+1].ID;
                RankingT[j+1].ID = swapID;
            }
        }
    }
    for(i = 0 ; i < 100 ; i++){
        printf("ID: %d | Visualizaciones: %d \n",RankingT[i].ID,RankingT[i].VisT);
    }

    GRABAR_TOP10(RankingT, Pel);
}

float VALORACION_PONDERADA(struct Peli Pel[], struct Datos7D DatosUltDias[], int size, int IDPeli){
    char nombre[60];
    float ValoracionPond = 0;
    int CantVis = 0, i;

    for(i = 0 ; i < 100 ; i++){
        if(IDPeli == Pel[i].ID){
            strcpy(nombre,Pel[i].nombre);
        }
    }

    for(i = 0 ; i < size ; i++){
        if(IDPeli == DatosUltDias[i].ID){
            ValoracionPond = ValoracionPond + (DatosUltDias[i].Val * DatosUltDias[i].Vis);
            CantVis = CantVis + DatosUltDias[i].Vis;
        }
    }
    if(ValoracionPond!=0){
        ValoracionPond = ValoracionPond/CantVis;
    }
    printf("La valoracion semanal de la pelicula %d - %s es: ", IDPeli, nombre);
    return(ValoracionPond);
}

void DIA_MAS(struct Datos7D DatosUltDias[], int size){
    int Dia[7][2], swapNDia, swap, i, j;

    for(i = 0 ; i < 7 ; i++){
        Dia[i][0] = 0;
        Dia[i][1] = i + 1;
    }

    for(i = 0 ; i < size ; i++){
        switch(DatosUltDias[i].Dia){
            case 1:
                Dia[0][0] = Dia[0][0] + DatosUltDias[i].Vis;
                break;
            case 2:
                Dia[1][0] = Dia[1][0] + DatosUltDias[i].Vis;
                break;
            case 3:
                Dia[2][0] = Dia[2][0] + DatosUltDias[i].Vis;
                break;
            case 4:
                Dia[3][0] = Dia[3][0] + DatosUltDias[i].Vis;
                break;
            case 5:
                Dia[4][0] = Dia[4][0] + DatosUltDias[i].Vis;
                break;
            case 6:
                Dia[5][0] = Dia[5][0] + DatosUltDias[i].Vis;
                break;
            case 7:
                Dia[6][0] = Dia[6][0] + DatosUltDias[i].Vis;
                break;
        }
    }

    for(i = 0 ; i < 7 ; i++){
        for(j = 0 ; j < 7 - i ; j++){
            if(Dia[j][0] < Dia[j + 1][0]){
                swapNDia = Dia[j][0];
                Dia[j][0] = Dia[j + 1][0];
                Dia[j + 1][0] = swapNDia;
                swap = Dia[j][1];
                Dia[j][1] = Dia[j + 1][1];
                Dia[j + 1][1] = swap;
            }
        }
    }

    switch(Dia[0][1]){
        case 1:
            printf("El dia que hubieron mas visualizaciones fue el Lunes\n\n");
            break;
        case 2:
            printf("El dia que hubieron mas visualizaciones fue el Martes\n\n");
            break;
        case 3:
            printf("El dia que hubieron mas visualizaciones fue el Miercoles\n\n");
            break;
        case 4:
            printf("El dia que hubieron mas visualizaciones fue el Jueves\n\n");
            break;
        case 5:
            printf("El dia que hubieron mas visualizaciones fue el Viernes\n\n");
            break;
        case 6:
            printf("El dia que hubieron mas visualizaciones fue el Sabado\n\n");
            break;
        case 7:
            printf("El dia que hubieron mas visualizaciones fue el Domingo\n\n");
            break;
    }
}

int main()
{
    int i = -1, j, lines, IDPeli, b, VisMax, CantVis = 0, band = 0;
    float ValPond;
    struct Peli Pel[100];
    FILE *peliculas, *ultdias;

    peliculas = fopen("peliculas.txt","r");
    ultdias = fopen("datos_7dias.txt","r");

    while(!feof(peliculas)){
        i++;
        fscanf(peliculas,"%d %s",&Pel[i].ID,Pel[i].nombre);
    }
    fclose(peliculas);

    lines = countlines();
    struct Datos7D DatosUltDias[lines];
    i = -1;

    while(!feof(ultdias)){
        i++;
        fscanf(ultdias,"%d %d %d %f",&DatosUltDias[i].ID,&DatosUltDias[i].Dia,&DatosUltDias[i].Vis,&DatosUltDias[i].Val);
    }
    fclose(ultdias);

    printf("Ingrese un numero para elegir entre las siguientes opciones: \n");
    printf("1) Ranking y Top 10\n");
    printf("2) Valoracion semanal\n");
    printf("3) Filtro por cantidad\n");
    printf("4) Pico de visualizaciones\n");
    printf("0) Finalizar\n");

    int Opcion;
    scanf("%d",&Opcion);

    while(Opcion != 0){
        while(Opcion != 0 && Opcion != 1 && Opcion != 2 && Opcion != 3 && Opcion != 4){
            printf("Numero invalido, reingrese valor: ");
            scanf("%d",&Opcion);
        }
        switch (Opcion){

            case 0:{
                break;
            }

            case 1:{
                RANKING(Pel, DatosUltDias, lines);
                break;
            }

            case 2:{
                b = 0;
                printf("Ingrese ID de la pelicula: ");
                scanf("%d",&IDPeli);
                for(i = 0 ; i < 100 ; i++){
                    if(Pel[i].ID == IDPeli){
                        b = 1;
                        break;
                    }
                }
                if(b == 1){
                    ValPond = VALORACION_PONDERADA(Pel, DatosUltDias, lines, IDPeli);
                    printf("%0.1f \n\n",ValPond);
                }
                else {
                    printf("El ID ingresado no existe\n");
                }
                break;
            }

            case 3:{
                printf("Ingrese una cantidad total de visualizaciones: ");
                scanf("%d",&VisMax);
                band = 0;
                for(i = 0 ; i < 100 ; i++){
                    CantVis = 0;
                    for(j = 0 ; j < lines ; j++){
                        if(Pel[i].ID == DatosUltDias[j].ID){
                            CantVis = CantVis + DatosUltDias[j].Vis;
                        }
                    }
                    if(CantVis > VisMax){
                        if(band == 0){
                            printf("Las siguientes peliculas superan la cantidad de visualicaciones ingresadas: \n");
                        }
                        band = 1;
                        printf("%s | %d\n",Pel[i].nombre,CantVis);
                    }
                }
                if(band == 0){
                    printf("Ningun titulo fue visto tantas veces!\n\n");
                }
                break;
            }

            case 4:{
                DIA_MAS(DatosUltDias, lines);
                break;
            }
        }

    printf("\n");
    printf("Ingrese un numero nuevamente: \n");
    printf("1) Ranking y Top 10\n");
    printf("2) Valoracion semanal\n");
    printf("3) Filtro por cantidad\n");
    printf("4) Pico de visualizaciones\n");
    printf("0) Finalizar\n");
    scanf("%d",&Opcion);
    printf("\n\n");
    }
    return 0;
}
