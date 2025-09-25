#include <iostream>
#include <fstream>

int mostrarMenu();
void consultarProducto(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam);
void actualizarStock(int codigos[], std::string nombres[], int stock[], std::string ubicaciones[], int tam);
void agregarProducto(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam);
void generarReporte(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam);
void buscarProductoMasCaro(std::string nombres[], float precios[], int tam);
void guardarDatos(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam);

/* La función main tiene las variables int de opcion_selec (la cual recibe el resultado de mostrarMenu()),
tam (los productos proedefinidos, con el proposito de no recorrer los otros 95 indices vacios de los arrays)
y tam_max (tamaño total de los arreglos). Así como los 4 arrays paralelos: codigos, nombres, stock y precios,
cada uno de ellos con 5 datos predefinidos.

El while principal del sistema es aquel en donde opcion_selec recibe la opción seleccionada en el menú que,
mientras no sea 5 (los valores mayores a 5 o menores a 1 ya se encuentran validados en la funcion de mostrarMenu()),
se seguira ejecutando en bucle, llamando a las funciones correspondientes en cada caso con la estructura switch. */

int main() {
    int opcion_selec = 0, tam = 5, tam_max = 100;

    int codigos[tam_max];
    std::string nombres[tam_max];
    int stock[tam_max];
    float precios[tam_max];
    std::string ubicaciones[tam_max];

    const std::string dataFileLocation = "C:\\Users\\as_co\\C\\actividad-cont-1b-vectores-paralelos-AngelCS0124\\data\\inventario.txt";
	std::ifstream dataFile;

	dataFile.open(dataFileLocation, std::ios::in);
    if (!dataFile) {
        std::cerr << "No se pudo abrir el archivo: " << dataFileLocation << std::endl;
    } else {
        std::cout << "Cargando datos desde el archivo: " << dataFileLocation <<
        std::endl;
        std::string linea;
        int indice = 0;
        std::getline(dataFile, linea);
        while (std::getline(dataFile, linea) && indice < tam_max) {
            size_t pos = 0;
            std::string token;

            pos = linea.find(',');
            token = linea.substr(0, pos);
            codigos[indice] = std::stoi(token);
            linea.erase(0, pos + 1);

            pos = linea.find(',');
            token = linea.substr(0, pos);
            nombres[indice] = token;
            linea.erase(0, pos + 1);

            pos = linea.find(',');
            token = linea.substr(0, pos);
            stock[indice] = std::stoi(token);
            linea.erase(0, pos + 1);

            pos = linea.find(',');
            token = linea.substr(0, pos);
            precios[indice] = std::stof(token);
            linea.erase(0, pos + 1);

            ubicaciones[indice] = linea;
            indice++;
        }
        dataFile.close();
        tam = indice;
        std::cout << "Datos cargados correctamente. " << tam << " productos leidos." << std::endl;
    }
    
    while (opcion_selec != 6){

        opcion_selec = mostrarMenu();

        switch (opcion_selec) {
            case 1:
                consultarProducto(codigos, nombres, stock, precios, ubicaciones, tam);
                break;
            case 2:
                actualizarStock(codigos, nombres, stock, ubicaciones, tam);
                break; 
            case 3:
                agregarProducto(codigos, nombres, stock, precios, ubicaciones, tam);
                break;
            case 4:
                generarReporte(codigos, nombres, stock, precios, ubicaciones, tam);
                break;  
            case 5:
                buscarProductoMasCaro(nombres, precios, tam);
                break;  
            case 6:
                std::cout << "Guardando y saliendo del sistema..." << std::endl;
                guardarDatos(codigos, nombres, stock, precios, ubicaciones, tam);
                break;    
        }

        std::cout << "\n";
    }
    
    return 0;
}

/* Esta función no recibe ningun parametro pero si devuelve la elección que el usuario ingreso de las
opciones del menú, la eleccion se almacena en la variable opcion, el menú se imprime mientras que 
la opción selecionada no entre entre 1 y 5, mostrandole un mensaje claro al usuario de que debe 
ingresar una opción válida, al final, cuando se ingresa una opción válida, se retorna la elección 
del usuario. */

