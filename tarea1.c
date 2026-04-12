#include "tdas/list.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct{
  char nombre[50];
} categoria;

typedef struct{
  char categoria[50];
  char descripcion[100];
  char hora[20];
} tarea;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de Gestión Tareas");
  puts("========================================");

  puts("1) Nueva Categoría");
  puts("2) Eliminar Categoría");
  puts("3) Mostrar Categorías");
  puts("4) Registrar Tarea");
  puts("5) Atender Siguiente");
  puts("6) Visualización del Tablero General");
  puts("7) Filtrado por Categoría");
  puts("8) Salir");
}

void registrar_categorias(List *listCategorias) {
  printf("Registrar nueva categoría\n");
  char nombre[50];
  printf("Ingresa nombre de la Categoría: ");
  scanf(" %49s", nombre);
  categoria *aux = (categoria*)malloc(sizeof(categoria));
  if(aux != NULL){
    strcpy(aux->nombre, nombre);
    list_pushBack(listCategorias, aux);
  }
}

void mostrar_categorias(List *categorias) {
  if(categorias == NULL) printf("No hay categorias\n")return;
  printf("Categorías:\n");
  categoria *aux = (categoria*) list_first(categorias);
  while(aux != NULL){
    printf("%s\n", aux->nombre);
    aux = (categoria*) list_next(categorias);
  }
}

void eliminar_categoria(List *categorias, List *tareas){
  if(categorias == NULL) printf("No hay categorias\n")return;
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
  tarea *tareaActual = (tarea*)list_first(tareas);
  while(tareaActual != NULL){
    if(strcmp(tareaActual->categoria, aEliminar) == 0){
      list_popCurrent(tareas);
      free(tareaActual);
      tareaActual = (tarea*)list_first(tareas);
    }
    else tareaActual = (tarea*)list_next(tareas);
  }
}

void registrarTarea(List *categorias, List *tareas){
  if(list_first(categorias) == NULL){
    printf("Primero Ingrese una Categoria\n");
    return;
  }
  char catNombre[50];
  printf("Escriba el nombre de su categoria: ");
  scanf(" %49s", catNombre);

  categoria *catAux = (categoria*) list_first(categorias);
  
  while(catAux != NULL){
    if(strcmp(catAux->nombre, catNombre) == 0) break;
    catAux = (categoria*) list_next(categorias);
  }
  if(catAux == NULL){
    printf("Categoria inexistente %s\n", catNombre);
    return;
  }

  tarea *newTarea = (tarea*)malloc(sizeof(tarea));
  if(newTarea != NULL){
    strcpy(newTarea->categoria, catNombre);
    printf("Ingrese la descripcion de su tarea: ");
    scanf(" %99[^\n]", newTarea->descripcion);
    time_t tiempo = time(NULL);
    struct tm *tiempoHMS = localtime(&tiempo);
    strftime(newTarea->hora, 20, "%H:%M:%S", tiempoHMS);
    list_pushBack(tareas, newTarea);
  }
}

void atenderSiguiente(List *tareas){
  if(list_first(tareas) == NULL){
    printf("¡Libre de pendientes!\n");
    return;
  }
  tarea *tareaActual = (tarea*)list_popFront(tareas);
  printf("Atendiendo: [%s] | Categoría: [%s] | Registrada a las: [%s]", tareaActual->descripcion, tareaActual->categoria, tareaActual->hora);
  free(tareaActual);
}


void tableroGeneral(List *tareas){
  tarea *tareaActual = (tarea*)list_first(tareas);
  if(tareaActual == NULL){
    printf("No Hay pendientes, Tablero Vacio.");
    return;
  }
  while(tareaActual != NULL){
    printf("Descripcion: %s | Categoria: %s\n", tareaActual->descripcion, tareaActual->categoria);
    tareaActual = (tarea*) list_next(tareas);
  }
}

void filtrarPorCategoria(List *categorias, List *tareas){
  if(categorias == NULL) printf("No hay categorias\n")return;
  char catElegida[50];
  printf("Escoga la categoria a mostrar: ");
  scanf("%s", catElegida);

  tarea *tareaActual = (tarea*) list_first(tareas);
  printf("Tareas en la Categoria %s\n", catElegida);
  int contTareas = 0;
  while(tareaActual != NULL){
    if(strcmp(tareaActual->categoria, catElegida) == 0){
      printf("%s\n", tareaActual->descripcion);
      contTareas++;
    }
    tareaActual = (tarea*)list_next(tareas);
  }
  if(contTareas == 0) printf("No hay tareas en esta categoria");
}

int main() {
  char opcion;
  List *categorias = list_create(); // Lista para almacenar categorías
  List *tareas = list_create();

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
      eliminar_categoria(categorias, tareas);
      break;
    case '3':
      mostrar_categorias(categorias);
      break;
    case '4':
      // Lógica para registrar Tarea
      registrarTarea(categorias, tareas);
      break;
    case '5':
      // Lógica para atender la tarea siguiente
      atenderSiguiente(tareas);
      break;
    case '6':
      // Lógica para mostrar el tablero general
      tableroGeneral(tareas);
      break;
    case '7':
      // Lógica para filtrar por categoría
      filtrarPorCategoria(categorias, tareas);
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
  list_clean(tareas);

  return 0;
}
