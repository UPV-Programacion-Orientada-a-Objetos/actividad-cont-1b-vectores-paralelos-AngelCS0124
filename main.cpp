#include <iostream>
#include <fstream>

bool validarEntradaNumerica(int& valor, const std::string& mensaje) {
    while (true) {
        std::cout << mensaje;
        
        if (std::cin >> valor) {
            return true;
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "WARNING: Por favor, ingrese un numero entero valido." << std::endl;
        }
    }
}

bool validarEntradaDecimal(float& valor, const std::string& mensaje) {
    while (true) {
        std::cout << mensaje;
        
        if (std::cin >> valor) {
            return true;
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "WARNING: Por favor, ingrese un numero decimal valido." << std::endl;
        }
    }
}

int mostrarMenu();
void consultarProducto(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam);
void actualizarStock(int codigos[], std::string nombres[], int stock[], std::string ubicaciones[], int tam);
void agregarProducto(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam);
void generarReporte(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam);
void buscarProductoMasCaro(std::string nombres[], float precios[], int tam);
void guardarDatos(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam);

int main() {
    int opcion_selec = 0, tam = 0, tam_max = 200;

    int codigos[tam_max];
    std::string nombres[tam_max];
    int stock[tam_max];
    float precios[tam_max];
    std::string ubicaciones[tam_max];

    const std::string dataFileLocation = "C:\\Users\\as_co\\C\\actividad-cont-1b-vectores-paralelos-AngelCS0124\\data\\inventario.csv";
    std::ifstream dataFile;

    dataFile.open(dataFileLocation, std::ios::in);
    if (!dataFile) {
        std::cout << "WARNING: No se pudo abrir el archivo: " << dataFileLocation << std::endl;
        std::cout << "0 productos encontrados. Inventario vacio." << std::endl;
    } else {
        std::cout << "Cargando datos desde el archivo: " << dataFileLocation << std::endl;
        std::string linea;
        int indice = 0;
        std::getline(dataFile, linea);
        
        while (std::getline(dataFile, linea) && indice < tam_max) {
            if (linea.empty()) continue;
            
            size_t pos = 0;
            std::string token;

            try {
                pos = linea.find(',');
                if (pos == std::string::npos) continue;
                token = linea.substr(0, pos);
                codigos[indice] = std::stoi(token);
                linea.erase(0, pos + 1);

                pos = linea.find(',');
                if (pos == std::string::npos) continue;
                token = linea.substr(0, pos);
                ubicaciones[indice] = token;
                linea.erase(0, pos + 1);

                pos = linea.find(',');
                if (pos == std::string::npos) continue;
                token = linea.substr(0, pos);
                nombres[indice] = token;
                linea.erase(0, pos + 1);

                pos = linea.find(',');
                if (pos == std::string::npos) continue;
                token = linea.substr(0, pos);
                int stock_aux = std::stoi(token);
                if (stock_aux > 50) {
                    std::cout << "Hay sobrestock de " << nombres[indice] << ". Stock de " << stock_aux << " unidades" << std::endl;
                }
                stock[indice] = stock_aux;
                linea.erase(0, pos + 1);

                precios[indice] = std::stof(linea);
                indice++;
            } catch (const std::exception& e) {
                std::cout << "ADVERTENCIA: Error al procesar linea: " << linea << std::endl;
                continue;
            }
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
                if (tam < tam_max) {
                    agregarProducto(codigos, nombres, stock, precios, ubicaciones, tam);
                    tam++;
                } else {
                    std::cout << "WARNING: No se pueden agregar mas productos. Capacidad maxima alcanzada." << std::endl;
                }
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

int mostrarMenu(){
    int opcion = 0;
    
    while (true){
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
            if (opcion >= 1 && opcion <= 6) {
                std::cout << "\n";
                return opcion;
            } else {
                std::cout << "WARNING: Ingrese un numero entre 1 y 6." << std::endl;
            }
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "WARNING: Por favor, ingrese un numero entero valido." << std::endl;
        }
    }
}

void consultarProducto(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam){
    int codigo_a_buscar;
    
    if (!validarEntradaNumerica(codigo_a_buscar, "Ingrese el codigo del producto a consultar: ")) {
        return;
    }

    bool encontrado = false;
    int indice_encontrado = -1;

    for (int i = 0; i < tam; i++){
        if (codigo_a_buscar == codigos[i]) {
            encontrado = true;
            indice_encontrado = i;
            break;
        }
    }

    if (!encontrado){
        std::cout << "WARNING: No existe un producto con este codigo." << std::endl;
    } else {
        std::cout << "\nInformacion del producto #" << codigo_a_buscar << std::endl;
        std::cout << "Nombre: " << nombres[indice_encontrado] << std::endl;
        std::cout << "Cantidad en stock: " << stock[indice_encontrado] << std::endl;
        std::cout << "Precio unitario: $" << precios[indice_encontrado] << std::endl;
        std::cout << "Ubicacion: " << ubicaciones[indice_encontrado] << std::endl;
    }
}

void actualizarStock(int codigos[], std::string nombres[], int stock[], std::string ubicaciones[], int tam){
    std::string ubicacion_a_encontrar;
    
    std::cout << "Ingrese la ubicacion del producto al que actualizara el stock: ";
    std::cin >> ubicacion_a_encontrar;

    bool encontrado = false;
    int indice_encontrado = -1;

    for (int i = 0; i < tam; i++){
        if (ubicacion_a_encontrar == ubicaciones[i]) {
            encontrado = true;
            indice_encontrado = i;
            break;
        }
    }

    if (!encontrado){
        std::cout << "WARNING: No existe un producto con esta ubicacion." << std::endl;
        return;
    }

    int cant_p_actualizar;
    bool actualizacion_valida = false;

    while (!actualizacion_valida) {
        std::cout << "\nProducto: " << nombres[indice_encontrado] << std::endl;
        std::cout << "Stock actual del producto: " << stock[indice_encontrado] << std::endl;
        
        if (!validarEntradaNumerica(cant_p_actualizar, "Ingrese una cantidad para actualizar el stock (positiva para sumar y negativa para restar): ")) {
            return;
        }

        if ((stock[indice_encontrado] + cant_p_actualizar) < 0) {
            std::cout << "WARNING: El stock del producto no puede ser menor a 0." << std::endl;
        } else {
            actualizacion_valida = true;
        }
    }

    stock[indice_encontrado] += cant_p_actualizar;
    std::cout << "El stock de '" << nombres[indice_encontrado] << "' se actualizo a " << stock[indice_encontrado] << " unidades." << std::endl;
}

void agregarProducto(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam){
    int codigo;
    std::string nombre;
    float precio;
    int stock_valor;
    std::string ubicacion;

    bool codigo_valido = false;
    while (!codigo_valido) {
        if (!validarEntradaNumerica(codigo, "Ingrese el codigo del producto: ")) {
            return;
        }

        bool codigo_duplicado = false;
        for (int i = 0; i < tam; i++) {
            if (codigo == codigos[i]) {
                codigo_duplicado = true;
                break;
            }
        }

        if (codigo_duplicado) {
            std::cout << "WARNING: El codigo ya existe, ingrese uno diferente." << std::endl;
        } else {
            codigo_valido = true;
        }
    }

    std::cout << "Ingrese el nombre del producto: ";
    std::cin.ignore();
    std::getline(std::cin, nombre);

    bool precio_valido = false;
    while (!precio_valido) {
        if (!validarEntradaDecimal(precio, "Ingrese el precio unitario del producto: ")) {
            return;
        }

        if (precio < 0) {
            std::cout << "WARNING: El precio no puede ser menor a 0." << std::endl;
        } else {
            precio_valido = true;
        }
    }

    bool stock_valido = false;
    while (!stock_valido) {
        if (!validarEntradaNumerica(stock_valor, "Ingrese la cantidad en stock del producto: ")) {
            return;
        }

        if (stock_valor < 0) {
            std::cout << "WARNING: La cantidad en stock no puede ser menor a 0." << std::endl;
        } else {
            stock_valido = true;
        }
    }

    bool ubicacion_valida = false;
    while (!ubicacion_valida) {
        std::cout << "Ingrese la ubicacion del producto: ";
        std::cin >> ubicacion;

        bool ubicacion_duplicada = false;
        for (int i = 0; i < tam; i++) {
            if (ubicacion == ubicaciones[i]) {
                ubicacion_duplicada = true;
                break;
            }
        }

        if (ubicacion_duplicada) {
            std::cout << "WARNING: La ubicacion ya existe, ingrese una diferente." << std::endl;
        } else {
            ubicacion_valida = true;
        }
    }

    codigos[tam] = codigo;
    nombres[tam] = nombre;
    precios[tam] = precio;
    stock[tam] = stock_valor;
    ubicaciones[tam] = ubicacion;

    std::cout << "Producto '" << nombre << "' agregado exitosamente." << std::endl;
}

void generarReporte(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam){
    std::cout << "----------- REPORTE DE BAJO STOCK -----------" << std::endl;
    std::cout << "Codigo | Nombre | Stock | Precio | Ubicacion" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;

    int productos_bajo_stock = 0;

    for (int i = 0; i < tam; i++){
        if (stock[i] < 5){
            std::cout << codigos[i] << " | " << nombres[i] << " | " << stock[i] << " | $" << precios[i] << " | " << ubicaciones[i] << std::endl;
            productos_bajo_stock++;
        }
    }

    if (productos_bajo_stock == 0){
        std::cout << "No hay productos con bajo stock (< 10 unidades)" << std::endl;
    } else {
        std::cout << "Total de productos con bajo stock: " << productos_bajo_stock << std::endl;
    }
    
    std::cout << "---------------------------------------------" << std::endl;
}

void buscarProductoMasCaro(std::string nombres[], float precios[], int tam){
    if (tam == 0) {
        std::cout << "No hay productos en el inventario." << std::endl;
        return;
    }

    float precioMasAlto = precios[0];
    std::string nombreProductoMasCaro = nombres[0];
    
    for (int i = 1; i < tam; i++) {
        if (precios[i] > precioMasAlto) {
            precioMasAlto = precios[i];
            nombreProductoMasCaro = nombres[i];
        }
    }
    
    std::cout << "El producto mas caro es: '" << nombreProductoMasCaro << "' con un precio de $" << precioMasAlto << std::endl;
}

void guardarDatos(int codigos[], std::string nombres[], int stock[], float precios[], std::string ubicaciones[], int tam) {
    const std::string dataFileLocation = "C:\\Users\\as_co\\C\\actividad-cont-1b-vectores-paralelos-AngelCS0124\\data\\inventario.csv";
    std::ofstream dataFile;

    dataFile.open(dataFileLocation, std::ios::out);
    if (!dataFile) {
        std::cout << "WARNING: No se pudo abrir el archivo para guardar: " << dataFileLocation << std::endl;
    } else {
        dataFile << "Código,Ubicación,Nombre,Cantidad,Precio" << std::endl;
        for (int i = 0; i < tam; i++) {
            dataFile << codigos[i] << "," << ubicaciones[i] << "," << nombres[i] << "," << stock[i] << "," << precios[i] << std::endl;
        }

        dataFile.close();
        std::cout << "Datos guardados correctamente en " << dataFileLocation << std::endl;
        std::cout << "Total de productos guardados: " << tam << std::endl;
    }
}