int mostrarMenu(){
    int opcion = 0;
    bool input_valido = false;
    
    while (!input_valido || opcion < 1 || opcion > 6){
        std::cout << "--- Ferreteria 'El Martillo' ---" << std::endl;
        std::cout << "1. Consultar un producto" << std::endl;
        std::cout << "2. Actualizar inventario por ubicacion" << std::endl;
        std::cout << "3. Registrar un nuevo producto" << std::endl;
        std::cout << "4. Generar reporte de bajo stock" << std::endl;
        std::cout << "5. Encontrar el producto mas caro" << std::endl;
        std::cout << "6. Guardar y salir" << std::endl;
        std::cout << "--------------------------------" << std::endl;
        std::cout << "Digite una opcion: ";

        if (std::cin >> opcion){
            input_valido = true;

            std::cout << "\n";

            if (opcion < 1 || opcion > 6) {
                std::cout << "ERROR: Ingrese un numero valido." << std::endl;
            }
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n";
            std::cout << "ERROR: Por favor, ingrese un numero entero valido." << std::endl;
            input_valido = false;
        }
    }

    return opcion;
}

/* Esta función recibe como argumentos los 4 arrays paralelos, asi como el numero de productos actual o indices 
utilizados actualmente en los arrays, se definen las variables de ingreso de dato del codigo que busca el usuario
llamada codigo_a_buscar, la bandera de si se encontró el código ingresado y el indice en que se encontró. 

Se recorre el array de codigos, si no existe dicho producto, se le notifica al usuario, por otro lado, si
existe se guarda el indice. Se evalua la bandera encontrado, si es false, se le muestra un mensaje claro al 
usuario, si es true, se proceden a mostrar los datos de los aarrays paralelos en el indice en que se encontró
el código. */

void consultarProducto(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam){
    float encontrado = false;
    int indice_encontrado, codigo_a_buscar;
    bool input_valido = false;
    
    std::cout << "Ingrese el codigo del producto a consultar: ";

    if (std::cin >> codigo_a_buscar){
        input_valido = true;

        std::cout << "\n";
    } else {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "\n";
        std::cout << "ERROR: Por favor, ingrese un codigo numerico valido." << std::endl;
        input_valido = false;
    }

    if (input_valido) {
        for (int i = 0; i < tam; i++){
            if (codigo_a_buscar == codigos[i]) {
                encontrado = true;
                indice_encontrado = i;
                break;
            }
        }
    
        std::cout << "\n";

        if (!encontrado){
            std::cout << "ERROR: No existe un producto con este codigo." << std::endl;
        } else {
            std::cout << "Informacion del producto #" << codigo_a_buscar << std::endl;
            std::cout << "Nombre: " << nombres[indice_encontrado] << std::endl;
            std::cout << "Cantidad en stock: " << stock[indice_encontrado] << std::endl;
            std::cout << "Precio unitario: " << precios[indice_encontrado] << std::endl;
            std::cout << "Ubicacion: " << ubicaciones[indice_encontrado] << std::endl;
        }
    }
}

/* Esta función recibe como argumentos los arrays paralelos de codigos, nombres y stock, asi como
el numero de productos actual o indices utilizados actualmente en los arrays, se definen las variables
de ingreso de dato del codigo que busca el usuario llamada codigo_a_buscar, la bandera de si se encontró
el código ingresado, el indice en que se encontró y la cantidad con la que se actualiza el stock. 

Se recorre el array de codigos, si no existe dicho producto, se le notifica al usuario, por otro lado, si
existe se guarda el indice y la variable de cant_p_actualizar se modifica a un valor que no cumpla con la
condición de que la suma de mayor o igual a 0, esto para asegurarse de que el usuario siempre ingrese una
cantidad válida y el stock nunca sea menor que 0, por lo que se le pedira una cantidaad adecuada hasta que 
el stock no resulte negativo, se le muestra el error y se le vuelve a pedir el dato. Al final, cuando el stock 
se actualice a un valor válido se aplica a el array stock en el indice encontrado. */

