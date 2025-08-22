#include <stdio.h>
#include <stdlib.h>

/* ventas descendentes.
El programa, al recibir como datos tres valores que representan las ventas
de los vendedores de una tienda de discos, escribe las ventas en
orden descendente.

P, S Y R: variables de tipo real. */

void main(void)
{
    float P, S, R;
    printf("\nIntegre las ventas de los tres vendedores: ");
    scanf("%f %f %f" , &P, &S, &R);
    if (P > S)
        if(P > R)
        if(S > R)
        printf("\n\n El orden es P, S Y R: %8.2f %8.2f %8.2f", P ,S , R);
    else
        printf("\n\n El orden es P, R Y S: %8.2f %8.2f %8.2f", P ,R , S);
    else
        printf("\n\n El orden es R, P Y S: %8.2f %8.2f %8.2f", R ,P , S);
    else
        if (S > R)
        if (P > R)
        printf("\n\n El orden es S, P Y R: %8.2f %8.2f %8.2f", S ,P , R);
    else
        printf("\n\n El orden es S, R Y P: %8.2f %8.2f %8.2f", S ,R , P);
    else
        printf("\n\n El orden es R, S Y P: %8.2f %8.2f %8.2f", R ,S , P);

}
