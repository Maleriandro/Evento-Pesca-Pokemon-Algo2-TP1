FUNCIONAMIENTO DEL PROGRAMA

Los #define los use para definir los parametros del programa, teniendo que modificarse en el caso
de querer modificar el funcionamiento del programa.

La funcion crear_arrecife() obtiene los pokemons a travez del arrecife.txt iterando sobre cada linea del
archivo, hasta que el archivo se termine, o no se devuelvan los 4 parametros esperados. 
Por cada linea del archivo se hace un realloc para agrandar el vector de pokemons, para albergar un
pokemon más. En el caso de que un realloc falle, tambien se termina la iteracion de las lineas, y se devuelve
los pokemons que si se pudieron guardar.


transladar_pokemons() primeto itera sobre los pokemons del arrecife viejo, y en un vector inicializado en ceros,
va indicando en que indice se van encontrando los pokemons validos, además de contar si hay suficientes pokemons
en el arrecife. En el caso de que no haya suficientes se devuelven el acuario y arrecife originales.


Si hay suficientes pokemons, se itera sobre el vector que indica los pokemons validos, y sobre el arrecife viejo,
si se encuentra un pokemon valido, lo agrega al acuario que fue previamente agrandado. Si el pokemon no es valido,
lo agrega a un nuevo arrecife vacío, donde van a quedar los pokemons no validos. 



aclaracion: el archivo del arrecife contiene pokemons que pueden no ser tipo agua



COMPILACION

Se necesita el main.c, evento_pesca.c y evento_pesca.h para poder compilar el programa.
Se compila con el comando "gcc *.c -Wall -Werror -Wconversion -std=c99 -o evento_pesca" sin las comillas.

Eso crea un archivo evento_pesca, que puede ser ejecutado con la consola, estando en el mismo
directorio que el archivo, con el comando "./evento_pesca". Antes asegurarse de que el archivo arrecife.txt
se encuentra en el mismo directorio que el ejecutable.



EXPLICACION DE CONCEPTOS

1. Un puntero es un tipo de dato que contiene una ubicacion de memoria. Lo que hace esto,
es que este puntero "haga referencia" a otro dato, pudiendo usar directamente la ubicacion
de memoria, o "recorrer el puntero" para poder llegar al dato que se encuentra en la ubicacion
de memoria que contiene el puntero.

2. La aritmetica de punteros, es la manera en la que se puede modificar e interactuar con punteros
u otras variables, interpretandolas como punteros.
Estas maneras son
& que nos permite acceder a la ubicacion de memoria de una variable,
* que nos permite declarar un puntero, o recorrer un puntero para llegar a la ubicacion de memoria que referencia
[numero] que funciona de una manera similar a *, pudiendo acceder a la ubicacion de memoria que referencia
el puntero usando el cero: [0]; o pudiendo aumentar el numero para poder acceder a las ubicaciones de memorias
adyacentes a la que hace referencia el puntero, siendo esto usado principalmente para los vectores.

3. Un puntero a funcion, permite guardar la ubicacion de una funcion en una variable, pudiendo pasarla como parametro
a otra funcion, y ejecutarla alli dentro, o para poder devolverla como resultado de otra funcion. Tambien puede ser
usado para meter la funcion dentro de un vector. Estas funciones se crean, poniendo un asterisco antes del nombre,
y encerrarlo en parentesis:  ... (*funcion) ...

4. malloc() y realloc() son funciones que nos permiten obtener memoria dinamica ubicada en el heap. Esto se puede usar
para crear vectores de un tamaño que no esta definido previamente.
malloc() obtiene este espacio de memoria dinamica con el tamaño en bytes que le pasemos por parametro, y devuelve un
puntero a la ubicacion de ese espacio de memoria dinamica, o en el caso de que no pueda obtener esa cantidad de memoria,
devuelve el puntero NULL.
realloc() funciona de manera similar, solo que en vez de obtener un espacio de memoria nuevo, agranda o achica un espacio
de memoria previamente existente conservando el contenido que tenía previamnte. Devuelve la ubicacion de memoria donde
se encuentra el espacio reservado, pudiendo ser este el mismo de antes o uno nuevo; o devuelve NULL en el caso de que no
pudiera obtener más memoria

Al ser memoria administrada por el programador, esta no se destruye en ningun momento, sino que en el momento que el
programa no la necesita mas, tiene que ser liberada con la funcion free().