void actualizarStock(int codigos[], std::string nombres[], int stock[], std::string ubicaciones[], int tam){
    float encontrado = false;
    int indice_encontrado, cant_p_actualizar;
    std::string ubicacion_a_encontrar;
    bool input_valido = false;

    std::cout << "Ingrese la ubicacion del producto al que actualizara el stock: ";
    
    if (std::cin >> ubicacion_a_encontrar){
        input_valido = true;

        std::cout << "\n";
    } else {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "\n";
        std::cout << "ERROR: Por favor, ingrese una ubicacion valida." << std::endl;
        input_valido = false;
    }

    if (input_valido) {
        for (int i = 0; i < tam; i++){
            if (ubicacion_a_encontrar == ubicaciones[i]) {
                encontrado = true;
                indice_encontrado = i;
                break;
            }
        }
    
        std::cout << "\n";

        input_valido = false;

        if (!encontrado){
            std::cout << "ERROR: No existe un producto con esta ubicacion." << std::endl;
        } else {
            cant_p_actualizar = ((stock[indice_encontrado])*(-1)) - 1;

            while ((stock[indice_encontrado] + cant_p_actualizar) < 0 || !input_valido) {
                std::cout << "Producto: " << nombres[indice_encontrado] << std::endl;
                std::cout << "Stock actual del producto: " << stock[indice_encontrado] << std::endl;
                std::cout << "Ingrese una cantidad para actualizar el stock (positiva para sumar y negativa para restar): ";

                if (std::cin >> cant_p_actualizar){
                    input_valido = true;

                    std::cout << "\n";
                } else {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "\n";
                    std::cout << "ERROR: Por favor, ingrese un valor numerico valido." << std::endl;
                    input_valido = false;
                }

                if (input_valido) {
                    if ((stock[indice_encontrado] + cant_p_actualizar) < 0) {
                        std::cout << "ERROR: El stock del producto no puede ser menor a 0." << std::endl;
                    }
                }
            }

            stock[indice_encontrado] += cant_p_actualizar;
            std::cout << "El stock de " << nombres[indice_encontrado] << " es de " << stock[indice_encontrado] << std::endl;
        }
    }
}

void agregarProducto(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam){
    int codigo = 101, stock_valor;
    std::string ubicacion = "A-01", nombre;
    float precio;
    bool cod_duplicado = true, ubi_duplicado = true, input_valido = false;

    while (cod_duplicado || !input_valido){
        std::cout << "Ingrese el codigo del producto: ";
        if (std::cin >> codigo){
            input_valido = true;

            std::cout << "\n";
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n";
            std::cout << "ERROR: Por favor, ingrese un codigo numerico valido." << std::endl;
            input_valido = false;
        }
        if (input_valido) {
            for (int i = 0; i < tam; i++){
                if (codigo == codigos[i]){
                    cod_duplicado = true;
                    break;
                } else {
                    cod_duplicado = false;
                }
            }

            if (!cod_duplicado){
                codigos[tam] = codigo;
            } else {
                std::cout << "ERROR: El codigo ya existe, ingrese uno diferente." << std::endl;
            }
        }
    }

    std::cout << "Ingrese el nombre del producto: ";
    std::cin >> nombre;
    nombres[tam] = nombre;

    input_valido = false;

    while (!input_valido){
        std::cout << "Ingrese el precio unitario del producto: ";
        if (std::cin >> precio){
            input_valido = true;

            std::cout << "\n";
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n";
            std::cout << "ERROR: Por favor, ingrese un precio numerico valido." << std::endl;
            input_valido = false;
        }

        if (input_valido && precio < 0) {
            std::cout << "ERROR: El precio no puede ser menor a 0." << std::endl;
        } else {
            precios[tam] = precio;
        }
    }
    
    input_valido = false;

    while (!input_valido){
        std::cout << "Ingrese la cantidad en stock del producto: ";
        if (std::cin >> stock_valor){
            input_valido = true;

            std::cout << "\n";
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n";
            std::cout << "ERROR: Por favor, ingrese una cantidad numerica valida." << std::endl;
            input_valido = false;
        }

        if (input_valido && stock_valor < 0) {
            std::cout << "ERROR: La cantidad en stock no puede ser menor a 0." << std::endl;
        } else {
            stock[tam] = stock_valor;
        }
    }

    input_valido = false;

    while (ubi_duplicado || !input_valido){
        std::cout << "Ingrese la ubicacion del producto: ";
        if (std::cin >> ubicacion){
            input_valido = true;

            std::cout << "\n";
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n";
            std::cout << "ERROR: Por favor, ingrese una ubicacion valida." << std::endl;
            input_valido = false;
        }
        if (input_valido) {
            for (int i = 0; i < tam; i++){
                if (ubicacion == ubicaciones[i]){
                    ubi_duplicado = true;
                    break;
                } else {
                    ubi_duplicado = false;
                }
            }

            if (!ubi_duplicado){
                ubicaciones[tam] = ubicacion;
            } else {
                std::cout << "ERROR: La ubicacion ya existe, ingrese una diferente." << std::endl;
            }
        }
    }

    std::cout << "Producto agregado exitosamente." << std::endl;
    
}

