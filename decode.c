#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <malloc.h>

struct Node {
    wchar_t character;
    unsigned long int bits; /* Valor de la codificación de la letra */
    char nbits; 
    struct Node *l, *r;
};


/* Función recursiva para borrar un arbol */
void BorrarArbol(struct Node *n)
{
   if(n->l) BorrarArbol(n->l);
   if(n->r)  BorrarArbol(n->r);
   free(n);
}

int main(int argc, char *argv[]){
   setlocale(LC_CTYPE, "");
   struct Node *Arbol;        /* Arbol de codificación */
   long int Longitud;      /* Longitud de fichero */
   int nElementos;         /* Elementos de árbol */
   unsigned long int bits; /* Almacen de bits para decodificación */
   FILE *fe, *fs;          /* Ficheros de entrada y salida */

   struct Node *p, *q;        /* Auxiliares */
   wchar_t a;
   int i, j;
   char dext[]=".txt";

   if(argc<=2)
   {
      //printf("Usage:\n %s <file-to-unzip> <destination-filename>\n",argv[0]);
      if(argc==2)
      {
         argv[2]=(char *)malloc(sizeof(char)*(strlen(argv[1])+strlen(dext)+1));
         strcpy(argv[2],argv[1]);
         strcat(argv[2],dext);
         argc++;
      }
      else	
         return 0;
   }

   /* Crear un arbol con la información de la tabla */
   Arbol = (struct Node *)malloc(sizeof(struct Node)); /* un nodo nuevo */
   Arbol->character = 0;
   Arbol->r = Arbol->l = NULL;
   fe = fopen(argv[1], "rb, ccs=UTF-8");
   fread(&Longitud, sizeof(long int), 1, fe); /* Lee el número de caracteres */
   fread(&nElementos, sizeof(int), 1, fe); /* Lee el número de elementos */
   for(i = 0; i < nElementos; i++) /* Leer todos los elementos */
   {
      p = (struct Node *)malloc(sizeof(struct Node)); /* un nodo nuevo */
      fread(&p->character, sizeof(wchar_t), 1, fe); /* Lee el carácter */
      fread(&p->bits, sizeof(unsigned long int), 1, fe); /* Lee el código */
      fread(&p->nbits, sizeof(char), 1, fe); /* Lee la longitud */
      p->l = p->r = NULL;
      /* Insertar el nodo en su lugar */
      j = 1 << (p->nbits-1);
      q = Arbol;
      while(j > 1)
      {
         if(p->bits & j) /* es un uno*/
            if(q->r) q = q->r;   /* Si el nodo existe, nos movemos a él */
            else                     /* Si no existe, lo creamos */
            {
               q->r = (struct Node *)malloc(sizeof(struct Node)); /* un nodo nuevo */
               q = q->r;
               q->character = 0;
               q->r = q->l = NULL;
            }
         else /* es un cero */
            if(q->l) q = q->l; /* Si el nodo existe, nos movemos a él */
            else                     /* Si no existe, lo creamos */
            {
               q->l = (struct Node *)malloc(sizeof(struct Node)); /* un nodo nuevo */
               q = q->l;
               q->character = 0;
               q->r = q->l = NULL;
            }
         j >>= 1;  /* Siguiente bit */
      }
      /* Ultimo Bit */
      if(p->bits & 1) /* es un uno*/
         q->r = p;
      else            /* es un cero */
         q->l = p;
   }
   bits = 0;
   fs = fopen(argv[2], "w, ccs=UTF-8");
   if (fs == NULL) {
        perror("Error al abrir el archivo de salida");
        fclose(fe);
        return 1;
    }
   /* Lee los primeros cuatro bytes en la dobel palabra bits */
   fread(&a, 1, 1, fe);
   bits |= a;
   bits <<= 8;
   fread(&a, 1, 1, fe);
   bits |= a;
   bits <<= 8;
   fread(&a, 1, 1, fe);
   bits |= a;
   bits <<= 8;
   fread(&a, 1, 1, fe);
   bits |= a;
   j = 0; /* Cada 8 bits leemos otro byte */
   q = Arbol;
   while(Longitud){
      if(bits & 0x80000000) q = q->r; else q = q->l; /* Rama adecuada */
      bits <<= 1;           /* Siguiente bit */
      j++;
      if(8 == j)            /* Cada 8 bits */
      {
         i = fread(&a, 1, 1, fe); /* Leemos un byte desde el fichero */
         bits |= a;                    /* Y lo insertamos en bits */
         j = 0;                        /* No quedan huecos */
      }                                
      if(!q->r && !q->l)          /* Si el nodo es una letra */
      {
         putwc(q->character, fs);           /* La escribimos en el fich de salida */
         Longitud--;                   /* Actualizamos longitud que queda */
         q=Arbol;                      /* Volvemos a la raiz del árbol */
      }
   }                   /* Hasta que acabe el fichero */
   /* Procesar la cola */

   fclose(fs);                         /* Cerramos ficheros */
   fclose(fe);

   BorrarArbol(Arbol);                 /* Borramos el árbol */
   return 0;
}

