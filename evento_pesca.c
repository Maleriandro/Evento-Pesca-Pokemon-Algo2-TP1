#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "evento_pesca.h"

#define FORMATO_LECTURA "%[^;];%d;%d;%[^\n]\n"
#define FORMATO_ESCRITURA "%s;%i;%i;%s\n"
#define CANT_CARACTERISTICAS_POKEMON 4

//NO MODIFICAR
#define ERROR -1
#define SUCCESS 0


arrecife_t* crear_arrecife(const char* ruta_archivo) {
    FILE* archivo = fopen(ruta_archivo,"r");
    if(!archivo){
        printf("No se pudo cargar ningun dato del Arrefice\n");
        return NULL;
    }


    bool alloc_ok = true;  //Indica si el malloc o realloc se realizó correctamente
    pokemon_t *pokemon = malloc(sizeof(pokemon_t)); 

    if (pokemon == NULL) alloc_ok = false;

    int nro_pokemon = 0; //Indice de pokemon a leer

    int leidos;
    
    if (alloc_ok) leidos = fscanf(archivo, FORMATO_LECTURA,
                        pokemon[0].especie, &pokemon[0].velocidad,
                        &pokemon[0].peso, pokemon[0].color);

    while(leidos != EOF && leidos == CANT_CARACTERISTICAS_POKEMON && alloc_ok){
        nro_pokemon++;

        pokemon_t *new_pokemon = realloc(pokemon, (sizeof(pokemon_t) * ((size_t)nro_pokemon +1)));

        if (new_pokemon == NULL) {
            alloc_ok = false;
        } else {
            pokemon = new_pokemon;

            //Esta linea lee y almacena la ultima linea, sea EOF o una linea erronea, pero el
            //nro_pokemon no aumenta, por lo que si bien el dato queda dentro del "bloque", nunca 
            //llega a utilizarse por estar afuera del "nro_pokemon"
            leidos = fscanf(archivo, FORMATO_LECTURA,
                            pokemon[nro_pokemon].especie, &pokemon[nro_pokemon].velocidad,
                            &pokemon[nro_pokemon].peso, pokemon[nro_pokemon].color); 
        }
    }
    fclose(archivo);

    // Achica el vector, ya que el ultimo parametro es invalido, siempre y cuando
    // el for no haya terminado por un error del realloc
    if (alloc_ok) {
        pokemon_t *new_pokemon = realloc(pokemon, (sizeof(pokemon_t) * ((size_t)nro_pokemon)));

        // En el caso de que no pueda achicar el vector, no devuelvo error ya que igualmente
        // el ultimo elemento del vector no se usaba en el resto del programa por la
        // cantidad_pokemon
        if (new_pokemon) pokemon = new_pokemon;

    }

    arrecife_t *arrecife = malloc(sizeof(arrecife_t));

    if (arrecife) {

    (*arrecife).pokemon = pokemon;
    (*arrecife).cantidad_pokemon = nro_pokemon;
    }
    

    return arrecife;
}


acuario_t* crear_acuario() {
    acuario_t* acuario = malloc(sizeof(acuario_t));

    (*acuario).cantidad_pokemon = 0;
    (*acuario).pokemon = NULL;


    return acuario;
}

/*
Función que deberá sacar del arrecife a todos los pokémon que satisfagan la condición dada por
el puntero a función (que devuelvan true) y trasladarlos hacia el acuario. El parámetro cant_seleccion
especifica la cantidad máximade pokémon que serán trasladados. En caso de que haya menos pokémon trasladables en el
arrecife que los pedidos, no se deberá mover ninguno para conservar los pocos existentes. El vector de pokemones
del arrecife quedará solo con aquellos que no fueron trasladados (su tamaño se ajustará luego de cada traslado).
El vector de pokemones del acuarió quedará con aquellos que fueron trasladados esta vez más los que ya había en el
acuario (su tamaño se ajustará luego de cada traslado). Devuelve -1 en caso de error o 0 en caso contrario.
 */