/* Esta función recibe como argumentos los 4 arrays paralelos y la cantidad de productos actualmente (solo 5 de los 100 
indices de los arrays se utilizan).

Se recorren los 4 arrays desde el indice 0 hasta que sea menor a tam (menor a 5), mientras tanto, se imprimen los
datos de los 4 arrays en cada indice, con el fin de mostrar todos los productos y sus detalles. */

void generarReporte(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam){

    std::cout << "----------- REPORTE DE BAJO STOCK -----------" << std::endl;
    std::cout << " Codigo | Nombre | Stock | Precio unitario" << std::endl;

    int lineas = 0;

    for (int i = 0; i < tam; i++){
        if (stock[i] < 10){
            std::cout << codigos[i] << " | " << nombres[i] << " | " << stock[i] << " | " << precios[i] << " | "  << ubicaciones[i] << std::endl;
            lineas++;
        }
    }

    if (lineas == 0){
            std::cout << "No hay productos con bajo stock (> 10)" << std::endl;
    }
    
    std::cout << "--------------------------------------------" << std::endl;
}

/* Esta función recibe como argumentos los arrays de nombre y precios, además la cantidad de productos actualmente.

Se recorre el array de precio y se compara con la variable de precioMasAlto, la cual va almacenando el más
alto que se vaya encontrando, es decir, si se encuntrea uno mayor al anteriormente guardado entonces se reemplaza, 
se almacenan en las variable precioMasAlto y nombreProductoMasCaro, al final se muestra el producto más caro en un mensaje. */

void buscarProductoMasCaro(std::string nombres[], float precios[], int tam){
    float precioMasAlto = 0.00;
    std::string nombreProductoMasCaro;
    for (int i = 0; i < tam; i++) {
        if (precios[i] > precioMasAlto) {
            precioMasAlto = precios[i];
            nombreProductoMasCaro = nombres[i];
        }
    }
    
    std::cout << "'" << nombreProductoMasCaro << "' es el producto mas caro con un precio de $" << precioMasAlto << std::endl;
}

void guardarDatos(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam) {
    const std::string dataFileLocation = "C:\\Users\\as_co\\C\\actividad-cont-1b-vectores-paralelos-AngelCS0124\\data\\inventario.txt";
    std::ofstream dataFile;

    dataFile.open(dataFileLocation, std::ios::out);
    if (!dataFile) {
        std::cerr << "No se pudo abrir el archivo para guardar: " << dataFileLocation << std::endl;
    } else {
        std::cout << "Guardando datos en el archivo: " << dataFileLocation << std::endl;

        dataFile << "Código,Nombre,Cantidad,Precio,Ubicación" << std::endl;
        for (int i = 0; i < tam; i++) {
            dataFile << codigos[i] << "," << nombres[i] << "," << stock[i] << "," << precios[i] << "," << ubicaciones[i] << std::endl;
        }

        dataFile.close();
        std::cout << "Datos guardados correctamente." << std::endl;
    }
}