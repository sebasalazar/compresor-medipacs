#include "compresor.h"

void comprimir(long id, char* nombreArchivo) {
    char consulta[1025];
    char nombre[1025];
    char ruta[1025];
    char buffer[4096];
    int i = 0, k = 0, num = 0, error = 0, len = 0;
    PGconn* conexion;
    PGresult* resultado;
    long tamano = 0;
    examen* examenes = NULL;
    zip* comprimido = NULL;
    zip_source* temporal = NULL;

    if (id > 0) {
        tamano = sizeof (char)*1025;
        memset(consulta, 0, tamano);
        snprintf(consulta, 1024, "SELECT patient.pat_name, files.filepath FROM patient INNER JOIN study ON (patient.pk = study.patient_fk) INNER JOIN series ON (study.pk = series.study_fk) INNER JOIN instance ON (series.pk = instance.series_fk ) INNER JOIN files ON (instance.pk = files.instance_fk) INNER JOIN filesystem ON (files.filesystem_fk = filesystem.pk) WHERE (study.pk = '%ld')", id);
        conexion = dbconnect(SERVIDOR, PUERTO, NOMBREDB, USUARIODB, PASSDB);
        if (conexion != NULL) {
            resultado = dbquery(conexion, consulta);
            num = dbnumrows(resultado);
            if (num > 0) {
                examenes = (examen *) malloc(num * sizeof (examen));
                for (i = 0; i < num; i++) {
                    memset(nombre, 0, tamano);
                    memset(ruta, 0, tamano);

                    sprintf(nombre, 1024, "%s", getString(dbresult(resultado, i, 0)));
                    sprintf(ruta, 1024, "%s/%s", RUTA_DCM, getString(dbresult(resultado, i, 1)));

                    if (existeArchivo(ruta)) {
                        // Si el archivo existe, lleno la estructura y aumento el contador
                        examenes[k].paciente = (char *) calloc(strlen(nombre) + 1, sizeof (char));
                        snprintf(examenes[k].paciente, 1024, "%s", nombre);
                        examenes[k].archivo = (char *) calloc(strlen(ruta) + 1, sizeof (char));
                        snprintf(examenes[k].archivo, 1024, "%s", ruta);
                        k += 1;
                    } else {
                        fprintf(stderr, "\n[ERROR] No existe el archivo: %s", ruta);
                    }
                }
                dbfree(resultado);
            }
            dbclose(conexion);
        }

        // Proceso de Compresion
        if (k > 0) {
            // Elimino archivo si este existe.
            if (existeArchivo(nombreArchivo)) {
                remove(nombreArchivo);
            }

            comprimido = zip_open(nombreArchivo, ZIP_CREATE, &error);
            if (comprimido != NULL) {
                for (i = 0; i < k; i++) {
                    len = 4096 * sizeof (char);
                    memset(buffer, 0, len);
                    temporal = zip_source_buffer(comprimido, buffer, len, 1);
                    if (temporal != NULL) {
                        if (zip_add(comprimido, examenes[i].archivo, temporal) >= 0) {
                            zip_source_free(temporal);
                        } else {
                            fprintf(stderr, "\n[ERROR] No se pudo agregar archivo comprimido: %s", zip_strerror(comprimido));
                        }
                    } else {
                        fprintf(stderr, "\n[ERROR] No se pudo crear buffer: %s", zip_strerror(comprimido));
                    }
                }
                zip_close(comprimido);
            } else {
                fprintf(stderr, "\n[Error] No fue posible crear archivo comprimido: %s", zip_strerror(comprimido));
            }
            free(examenes);
        }
    }
}

