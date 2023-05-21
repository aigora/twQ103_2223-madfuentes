#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* Definición de alertas para visualización de datos */
#define ALERTA_SUP_PH 9.5
#define ALERTA_INF_PH 6.5
#define ALERTA_SUP_COND 500
#define ALERTA_INF_COND 50
#define ALERTA_SUP_TURB 2
#define ALERTA_SUP_COLI 0

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
void carga_ficheros(struct distrito distrito_cargado[100]);
struct estadisticas obtener_valores_estadisticos(float datos_estadistica[50], int num_fuentes);
void regresion_lineal(struct distrito distrito_cargado[12]);
void imprimir_valores_estadisticos(struct distrito mi_distrito);
void estadisticas_ficheros(struct distrito distrito_cargado[100]);
void imprimir_fichero_mensual(struct distrito mi_distrito);
void imprimir_dato(struct distrito mi_distrito, int indice, char *dato);
void visualizacion_ficheros(struct distrito distrito_cargado[100]);
void banner(void);
void menu_principal(void);
void menu_carga_ficheros(void);
void menu_visualizacion_datos(void);
void menu_estadisticas(void);


/* Función principal */
int main()
{
    int option, mostrar_menu=1;
    struct distrito distrito_cargado[100];

    /* Lanzar banner con titulo */
    banner();

    /* Lanzamos menú con opciones */
    do{
        menu_principal();

        // Le pide al usuario que desea hacer
        scanf("%d",&option);
        switch(option) {
            case 1:
            {
                carga_ficheros(distrito_cargado);
                mostrar_menu=1;
                break;
            }
            case 2:
            {
                visualizacion_ficheros(distrito_cargado);
                mostrar_menu=1;
                break;
            }
            case 3:
            {
		estadisticas_ficheros(distrito_cargado);
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
    char temp_anio[5];
    char temp_mes[3];
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
    temp_anio[i+1] = '\0';

    //cogemos las dos siguientes para el mes
    for (i=4; i<6; i++) {
        temp_mes[i-4] = nombre_fichero[i];
    }
    temp_mes[i+1] = '\0';

    //saltamos el guion en el siguiente bucle y cogemos el nombre del distrito
    for (i=7; i<31; i++) {
        if (nombre_fichero[i] == '.') {
            temp_nomdistrito[i-7] = '\0';
            break;
        }
        else {
            temp_nomdistrito[i-7] = nombre_fichero[i];
        }
    }

    strcpy(mi_distrito.nom_distrito,temp_nomdistrito);
    mi_distrito.anio=atoi(temp_anio); //pasamos cadenas de caracteres a entero
    mi_distrito.mes=atoi(temp_mes); //pasamos cadenas de caracteres a entero

    /* Leemos y descartamos linea de cabecera */
    char cabecera[128];
    fscanf(fichero_nuevo, " %[^\n]\n", cabecera);

    /* Leemos y almacenamos lineas restantes */
    while(fscanf(fichero_nuevo, "%s\t%f\t%i\t%i\t%i\n", mi_distrito.datos_fuente[j].nom_fuente, &mi_distrito.datos_fuente[j].pH, &mi_distrito.datos_fuente[j].conductividad, &mi_distrito.datos_fuente[j].turbidez, &mi_distrito.datos_fuente[j].coliformes) != EOF) {
        j++;
    }
    mi_distrito.num_fuentes=j;

    fclose(fichero_nuevo);

    printf("Fichero cargado correctamente\n");

    return mi_distrito;
}

/*Función de carga de ficheros*/
void carga_ficheros(struct distrito distrito_cargado[100])
{
    // Submenú de carga de ficheros
    int i, option, ver_menu=0;

    do {
        menu_carga_ficheros();

        scanf("%d",&option);
        switch (option) {
            case 1:
            {
                char nombre_fichero[32];
                int lleno=1;

                system("cls"); //borramos la consola
                printf("Nombre del fichero a cargar (Formato: AAAAMM_Distrito.csv):\n");
                scanf("%s",nombre_fichero);
                for (i=0; i<100; i++) {
                    // Comprobamos que la estructura esta vacia y cargamos el fichero
                    if (strcmp(distrito_cargado[i].nom_distrito, "") == 0) {
                        distrito_cargado[i] = cargar_fichero_nuevo(nombre_fichero);
                        lleno=0;
                        break;
                    }
                }
                if (lleno == 1)
                    printf("\nBase de datos llena. No se pueden cargar m%cs ficheros\n", 160);

                Sleep(3000); // Esperamos 3 segundos para mostrar si los ficheros cargan o no

                ver_menu=1;
                break;
            }
            case 2:
            {
                char nombre_distrito[26], distrito_anual[12][37], temp_anyo[5];
                char temp_mes[12][3] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
                int i, j, lleno=1;

                system("cls"); //borramos la consola
                printf("Nombre del distrito a cargar (Formato: Distrito):\n");
                scanf("%25s", nombre_distrito);
                printf("A%co a cargar (Formato: AAAA):\n", 164);
                scanf("%4s", temp_anyo);

                // Vaciamos el vector de cadenas de caracteres
                for (i=0; i<12; i++) {
                    strcpy(distrito_anual[i], "");
                }

                // Generamos los nombres de los archivos y lo cargamos
                for (i=0; i<12; i++) {
                    strcpy(distrito_anual[i],temp_anyo);
                    strcat(distrito_anual[i],temp_mes[i]);
                    strcat(distrito_anual[i],"_");
                    strcat(distrito_anual[i],nombre_distrito);
                    strcat(distrito_anual[i],".csv");

                    for (j=0; j<100; j++) {
                    // Comprobamos que la estructura esta vacia y cargamos el fichero
                        if (strcmp(distrito_cargado[j].nom_distrito, "") == 0) {
                            distrito_cargado[j] = cargar_fichero_nuevo(distrito_anual[i]);
                            lleno=0;
                            break;
                        }
                    }
                }
                if (lleno == 1)
                    printf("\nBase de datos llena. No se pueden cargar m%cs ficheros\n", 160);

                Sleep(2000); // Esperamos 2 segundos para mostrar si los ficheros cargan o no

                ver_menu=1;
                break;
            }
            case 3:
            {
                system("cls"); //borramos la consola
                printf("Lista de ficheros cargados:\n\n");
                for (i=0; i<100; i++) {
                    // Comprobamos que la estructura esta vacia y cargamos el fichero
                    if (strcmp(distrito_cargado[i].nom_distrito, "") == 0) {
                        break;
                    }
                    else {
                        printf("Fichero %03d: %d%02d_%s.csv\n", i+1, distrito_cargado[i].anio, distrito_cargado[i].mes, distrito_cargado[i].nom_distrito);
                    }
                }
                Sleep(4000); //espera de segundos 
                ver_menu=1;
                break;
            }
            case 4:
            {
                ver_menu=0;
                break;
            }
            default:
            {
                printf("\n");
                printf("ERROR, la opci%cn elegida no est%c disponible, vuelva a introducir una opci%cn v%clida\n", 162, 161, 162, 161);
                printf("\n");
            }
        }
    } while(ver_menu!=0);

    return;
}

/*Función para obtener valores estadísticos*/
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
    for (i=1;i<num_fuentes;i++){
        if (datos_estadistica[i]>maximo)
            maximo=datos_estadistica[i];
    }
    mi_estadistica.maximo=maximo;

     /*Calculamos el minimo*/
    for (i=1;i<num_fuentes;i++){
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

/*Función para imprimir por pantalla los valores estadísticos calculados en la función anterior*/
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

/* Función para calcular la tendencia, se busca la recta de ajuste por el método de mínimmos cuadrados */

void regresion_lineal(struct distrito distrito_cargado[12]) {
    float suma_x = 0.0f, suma_x2 = 0.0f;
    float suma_y_pH=0.0f, suma_y_cond=0.0f, suma_y_turb=0.0f, suma_y_coli=0.0f;
    float suma_xy_pH=0.0f, suma_xy_cond=0.0f, suma_xy_turb=0.0f, suma_xy_coli=0.0f;
    float pH, conductividad, turbidez, coliformes;
    int i, j;

    printf("Tendencias de incrementos mensuales en %s, a partir de los datos del a%co %d\n\n", distrito_cargado[0].nom_distrito, 164, distrito_cargado[0].anio);
    printf("%-15s %-15s %-15s %-15s %-15s\n", "Parametros", "pH", "Conductividad", "Turbidez", "Coliformes");

    for (i=0; i<distrito_cargado[0].num_fuentes; i++) {
    	for (j=0; j<12; j++) {
        	suma_x += j+1;
        	suma_y_pH += distrito_cargado[j].datos_fuente[i].pH;
        	suma_xy_pH += (j+1)*distrito_cargado[j].datos_fuente[i].pH;
        	suma_y_cond += distrito_cargado[j].datos_fuente[i].conductividad;
        	suma_xy_cond += (j+1)*distrito_cargado[j].datos_fuente[i].conductividad;
        	suma_y_turb += distrito_cargado[j].datos_fuente[i].turbidez;
        	suma_xy_turb += (j+1)*distrito_cargado[j].datos_fuente[i].turbidez;
        	suma_y_coli += distrito_cargado[j].datos_fuente[i].coliformes;
        	suma_xy_coli += (j+1)*distrito_cargado[j].datos_fuente[i].coliformes;
        	suma_x2 += (j+1)*(j+1);
    	}

    	// Calculamos la tendencia para los 4 parametros
    	pH = (12*suma_xy_pH - suma_x*suma_y_pH)/(12*suma_x2 - suma_x*suma_x);
    	conductividad = (12*suma_xy_cond - suma_x*suma_y_cond)/(12*suma_x2 - suma_x*suma_x);
    	turbidez = (12*suma_xy_turb - suma_x*suma_y_turb)/(12*suma_x2 - suma_x*suma_x);
    	coliformes = (12*suma_xy_coli - suma_x*suma_y_coli)/(12*suma_x2 - suma_x*suma_x);

    	printf("%-15s %-15.2f %-15.2f %-15.2f %-15.2f\n", distrito_cargado[0].datos_fuente[i].nom_fuente, pH, conductividad, turbidez, coliformes);

    	// reseteamos los valores de las sumas para la siguiente fuente
    	suma_y_pH=suma_y_cond=suma_y_turb=suma_y_coli=0.0f;
    	suma_xy_pH=suma_xy_cond=suma_xy_turb=suma_xy_coli=0.0f;
    }

	return;
}

/*Función para el análisis de estadísticas */
void estadisticas_ficheros(struct distrito distrito_cargado[100])
{
    // Submenú de estadisticas
    int i, j, k, l, option, ver_menu=0;
    char tecla_exit[26];

    do {
    	menu_estadisticas();

    	scanf("%d",&option);
    	switch (option) {
            case 1:
            {
            	system("cls");
            	printf("Ficheros cargados disponibles para obtener estad%csticas b%csicas:\n", 161, 160);
            	for (i=0; i<100; i++) {
                	// Cuando la estructura esté vacia salimos del bucle. Ya no hay nada más que imprimir
                	if (strcmp(distrito_cargado[i].nom_distrito, "") == 0) {
                    	    break;
                	}
                	else {
                    	    printf("Fichero %d: %d%02d_%s.csv\n", i+1, distrito_cargado[i].anio, distrito_cargado[i].mes, distrito_cargado[i].nom_distrito);
                	}
            	}

            	printf("\nIntroduzca por teclado el n%cmero del fichero sobre el que obtener las estad%csticas:\n", 163, 161);

            	// Obtenemos por teclado el distrito a visualizar
            	scanf("%2d", &j);

            	system("cls");
            	printf("Distrito: %s, Mes: %d, A%co: %d\n\n", distrito_cargado[j-1].nom_distrito, distrito_cargado[j-1].mes, 164, distrito_cargado[j-1].anio);

            	// Imprimimos el distrito a visualizar
            	imprimir_valores_estadisticos(distrito_cargado[j-1]);

            	printf("\nPulsa cualquier tecla para volver al submen%c anterior\n", 163);
            	scanf("%25s", tecla_exit);

            	ver_menu=1;
            	break;
            }
            case 2:
            {
            	char nombre_distrito[26];
            	char tecla_exit[26];
            	int temp_anio, bandera=0;
            	struct distrito distrito_tendencias[12];

            	system("cls");
            	printf("Introduce el nombre del distrito sobre el que analizar las tendencias (Formato: Distrito):\n");
            	scanf("%25s", nombre_distrito);
            	printf("Introduce el a%co a cargar (Formato: AAAA):\n", 164);
            	scanf("%d", &temp_anio);

            	// Obtenemos los datos anuales de todo el distrito
            	for (k=1; k<13; k++) {
                    for (l=0; l<100; l++) {
                    	if (strcmp(distrito_cargado[l].nom_distrito, nombre_distrito) == 0) {
                            if (distrito_cargado[l].anio == temp_anio) {
                            	if (distrito_cargado[l].mes == k) {
                                    distrito_tendencias[k-1] = distrito_cargado[l];
                                    bandera++;
                                    break;
                            	}
                            }
                    	}
                    }
            	}

            	// Si ha encontrado los 12 meses calculamos tendencias
            	if (bandera == 12) {
                    // Imprimos las tendencias
                    system("cls");
                    regresion_lineal(distrito_tendencias);
            	}
            	else {
                	printf("No hemos localizado todos los datos para calcular las tendencias\n");
            	}

            	printf("\nPulsa cualquier tecla para volver al submen%c anterior\n", 163);
            	scanf("%25s", tecla_exit);

            	ver_menu=1;
            	break;
            }
            case 3:
            {
            	ver_menu=0;
            	break;
            }
            default:
            {
            	printf("\n");
            	printf("ERROR, la opci%cn elegida no est%c disponible, vuelva a introducir una opci%cn v%clida\n", 162, 161, 162, 161);
            	printf("\n");
            }
    	}
    } while(ver_menu!=0);

    return;
}


/*Función para visualizar un fichero mensual*/
void imprimir_fichero_mensual(struct distrito mi_distrito) {
    int i;

    printf("%-15s %-5s %-14s %-9s %-11s\n", "Parametros", "pH", "Conductividad", "Turbidez", "Coliformes");
    for (i=0; i<mi_distrito.num_fuentes; i++) {
        printf("%-15s ", mi_distrito.datos_fuente[i].nom_fuente);
        if ((mi_distrito.datos_fuente[i].pH>ALERTA_SUP_PH) || (mi_distrito.datos_fuente[i].pH<ALERTA_INF_PH)){
            printf("\x1b[41m\x1b[33m%-5.2f\x1b[0m ", mi_distrito.datos_fuente[i].pH);
        }
        else {
            printf("%-5.2f ", mi_distrito.datos_fuente[i].pH);
        }
        if ((mi_distrito.datos_fuente[i].conductividad>ALERTA_SUP_COND) || (mi_distrito.datos_fuente[i].conductividad<ALERTA_INF_COND)) {
            printf("\x1b[41m\x1b[33m%-14d\x1b[0m ", mi_distrito.datos_fuente[i].conductividad);
        }
        else {
            printf("%-14d ", mi_distrito.datos_fuente[i].conductividad);
        }
        if (mi_distrito.datos_fuente[i].turbidez>ALERTA_SUP_TURB) {
            printf("\x1b[41m\x1b[33m%-9d\x1b[0m ", mi_distrito.datos_fuente[i].turbidez);
        }
        else {
            printf("%-9d ", mi_distrito.datos_fuente[i].turbidez);
        }
        if (mi_distrito.datos_fuente[i].coliformes>ALERTA_SUP_COLI) {
            printf("\x1b[41m\x1b[33m%-11d\x1b[0m\n", mi_distrito.datos_fuente[i].coliformes);
        }
        else {
            printf("%-11d\n", mi_distrito.datos_fuente[i].coliformes);
        }
    }
}
/*Función para visualizar un dato específico*/
void imprimir_dato(struct distrito mi_distrito, int indice, char* dato) {
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
        printf("Dato inv%clido\n", 160);
    }
}

void visualizacion_ficheros(struct distrito distrito_cargado[100])
{
    // Submenú de visualización de ficheros y busqueda de datos
    int i, j, k, l, option, ver_menu=0;
    char tecla_exit[26];

    do {
        menu_visualizacion_datos();

        scanf("%d",&option);
        switch (option) {
            case 1:
            {
                system("cls");
                printf("Ficheros cargados disponibles para visualizar:\n");
                for (i=0; i<100; i++) {
                    // Cuando la estructura esté vacia salimos del bucle. Ya no hay nada más que imprimir
                    if (strcmp(distrito_cargado[i].nom_distrito, "") == 0) {
                        break;
                    }
                    else {
                        printf("Fichero %d: %d%02d_%s.csv\n", i+1, distrito_cargado[i].anio, distrito_cargado[i].mes, distrito_cargado[i].nom_distrito);
                    }
                }

                printf("\nIntroduzca por teclado el n%cmero del fichero que quiere visualizar:\n", 163);

                // Obtenemos por teclado el distrito a visualizar
                scanf("%2d", &j);

                system("cls");
                printf("Distrito: %s, Mes: %d, A%co: %d\n\n", distrito_cargado[j-1].nom_distrito, distrito_cargado[j-1].mes, 164, distrito_cargado[j-1].anio);

                // Imprimimos el distrito a visualizar
                imprimir_fichero_mensual(distrito_cargado[j-1]);

                printf("\nPulsa cualquier tecla para volver al submen%c anterior\n", 163);
                scanf("%25s", tecla_exit);

                ver_menu=1;
                break;
            }
            case 2:
            {
                char nombre_distrito[26];
                char parametro[26], tecla_exit[26];
                int temp_mes, temp_anio, temp_fuente, bandera=0;

                system("cls");
                printf("Introduce el par%cmetro a visualizar (Formato: pH, Conductividad, Turbidez o Coliformes):\n", 160);
                scanf("%25s", parametro);
                printf("Introduce el nombre del distrito (Formato: Distrito):\n");
                scanf("%25s", nombre_distrito);
                printf("Introduce el a%co a cargar (Formato: AAAA):\n", 164);
                scanf("%d", &temp_anio);
                printf("Introduce el mes (Formato: Mes en n%cmero):\n", 163);
                scanf("%d", &temp_mes);
                printf("Introduce el n%cmero de fuente:", 163);
                scanf("%d", &temp_fuente);

                // Buscamos el distrito, año y mes exactos
                for (k=0; k<100; k++) {
                    if (strcmp(distrito_cargado[k].nom_distrito, nombre_distrito) == 0) {
                        if (distrito_cargado[k].anio == temp_anio) {
                            if (distrito_cargado[k].mes == temp_mes) {
                                l = k;
                                bandera=1;
                                break;
                            }
                        }
                    }
                }

                if (bandera == 1) {
                    // Imprimos el dato solicitado
                    system("cls");
                    imprimir_dato(distrito_cargado[l], temp_fuente-1, parametro);
                }
                else {
                    printf("Par%cmetro no localizado\n", 160);
                }

                printf("\nPulsa cualquier tecla para volver al submen%c anterior\n", 163);
                scanf("%25s", tecla_exit);

                ver_menu=1;
                break;
            }
            case 3:
            {
                ver_menu=0;
                break;
            }
            default:
            {
                printf("\n");
                printf("ERROR, la opci%cn elegida no est%c disponible, vuelva a introducir una opci%cn v%clida\n", 162, 161, 162, 161);
                printf("\n");
            }
        }
    } while(ver_menu!=0);

    return;
}


/*Banner*/
void banner(void)
{
	
char caracter;	
printf("\n\n\n");                
printf("\x1B[34m    _____                        _____                                                       \033[0m \n");                   
printf("\x1B[34m ___|    _|__  ____    _____   __|___  |__  __   _  ______  ____   _    __    ______  ______   \033[0m\n");
printf("\x1B[34m|    \\  /  | ||    \\  |     \\ |   ___|    ||  | | ||   ___||    \\ | | _|  |_ |   ___||   ___|  \033[0m\n");
printf("\x1B[34m|     \\/   | ||     \\ |      \\|   ___|    ||  |_| ||   ___||     \\| ||_    _||   ___| `-.`-.  \033[0m \n");
printf("\x1B[34m|__/\\__/|__|_||__|\\__\\|______/|___|     __||______||______||__/\\____|  |__|  |______||______|  \033[0m\n");
printf("\x1B[34m    |_____|                      |_____|                                                       \033[0m\n");
printf("\n\n\n");
printf("\x1B[34m                   El programa definitivo para el an%lisis de agua \033[0m\n", 160);
printf(" \n\n                       Pulse cualquier tecla para continuar");

scanf("%c", &caracter);
system("cls");

    return 0;



}
/* Menú principal */
void menu_principal(void)
{
    	system("cls"); //limpiamos la consola
    	printf("Bienvenido al men%c del programa de registro de fuentes MADFUENTES.\n", 163);
    	printf("------------------------------------------------------------------\n\n");
    	printf("Qu%c desea hacer?\n", 130);
    	printf("----------------\n");
    	printf("\n");
    	printf("1 - Carga de ficheros.\n");
    	printf("\n");
    	printf("2 - Visualizaci%cn de datos.\n", 162);
    	printf("\n");
    	printf("3 - Mostrar estad%csticas.\n", 161);
    	printf("\n");
    	printf("4 - Cerrar el programa.\n");
    	printf("\n");
}

/* Menú carga de ficheros */
void menu_carga_ficheros(void)
{
    	system("cls"); //limpiamos la consola
    	printf("Submen%c de carga de ficheros.\n", 163);
    	printf("------------------------------\n\n");
    	printf("Qu%c desea hacer?\n", 130);
    	printf("----------------\n");
    	printf("\n");
    	printf("1 - Carga simple mensual de un fichero.\n");
    	printf("\n");
    	printf("2 - Carga anual de ficheros de un distrito.\n");
    	printf("\n");
    	printf("3 - Lista de ficheros cargados.\n");
    	printf("\n");
    	printf("4 - Volver al men%c principal.\n", 163);
    	printf("\n");
}

/* Menú visualización de datos */
void menu_visualizacion_datos(void)
{
    	system("cls"); //limpiamos la consola
    	printf("Submen%c de visualizaci%cn de datos.\n", 163, 162);
    	printf("----------------------------------\n\n");
    	printf("Qu%c desea hacer?\n", 130);
    	printf("----------------\n");
    	printf("\n");
    	printf("1 - Visualizaci%cn de fichero mensual.\n", 162);
    	printf("\n");
    	printf("2 - Visualizaci%cn de dato espec%cfico.\n", 162, 161);
    	printf("\n");
    	printf("3 - Volver al men%c principal.\n", 163);
    	printf("\n");
}

/* Menú cálculo estadísticas */
void menu_estadisticas(void)
{
    	system("cls"); //limpiamos la consola
    	printf("Submen%c de an%clisis estad%cstico.\n", 163, 160, 161);
    	printf("--------------------------------\n\n");
    	printf("Qu%c desea hacer?\n", 130);
    	printf("----------------\n");
    	printf("\n");
    	printf("1 - Estad%csticas b%csicas de fichero mensual.\n", 161, 160);
    	printf("\n");
    	printf("2 - Tendencia anual de un distrito.\n");
    	printf("\n");
    	printf("3 - Volver al men%c principal.\n", 163);
    	printf("\n");
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
