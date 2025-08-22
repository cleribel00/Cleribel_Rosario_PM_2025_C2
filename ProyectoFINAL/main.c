#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h> // Para la fecha dinamica
#include <windows.h> // Para colores en Windows

// colores para windows
#define COLOR_BORDE_APP 15 // Azul para bordes y texto normal
#define COLOR_TITULO_APP 11 // Rosado para titulo y lineas
#define COLOR_MENU_APP 10  // Amarillo para valores monetarios Y OPCIONES DEL PRINCIPIO
#define COLOR_ERROR 12     // Verde para errores
#define COLOR_YELLOW 14    // Rojo para mensaje final

// Utilidades de consola
#define CLEAR() system("cls")
#define PAUSE() system("pause")

// Constantes generales
#define MAX_VEHICULOS 100

//  Estructuras de datos
typedef struct
{
    char placa[10];
    char marca[20];
    char modelo[20];
    char tipoCombustible[15];
    float kmPorGalonCiudad;
    float kmPorGalonCarretera;
    float costoGomas;
    float kmPorGomas;
    float costoSeguro;
    float kmPorAnio;
    float costoMantenimiento;
    float kmMantenimiento;
    float costoVehiculo;
    int vidaUtilAnio;
    float depreciacion;
} Vehiculo;

typedef struct
{
    float gasolina;
    float gasoil;
} DatosGenerales;

// Variables globales
static Vehiculo g_lista[MAX_VEHICULOS];
static int g_total = 0;
static DatosGenerales g_datos = {0.00, 0.00};
static HANDLE hConsole; // Manejador de consola para colores

// Prototipos
void setColor(int color);
void mostrarLogo(void);
bool leerLinea(const char *prompt, char *buf, size_t n, bool permitirVacio, bool *fueCancelado);
bool leerCadena(const char *prompt, char *dest, size_t n, bool obligatorio, bool *fueCancelado);
bool leerFloat(const char *prompt, float *valor, float min, float max, bool obligatorio, bool *fueCancelado);
bool leerEntero(const char *prompt, int *valor, int min, int max, bool obligatorio, bool *fueCancelado);
void strToUpper(char *s);
int buscarPlaca(const char *placa);
bool confirmar(const char *pregunta);
void guardarVehiculos(void);
void cargarVehiculos(void);
void guardarDatosGenerales(void);
void cargarDatosGenerales(void);
void menuPrincipal(void);
void menuVehiculos(void);
void crearVehiculo(void);
void listarVehiculos(bool pausar);
void borrarVehiculo(void);
void modificarVehiculo(void);
void configurarCombustible(void);
void mostrarDatosCombustible(void);
float calcularDepreciacion(const Vehiculo *v, float kmViaje);
float costoCombustible(const Vehiculo *v, float kmViaje, float porcentajeCiudad);
float costoMantenimiento(const Vehiculo *v, float kmViaje);
float costoGomas(const Vehiculo *v, float kmViaje);
float costoSeguro(const Vehiculo *v, float kmViaje);
float costoTotalViaje(const Vehiculo *v, float kmViaje, float porcentajeCiudad);
void calcularCostoViaje(void);

//MAIN
int main(void)
{
    // Configurar colores en la consola
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    cargarVehiculos();
    cargarDatosGenerales();
    menuPrincipal();
    setColor(COLOR_BORDE_APP); // Restaurar color blanco al salir
    return 0;
}

// FUNCIONES DE INTERFAZ
void setColor(int color)
{
    // Cambia el color del texto en la consola
    SetConsoleTextAttribute(hConsole, color);
}

void mostrarLogo(void)
{
    // Muestra el logo de la agencia con colores
    setColor(COLOR_BORDE_APP);
    printf("%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%\n");
    setColor(COLOR_TITULO_APP);
    printf("               STAR CAR SMITH          \n");
    setColor(COLOR_BORDE_APP);
    printf("%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%\n\n");
}

