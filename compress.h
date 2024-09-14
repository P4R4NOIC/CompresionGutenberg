

// Structure for tree nodes
struct Node {
    wchar_t character;
    int freq;
    struct Node *l, *r, *sig;
};

struct Table
{
   wchar_t character;      /* Letra a la que hace referencia el nodo */
   unsigned long int bits; /* Valor de la codificación de la letra */
   char nbits;      /* Número de bits de la codificación */
   struct Table *sig;     /* Siguiente elemento de la tabla */
};  

struct Table *Tabla;


/* Buscar un caracter en la tabla, devielve un puntero al elemento de la tabla */
struct Table *BuscaCaracter(struct Table *Tabla, wchar_t c)
{
   struct Table *t;

   t = Tabla;
   while(t && t->character != c) t = t->sig;
   return t;
}

/* Función recursiva para borrar un arbol */
void BorrarArbol(struct Node *n)
{
   if(n->l) BorrarArbol(n->l);
   if(n->r)  BorrarArbol(n->r);
   free(n);
}

/* Insertar un elemento en la tabla */
void InsertarTabla(wchar_t c, int l, int v)
{
   struct Table *t, *p, *a;

   t = (struct Table *)malloc(sizeof(struct Table)); /* Crea un elemento de tabla */
   t->character = c;                               /* Y lo inicializa */
   t->bits = v;
   t->nbits = l;

   if(!Tabla)         /* Si tabla es NULL, entonces el elemento t es el 1º */
   {
      Tabla = t;
      Tabla->sig = NULL;
   }
   else
   {
       /* Buscar el caracter en la lista (ordenada por frecuencia) */
       p = Tabla;
       a = NULL;
       while(p && p->character < t->character)
       {
          a = p;      /* Guardamos el elemento actual para insertar */
          p = p->sig; /* Avanzamos al siguiente */
       }
       /* Insertar el elemento */
       t->sig = p;
       if(a) a->sig = t;  /* Insertar entre a y p */
       else Tabla = t;    /* el nuevo es el primero */
    }
}

/* Función recursiva para crear Tabla */
/* Recorre el árbol cuya raiz es n y le asigna el código v de l bits */
void CrearTabla(struct Node *n, int l, int v)
{
   if(n->r)  CrearTabla(n->r, l+1, (v<<1)|1);
   if(n->l) CrearTabla(n->l, l+1, v<<1);
   if(!n->r && !n->l) InsertarTabla(n->character, l, v);
}
// Función para contar la frecuencia de caracteres
void Cuenta(struct Node** Lista, wchar_t c) {
    struct Node *p, *a, *q;

    if (*Lista == NULL) {  /* Si la lista está vacía */
        *Lista = (struct Node*)malloc(sizeof(struct Node));  /* Crear un nuevo nodo */
        (*Lista)->character = c;
        (*Lista)->freq = 1;  /* Primera aparición */
        (*Lista)->sig = (*Lista)->l = (*Lista)->r = NULL;
    } else {
        /* Buscar el caracter en la lista */
        p = *Lista;
        a = NULL;
        while (p && p->character < c) {
            a = p;
            p = p->sig;
        }
        /* Dos casos: */
        if (p && p->character == c) {
            p->freq++;  /* Actualizar frecuencia */
        } else {
            /* Insertar un nuevo nodo */
            q = (struct Node*)malloc(sizeof(struct Node));
            q->character = c;
            q->freq = 1;
            q->l = q->r = NULL;
            q->sig = p;        /* Insertar entre p y a */
            if (a) {
                a->sig = q;
            } else {
                *Lista = q;  /* Si a es NULL, el nuevo nodo es el primero */
            }
        }
    }
}

// Función para insertar un nodo en la lista ordenada por frecuencia
void InsertarOrden(struct Node** Cabeza, struct Node* e) {
    struct Node *p, *a;

    if (*Cabeza == NULL || (*Cabeza)->freq >= e->freq) {  /* Si Cabeza es NULL o e debe ir al principio */
        e->sig = *Cabeza;
        *Cabeza = e;
    } else {
        p = *Cabeza;
        a = NULL;
        while (p && p->freq < e->freq) {
            a = p;
            p = p->sig;
        }
        e->sig = p;
        if (a) {
            a->sig = e;
        } else {
            *Cabeza = e;
        }
    }
}

// Función para ordenar la lista por frecuencia
void Ordenar(struct Node** Lista) {
    struct Node *Lista2, *a;

    if (*Lista == NULL) return; /* Lista vacía */
    Lista2 = *Lista;
    *Lista = NULL;
    while (Lista2) {
        a = Lista2;              /* Toma los elementos de Lista2 */
        Lista2 = a->sig;
        InsertarOrden(Lista, a); /* Inserta por orden en Lista */
    }
}

// Función para imprimir la lista
void ImprimirLista(struct Node* Lista) {
    while (Lista) {
        wprintf(L"Caracter: %lc, Frecuencia: %d\n", Lista->character, Lista->freq);
        Lista = Lista->sig;
    }
}

