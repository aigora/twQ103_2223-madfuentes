#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Estructuras */
 typedef struct fuente {
    char nom_fuente[32];
    float pH;
    int conductividad;
    int turbidez;
    int coliformes;
} fuente;

struct distrito {
    char nom_distrito[26];
    int anio;
    int mes;
    int num_fuentes;
    fuente datos_fuente[50];
};

struct estadisticas {
    float maximo;
    float minimo;
    float media;
    float mediana;
    float moda;
};

/* Declaracion de Funciones auxiliares */
struct distrito cargar_fichero_nuevo(char nombre_fichero[32]);
struct estadisticas obtener_valores_estadisticos(float datos_estadistica[50], int num_fuentes);
void imprimir_valores_estadisticos(struct distrito mi_distrito);
void imprimir_lista(struct distrito mi_distrito);
void banner(void);

/* Función principal */
int main()
{
    int option, mostrar_menu=1;
    struct distrito distrito_cargado;

    /* Lanzar banner con titulo */
    banner();

    /* Lanzamos menú con opciones */
    do{
        printf("Bienvenido al menu del programa de registro de fuentes MADFUENTES.\n");
        printf("__________________________________________________________________\n");
        printf("Que desea hacer?\n");
        printf("_________________\n");
        printf("\n");
        printf("1-Cargar ficheros.\n");
        printf("\n");
        printf("2-Imprimir fichero.\n");
        printf("\n");
        printf("3-Mostrar estadísticas.\n");
        printf("\n");
        printf("4-Cerrar programa.\n");
        printf("\n");

        // Le piden al usuario que desea hacer
        scanf("%d",&option);
        switch(option){
            case 1:
            {
                // Carga el fichero
                char nombre_fichero[32];
                printf("Nombre del fichero a cargar:\n");
                scanf("%s",nombre_fichero);
                distrito_cargado = cargar_fichero_nuevo(nombre_fichero);
                mostrar_menu=1;
                break;
            }
            case 2:
            {
                // Imprime la lista de fuentes
                printf("\n");
                imprimir_lista(distrito_cargado);
                printf("\n");
                mostrar_menu=1;
                break;
            }
            case 3:
            {
                // Muestra las estadísticas
                printf("\n");
                imprimir_valores_estadisticos(distrito_cargado);
                printf("\n");
                mostrar_menu=1;
                break;
            }
            case 4:
            {
                //Cierra el programa
                printf("\n");
                printf("CERRANDO PROGRAMA\n");
                printf("\n");
                mostrar_menu=0;
                break;
            }
            default:
            {
                // De no introducir una respuesta correcta, el programa le informa, repitiendo el bucle hasta que la opción introducida sea correcta
                printf("\n");
                printf("ERROR, la opción elegida no está disponible, vuleva a introducir una opción valida\n");
                printf("\n");
                mostrar_menu=1;
                break;
            }
        }
    } while(mostrar_menu != 0);

    return 0;
}

/* Definición de funciones auxiliares */

/*Función para leer el fichero de carga y transformarlo a la estructura para almacenar*/
struct distrito cargar_fichero_nuevo(char nombre_fichero[32]) {

    FILE *fichero_nuevo;
    struct distrito mi_distrito;
    int i=0,j=0;
    char temp_anio[4];
    char temp_mes[2];
    char temp_nomdistrito[26];

    //abrimos fichero
    fichero_nuevo=fopen(nombre_fichero,"r");
    //si el fichero que vamos a cargar está vacío nos da mensaje de error
    if (fichero_nuevo==NULL) {
        printf("Error al abrir el fichero\n");
        return mi_distrito;
    }

    //cogemos las cuatro primeras cifras del nombre del fichero para tener el año
    for (i=0; i<4; i++) {
        temp_anio[i] = nombre_fichero[i];
    }

    //cogemos las dos siguientes para el mes
    for (i=4; i<6; i++) {
        temp_mes[i-4] = nombre_fichero[i];
    }