// ENTRADA ROBUSTA
bool leerLinea(const char *prompt, char *buf, size_t n, bool permitirVacio, bool *fueCancelado)
{
    // Lee una linea de entrada, permite cancelar con "##"
    if (fueCancelado) *fueCancelado = false;
    while (1)
    {
        if (prompt)
        {
            setColor(COLOR_BORDE_APP);
            printf("%s", prompt);
            fflush(stdout);
        }
        if (!fgets(buf, (int)n, stdin))
        {
            if (fueCancelado) *fueCancelado = true;
            return false;
        }
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
        while (*buf == ' ' || *buf == '\t') memmove(buf, buf+1, strlen(buf));
        if (strcmp(buf, "##") == 0)
        {
            if (fueCancelado) *fueCancelado = true;
            return false;
        }
        if (permitirVacio || buf[0] != '\0') return true;
        setColor(COLOR_ERROR);
        printf("Entrada vacia. Escriba un valor o '--' para cancelar.\n");
        setColor(COLOR_BORDE_APP);
    }
}

bool leerCadena(const char *prompt, char *dest, size_t n, bool obligatorio, bool *fueCancelado)
{
    // Lee una cadena de texto con validacion
    char tmp[256];
    bool cancel = false;
    bool ok = leerLinea(prompt, tmp, sizeof(tmp), !obligatorio, &cancel);
    if (!ok || cancel)
    {
        if (fueCancelado) *fueCancelado = true;
        return false;
    }
    if (tmp[0] == '\0' && obligatorio)
    {
        setColor(COLOR_ERROR);
        printf("Valor requerido.\n");
        setColor(COLOR_BORDE_APP);
        return leerCadena(prompt, dest, n, obligatorio, fueCancelado);
    }
    if (tmp[0] != '\0')
    {
        strncpy(dest, tmp, n-1);
        dest[n-1] = '\0';
    }
    return true;
}

bool leerFloat(const char *prompt, float *valor, float min, float max, bool obligatorio, bool *fueCancelado)
{
    // Lee un numero decimal con validacion de rango
    char tmp[256];
    while (1)
    {
        bool cancel = false;
        bool ok = leerLinea(prompt, tmp, sizeof(tmp), !obligatorio, &cancel);
        if (!ok || cancel)
        {
            if (fueCancelado) *fueCancelado = true;
            return false;
        }
        if (tmp[0] == '\0' && !obligatorio) return true;
        for (char *p = tmp; *p; ++p) if (*p == ',') *p = '.'; // Reemplaza coma por punto
        char *end = NULL;
        double v = strtod(tmp, &end);
        if (end == tmp || *end != '\0')
        {
            setColor(COLOR_ERROR);
            printf("Numero invalido. Intente de nuevo o '--' para cancelar.\n");
            setColor(COLOR_BORDE_APP);
            continue;
        }
        if (v < min || v > max)
        {
            setColor(COLOR_ERROR);
            printf("Fuera de rango (%.2f a %.2f). Intente de nuevo.\n", min, max);
            setColor(COLOR_BORDE_APP);
            continue;
        }
        *valor = (float)v;
        return true;
    }
}

bool leerEntero(const char *prompt, int *valor, int min, int max, bool obligatorio, bool *fueCancelado)
{
    // Lee un numero entero con validación de rango
    char tmp[256];
    while (1)
    {
        bool cancel = false;
        bool ok = leerLinea(prompt, tmp, sizeof(tmp), !obligatorio, &cancel);
        if (!ok || cancel)
        {
            if (fueCancelado) *fueCancelado = true;
            return false;
        }
        if (tmp[0] == '\0' && !obligatorio) return true;
        bool soloDigitos = (tmp[0] != '\0');
        for (char *p = tmp; *p; ++p)
        {
            if (*p == '-' && p == tmp) continue;
            if (!isdigit((unsigned char)*p))
            {
                soloDigitos = false;
                break;
            }
        }
        if (!soloDigitos)
        {
            setColor(COLOR_ERROR);
            printf("Entero invalido. Intente de nuevo o '--' para cancelar.\n");
            setColor(COLOR_BORDE_APP);
            continue;
        }
        long v = strtol(tmp, NULL, 10);
        if (v < min || v > max)
        {
            setColor(COLOR_ERROR);
            printf("Fuera de rango (%d a %d). Intente de nuevo.\n", min, max);
            setColor(COLOR_BORDE_APP);
            continue;
        }
        *valor = (int)v;
        return true;
    }
}

