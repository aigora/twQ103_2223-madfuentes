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

//MENÚ
int main (){
	int option;
	printf("BIENVENIDO AL REGISTRO DE CALIDAD Y PROPIEDADES DE FUENTES DE MADFUENTES:\n");
	printf("¿Qué desea hacer?\n");
	printf("-Consultar información sobre las fuentes (PRESIONE 1)\n");
	printf("-Modificar información sobre las fuentes (PRESIONE 2)\n");
	scanf("%c",&option);
	switch (option){
		case '1':
			printf("A");//EL PROGRAMA IMPRIME LOS DATOS GUARDADOS
	}
	switch (option){
		case '2':
			printf("B");//EL PRONGRMA IMPREIME LOS DATOS GUARDADOS Y LE PIDE AL USUARIO QUE DESEA CAMBIAR
	}
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

    return mi_distrito;
}
