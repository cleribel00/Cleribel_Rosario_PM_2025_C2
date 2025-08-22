#include <stdio.h>
#include <string.h>

struct Vehicle {
    char placa[10];
    char marca[20];
    char modelo[20];
    char tipoCombustible[20];
    float kmGalCarretera;
    float kmGalCiudad;
    float costoGomasKm;
    float costoSeguro12Meses;
    float costoMantenimientoKm;
    float costoVehiculo;
    int vidaUtil;
    float depreciacion;
    float kmAnioPromedio;
};

void crearVehiculo(struct Vehicle *v) {
    printf("Ingrese placa: ");
    scanf("%s", v->placa);
    printf("Ingrese marca: ");
    scanf("%s", v->marca);
    printf("Ingrese modelo: ");
    scanf("%s", v->modelo);
    printf("Ingrese tipo de combustible: ");
    scanf("%s", v->tipoCombustible);
    printf("Ingrese Km x Galon Carretera: ");
    scanf("%f", &v->kmGalCarretera);
    printf("Ingrese Km x Galon Ciudad: ");
    scanf("%f", &v->kmGalCiudad);
    printf("Ingrese Costo Gomas y Km de gomas: ");
    scanf("%f", &v->costoGomasKm);
    printf("Ingrese Costo Seguro por 12 meses: ");
    scanf("%f", &v->costoSeguro12Meses);
    printf("Ingrese Costo Mantenimiento y cantidad kilometros: ");
    scanf("%f", &v->costoMantenimientoKm);
    printf("Ingrese Costo Vehiculo: ");
    scanf("%f", &v->costoVehiculo);
    printf("Ingrese Vida Util (year): ");
    scanf("%d", &v->vidaUtil);
    printf("Ingrese Depreciacion: ");
    scanf("%f", &v->depreciacion);
    printf("Ingrese Km por year promedio: ");
    scanf("%f", &v->kmAnioPromedio);
}

void mostrarDatosGenerales(struct Vehicle v) {
    printf("Placa: %s\n", v.placa);
    printf("Marca: %s\n", v.marca);
    printf("Modelo: %s\n", v.modelo);
    printf("Tipo Combustible: %s\n", v.tipoCombustible);
    printf("Km/Gal Carretera: %.2f\n", v.kmGalCarretera);
    printf("Km/Gal Ciudad: %.2f\n", v.kmGalCiudad);
    printf("Costo Gomas: %.2f\n", v.costoGomasKm);
    printf("Costo Seguro: %.2f\n", v.costoSeguro12Meses);
    printf("Costo Mantenimiento: %.2f\n", v.costoMantenimientoKm);
    printf("Costo Vehiculo: %.2f\n", v.costoVehiculo);
    printf("Vida Util: %d\n", v.vidaUtil);
    printf("Depreciacion: %.2f\n", v.depreciacion);
    printf("Km promedio/año: %.2f\n", v.kmAnioPromedio);
}

void guardarVehiculo(struct Vehicle v) {
    FILE *f = fopen("vehiculos.txt", "a"); // "a" = append, agrega al final
    if (f == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    fprintf(f, "%s %s %s %s %.2f %.2f %.2f %.2f %.2f %.2f %d %.2f %.2f\n",
            v.placa, v.marca, v.modelo, v.tipoCombustible,
            v.kmGalCarretera, v.kmGalCiudad, v.costoGomasKm,
            v.costoSeguro12Meses, v.costoMantenimientoKm,
            v.costoVehiculo, v.vidaUtil, v.depreciacion,
            v.kmAnioPromedio);
    fclose(f);
    printf("Vehiculo guardado en archivo.\n");
}

void mostrarVehiculosGuardados() {
    FILE *f = fopen("vehiculos.txt", "r");
    if (f == NULL) {
        printf("No hay vehiculos guardados.\n");
        return;
    }

    struct Vehicle v;
    while (fscanf(f, "%s %s %s %s %f %f %f %f %f %f %d %f %f",
                  v.placa, v.marca, v.modelo, v.tipoCombustible,
                  &v.kmGalCarretera, &v.kmGalCiudad, &v.costoGomasKm,
                  &v.costoSeguro12Meses, &v.costoMantenimientoKm,
                  &v.costoVehiculo, &v.vidaUtil, &v.depreciacion,
                  &v.kmAnioPromedio) == 13) {
        mostrarDatosGenerales(v);
        printf("-----------------------------\n");
    }

    fclose(f);
}

int main() {
    struct Vehicle vehiculo;
    int opcion;

    do {
        printf("\n1. Crear y Guardar Vehiculo\n2. Mostrar Vehiculos Guardados\n3. Salir\n");
        printf("Ingrese opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                crearVehiculo(&vehiculo);
                guardarVehiculo(vehiculo);
                break;
            case 2:
                mostrarVehiculosGuardados();
                break;
            case 3:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion invalida\n");
        }
    } while(opcion != 3);

    return 0;
}


