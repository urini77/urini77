#include <stdio.h>
#include <string.h>

/****************************** ESTRUCTURAS ******************************/

// Registro para guardar el ID de las pelculas y sus nombres correspondientes
struct Pelicula {
    int id;
    char nombre[60];
};

// Registro para guardar el ID, dia, cantidad de visualizaciones y valoracion de las peliculas
struct Datos7D {
    int id;
    int dia;
    int vis;
    float val;
};

// Registro para guardar el ranking de peliculas
struct RankedPeli {
    int id;
    int totVis;
};

// Registro para guardar el dia con mas visualizaciones
struct visXdia {
    char nombreDia[10];
    int vis;
};

/*************************************************************************/

/******************************* FUNCIONES *******************************/

// Cuenta la cantidad de lineas del archivo "datos_7dias.txt"
int contarDatos() {
    int lineas = 0, ch = 0;
    FILE *ultdias = fopen("datos_7dias.txt", "r");
    while( !feof(ultdias) ) {
        ch = fgetc(ultdias);
        if(ch == '\n') lineas++;
    }
    fclose(ultdias);
    
    return lineas;
}

// Inicializar el arreglo de las peliculas
void inicPelis(struct Pelicula pelis[]) {
    FILE *peliculas = fopen("peliculas.txt", "r");
    for(int i = 0; !feof(peliculas); i++)
        fscanf(peliculas, "%d %s", &pelis[i].id, pelis[i].nombre);
    fclose(peliculas);
}

// Inicializar el arreglo de los ultimos 7 dias
void inicUlt7Dias(struct Datos7D ult7dias[]) {
    FILE *ultimosdias = fopen("datos_7dias.txt", "r");
    for(int i = 0; !feof(ultimosdias); i++)
        fscanf(ultimosdias, "%d %d %d %f", &ult7dias[i].id, &ult7dias[i].dia, &ult7dias[i].vis, &ult7dias[i].val);
    fclose(ultimosdias);
}

// Inicializo los dias de la semana con su nombre correspondiente y las visualizaciones en 0
void inicDias(struct visXdia dias[]) {
    for(int i = 0; i < 7; i++) {
        dias[i].vis = 0;
        switch(i) {
            case 0:
                strcpy(dias[i].nombreDia, "Lunes");
                break;
            case 1:
                strcpy(dias[i].nombreDia, "Martes");
                break;
            case 2:
                strcpy(dias[i].nombreDia, "Miercoles");
                break;
            case 3:
                strcpy(dias[i].nombreDia, "Jueves");
                break;
            case 4:
                strcpy(dias[i].nombreDia, "Viernes");
                break;
            case 5:
                strcpy(dias[i].nombreDia, "Sabado");
                break;
            case 6:
                strcpy(dias[i].nombreDia, "Domingo");
                break;
        }
    }
}

// Muestra el menu. Posee un parametro para determinar si dar la bienvenida o no
void mostrarMenu(int bienvenida) {
    printf("\n--- MENU PRINCIPAL ---\n\n");
    if( bienvenida )
        printf("Bienvenido/a! ");
    printf("Escoja una opcion ingresando el numero correspondiente:\n");
    printf("\t1) Ranking y Top 10\n");
    printf("\t2) Valoracion semanal\n");
    printf("\t3) Filtro por cantidad\n");
    printf("\t4) Pico de visualizaciones\n");
    printf("\t0) Finalizar\n\n");
}

// Intercambia dos entradas del ranking
void swap(struct RankedPeli *a, struct RankedPeli *b) {
    struct RankedPeli temp = *a;
    *a = *b;
    *b = temp;
}

// Implementacion de Bubble Sort para ordenar el ranking de mas a menos visualizaciones
void bubbleSort(struct RankedPeli rank[]) {
    for(int i = 0; i < 100; i++)
        for(int j = 0; j < 100 - i; j++)
            if( rank[j].totVis < rank[j+1].totVis )
                swap(&rank[j], &rank[j+1]);
}

// Guarda el top 10 peliculas mas vista en un archivo 'top10.txt'
void GRABAR_TOP10(struct RankedPeli rankingTot[], struct Pelicula pelis[]) {
    FILE *top10 = fopen("top10.txt", "w");

    int j;
    for(int i = 0; i < 10; i++) {
        // Busco el nombre de la pelicula segun su ID
        for(j = 0; rankingTot[i].id != pelis[j].id; j++);
        // Escribo todo en el archivo
        fprintf(top10, "ID: %d\t|\tNombre: %s\t|\tVisualizaciones totales: %d\n", rankingTot[i].id, pelis[j].nombre, rankingTot[i].totVis);
    }
    
    fclose(top10);
}

// Muestra en pantalla todas las peliculas ordenadas de mas a menos visualizaciones
void RANKING(struct Pelicula pelis[], struct Datos7D ult7dias[], int cantDatos){
    struct RankedPeli rankingTot[100];

    // Inicializo las visitas totales en 0
    for(int i = 0; i < 100 ; i++)
        rankingTot[i].totVis = 0;

    // Ubico las 100 peliculas por ID con su cantidad de visualizaciones totales en el ranking
    for(int i = 0; i < 100; i++) {
        rankingTot[i].id = pelis[i].id;
        for(int j = 0; j < cantDatos; j++)
            if( pelis[i].id == ult7dias[j].id )
                rankingTot[i].totVis += ult7dias[j].vis;
    }

    // Y ahora ordeno el ranking
    bubbleSort(rankingTot);

    printf("--- RANKING ---\n\n");
    char vis[30];
    for(int i = 0; i < 100; i++) {
        printf("ID: %5d  |  Visualizaciones: %5d\n", rankingTot[i].id, rankingTot[i].totVis);
    }
    printf("\n");

    GRABAR_TOP10(rankingTot, pelis);
}

