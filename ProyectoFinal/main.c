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
    printf("Costo Combustible Gasolina: %.2f\n", v.kmGalCarretera);
    printf("Costo Combustible Gasoil: %.2f\n", v.kmGalCiudad);
    printf("Costo Gomas: %.2f\n", v.costoGomasKm);
    printf("Costo Seguro: %.2f\n", v.costoSeguro12Meses);
    printf("Costo Mantenimiento: %.2f\n", v.costoMantenimientoKm);
    printf("Costo Vehiculo: %.2f\n", v.costoVehiculo);
}

void modificarVehiculo(struct Vehicle *v) {
    printf("Modificar placa (%s): ", v->placa);
    scanf("%s", v->placa);
    printf("Modificar marca (%s): ", v->marca);
    scanf("%s", v->marca);
    printf("Modificar modelo (%s): ", v->modelo);
    scanf("%s", v->modelo);
    printf("Modificar tipo de combustible (%s): ", v->tipoCombustible);
    scanf("%s", v->tipoCombustible);
    printf("Modificar Km x Galon Carretera (%.2f): ", v->kmGalCarretera);
    scanf("%f", &v->kmGalCarretera);
    printf("Modificar Km x Galon Ciudad (%.2f): ", v->kmGalCiudad);
    scanf("%f", &v->kmGalCiudad);
    printf("Modificar Costo Gomas y Km de gomas (%.2f): ", v->costoGomasKm);
    scanf("%f", &v->costoGomasKm);
    printf("Modificar Costo Seguro por 12 meses (%.2f): ", v->costoSeguro12Meses);
    scanf("%f", &v->costoSeguro12Meses);
    printf("Modificar Costo Mantenimiento y cantidad kilometros (%.2f): ", v->costoMantenimientoKm);
    scanf("%f", &v->costoMantenimientoKm);
    printf("Modificar Costo Vehiculo (%.2f): ", v->costoVehiculo);
    scanf("%f", &v->costoVehiculo);
    printf("Modificar Vida Util (year) (%d): ", v->vidaUtil);
    scanf("%d", &v->vidaUtil);
    printf("Modificar Depreciacion (%.2f): ", v->depreciacion);
    scanf("%f", &v->depreciacion);
    printf("Modificar Km por year promedio (%.2f): ", v->kmAnioPromedio);
    scanf("%f", &v->kmAnioPromedio);
}

void borrarVehiculo(struct Vehicle *v) {
    strcpy(v->placa, "");
    strcpy(v->marca, "");
    strcpy(v->modelo, "");
    strcpy(v->tipoCombustible, "");
    v->kmGalCarretera = 0;
    v->kmGalCiudad = 0;
    v->costoGomasKm = 0;
    v->costoSeguro12Meses = 0;
    v->costoMantenimientoKm = 0;
    v->costoVehiculo = 0;
    v->vidaUtil = 0;
    v->depreciacion = 0;
    v->kmAnioPromedio = 0;
    printf("Vehiculo borrado.\n");
}

int main() {
    struct Vehicle vehiculo;
    int opcion;

    do {
        printf("\n1. Crear Vehiculo\n2. Mostrar Datos Generales\n3. Modificar Vehiculo\n4. Borrar Vehiculo\n5. Salir\n");
        printf("Ingrese opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                crearVehiculo(&vehiculo);
                break;
            case 2:
                mostrarDatosGenerales(vehiculo);
                break;
            case 3:
                modificarVehiculo(&vehiculo);
                break;
            case 4:
                borrarVehiculo(&vehiculo);
                break;
            case 5:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion invalida\n");
        }
    } while(opcion != 5);

    return 0;
}
