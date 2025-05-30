#include <stdio.h>
#include <stdlib.h>
//escribir un programa que determine el rango de esdad
//de una persona
// 1- de 0 años 1 niño- recien nacido
// 2- de mayor de1 a 11 es niño
// 3- de 12 a 17 es adolecente
// 4- de 18 a 38 es adulto
// 5- de 39 a 75 es adulto mayor
// 6- de 76 a 86 es envejeciente
// 7- mayor de 86 es anciano

int main()
{
 //declara la variable y se inicializa
 int edad = 0;
 printf("\nEscriba la edad del paciente:");
 scanf("%i", &edad);
 if(edad >= 0 && edad <130)//validar datos (criterio)
 {
     if (edad <= 1)
     {
         printf("\nEs un infante dosis recomendada 0");
     }else if (edad> 1 && edad <12)
     {
         printf("\nEs un niño dosis recomendada 0.5ml");
     }else if (edad >= 12 && edad <18)
     {
         printf("\nEs un adolecente dosis recomendada 0.75ml");
     }else if(edad >= 18 && edad <39)
     {
         printf("\nEs un adulto dosis recomendada 1.0ml");
     }else if (edad >=39 && edad <76)
     {
         printf("\nEs un adulto mayor dosis recomendad 2.1ml");
     }else if (edad >=76 && edad <87)
     {
         printf("\nEdad no evaluada");
     }
 }else
 {
     printf("\nValores no validos");
 }
    printf("\nFin de programa");
    return 0;
}
