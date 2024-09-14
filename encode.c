#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<wchar.h>
#include<locale.h>

#include"compress.h"




int main(int argc, char *argv[]) {
    setlocale(LC_CTYPE, "");
    struct Node* Lista = NULL; /* Lista de letras y frecuencias */
    struct Node* Arbol = NULL;
    FILE *fe, *fs; /* Ficheros de entrada y salida */
    wchar_t c;   /* Variable auxiliar */
    struct Node *p;
    struct Table *t;
    int nElementos;        /* Número de elementos en tabla */
    long int Longitud = 0;
    unsigned long int dWORD; /* Soble palabra usada durante la codificación */
    int nBits;               /* Número de bits usados de dWORD */

    if (argc < 3) {
        printf("Usar:\n%s <fichero_entrada> <fichero_salida>\n", argv[0]);
        return 1;
    }

    /* Fase 1: contar frecuencias */
    fe = fopen(argv[1], "r, ccs=UTF-8");
    if (fe == NULL) {
        perror("Error al abrir el fichero de entrada");
        return 1;
    }

    while ((c = fgetwc(fe)) != WEOF) {
        Longitud++;       /* Actualiza la cuenta de la longitud del fichero */
        Cuenta(&Lista, c); /* Actualiza la lista de frecuencias */
    }
    fclose(fe);

    /* Ordenar la lista de menor a mayor por frecuencias */
    Ordenar(&Lista);
    //ImprimirLista(Lista);
    
    Arbol = Lista;
    while(Arbol && Arbol->sig){
        p = (struct Node*)malloc(sizeof(struct Node));
        p->character = 0;
        
        p->r = Arbol;
        Arbol = Arbol->sig;
        p->l = Arbol;                          /* Rama cero */
        Arbol = Arbol->sig;                       /* Siguiente nodo */
        p->freq = p->r->freq +
                        p->l->freq;      /* Suma de frecuencias */
        InsertarOrden(&Arbol, p);        
    }
    Tabla = NULL;
    CrearTabla(Arbol, 0, 0);
    
    remove(argv[2]);
    /* Crear fichero comprimido */
    fs = fopen(argv[2], "wb");
    /* Escribir la longitud del fichero */
    fwrite(&Longitud, sizeof(long int), 1, fs);
    /* Cuenta el número de elementos de tabla */
    nElementos = 0;
    t = Tabla;
    while(t){
        nElementos++;
        t = t->sig;
    }
    
    fwrite(&nElementos, sizeof(int), 1, fs);
    
    t = Tabla;
    
    while(t){
        fwrite(&t->character, sizeof(wchar_t), 1, fs);
        fwrite(&t->bits, sizeof(unsigned long int), 1, fs);
        fwrite(&t->nbits, sizeof(char), 1, fs);
        t = t->sig;
   }

  /* Codificación del fichero de entrada */
   fe = fopen(argv[1], "rb, ccs=UTF-8");
   dWORD = 0; /* Valor inicial. */
   nBits = 0; /* Ningún bit */
   while((c = fgetwc(fe)) != WEOF)
   {
      /* Busca c en tabla: */
      t = BuscaCaracter(Tabla, c);
      /* Si nBits + t->nbits > 32, sacar un byte */
      while(nBits + t->nbits > 32)
      {
         c = dWORD >> (nBits-8);           /* Extrae los 8 bits de mayor peso */
         fwrite(&c, 1, 1, fs);  /* Y lo escribe en el fichero */
         nBits -= 8;                       /* Ya tenemos hueco para 8 bits más */
      }
      dWORD <<= t->nbits; /* Hacemos sitio para el nuevo caracter */
      dWORD |= t->bits;   /* Insertamos el nuevo caracter */
      nBits += t->nbits;  /* Actualizamos la cuenta de bits */
   }
   /* Extraer los cuatro bytes que quedan en dWORD*/
   while(nBits>0)
   {
      if(nBits>=8) c = dWORD >> (nBits-8);
      else c = dWORD << (8-nBits);
      fwrite(&c, 1, 1, fs);
      nBits -= 8;
   }

   fclose(fe);  /* Cierra los ficheros */
   fclose(fs);

   /* Borrar Arbol */
   BorrarArbol(Arbol);

   /* Borrar Tabla */
   while(Tabla)
   {
      t = Tabla;
      Tabla = t->sig;
      free(t);
   }

    

    return 0;
}

