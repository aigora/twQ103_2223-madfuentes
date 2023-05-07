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

/* Declaracion de Funciones auxiliares */
struct distrito cargar_fichero_nuevo(char nombre_fichero[32]));
struct estadisticas obtener_valores_estadisticos(float datos_estadistica[50],num_fuentes);

//MENÚ
int main (){
	int option,a;
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
	do{
		scanf("%d",&option);
			switch(option){
				case '1':
					printf("\n");
					printf("CARGANDO FICHEROS\n");
					printf("\n");
					a=1;
					break;
				case '2':
					printf("\n");
					printf("IMPRIMIENDO LA LISTA DE FUENTES\n");
					printf("\n");
					a=1;		
					break;
				case '3':
					printf("\n");
					printf("MOSTRANDO ESTADÍSTICAS\n");
					printf("\n");
					a=1;
					break;
				case '4':
					printf("\n");
					printf("CERRANDO PROGRAMA\n");
					printf("\n");
					a=1;
					break;
				default:
					printf("\n");
					printf("ERROR, la opción elegida no está disponible, vuleva a introducir una opción valida");
					printf("\n");
					a=0;	
					break;	
			}
	} while(a != 1);
	return 0;
}

/* Definicion de funciones auxiliares */

/*Función para leer el fichero de carga y transformarlo a la estructura para almacenar en el fichero general*/
struct distrito cargar_fichero_nuevo(char nombre_fichero[32]) {

    FILE *fichero_nuevo;
    struct distrito mi_distrito;
    int i=0,j=0;
    char temp_anio[4];
    char temp_mes[2];
    char temp_nomdistrito[26];

    for (i=0; i<4; i++) {
        temp_anio[i] = nombre_fichero[i];
    }

    for (i=4; i<6; i++) {
        temp_mes[i-4] = nombre_fichero[i];
    }

    for (i=6; i<32; i++) {
        if (nombre_fichero[i] == '.') {
            temp_nomdistrito[i-6] = '\0';
            break;
        }
        else {
            temp_nomdistrito[i-6] = nombre_fichero[i];
        }
    }
    strcpy(mi_distrito.nom_distrito,temp_nomdistrito);
    mi_distrito.anio=atoi(temp_anio);
    mi_distrito.mes=atoi(temp_mes);

    fichero_nuevo=fopen(nombre_fichero,"r");
    if (fichero_nuevo==NULL) //si el fichero que vamos a cargar está vacío nos da mensaje de error
    {
        printf("Error al abrir el fichero\n");
        return;
    }

    /* Leemos y descartamos linea de cabecera */
    char cabecera[128];
    fscanf(fichero_nuevo, " %[^\n]\n", cabecera);

    /* Leemos y almacenamos lineas restantes */
    while(fscanf(fichero_nuevo, "%s\t%f\t%i\t%i\t%i\n", mi_distrito.datos_fuente[i].nom_fuente, &mi_distrito.datos_fuente[i].ph, &mi_distrito.datos_fuente[i].conductividad, &mi_distrito.datos_fuente[i].turbidez, &mi_distrito.datos_fuente[i].coliformes) != EOF) {
        j++;
    }
    mi_distrito.num_fuentes=j;
	
    fclose(fichero_nuevo)

    return mi_distrito;
}

struct estadisticas obtener_valores_estadisticos(float datos_estadistica[50],num_fuentes)
{
    struct estadisticas mi_estadistica;
    int i, j, suma=0, contador_moda=0;
    float media, aux, maximo=datos_estadistica[0], minimo=datos_estadistica[0], mediana, moda=datos_estadistica[0];

    /*Calculamos la media*/
    for (i=0;i<num_fuentes;i++) {
        suma+=datos_estadistica[i];
    }
    media=suma/num_fuentes;
    mi_estadistica.media=media;

    /*Calculamos el maximo*/
    for (i=1;i=num_fuentes;i++){
        if datos_estadistica[i]>maximo
            maximo=datos_estadistica[i];
    }
    mi_estadistica.maximo=maximo;

     /*Calculamos el minimo*/
    for (i=1;i=num_fuentes;i++){
        if datos_estadistica[i]<minimo
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
    mediana=datos_estadistica[int(num_fuentes/2)];
    mi_estadistica.mediana=mediana;

    /*Calculamos la moda */
    for (i=0;i<num_fuentes;i++){
            int contador_temporal = 0;
            for (j=i+1;j<num_fuentes+1;j++){
                if (datos_estadistica[j]=datos_estadistica[i])
                    contador_temporal++;
            }
            if (contador_moda<contador_temporal)
                moda=datos_estadistica[i];

    }
    mi_estadistica.moda=moda;

    return mi_estadistica;
	
    /*Consultar lista*/
    void imprimir_lista(struct distrito mi_distrito) {
    int i;

    printf("%-15s %-5s %-14s %-9s %-11s\n", "Parametros", "pH", "Conductividad", "Turbidez", "Coliformes");
    for (i=0; i<mi_distrito.num_fuentes; i++) {
        printf("%-15s %-5.2f %-14d %-9d %-11d\n", mi_distrito.datos_fuente[i].nom_fuente, mi_distrito.datos_fuente[i].pH, mi_distrito.datos_fuente[i].conductividad, mi_distrito.datos_fuente[i].turbidez, mi_distrito.datos_fuente[i].coliformes);
    }
}
}

