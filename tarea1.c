#include "tdas/list.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Creamos 2 struct una para las categorias y otra para las tareas, la struct de tareas contiene lo pedido
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
  // en esta funcion registramos las categorias agregandolas a una lista creada en el main, usamos metodos de cadenas para copiar nombres
  // usamos tdas para agregar a nuestra lista
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
  //recorremos la lista de categorias y vamos mostrando las categorias de esta
  if(list_first(categorias) == NULL){
    printf("No hay categorias\n");
    return;
  }
  printf("Categorías:\n");
  categoria *aux = (categoria*) list_first(categorias);
  while(aux != NULL){
    printf("%s\n", aux->nombre);
    aux = (categoria*) list_next(categorias);
  }
}

void eliminar_categoria(List *categorias, List *tareas){
  if(list_first(categorias) == NULL){
    printf("No hay categorias\n");
    return;
  }
  //leemos el nombre de la categoria a eliminar, vamos recorriendo la lista y comparando si los nombres coinciden si coinciden se elimina
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
  //buscamos las tareas que pertenezcan a esa categoria en nuestra lista de tareas y las eliminamos
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
  //registramos el nombre de la categoria de nuestra nueva tarea
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
//registramos la descripcion y la hora de nuestra nueva tarea y la agregamos a nuestra lista de tareas
  tarea *newTarea = (tarea*)malloc(sizeof(tarea));
  if(newTarea != NULL){
    strcpy(newTarea->categoria, catNombre);
    printf("Ingrese la descripcion de su tarea: ");
    scanf(" %99[^\n]", newTarea->descripcion);
    time_t tiempo = time(NULL);
    tiempo -= 14400;
    struct tm *tiempoHMS = localtime(&tiempo);
    strftime(newTarea->hora, 20, "%H:%M:%S", tiempoHMS);
    list_pushBack(tareas, newTarea);
  }
}

void atenderSiguiente(List *tareas){
  //atendemos la tarea mas antigua mostramos sus datos y la eliminamos
  if(list_first(tareas) == NULL){
    printf("¡Libre de pendientes!\n");
    return;
  }
  tarea *tareaActual = (tarea*)list_popFront(tareas);
  printf("Atendiendo: [%s] | Categoría: [%s] | Registrada a las: [%s]\n", tareaActual->descripcion, tareaActual->categoria, tareaActual->hora);
  free(tareaActual);
}


void tableroGeneral(List *tareas){
  //mostramos todas las tareas con sus respectivas categorias
  tarea *tareaActual = (tarea*)list_first(tareas);
  if(tareaActual == NULL){
    printf("No Hay pendientes, Tablero Vacio.");
    return;
  }
  while(tareaActual != NULL){
    printf("Descripcion: %s | Categoria: %s | Hora [%s]\n", tareaActual->descripcion, tareaActual->categoria, tareaActual->hora);
    tareaActual = (tarea*) list_next(tareas);
  }
}

void filtrarPorCategoria(List *categorias, List *tareas){
  //leemos la categoria escogida a mostrar y mostramos todas sus tareas
  if(list_first(categorias) == NULL){
    printf("No hay categorias\n");
    return;
  }
  char catElegida[50];
  printf("Escoga la categoria a mostrar: ");
  scanf("%49s", catElegida);

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
  List *categorias = list_create(); // Lista para almacenar categorías con struct categorias
  List *tareas = list_create(); // Lista para almacenar tareas con struct tarea

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
