#include <stdio.h>
#include <stdlib.h>
/*Ingremento de precio.
El programa, al recibir como dato el precio de un producto importado,
incrementa 11% el mismo si este es inferior a $1,500.
PRE Y NPR: variable detipo real. */

void main(void)
{
    float PRE,NPR;
    printf("INGRESE EL PRECIO DEL PRODUCTO: ");
    scanf("%f", &PRE);
    if (PRE > 1500)
    {
        NPR=PRE * 1.11;
        printf("\nNuevo precio: %7.2f" , NPR);
    }
}
