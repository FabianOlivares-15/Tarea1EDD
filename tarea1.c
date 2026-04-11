#include "tdas/list.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
  char nombre[50];
} categoria;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de Gestión Tareas");
  puts("========================================");

  puts("1) Nueva Categoría");
  puts("2) Eliminar Categoría");
  puts("3) Mostrar Categorías");
  //puts("4) Registrar Tarea");
  //puts("5) Atender Siguiente");
  //puts("6) Visualización del Tablero General");
  //puts("7) Filtrado por Categoría");
  puts("8) Salir");
}

void registrar_categorias(List *listCategorias) {
  printf("Registrar nueva categoría\n");
  char nombre[50];
  printf("Ingresa nombre de la Categoría:");
  scanf(" %49s", nombre);
  categoria *aux = (categoria*)malloc(sizeof(categoria));
  if(aux != NULL){
    strcpy(aux->nombre, nombre);
    list_pushBack(listCategorias, aux);
  }
}

void mostrar_categorias(List *categorias) {
  printf("Categorías:\n");
  if(categorias == NULL) return;
  categoria *aux = (categoria*) list_first(categorias);
  while(aux != NULL){
    printf("%s\n", aux->nombre);
    aux = (categoria*) list_next(categorias);
  }
}

void eliminar_categoria(List *categorias){
  char aEliminar[50];
  printf("Ingresa nombre de la Categoría a eliminar: ");
  scanf(" %49s", aEliminar);
  categoria *catActual = (categoria*) list_first(categorias);
  while(catActual != NULL){
     if(strcmp(catActual->nombre, aEliminar) == 0){
       list_popCurrent(categorias);
       free(catActual);
       break;
     }
  catActual = (categoria*) list_next(categorias);
  }
}

int main() {
  char opcion;
  List *categorias = list_create(); // Lista para almacenar categorías

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
                           // newline anterior

    switch (opcion) {
    case '1':
      registrar_categorias(categorias);
      break;
    case '2':
      eliminar_categoria(categorias);
      break;
    case '3':
      mostrar_categorias(categorias);
      break;
    case '4':
      // Lógica para registrar Tarea
      break;
    case '5':
      // Lógica para atender la tarea siguiente
      break;
    case '6':
      // Lógica para mostrar el tablero general
      break;
    case '7':
      // Lógica para filtrar por categoría
      break;
    case '8':
      puts("Saliendo del sistema de gestión de tareas...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  list_clean(categorias);

  return 0;
}
