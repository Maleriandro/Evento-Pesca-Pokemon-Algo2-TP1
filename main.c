#include "evento_pesca.h"
#include <string.h>

#define RUTA_ARRECIFE "./arrecife.txt"
#define RUTA_DATOS_ACUARIO "./acuario_guardado.txt"
#define FORMATO_PRINTF "%s;%i;%i;%s\n"
#define CANTIDAD_CONDICIONES 5

//NO MODIFICAR
#define ERROR -1
#define SUCCESS 0

/*
Imprime el pokemon pasado por parametro 
*/
void imprimir_pokemon(pokemon_t* pokemon) {
    printf(FORMATO_PRINTF, (*pokemon).especie, (*pokemon).velocidad,
                              (*pokemon).peso, (*pokemon).color);
}

bool es_color(pokemon_t* pokemon, char* color) {
    return (strcmp(color, (*pokemon).color) == 0);
}

bool es_especie(pokemon_t* pokemon, char* especie) {
    return (strcmp(especie, (*pokemon).especie) == 0);
}

bool pesa_mas_de_40(pokemon_t* pokemon) {
    return ((*pokemon).peso > 40);
}

bool es_de_color_primario(pokemon_t* pokemon) {
    return (es_color(pokemon, "rojo") ||
            es_color(pokemon, "azul") ||
            es_color(pokemon,"verde") );
}

bool es_como_sonic(pokemon_t* pokemon) {
    return (((*pokemon).velocidad >= 30) && es_color(pokemon, "azul"));
}

bool es_lamentable(pokemon_t* pokemon) {
    return (es_especie(pokemon, "Magikarp"));
}

bool es_pesado_y_lento(pokemon_t* pokemon) {
    return (((*pokemon).velocidad <= 15) && ((*pokemon).peso >= 40));
}




int main() {

    int resultado_del_programa = SUCCESS;

    arrecife_t* arrecife = crear_arrecife(RUTA_ARRECIFE);

    acuario_t* acuario = crear_acuario();

    if (!arrecife || !acuario) return ERROR;


    // Agrego todos los punteros a funciones a la lista, para luego poder iterarla para ejecutar todas las funciones
    bool (*condicion_pokemon[CANTIDAD_CONDICIONES]) (pokemon_t*) = {pesa_mas_de_40,
                                                                    es_de_color_primario,
                                                                    es_como_sonic,
                                                                    es_lamentable,
                                                                    es_pesado_y_lento};
    
    int cant_pokemons_sacados[CANTIDAD_CONDICIONES] = {10,10,2,2,12};
    
    int transferencia_invalida = 0;

    // primero ejecuto la primera iteracion, para saber si debo censar el arrecife sin tener que incluir un if adentro del for
    transferencia_invalida = trasladar_pokemon(arrecife, acuario, condicion_pokemon[0], cant_pokemons_sacados[0]);

    for (int i = 1; i < CANTIDAD_CONDICIONES && transferencia_invalida == 0; i++) {
        censar_arrecife(arrecife, imprimir_pokemon);

        transferencia_invalida = trasladar_pokemon(arrecife, acuario, condicion_pokemon[i], cant_pokemons_sacados[i]);
    }

    if (transferencia_invalida != 0) {
        printf("Hubo un error en el programa, invalidando los datos de salida. Intentelo de nuevo\n");        
        resultado_del_programa = ERROR;
    } else {
 
        guardar_datos_acuario(acuario, RUTA_DATOS_ACUARIO);

    }
    

    liberar_acuario(acuario);
    liberar_arrecife(arrecife);


    return resultado_del_programa;
}