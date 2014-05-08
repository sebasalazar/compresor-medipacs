/* 
 * File:   variables.h
 * Author: seba
 *
 * Created on May 8, 2014, 3:04 PM
 */

#ifndef VARIABLES_H
#define	VARIABLES_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SERVIDOR "127.0.0.1"
#define PUERTO 6432
#define NOMBREDB "pacsdb"
#define USUARIODB "pacs"
#define PASSDB "pacs"

#define RUTA_ZIP "/srv/web/medipacs.cl/www/htdocs/zip"
#define RUTA_DCM "/opt/dcm4chee/server/default/archive"

    typedef struct {
        char* paciente;
        char* archivo;
    } examen;

#ifdef	__cplusplus
}
#endif

#endif	/* VARIABLES_H */

