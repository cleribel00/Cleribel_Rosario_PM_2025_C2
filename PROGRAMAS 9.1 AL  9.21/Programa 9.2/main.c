#include <stdio.h>
#include <stdlib.h>

/* Archivos y caracteres.
El programa lee caracteres de un archivo. */

void main(void)
{
char p1;
FILE *ar;
FILE *ar2;
ar = fopen("c: \\temp\\arc.text", "r");
ar2 = fopen("c: \\temp\\arc2.text", "w");

if ((ar != NULL && ar2 != NULL

{
while (!feof(ar))
{
p1 = fgetc(ar); /* Lee el caracter del archivo. */
putchar(p1); /* Despliega el caracter en la pantalla. */
fputc(P1, ar2)
}
fclose(ar);
fclose(ar2);
}
else
{
    if (ar== NULL )
}
printf("No se puede abrir el archivo");
}
