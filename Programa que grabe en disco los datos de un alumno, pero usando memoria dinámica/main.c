#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nombre[30];
    int edad;
    float promedio;
} Alumno;

void guardarAlumno(const char *archivo);
Alumno* leerAlumnos(const char *archivo, int *cantidad);
void imprimirAlumnos(Alumno *alumnos, int cantidad);

int main() {
    int opcion, cantidad;
    Alumno *lista;

    char archivo[] = "alumnos.dat";

    do {
        printf("\n1. Agregar alumno\n2. Ver alumnos\n3. Salir\nOpción: ");
        scanf("%d", &opcion);
        getchar(); // limpiar buffer

        switch (opcion) {
            case 1:
                guardarAlumno(archivo);
                break;
            case 2:
                lista = leerAlumnos(archivo, &cantidad);
                if (lista != NULL) {
                    imprimirAlumnos(lista, cantidad);
                    free(lista); // liberar memoria dinámica
                }
                break;
        }
    } while (opcion != 3);

    return 0;
}

void guardarAlumno(const char *archivo) {
    Alumno a;
    FILE *f = fopen(archivo, "ab"); // abrir en modo añadir binario
    if (!f) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    printf("Nombre: ");
    fgets(a.nombre, sizeof(a.nombre), stdin);
    a.nombre[strcspn(a.nombre, "\n")] = '\0';

    printf("Edad: ");
    scanf("%d", &a.edad);

    printf("Promedio: ");
    scanf("%f", &a.promedio);
    getchar(); // limpiar buffer

    fwrite(&a, sizeof(Alumno), 1, f);
    fclose(f);
    printf("Alumno guardado con éxito.\n");
}

Alumno* leerAlumnos(const char *archivo, int *cantidad) {
    FILE *f = fopen(archivo, "rb");
    if (!f) {
        printf("No se pudo abrir el archivo.\n");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    rewind(f);

    *cantidad = tam / sizeof(Alumno);
    Alumno *alumnos = (Alumno *)malloc(tam);
    if (alumnos == NULL) {
        printf("No hay suficiente memoria.\n");
        fclose(f);
        return NULL;
    }

    fread(alumnos, sizeof(Alumno), *cantidad, f);
    fclose(f);

    return alumnos;
}

void imprimirAlumnos(Alumno *alumnos, int cantidad) {
    printf("\n%-30s %-5s %-8s\n", "Nombre", "Edad", "Promedio");
    printf("-----------------------------------------------\n");

    for (int i = 0; i < cantidad; i++) {
        Alumno *ptr = &alumnos[i]; // movernos con puntero
        printf("%-30s %-5d %-8.2f\n", ptr->nombre, ptr->edad, ptr->promedio);
    }
}