//ayudas
void strToUpper(char *s)
{
    // Convierte una cadena a mayusculas
    for (; *s; ++s) *s = (char)toupper((unsigned char)*s);
}

int buscarPlaca(const char *placa)
{
    // Busca un vehiculo por su placa
    for (int i = 0; i < g_total; ++i)
    {
        if (strcmp(g_lista[i].placa, placa) == 0) return i;
    }
    return -1;
}

bool confirmar(const char *pregunta)
{
    // Pide confirmacion (s/n)
    char r[16];
    while (1)
    {
        setColor(COLOR_BORDE_APP);
        printf("%s (s/n): ", pregunta);
        if (!fgets(r, sizeof(r), stdin)) return false;
        if (r[0] == 's' || r[0] == 'S') return true;
        if (r[0] == 'n' || r[0] == 'N') return false;
        setColor(COLOR_ERROR);
        printf("Responda 's' o 'n'.\n");
        setColor(COLOR_BORDE_APP);
    }
}

// PERSISTENCIA
void guardarVehiculos(void)
{
    // Guarda los vehículos en un archivo
    FILE *f = fopen("vehiculos.data", "wb");
    if (!f) return;
    fwrite(&g_total, sizeof(int), 1, f);
    if (g_total > 0) fwrite(g_lista, sizeof(Vehiculo), g_total, f);
    fclose(f);
}

void cargarVehiculos(void)
{
    // Carga los vehículos desde un archivo
    FILE *f = fopen("vehiculos.data", "rb");
    if (!f)
    {
        g_total = 0;
        return;
    }
    fread(&g_total, sizeof(int), 1, f);
    if (g_total > 0 && g_total <= MAX_VEHICULOS)
    {
        fread(g_lista, sizeof(Vehiculo), g_total, f);
    }
    else
    {
        g_total = 0;
    }
    fclose(f);
}

void guardarDatosGenerales(void)
{
    // Guarda los costos de combustible
    FILE *f = fopen("datos_generales.dat", "wb");
    if (!f) return;
    fwrite(&g_datos, sizeof(DatosGenerales), 1, f);
    fclose(f);
}

void cargarDatosGenerales(void)
{
    // Carga los costos de combustible
    FILE *f = fopen("datos_generales.dat", "rb");
    if (!f)
    {
        g_datos.gasolina = 5.00;
        g_datos.gasoil = 4.50;
        return;
    }
    fread(&g_datos, sizeof(DatosGenerales), 1, f);
    fclose(f);
}

// MENUS
void menuPrincipal(void)
{
    // Menú principal del programa
    int opcion;
    do
    {
        CLEAR();
        mostrarLogo();
        setColor(COLOR_MENU_APP);
        printf("1. Gestion de Vehiculos\n");
        printf("2. Gestion de Datos Generales\n");
        printf("3. Calculo de Costo de Viaje\n");
        printf("0. Salir\n");
        setColor(COLOR_BORDE_APP);
        printf("Selecciona una opcion: ");
        bool cancel = false;
        if (!leerEntero("", &opcion, 0, 3, true, &cancel)) continue;
        CLEAR();
        switch (opcion)
        {
        case 1:
            menuVehiculos();
            break;
        case 2:
            configurarCombustible();
            break;
        case 3:
            calcularCostoViaje();
            break;
        case 0:
            break;
        default:
            setColor(COLOR_ERROR);
            printf("Opcin invalida, intenta otra vez.\n\n");
            setColor(COLOR_BORDE_APP);
            PAUSE();
        }
    }
    while (opcion != 0);
}
void menuVehiculos(void)
{
    // Menu para gestionar vehiculos
    int opcion;
    do
    {
        CLEAR();
        mostrarLogo();
        setColor(COLOR_MENU_APP);
        printf("---- Gestion de Vehiculos ----\n");
        printf("1. Crear\n");
        printf("2. Listar\n");
        printf("3. Modificar\n");
        printf("4. Borrar\n");
        printf("0. Volver\n");
        setColor(COLOR_BORDE_APP);
        printf("Selecciona opcion: ");
        bool cancel = false;
        if (!leerEntero("", &opcion, 0, 4, true, &cancel)) continue;
        CLEAR();
        switch (opcion)
        {
        case 1:
            crearVehiculo();
            break;
        case 2:
            listarVehiculos(true);
            break;
        case 3:
            modificarVehiculo();
            break;
        case 4:
            borrarVehiculo();
            break;
        case 0:
            break;
        default:
            setColor(COLOR_ERROR);
            printf("Opcion invalida.\n");
            setColor(COLOR_BORDE_APP);
            PAUSE();
        }
    }
    while (opcion != 0);
}