// Calcula la valoracion ponderada de una pelicula basandose en los datos de la ultima semana
float VALORACION_PONDERADA(struct Datos7D ult7dias[], int cantDatos, int idPeli) {
    float sumaPond = 0.0;
    int ponderacion = 0;

    // Obtiene los nros. necesarios para calcular la valoracion ponderada
    for(int i = 0; i < cantDatos; i++)
        if( idPeli == ult7dias[i].id ) {
            sumaPond += (ult7dias[i].val * ult7dias[i].vis);
            ponderacion += ult7dias[i].vis;
        }

    // Calculo la valoracion ponderada
    float valPond;
    if( ponderacion != 0 )
        valPond = sumaPond / (ponderacion * 1.0);
    else
        valPond = -1.0;
    
    return valPond;
}

// Muestra en que dia de la semana hubo mas visualizaciones
void DIA_MAS(struct Datos7D ult7Dias[], int cantDatos) {
    struct visXdia dias[7];
    inicDias(dias);

    // Relleno las visualizaciones por cada dia
    int dia;
    for(int i = 0; i < cantDatos; i++) {
        dia = ult7Dias[i].dia - 1;
        dias[dia].vis += ult7Dias[i].vis;
    }

    // Busco el mayor numero de visualizaciones
    int visMax = dias[0].vis;
    for(int i = 1; i < 7; i++)
        if( dias[i].vis > visMax )
            visMax = dias[i].vis;
    
    // Y ahora busco el dia al que corresponde dicho nro. de visualizaciones
    for(int i = 0; i < 7; i++)
        if( dias[i].vis == visMax ) {
            printf("El dia con mas visualizaciones fue el %s, con %d visualizaciones.\n\n", dias[i].nombreDia, visMax);
        }
}

/*************************************************************************/

int main() {
    struct Pelicula pelis[100];
    int cantDatos = contarDatos();
    struct Datos7D ult7dias[cantDatos];

    // Inicializo ambos arreglos
    inicPelis(pelis);
    inicUlt7Dias(ult7dias);

    int opcion, bienvenida = 1;
    // Empieza la interaccion con el usuario
    do {
        mostrarMenu(bienvenida);
        bienvenida = 0;
        printf("Ingrese aqui la opcion deseada: ");
        scanf("%d", &opcion);
        printf("\n");
        // Valido que la entrada sea correcta
        while( !(opcion >= 0 && opcion <= 4) ) {
            printf("La opcion ingresada no es valida, intente otra vez: ");
            scanf("%d", &opcion);
        }

        // Luego de ingresar la opcion, ejecuto la subrutina elegida
        switch(opcion) {
            // Opcion FINALIZAR seleccionada
            case 0:
                break;
            
            // Opcion RANKING Y TOP 10 seleccionada
            case 1: {
                RANKING(pelis, ult7dias, cantDatos);
                break;
            }
            
            // Opcion VALORACION SEMANAL seleccionada
            case 2: {
                int idPeli, peliExiste = 0;
                float valPond;
                printf("\nIngrese el ID de la pelicula cuya valoracion desea calcular: ");
                scanf("%d", &idPeli);
                
                // Verifico que exista la pelicula en la base de datos
                for(int i = 0; i < 100; i++)
                    if( idPeli == pelis[i].id ) {
                        peliExiste = 1;
                        break;
                    }

                // Si existe, calculo y muestra la valoracion semanal
                if( peliExiste ) {
                    valPond = VALORACION_PONDERADA(ult7dias, cantDatos, idPeli);
                    printf("La valoracion semanal de la pelicula %d - ", idPeli);
                    for(int i = 0; i < 100; i++)
                        if( idPeli == pelis[i].id ) {
                            printf("%s", pelis[i].nombre);
                            break;
                        }
                    printf(" es: %.1f\n\n", valPond);
                }
                // Si no, vuelvo al menu principal
                else
                    printf("El ID ingresado no existe. Sera redirigido al menu principal.\n\n");

                break;
            }
            
            // Opcion FILTRO POR CANTIDAD seleccionada
            case 3: {
                int visMax, cantVis, ninguna = 1;
                printf("Ingrese una cantidad de visualizaciones: ");
                scanf("%d", &visMax);
                
                printf("Las siguientes peliculas superan las %d visualicaciones:\n", visMax);
                for(int i = 0; i < 100 ; i++) {
                    cantVis = 0;
                    // Calculo la cantidad de visualizaciones de cada pelicula
                    for(int j = 0; j < cantDatos; j++)
                        if( pelis[i].id == ult7dias[j].id )
                            cantVis += ult7dias[j].vis;
                    // Si superan la cantidad ingresada, muestro su nombre y sus visualizaciones
                    if( cantVis > visMax ) {
                        ninguna = 0;
                        printf("%-60s|  %-8d\n", pelis[i].nombre, cantVis);
                    }
                }

                // Si ninguna pelicula supera las visualizaciones ingresadas, aviso
                if( ninguna )
                    printf("Ningun titulo fue visto tantas veces!\n\n");
                else
                    printf("\n");
                
                break;
            }

            // Opcion PICO DE VISUALIZACIONES seleccionada
            case 4: {
                DIA_MAS(ult7dias, cantDatos);
                break;
            }
        }

    } while( opcion != 0 );

    return 0;
}