    //saltamos el guion en el siguiente bucle y cogemos el nombre del distrito
    for (i=7; i<32; i++) {
        if (nombre_fichero[i] == '.') {
            temp_nomdistrito[i-7] = '\0';
            break;
        }
        else {
            temp_nomdistrito[i-7] = nombre_fichero[i];
        }
    }
    strcpy(mi_distrito.nom_distrito,temp_nomdistrito);
    mi_distrito.anio=atoi(temp_anio); //pasamos a entero
    mi_distrito.mes=atoi(temp_mes); //pasamos a entero

    /* Leemos y descartamos linea de cabecera */
    char cabecera[128];
    fscanf(fichero_nuevo, " %[^\n]\n", cabecera);

    /* Leemos y almacenamos lineas restantes */
    while(fscanf(fichero_nuevo, "%s\t%f\t%i\t%i\t%i\n", mi_distrito.datos_fuente[j].nom_fuente, &mi_distrito.datos_fuente[j].pH, &mi_distrito.datos_fuente[j].conductividad, &mi_distrito.datos_fuente[j].turbidez, &mi_distrito.datos_fuente[j].coliformes) != EOF) {
        j++;
    }
    mi_distrito.num_fuentes=j;

    fclose(fichero_nuevo);

    return mi_distrito;
}

struct estadisticas obtener_valores_estadisticos(float datos_estadistica[50], int num_fuentes)
{
    struct estadisticas mi_estadistica;
    int i, j, contador_moda=0;
    float media, aux, maximo=datos_estadistica[0], minimo=datos_estadistica[0], mediana, moda=datos_estadistica[0];
    float suma = 0.0f;

    /*Calculamos la media*/
    for (i=0;i<num_fuentes;i++) {
        suma+=datos_estadistica[i];
    }
    media=suma/num_fuentes;
    mi_estadistica.media=media;

    /*Calculamos el maximo*/
    for (i=1;i<=num_fuentes;i++){
        if (datos_estadistica[i]>maximo)
            maximo=datos_estadistica[i];
    }
    mi_estadistica.maximo=maximo;

     /*Calculamos el minimo*/
    for (i=1;i<=num_fuentes;i++){
        if (datos_estadistica[i]<minimo)
            minimo=datos_estadistica[i];
    }
    mi_estadistica.minimo=minimo;

     /*Calculamos la mediana*/
    for (i=0;i<num_fuentes;i++){
            for (j=i+1;j<num_fuentes+1;j++){
                if (datos_estadistica[i]>datos_estadistica[j])
                {
                    aux=datos_estadistica[i];
                    datos_estadistica[i]=datos_estadistica[j];
                    datos_estadistica[j]=aux;
                }
            }

    }
    mediana=datos_estadistica[(int)(num_fuentes/2)];
    mi_estadistica.mediana=mediana;

    /*Calculamos la moda */
    for (i=0;i<num_fuentes;i++){
            int contador_temporal = 0;
            for (j=i+1;j<num_fuentes+1;j++){
                if (datos_estadistica[j]==datos_estadistica[i])
                    contador_temporal++;
            }
            if (contador_moda<contador_temporal){
                moda=datos_estadistica[i];
                contador_moda=contador_temporal;
            }

    }
    mi_estadistica.moda=moda;

    return mi_estadistica;
}