// CRUD VEHICULO
void crearVehiculo(void)
{
    // Crea un nuevo vehiculo con validacion
    if (g_total >= MAX_VEHICULOS)
    {
        setColor(COLOR_ERROR);
        printf("Limite de vehiculos alcanzado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    Vehiculo v;
    memset(&v, 0, sizeof(Vehiculo));
    bool cancel = false;

    mostrarLogo();
    printf("--- REGISTRO DE VEHICULO --- (escribe '--' para cancelar)\n\n");
    while (1)
    {
        if (!leerCadena("Placa: ", v.placa, sizeof(v.placa), true, &cancel))
        {
            setColor(COLOR_ERROR);
            printf("\nOperacion cancelada.\n");
            setColor(COLOR_BORDE_APP);
            PAUSE();
            return;
        }
        strToUpper(v.placa);
        if (buscarPlaca(v.placa) != -1)
        {
            setColor(COLOR_ERROR);
            printf("Esa placa ya existe. Intente otra.\n");
            setColor(COLOR_BORDE_APP);
            continue;
        }
        break;
    }

    if (!leerCadena("Marca: ", v.marca, sizeof(v.marca), true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!leerCadena("Modelo: ", v.modelo, sizeof(v.modelo), true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    while (1)
    {
        int tipo = 0;
        if (!leerEntero("Tipo combustible (1-Gasolina, 2-Gasoil): ", &tipo, 1, 2, true, &cancel))
        {
            setColor(COLOR_ERROR);
            printf("\nCancelado.\n");
            setColor(COLOR_BORDE_APP);
            PAUSE();
            return;
        }
        if (tipo == 1)
        {
            strcpy(v.tipoCombustible, "Gasolina");
            break;
        }
        if (tipo == 2)
        {
            strcpy(v.tipoCombustible, "Gasoil");
            break;
        }
        setColor(COLOR_ERROR);
        printf("Opción invalida.\n");
        setColor(COLOR_BORDE_APP);
    }

    if (!leerFloat("Rendimiento ciudad (km/galón): ", &v.kmPorGalonCiudad, 0.01f, 10000.0f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!leerFloat("Rendimiento carretera (km/galón): ", &v.kmPorGalonCarretera, 0.01f, 10000.0f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!leerFloat("Costo total de gomas: ", &v.costoGomas, 0.0f, 1e9f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!leerFloat("Vida util de gomas (km): ", &v.kmPorGomas, 1.0f, 1e9f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!leerFloat("Costo anual del seguro: ", &v.costoSeguro, 0.0f, 1e9f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!leerFloat("Km por año promedio: ", &v.kmPorAnio, 1.0f, 1e9f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!leerFloat("Costo de mantenimiento: ", &v.costoMantenimiento, 0.0f, 1e9f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!leerFloat("Frecuencia de mantenimiento (km): ", &v.kmMantenimiento, 1.0f, 1e9f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!leerFloat("Costo inicial del vehículo: ", &v.costoVehiculo, 0.0f, 1e12f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!leerEntero("Vida util (years): ", &v.vidaUtilAnio, 1, 100, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!leerFloat("Depreciacion anual (%%): ", &v.depreciacion, 0.0f, 100.0f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("\nResumen del vehiculo a guardar:\n");
    setColor(COLOR_BORDE_APP);
    printf("Placa: %s | %s %s | %s\n", v.placa, v.marca, v.modelo, v.tipoCombustible);
    if (!confirmar("¿Desea guardar este vehiculo?"))
    {
        setColor(COLOR_ERROR);
        printf("No guardado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    g_lista[g_total++] = v;
    guardarVehiculos();
    setColor(COLOR_MENU_APP);
    printf("Vehiculo guardado con exito.\n");
    setColor(COLOR_BORDE_APP);
    PAUSE();
}

void listarVehiculos(bool pausar)
{
    // Muestra la lista de vehiculos registrados
    CLEAR();
    mostrarLogo();
    setColor(COLOR_BORDE_APP);
    printf("=== LISTADO DE VEHÍCULOS (%d) ===\n", g_total);
    for (int i = 0; i < g_total; ++i)
    {
        const Vehiculo *v = &g_lista[i];
        printf("%d) %s - %s %s\n", i+1, v->placa, v->marca, v->modelo);
        printf("    Combustible: %s | Rendimiento: %.1f ciudad / %.1f carretera (km/gal)\n",
               v->tipoCombustible, v->kmPorGalonCiudad, v->kmPorGalonCarretera);
    }
    if (pausar)
    {
        printf("\n");
        PAUSE();
    }
}

void borrarVehiculo(void)
{
    // Borra un vehiculo de la lista
    if (g_total == 0)
    {
        setColor(COLOR_ERROR);
        printf("No hay vehiculos.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    listarVehiculos(false);
    int idx = -1;
    bool cancel = false;
    setColor(COLOR_BORDE_APP);
    if (!leerEntero("\nSeleccione numero de vehiculo a borrar (1..N) o '--' para cancelar: ",
                    &idx, 1, g_total, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!confirmar("¿Confirma borrado?"))
    {
        setColor(COLOR_ERROR);
        printf("Operación cancelada.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    for (int i = idx; i < g_total; ++i) g_lista[i-1] = g_lista[i];
    g_total--;
    guardarVehiculos();
    setColor(COLOR_MENU_APP);
    printf("Vehiculo borrado.\n");
    setColor(COLOR_BORDE_APP);
    PAUSE();
}

void modificarVehiculo(void)
{
    // Modifica un vehiculo existente
    if (g_total == 0)
    {
        setColor(COLOR_ERROR);
        printf("No hay vehiculos.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    listarVehiculos(false);
    int idxSel = -1;
    bool cancel = false;
    setColor(COLOR_BORDE_APP);
    if (!leerEntero("\nSeleccione numero de vehiculo a modificar (1..N) o '--' para cancelar: ",
                    &idxSel, 1, g_total, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    Vehiculo *v = &g_lista[idxSel-1];
    CLEAR();
    mostrarLogo();
    printf("--- MODIFICAR VEHICULO (%s - %s %s) ---\n", v->placa, v->marca, v->modelo);
    printf("Deje vacio para mantener el valor actual o escriba '--' para cancelar.\n\n");

    char tmpCad[64];
    float tmpF;
    int tmpI;
    setColor(COLOR_BORDE_APP);
    printf("Marca actual: %s\n", v->marca);
    if (leerCadena("Nueva marca: ", tmpCad, sizeof(tmpCad), false, &cancel))
    {
        if (tmpCad[0] != '\0') strncpy(v->marca, tmpCad, sizeof(v->marca)-1);
    }
    else
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("Modelo actual: %s\n", v->modelo);
    if (leerCadena("Nuevo modelo: ", tmpCad, sizeof(tmpCad), false, &cancel))
    {
        if (tmpCad[0] != '\0') strncpy(v->modelo, tmpCad, sizeof(v->modelo)-1);
    }
    else
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("Tipo combustible actual: %s\n", v->tipoCombustible);
    printf("(Deje vacio para mantener)\n");
    if (leerCadena("Nuevo tipo (Gasolina/Gasoil): ", tmpCad, sizeof(tmpCad), false, &cancel))
    {
        if (tmpCad[0] != '\0')
        {
            strToUpper(tmpCad);
            if (strncmp(tmpCad, "GASOLINA", 8) == 0) strcpy(v->tipoCombustible, "Gasolina");
            else if (strncmp(tmpCad, "GASOIL", 6) == 0) strcpy(v->tipoCombustible, "Gasoil");
            else
            {
                setColor(COLOR_ERROR);
                printf("Valor ignorado (no válido).\n");
                setColor(COLOR_BORDE_APP);
            }
        }
    }
    else
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("Rend. ciudad actual: %.2f\n", v->kmPorGalonCiudad);
    if (leerFloat("Nuevo rend. ciudad: ", &tmpF, 0.01f, 10000.0f, false, &cancel) && tmpF > 0) v->kmPorGalonCiudad = tmpF;
    else if (cancel)
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("Rend. carretera actual: %.2f\n", v->kmPorGalonCarretera);
    if (leerFloat("Nuevo rend. carretera: ", &tmpF, 0.01f, 10000.0f, false, &cancel) && tmpF > 0) v->kmPorGalonCarretera = tmpF;
    else if (cancel)
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("Costo gomas actual: %.2f\n", v->costoGomas);
    if (leerFloat("Nuevo costo gomas: ", &tmpF, 0.0f, 1e9f, false, &cancel)) v->costoGomas = (tmpF>0?tmpF:v->costoGomas);
    else if (cancel)
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("Vida util gomas (km) actual: %.2f\n", v->kmPorGomas);
    if (leerFloat("Nueva vida útil gomas: ", &tmpF, 1.0f, 1e9f, false, &cancel) && tmpF > 0) v->kmPorGomas = tmpF;
    else if (cancel)
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("Costo seguro por 12 meses: %.2f\n", v->costoSeguro);
    if (leerFloat("Nuevo costo seguro: ", &tmpF, 0.0f, 1e9f, false, &cancel)) v->costoSeguro = (tmpF>=0?tmpF:v->costoSeguro);
    else if (cancel)
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("Km por año promedio actual: %.2f\n", v->kmPorAnio);
    if (leerFloat("Nuevo km/año: ", &tmpF, 1.0f, 1e9f, false, &cancel) && tmpF > 0) v->kmPorAnio = tmpF;
    else if (cancel)
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("Costo mantenimiento actual: %.2f\n", v->costoMantenimiento);
    if (leerFloat("Nuevo costo mant.: ", &tmpF, 0.0f, 1e9f, false, &cancel)) v->costoMantenimiento = (tmpF>=0?tmpF:v->costoMantenimiento);
    else if (cancel)
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("Frecuencia mant. (km) actual: %.2f\n", v->kmMantenimiento);
    if (leerFloat("Nueva frecuencia mant.: ", &tmpF, 1.0f, 1e9f, false, &cancel) && tmpF > 0) v->kmMantenimiento = tmpF;
    else if (cancel)
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("Costo inicial vehículo actual: %.2f\n", v->costoVehiculo);
    if (leerFloat("Nuevo costo inicial: ", &tmpF, 0.0f, 1e12f, false, &cancel)) v->costoVehiculo = (tmpF>=0?tmpF:v->costoVehiculo);
    else if (cancel)
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("Vida util (years) actual: %d\n", v->vidaUtilAnio);
    if (leerEntero("Nueva vida útil: ", &tmpI, 1, 100, false, &cancel) && tmpI > 0) v->vidaUtilAnio = tmpI;
    else if (cancel)
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    printf("Depreciacion anual (%%) actual: %.2f\n", v->depreciacion);
    if (leerFloat("Nueva depreciación (%%): ", &tmpF, 0.0f, 100.0f, false, &cancel)) v->depreciacion = (tmpF>=0?tmpF:v->depreciacion);
    else if (cancel)
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    guardarVehiculos();
    setColor(COLOR_MENU_APP);
    printf("\nCambios guardados.\n");
    setColor(COLOR_BORDE_APP);
    PAUSE();
}

// DATOS GENERALES
void configurarCombustible(void)
{
    // Limpia la pantalla y muestra el logo
    CLEAR();
    mostrarLogo();
    setColor(COLOR_BORDE_APP);
    printf("=== CONFIGURAR COSTO DE COMBUSTIBLE ===\n\n");

    // Muestra los costos de combustible registrados
    mostrarDatosCombustible();

    // Pregunta si desea cambiar los costos
    if (!confirmar("Desea cambiar los costos de combustible"))
    {
        setColor(COLOR_BORDE_APP);
        printf("\nNo se realizaron cambios.\n");
        PAUSE();
        return;
    }

    // Pide nuevos costos si el usuario elige "sí"
    bool cancel = false;
    float nuevoGas = g_datos.gasolina;
    float nuevoGso = g_datos.gasoil;
    if (!leerFloat("Precio gasolina por galon: ", &nuevoGas, 0.0f, 1e6f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!leerFloat("Precio gasoil por galon:  ", &nuevoGso, 0.0f, 1e6f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    g_datos.gasolina = nuevoGas;
    g_datos.gasoil = nuevoGso;
    guardarDatosGenerales();
    setColor(COLOR_MENU_APP);
    printf("\nDatos guardados.\n");
    setColor(COLOR_BORDE_APP);
    PAUSE();
}
void mostrarDatosCombustible(void)
{
    // Muestra los costos de combustible
    setColor(COLOR_BORDE_APP);
    printf("\n=== DATOS GENERALES DE COMBUSTIBLE ===\n");
    printf("Precio gasolina: %.2f\n", g_datos.gasolina);
    printf("Precio gasoil:   %.2f\n", g_datos.gasoil);
    printf("--------------------------------------\n");
}

// CalCULOS
float calcularDepreciacion(const Vehiculo *v, float kmViaje)
{
    // Calcula la depreciación proporcional al viaje
    return (v->costoVehiculo * (v->depreciacion / 100.0f)) * (kmViaje / (v->kmPorAnio * v->vidaUtilAnio));
}

float costoCombustible(const Vehiculo *v, float kmViaje, float porcentajeCiudad)
{
    // Calcula el costo de combustible según ciudad y carretera
    float kmCiudad = kmViaje * (porcentajeCiudad / 100.0f);
    float kmCarretera = kmViaje - kmCiudad;
    float precio = (strcmp(v->tipoCombustible, "Gasolina") == 0) ? g_datos.gasolina : g_datos.gasoil;
    if (v->kmPorGalonCiudad <= 0 || v->kmPorGalonCarretera <= 0) return 0.0f;
    return (kmCiudad / v->kmPorGalonCiudad + kmCarretera / v->kmPorGalonCarretera) * precio;
}

float costoMantenimiento(const Vehiculo *v, float kmViaje)
{
    // Calcula el costo de mantenimiento segun los km
    if (v->kmMantenimiento <= 0) return 0.0f;
    return (kmViaje / v->kmMantenimiento) * v->costoMantenimiento;
}

float costoGomas(const Vehiculo *v, float kmViaje)
{
    // Calcula el costo de las gomas segun los km
    if (v->kmPorGomas <= 0) return 0.0f;
    return (kmViaje / v->kmPorGomas) * v->costoGomas;
}

float costoSeguro(const Vehiculo *v, float kmViaje)
{
    // Calcula el costo del seguro proporcional al viaje
    if (v->kmPorAnio <= 0) return 0.0f;
    return (kmViaje / v->kmPorAnio) * v->costoSeguro;
}

float costoTotalViaje(const Vehiculo *v, float kmViaje, float porcentajeCiudad)
{
    // Suma todos los costos del viaje
    return costoCombustible(v, kmViaje, porcentajeCiudad)
           + costoMantenimiento(v, kmViaje)
           + costoGomas(v, kmViaje)
           + costoSeguro(v, kmViaje)
           + calcularDepreciacion(v, kmViaje);
}
void calcularCostoViaje(void)
{
    // Verifica si hay vehículos registrados
    if (g_total == 0)
    {
        setColor(COLOR_ERROR);
        printf("No hay vehiculos registrados.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    // Limpia la pantalla y muestra el logo
    CLEAR();
    mostrarLogo();
    setColor(COLOR_BORDE_APP);
    printf("=== CALCULO DE COSTO DE VIAJE ===\n\n");
    // Muestra los costos de combustible (comentar si no se desea)
    // mostrarDatosCombustible();
    // Lista los vehículos disponibles
    listarVehiculos(false);
    // Pide al usuario que seleccione un vehículo
    setColor(COLOR_BORDE_APP);
    printf("Seleccione el numero de vehiculo (0 para volver): ");
    char linea[64];
    if (!fgets(linea, sizeof(linea), stdin)) return;
    int op = atoi(linea);
    if (op == 0) return;
    if (op < 1 || op > g_total)
    {
        setColor(COLOR_ERROR);
        printf("Opcion invalida.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    Vehiculo *v = &g_lista[op-1]; // Selecciona el vehículo

    // Limpia la pantalla, muestra logo, título y vehículo elegido
    CLEAR();
    mostrarLogo();
    setColor(COLOR_BORDE_APP);
    printf("=== CALCULO DE COSTO DE VIAJE ===\n\n");
    printf("Vehiculo elegido: %s %s (%s)\n\n", v->marca, v->modelo, v->placa);

    // Pide los kilometros del viaje y el porcentaje en ciudad
    bool cancel = false;
    float kmViaje = 0.0f, pCiudad = 0.0f;
    if (!leerFloat("\nKilometros del viaje: ", &kmViaje, 0.1f, 1e9f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }
    if (!leerFloat("Porcentaje de km en ciudad (0-100): ", &pCiudad, 0.0f, 100.0f, true, &cancel))
    {
        setColor(COLOR_ERROR);
        printf("\nCancelado.\n");
        setColor(COLOR_BORDE_APP);
        PAUSE();
        return;
    }

    // Calcula los costos del viaje
    float cComb = costoCombustible(v, kmViaje, pCiudad); // Costo de combustible
    float cMant = costoMantenimiento(v, kmViaje); // Costo de mantenimiento
    float cGom = costoGomas(v, kmViaje); // Costo de gomas
    float cSeg = costoSeguro(v, kmViaje); // Costo del seguro
    float cDep = calcularDepreciacion(v, kmViaje); // Costo de depreciación
    float total = cComb + cMant + cGom + cSeg + cDep; // Suma total
    float porKm = total / kmViaje; // Costo por kilómetro

    // Obtiene la fecha actual
    time_t ahora = time(NULL);
    struct tm *fecha = localtime(&ahora);
    char fechaStr[11];
    sprintf(fechaStr, "%02d/%02d/%04d", fecha->tm_mday, fecha->tm_mon + 1, fecha->tm_year + 1900);

    // Imprime la factura con colores
    setColor(COLOR_TITULO_APP);
    printf("\n----------------------------------------\n");
    printf("FACTURA DE VIAJE - %s\n", fechaStr);
    printf("----------------------------------------\n");
    setColor(COLOR_BORDE_APP);
    printf("Vehículo: %s %s (%s)\n", v->marca, v->modelo, v->placa);
    setColor(COLOR_MENU_APP);
    printf("Costo Combustible:   $ %9.2f\n", cComb);
    printf("Costo Mantenimiento: $ %9.2f\n", cMant);
    printf("Costo Gomas:         $ %9.2f\n", cGom);
    printf("Costo Seguro:        $ %9.2f\n", cSeg);
    printf("Costo Vehiculo:      $ %9.2f\n", cDep);
    setColor(COLOR_TITULO_APP);
    printf("----------------------------------------\n");
    setColor(COLOR_MENU_APP);
    printf("TOTAL:               $ %9.2f\n", total);
    printf("Costo por Km:        $ %9.2f\n", porKm);
    setColor(COLOR_TITULO_APP);
    printf("========================================\n");
    setColor(COLOR_YELLOW);
    printf("Gracias por elegir STAR CAR SMITH\n");
    setColor(COLOR_TITULO_APP);
    printf("========================================\n\n");
    setColor(COLOR_BORDE_APP);

    PAUSE();
}
