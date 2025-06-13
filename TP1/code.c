/*
Introducción
En un lenguaje de programación existen diversos sub lenguajes
regulares, por lo tanto, son una pieza fundamental. Se ha visto en
clase distintas maneras de identificar cuando un lenguaje es
regular.
El presente trabajo consiste en realizar un programa en lenguaje C
que permita generar aleatoriamente palabras de un lenguaje regular a
partir de una gramática ingresada por el usuario.
Desarrollo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_PRODUCCIONES 100
#define TAM_TERMINALES 10
#define TAM 100

/*
Las produciones tiene una forma determinada
Produccion = Izquierda -> Derecha
*/

typedef struct {
    char izquierda[TAM_TERMINALES];
    char derecha[TAM_TERMINALES];
} Produccion;

/*
La gramatica se ingresa en forma de cuatrupla
G = (símbolos terminales, símbolos no terminales, producciones, axioma)
*/

typedef struct {
    char terminales[TAM_TERMINALES]; // S -> ...
    int cant_terminales;
    char no_terminales[TAM_TERMINALES]; // ... -> a ... (Basicamente Nuestro Alfabeto)
    int cant_no_terminales;
    Produccion producciones[TAM_PRODUCCIONES];
    int cant_producciones;
    char axioma;
} Gramatica;

/*
1. Ingresar una gramática: esto quiere decir que se debe
poder ingresar la cuatrupla: símbolos terminales,
símbolos no terminales, producciones y axioma.
*/

void ingresar_gramatica(Gramatica *g) { //Espera un puntero (Una direccion de memoria) a una variable de tipo Gramatica
                                        //Como g es una direccion de memoria, uso -> para acceder a sus atributos 
                                        //Para acceder a los atributos de producciones uso .   

    printf("Ingrese Una Gramatica\n");

    //Ingresar Terminales
    printf("Cantidad de terminales: ");
    scanf("%d", &g -> cant_terminales); 

    printf("Ingrese los terminales:\n");
    for (int i = 0; i < g -> cant_terminales; i++) {
        printf("Terminal Numero %d: ", i+1);
        scanf(" %c", &g -> terminales[i]);
    }

    //Ingresar No Terminales
    printf("Cantidad de no terminales: ");
    scanf("%d", &g -> cant_no_terminales);

    printf("Ingrese los no terminales:\n");
    for (int i = 0; i < g -> cant_no_terminales; i++) {
        printf("No Terminal Numero %d: ", i+1);
        scanf(" %c", &g -> no_terminales[i]);
    }

    //Ingreso Producciones
    printf("Cantidad de producciones: ");
    scanf("%d", &g -> cant_producciones);

    printf("Ingrese las producciones (Formato: Izquierda -> Derecha):\n");
    for (int i = 0; i < g -> cant_producciones; i++) {
        printf("Producción %d:\n", i+1);
        printf("Izquierda: ");
        scanf("%s", g -> producciones[i].izquierda);
        printf("Derecha: ");
        scanf("%s", g -> producciones[i].derecha);
        printf("Producción Ingresada: %s -> %s \n", g -> producciones[i].izquierda, g -> producciones[i].derecha);
    }

    //Ingreso Axioma
    printf("Ingrese el axioma: ");
    scanf(" %c", &g->axioma);
}

/*
2. Validar si la gramática es regular o no: El programa no
debe permitir el ingreso de gramáticas que no sean
regulares.
*/

int es_regular(Gramatica g) {   //Recibo una copia de una variable Gramatica, no me interesa utilizar el original
                                //Como g es una copia, uso . para acceder a sus atributos 

    for (int i = 0; i < g.cant_producciones; i++) { //Para validar si la gramatica es regular analizamos las producciones

        int cant_caracteres_izquierda = strlen(g.producciones[i].izquierda);
        int cant_caracteres_derecha = strlen(g.producciones[i].derecha);
        
        /*
        strchr (char *cadena, int caracter)
        Primer parámetro → Cadena donde busca
        Segundo parámetro → Carácter a buscar
        
        strchr busca si el primer caracter de la parte izquierda de una produccion
        Produccion = Izquierda -> Derechaen, pertenece a los No Terminales.
        
        En caso de tener aS -> ... 
        No me interesa analizar el segundo caracter
        Las gramaticas reguales deben tener 1 no terminal en su parte izquierda
        */

        // Parte Izquierda: Debe ser 1 solo caracter (no terminal)
        if (cant_caracteres_izquierda != 1 || strchr(g.no_terminales, g.producciones[i].izquierda[0]) == NULL)
            return 0;

        /*
        Gramatica Regular A Izquierda

        Podriamos contemplar tambien el caso 
        Parte Derecha: Puede ser a o Ba
        */

        // Parte Derecha: Puede ser a  o  aB
        if (cant_caracteres_derecha == 1) { // Parte Derecha: a
            if (strchr( g.terminales, g.producciones[i].derecha[0]) == NULL)
                return 0;
        } 
        else {
            if (cant_caracteres_derecha == 2) { // Parte Izquieda aB
                if (strchr(g.terminales, g.producciones[i].derecha[0]) == NULL)
                    return 0;
                if (strchr(g.no_terminales, g.producciones[i].derecha[1]) == NULL)
                    return 0;
            } 
            else {
                return 0;
            }
        } 
    }

    return 1;
}

/*
3. Generar aleatoriamente palabras del lenguaje, cada vez
que genera una palabra nueva debe mostrar el proceso de
derivación que se realiza. Puede mostrar una derivación
horizontal.
*/

void generar_palabra(Gramatica g) {

    char palabra[TAM] = "";
    char no_terminal_actual = g.axioma;

    printf("Derivacion: %c", no_terminal_actual);

    while (1) { 
        
        Produccion candidatas[TAM_PRODUCCIONES];
        int cant_candidatas = 0;

        // Buscar producciones aplicables
        for (int i = 0; i < g.cant_producciones; i++) {
            if (g.producciones[i].izquierda[0] == no_terminal_actual) {
                candidatas[cant_candidatas++] = g.producciones[i];
            }
        }

        if (cant_candidatas == 0) {
            break; // Si no tenemos candidatas, rompemos el While
        }

        // Elegir una producción aleatoria
        int ran = rand() % cant_candidatas; // Genera un número aleatorio entre 0 y cant_candidatas - 1
        Produccion produccion_seleccionada = candidatas[ran];

        // Mostrar derivación
        printf(" => %s", produccion_seleccionada.derecha);

        // Agregar terminal
        palabra[strlen(palabra)] = produccion_seleccionada.derecha[0];
        palabra[strlen(palabra) + 1] = '\0'; 

        // Analizo la produccion
        if (strlen(produccion_seleccionada.derecha) == 1) { // Si la producción es solo terminal, terminamos
            break;
        } else { // Si no continuamos con el nuevo no terminal
            no_terminal_actual = produccion_seleccionada.derecha[1];
        }
    }

    printf("\nPalabra generada: %s\n", palabra);
}

int main() {

    srand(time(NULL)); //Esta linea permite que las secuencias que se generan sean diferentes

    Gramatica gramatica;

    ingresar_gramatica(&gramatica); //Envio la direccion de memoria de gramatica
    
    if (es_regular(gramatica)) {
        printf("La gramática es Regular.\n");

        char opcion;
        do {
            generar_palabra(gramatica);
            printf("¿Generar otra palabra? (s/n): ");
            scanf(" %c", &opcion);
        } while (opcion == 's' || opcion == 'S');
    } 
    else {
        printf("La gramática NO es Regular.\n");
    }

    return 0;
}