void imprimir_valores_estadisticos(struct distrito mi_distrito)
{
    struct estadisticas mi_pH, mi_conductividad, mi_turbidez, mi_coliformes;
    int i;
    float temp_pH[50], temp_conductividad[50], temp_turbidez[50], temp_coliformes[50];

    /* Inicializamos vectores temporales */
    for (i=0; i<50; i++) {
        temp_pH[i] = 0.0f;
        temp_conductividad[i] = 0.0f;
        temp_turbidez[i] = 0.0f;
        temp_coliformes[i] = 0.0f;
    }

    /* Pasamos los datos a vectores temporales */
    for (i=0; i<mi_distrito.num_fuentes; i++) {
        temp_pH[i] = mi_distrito.datos_fuente[i].pH;
        temp_conductividad[i] = (float)mi_distrito.datos_fuente[i].conductividad;
        temp_turbidez[i] = (float)mi_distrito.datos_fuente[i].turbidez;
        temp_coliformes[i] = (float)mi_distrito.datos_fuente[i].coliformes;
    }

    /* calculamos estadisticas para todas las fuentes del distritos por tipo */
    mi_pH = obtener_valores_estadisticos(temp_pH, mi_distrito.num_fuentes);
    mi_conductividad = obtener_valores_estadisticos(temp_conductividad, mi_distrito.num_fuentes);
    mi_turbidez = obtener_valores_estadisticos(temp_turbidez, mi_distrito.num_fuentes);
    mi_coliformes = obtener_valores_estadisticos(temp_coliformes, mi_distrito.num_fuentes);

    /* Imprimimos */
    printf("Estadisticas de %s\n\n", mi_distrito.nom_distrito);
    printf("%-15s %-15s %-15s %-15s %-15s\n", "Parametros", "pH", "Conductividad", "Turbidez", "Coliformes");
    printf("%-15s %-15.2f %-15.2f %-15.2f %-15.2f\n", "Maximo", mi_pH.maximo, mi_conductividad.maximo, mi_turbidez.maximo, mi_coliformes.maximo);
    printf("%-15s %-15.2f %-15.2f %-15.2f %-15.2f\n", "Minimo", mi_pH.minimo, mi_conductividad.minimo, mi_turbidez.minimo, mi_coliformes.minimo);
    printf("%-15s %-15.2f %-15.2f %-15.2f %-15.2f\n", "Media", mi_pH.media, mi_conductividad.media, mi_turbidez.media, mi_coliformes.media);
    printf("%-15s %-15.2f %-15.2f %-15.2f %-15.2f\n", "Mediana", mi_pH.mediana, mi_conductividad.mediana, mi_turbidez.mediana, mi_coliformes.mediana);
    printf("%-15s %-15.2f %-15.2f %-15.2f %-15.2f\n", "Moda", mi_pH.moda, mi_conductividad.moda, mi_turbidez.moda, mi_coliformes.moda);

    return;
}

/*Función para imprimir el fichero*/
void imprimir_lista(struct distrito mi_distrito) {
    int i;

    printf("%-15s %-5s %-14s %-9s %-11s\n", "Parametros", "pH", "Conductividad", "Turbidez", "Coliformes");
    for (i=0; i<mi_distrito.num_fuentes; i++) {
        printf("%-15s %-5.2f %-14d %-9d %-11d\n", mi_distrito.datos_fuente[i].nom_fuente, mi_distrito.datos_fuente[i].pH, mi_distrito.datos_fuente[i].conductividad, mi_distrito.datos_fuente[i].turbidez, mi_distrito.datos_fuente[i].coliformes);
    }
}

/*Banner*/
void banner(void)
{

}
/*Nuevo*/
void imprimir_dato(struct distrito mi_distrito, int indice, const char* dato) {
    if (indice < 0 || indice >= mi_distrito.num_fuentes) {
        printf("Índice inválido\n");
        return;
    }

    printf("%-15s", "Parametros");
    if (strcmp(dato, "pH") == 0) {
        printf("%-5s\n", "pH");
        printf("%-15s %-5.2f\n", mi_distrito.datos_fuente[indice].nom_fuente, mi_distrito.datos_fuente[indice].pH);
    } else if (strcmp(dato, "Conductividad") == 0) {
        printf("%-14s\n", "Conductividad");
        printf("%-15s %-14d\n", mi_distrito.datos_fuente[indice].nom_fuente, mi_distrito.datos_fuente[indice].conductividad);
    } else if (strcmp(dato, "Turbidez") == 0) {
        printf("%-9s\n", "Turbidez");
        printf("%-15s %-9d\n", mi_distrito.datos_fuente[indice].nom_fuente, mi_distrito.datos_fuente[indice].turbidez);
    } else if (strcmp(dato, "Coliformes") == 0) {
        printf("%-11s\n", "Coliformes");
        printf("%-15s %-11d\n", mi_distrito.datos_fuente[indice].nom_fuente, mi_distrito.datos_fuente[indice].coliformes);
    } else {
        printf("Dato inválido\n");
    }
}

