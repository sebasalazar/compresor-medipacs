/* 
 * File:   main.c
 * Author: seba
 *
 * Created on November 11, 2013, 6:52 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "db.h"
#include "utils.h"
#include "compresor.h"

/*
 * 
 */
int main(int argc, char** argv) {
    long id = 0;
    char *nombre;

    if (argc >= 3) {
        id = getLong(argv[1]);
        nombre = getString(argv[2]);

        fprintf(stdout, "\nGenerando Archivo %s con id %ld", nombre, id);
        comprimir(id, nombre);
        return EXIT_SUCCESS;
    } else {
        fprintf(stdout, "\nForma de uso: %s id nombre_archivo", argv[0]);
        return EXIT_FAILURE;
    }
}