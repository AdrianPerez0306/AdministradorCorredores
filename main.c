//Nombre: Adrian Maximiliano Perez
//NOMBRE DE ARCHIVOS: main.c , funcionMenu.c , funciones.c , funcionesAux
//=>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "funcionMenu.c"

int main()
{
    FILE*pArchivo;
    FILE*pArchivo2;
    FILE*pArchivo3;

    char txt[30]="corredores.txt";
    char data[30]="corredores.data";
    char eliminados[30]="competidoresBajas.xyz";
    int opcion,continuar=1,primeraVez=1;
    do{
        //FLAG PARA LA PRIMERA VEZ QUE INGRESA. SOLO DECORATIVO
        if(primeraVez==1){
            printf("Presione la tecla ENTER para comenzar.");
            primeraVez=0;
        }
        else{printf("\nPresione la tecla ENTER para continuar.");}
        getchar();
        emiteMenu(&opcion);
        switch(opcion){
        case 1: emiteTxtFile(pArchivo,txt); break;
        case 2: creaArchBinario(pArchivo,data); break;
        case 3: migra(pArchivo,pArchivo2,txt,data); break;
        case 4: emiteSegunOpcion(pArchivo2,data); break;
        case 5: altaCompetidor(pArchivo2,data); break;
        case 6: buscaCompetidor(pArchivo2,data); break;
        case 7: modificaTiempoEdad(pArchivo2,data); break;
        case 8: bajaLogica(pArchivo2,data),emiteActivos(pArchivo2,data); break;
        case 9: eliminaInactivos(pArchivo2,pArchivo3,data,eliminados); break;
        case 10: emiteBinFile(pArchivo3,eliminados); break;
        case 0: continuar=0; break;
        default: printf("Error: Opcion ingresada no valida. Por favor, intente nuevamente.");
        }
    }while(continuar==1);
    return 0;
}