int trasladar_pokemon(arrecife_t* arrecife, acuario_t* acuario, bool (*seleccionar_pokemon) (pokemon_t*), int cant_seleccion) {
    size_t cant_pkmn_arrecife_viejo = (size_t)(*arrecife).cantidad_pokemon;
    pokemon_t* pkmn_arrecife_viejo = ((*arrecife).pokemon);

    size_t cant_pkmn_acuario_viejo = (size_t)(*acuario).cantidad_pokemon;
    pokemon_t* pkmn_acuario_viejo = ((*acuario).pokemon);

    // Uso este vector inicializado con false (0) para poder ir asignando en cada posicion
    // si el pokemon del arrecife en esa posicion, es uno que debo transferir.
    bool *es_pkmn_encontrado = calloc(cant_pkmn_arrecife_viejo, sizeof(bool));
    if (!es_pkmn_encontrado) return ERROR;
    bool hay_suficientes_pokemons = false;


    size_t cant_pokemons_encontrados = 0;

    // En este for, itero sobre todos los pokemons del arrecife, para guardar en el vector
    // es_pkmn_encontrado cuales son los que debería transferir, y contar si hay suficientes
    // pokemons, o no tengo que transferir ninguno
    for (int i = 0; i < (int)cant_pkmn_arrecife_viejo; i++) {
        bool es_pokemon_buscado = seleccionar_pokemon(&pkmn_arrecife_viejo[i]);

        if (es_pokemon_buscado && (cant_pokemons_encontrados < cant_seleccion)) {

            cant_pokemons_encontrados++;
            es_pkmn_encontrado[i] = true;
        } else if (es_pokemon_buscado && (cant_pokemons_encontrados >= cant_seleccion)) {
            cant_pokemons_encontrados++;
        }
    }
    cant_pokemons_encontrados++;

    if (cant_pokemons_encontrados > cant_seleccion) hay_suficientes_pokemons = true;

    int resultado_de_funcion;
    if (!hay_suficientes_pokemons) {
        printf("no hay suficientes pokemones como para sacar\n");
        resultado_de_funcion = ERROR;
    } else {
        pokemon_t* nuevo_acuario  = realloc(pkmn_acuario_viejo, sizeof(pokemon_t) * (cant_pkmn_acuario_viejo + (size_t)cant_seleccion));
        pokemon_t* nuevo_arrecife = malloc(sizeof(pokemon_t)* (cant_pkmn_arrecife_viejo - (size_t)cant_seleccion));

        if (!nuevo_acuario || !nuevo_arrecife) return ERROR;

        int indice_nuevo_acuario = (int)cant_pkmn_acuario_viejo;
        int indice_nuevo_arrecife = 0;

        for (int i = 0; i < cant_pkmn_arrecife_viejo; i++) {
            pokemon_t pkmn_actual = pkmn_arrecife_viejo[i];
            if (es_pkmn_encontrado[i]) {
                nuevo_acuario[indice_nuevo_acuario] = pkmn_actual;
                indice_nuevo_acuario++;
            } else {
                nuevo_arrecife[indice_nuevo_arrecife] = pkmn_actual;
                indice_nuevo_arrecife++;
            }

        }
        (*acuario).cantidad_pokemon =  (int)(indice_nuevo_acuario);
        (*acuario).pokemon = nuevo_acuario;


        free((*arrecife).pokemon);
        (*arrecife).cantidad_pokemon =  (int)(indice_nuevo_arrecife);
        (*arrecife).pokemon = nuevo_arrecife;

        resultado_de_funcion = SUCCESS;
    }
    free(es_pkmn_encontrado);
    return resultado_de_funcion;
}


void censar_arrecife(arrecife_t* arrecife, void (*mostrar_pokemon)(pokemon_t*)){

    printf("Especie; Velocida; Peso; Color\n");
    for (int i = 0; i < (*arrecife).cantidad_pokemon; i++) {
        pokemon_t pokemon_actual = (*arrecife).pokemon[i];

        mostrar_pokemon(&pokemon_actual);
    }
    printf("\n");

}

int guardar_datos_acuario(acuario_t* acuario, const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo,"w");
    if(!archivo){
        printf("No se pudo guardar los datos del acuario\n");
        return ERROR;
    }

    for (int i = 0; i < (*acuario).cantidad_pokemon; i++) {
        pokemon_t pokemon_actual = (*acuario).pokemon[i];


        fprintf(archivo, FORMATO_ESCRITURA, pokemon_actual.especie, pokemon_actual.velocidad,
                                            pokemon_actual.peso, pokemon_actual.color);
        
    }

    fclose(archivo);

    return SUCCESS;
}

void liberar_acuario(acuario_t* acuario) {
    pokemon_t* vector_pokemons = (*acuario).pokemon;
    if (vector_pokemons) free(vector_pokemons);
    free(acuario);
}

void liberar_arrecife(arrecife_t* arrecife) {
    pokemon_t* vector_pokemons = (*arrecife).pokemon;

    if (vector_pokemons) free(vector_pokemons);
    free(arrecife);
}
