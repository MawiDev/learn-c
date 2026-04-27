/*
 * Lezione 03 (medi) — Strutture.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    double x;
    double y;
} Punto;

typedef struct {
    Punto  origine;
    double larghezza;
    double altezza;
} Rettangolo;

typedef struct Nodo {
    int           v;
    struct Nodo  *next;
} Nodo;

typedef struct {
    size_t len;
    char   data[];
} Buffer;

static double area(const Rettangolo *r) {
    return r->larghezza * r->altezza;
}

static Nodo *lista_push_front(Nodo *head, int v) {
    Nodo *n = malloc(sizeof *n);
    if (!n) return head;
    n->v = v;
    n->next = head;
    return n;
}

static void lista_stampa(const Nodo *head) {
    for (const Nodo *p = head; p != NULL; p = p->next) {
        printf("%d -> ", p->v);
    }
    printf("NULL\n");
}

static void lista_free(Nodo *head) {
    while (head != NULL) {
        Nodo *next = head->next;
        free(head);
        head = next;
    }
}

int main(void) {
    Rettangolo r = {
        .origine   = { .x = 0, .y = 0 },
        .larghezza = 4.0,
        .altezza   = 3.0
    };
    printf("area = %.2f\n", area(&r));

    /* Lista concatenata */
    Nodo *lista = NULL;
    for (int i = 1; i <= 5; ++i) {
        lista = lista_push_front(lista, i);
    }
    printf("lista: ");
    lista_stampa(lista);
    lista_free(lista);

    /* Confronto dimensioni per ordinamento campi */
    struct A { char a; int b; char c; };
    struct B { int b; char a; char c; };
    printf("sizeof(A) = %zu\n", sizeof(struct A));
    printf("sizeof(B) = %zu\n", sizeof(struct B));

    /* Flexible array member: singola malloc */
    const char *msg = "ciao Riccardo";
    size_t n = strlen(msg) + 1;
    Buffer *buf = malloc(sizeof *buf + n);
    if (!buf) return 1;
    buf->len = n;
    memcpy(buf->data, msg, n);
    printf("buffer flex (%zu byte): %s\n", buf->len, buf->data);
    free(buf);

    return 0;
}
