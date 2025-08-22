#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    char nombre[30];
    float promedio;
} Alumno;

// Prototipos
void guardarAlumnos(const char *archivo, Alumno alumnos[], int n);
int leerAlumnos(const char *archivo, Alumno alumnos[]);
void ordenarPorNombre(Alumno alumnos[], int n);
void ordenarPorPromedio(Alumno alumnos[], int n);
void imprimirAlumnos(Alumno alumnos[], int n);

int main() {
    Alumno alumnos[MAX];
    int n, opcion;
    char archivo[] = "alumnos.dat";

    printf("Ingrese el número de alumnos: ");
    scanf("%d", &n);
    getchar(); // limpiar buffer

    // Entrada de datos
    for (int i = 0; i < n; i++) {
        printf("\nAlumno %d:\n", i + 1);
        printf("Nombre: ");
        fgets(alumnos[i].nombre, sizeof(alumnos[i].nombre), stdin);
        alumnos[i].nombre[strcspn(alumnos[i].nombre, "\n")] = '\0';

        printf("Promedio: ");
        scanf("%f", &alumnos[i].promedio);
        getchar(); // limpiar buffer
    }

    // Guardar en archivo binario
    guardarAlumnos(archivo, alumnos, n);

    // Leer desde el archivo binario
    n = leerAlumnos(archivo, alumnos);

    // Selección de orden
    printf("\n1. Ordenar por nombre\n2. Ordenar por promedio\nOpción: ");
    scanf("%d", &opcion);

    if (opcion == 1)
        ordenarPorNombre(alumnos, n);
    else
        ordenarPorPromedio(alumnos, n);

    // Imprimir resultados
    imprimirAlumnos(alumnos, n);

    return 0;
}

// Guardar en archivo binario
void guardarAlumnos(const char *archivo, Alumno alumnos[], int n) {
    FILE *f = fopen(archivo, "wb");
    if (!f) {
        printf("Error al abrir el archivo para escritura.\n");
        exit(1);
    }
    fwrite(alumnos, sizeof(Alumno), n, f);
    fclose(f);
}

// Leer desde archivo binario
int leerAlumnos(const char *archivo, Alumno alumnos[]) {
    FILE *f = fopen(archivo, "rb");
    if (!f) {
        printf("Error al abrir el archivo para lectura.\n");
        exit(1);
    }

    int n = 0;
    while (fread(&alumnos[n], sizeof(Alumno), 1, f) == 1) {
        n++;
    }

    fclose(f);
    return n;
}

// BubbleSort por nombre (alfabético)
void ordenarPorNombre(Alumno alumnos[], int n) {
    Alumno temp;
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (strcmp(alumnos[j].nombre, alumnos[j + 1].nombre) > 0) {
                temp = alumnos[j];
                alumnos[j] = alumnos[j + 1];
                alumnos[j + 1] = temp;
            }
}

// BubbleSort por promedio (mayor a menor)
void ordenarPorPromedio(Alumno alumnos[], int n) {
    Alumno temp;
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (alumnos[j].promedio < alumnos[j + 1].promedio) {
                temp = alumnos[j];
                alumnos[j] = alumnos[j + 1];
                alumnos[j + 1] = temp;
            }
}

// Imprimir alumnos
void imprimirAlumnos(Alumno alumnos[], int n) {
    printf("\n%-30s %-10s\n", "Nombre", "Promedio");
    printf("--------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-30s %-10.2f\n", alumnos[i].nombre, alumnos[i].promedio);
    }
}


