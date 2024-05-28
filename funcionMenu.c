#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "funciones.c"

//PROTOTIPO DE FUNCIONES
void emiteBinFile(FILE *pA2,char *data);
//

//AQUI SE ENCUENTRAN LAS FUNCIONES PARA EMITIR EL MENU Y EJECUTAR CUALQUIER OPCION DEL MENU
//EN ALGUNOS CASOS EL MENU UTILIZA FUNCIONES DE "funcionesAux.c"
void emiteMenu(int *opcion){
    printf("\n1-Emite el archivo de texto.");
        printf("\n2-Crea un archivo binario.");
        printf("\n3-Migra los datos .");
        printf("\n4-Emite datos segun opcion:");
            printf("\n  a)Emite todos los datos.");
            printf("\n  b)Emite solo activos.");
            printf("\n  c)Emite segun pais.");
            printf("\n  d)Emite segun rango de tiempo.");
        printf("\n5-Alta de nuevo competidor.");
        printf("\n6-Busca datos de un competidor segun:");
            printf("\n  a)Nro de orden.");
            printf("\n  b)Nro de Corredor.");
        printf("\n7-Modifica tiempo o edad:");
            printf("\n  a)Nro de orden.");
            printf("\n  b)Nro de Corredor.");
        printf("\n8-Baja logica.");
        printf("\n9-Baja fisica.");
        printf("\n10-Imprime historial bajas fisicas.");
        printf("\n0-Fin de programa.");
        printf("\nIngrese una opcion:");
        scanf("%d",opcion);
}

//OPCION 1:EMITE EL ARCHIVO DE TEXTO
void emiteTxtFile(FILE *pA,char *nombre){
    struct competidor v;
    pA=fopen(nombre,"r");
    printf("                          FECHA                      \n");
    printf("Orden\tCorredor\tDia-Mes-Anio\tEdad\tPais\tTiempo\n");
    if(pA!=NULL){
        while(!feof(pA)){
           fscanf(pA,"%d%d%d%s%d%d%s%lf\n",&v.orden,&v.nroCorredor,&v.date.dia,&v.date.mes,&v.date.anio,&v.edad,&v.pais,&v.tiempo);
           printf("%d\t%d\t\t%d-%3s-%d\t%d\t%3s\t%lf\n",v.orden,v.nroCorredor,v.date.dia,v.date.mes,v.date.anio,v.edad,v.pais,v.tiempo);
        }
        fclose(pA);
    }
    else{printf("\nError en la apertura del archivo.");}
}

//OPCION 2:CREA EL ARCHIVO BINARIO
void creaArchBinario(FILE *pA,char *data){
    //EMITE SIEMPRE UNA VEZ DE MAS. NO SE PORQUE
    if( (pA=fopen(data,"wb+"))== NULL){
        printf("\nError: No se pudo crear el archivo.");
    }
    else {
        printf("\nArchivo creado exitosamente.\n");
        fclose(pA);
    }
}

//OPCION 3:MIGRA DATOS DE .TXT A .DATA
void migra(FILE *pA,FILE*pA2,char *txt,char *data){
    int registroActual=0;
    struct competidor v;
    v.activo=1;
    pA=fopen(txt,"r");
    pA2=fopen(data,"ab");
    //LEE TXT, ESCRIBE BINARIO
    if(pA!=NULL && pA2!=NULL){
        while(!feof(pA)){
            fscanf(pA,"%d%d%d%s%d%d%s%lf\n",&v.orden,&v.nroCorredor,&v.date.dia,&v.date.mes,&v.date.anio,&v.edad,&v.pais,&v.tiempo);
            fwrite(&v,sizeof(struct competidor),1,pA2);
        }
        fclose(pA);
        fclose(pA2);
        printf("\nLos datos se copiaron correctamente.\n");
    }
    else{printf("\nError:Error no se pudo abrir el archivo.");}
}

//OPCION 4:EMITE POR OPCION(ARCHIVO BINARIO)(Todos, solo activos, por pais, en rango de tiempo)
void emiteSegunOpcion(FILE *pA2,char *data){
    char opcion;
    int nroOpcion=4;
    fflush(stdin);
    printf("\na)Emite todos los datos.");
    printf("\nb)Emite solo activos.");
    printf("\nc)Emite segun pais.");
    printf("\nd)Emite segun rango de tiempo.");
    printf("\nIngrese opcion(caracter a,b,c,d):");
    scanf("%c",&opcion);
    verificaOpcion(&opcion,nroOpcion);
    fflush(stdin);
    switch(toupper(opcion)){
    case'A': emiteBinFile(pA2,data);
        break;
    case'B': emiteActivos(pA2,data);
        break;
    case'C': emitePais(pA2,data);
        break;
    case'D': emiteTimeRange(pA2,data);
        break;
    }
}

//OPCION 5:ALTA NUEVO COMPETIDOR
void altaCompetidor(FILE *pA2,char *data){
    //PARA ESTA FUNCION TRABAJO CON "struct competidorAux v", QUE RECIBE LAS MISMAS VARIABLESD QUE struct competidor v,PERO...
    //RECIBE ALGUNAS VARIABLES EN FORMATO CHAR, PARA PODER VERIFICAR SI INGRESA CARACTERES
    struct competidorAux v;
    int nroOrdenAlta;
    fflush(stdin);
    //DE AQUI HACIA ABAJO INGRESO Y VERIFICO CADA VARIABLE NECESARIA PARA EL ALTA
    printf("\nIngrese el numero de orden para el nuevo competidor:");
    scanf("%d",&nroOrdenAlta);
    fflush(stdin);
    verificaNroAlta(&nroOrdenAlta);         //EN CASO QUE SEA MENOR QUE CERO SE VERIFICA CON RECURSIVIDAD
    creaRegsNulos(nroOrdenAlta,pA2,data);   //SI SE INGRESA UN NRO MAYOR A LA CANT DE REGISTROS, CREA NUEVOS REGISTROS VACIOS
    fflush(stdin);
    verificaOcupado(&nroOrdenAlta,pA2,data);//
    v.orden=nroOrdenAlta;
    printf("\nIngrese numero de ID:");
    scanf("%5s",&v.nroCorredor);
    fflush(stdin);
    verificaCharNumerico(v.nroCorredor,5);
    chequeaIDValido(v.nroCorredor,pA2,data);
    printf("\nIngrese fecha(Anio minimo 2015)");
    ingresaVerificaFecha(v.date2.dia,v.date2.mes,v.date2.anio,pA2,data);
    ingresaVerificaEdad(v.edad);
    printf("\nIngrese pais:");
    scanf("%3s",v.pais);
    fflush(stdin);
    //CONVIERTO LA CADENA PAIS A MAYUSCULAS
    for(int j=0 ; v.pais[j]!='\0' ; j++ ){
        v.pais[j]=toupper(v.pais[j]);
    }
    fflush(stdin);
    verificaCharNumerico(v.pais,3);
    ingresaVerificaTiempo(v.tiempo);
    v.activo=1;
    cargaDatosAlta(pA2,data,v,nroOrdenAlta);
}

//OPCION 6:BUSCAR DATOS DE COMPETIDOR/A POR OPCION(Nro Orden/Nro Competidor)
void buscaCompetidor(FILE *pA2, char *data){
    char opcion;
    int nroOpcion=6;
    fflush(stdin);
    printf("\na)Por Nro.Orden.");
    printf("\nb)Por Nro.Corredor/a.");
    printf("\nIngrese opcion(caracter a,b):");
    scanf("%c",&opcion);
    verificaOpcion(&opcion,nroOpcion);
    fflush(stdin);
    switch(toupper(opcion)){
    case'A': emiteCorredorPorOrden(pA2,data);
        break;
    case'B': emiteCorredorPorID(pA2,data);
        break;
    }
}

//OPCION 7:MODIFICA TIEMPO EDAD O TIEMPO por opcion(Nro Orden/Nro Competidor)
void modificaTiempoEdad(FILE *pA2,char *data){
        char opcion;
        int nroOpcion=6;
        fflush(stdin);
        printf("\na)Por Nro.Orden.");
        printf("\nb)Por Nro.Corredor/a.");
        printf("\nIngrese opcion(caracter a,b):");
        scanf("%c",&opcion);
        verificaOpcion(&opcion,nroOpcion);
        fflush(stdin);
        switch(toupper(opcion)){
        case'A': modificaPorOrden(pA2,data);
            break;
        case'B': modificaPorID(pA2,data);
            break;
        }
}

//OPCION 8:BAJA LOGICA(A eleccion con numero de orden)
void bajaLogica(FILE *pA2,char *data){
    struct competidor v;
    int totalReg,registro,confirmar;
    pA2=fopen(data,"rb+");
    if(pA2!=NULL){
        fseek(pA2,0,SEEK_END);
        totalReg=ftell(pA2)/sizeof(struct competidor);
        emiteBinFile(pA2,data);
        printf("\nIngrese el numero de orden para dar baja logica:");
        scanf("%d",&registro);
        verificaReg(totalReg,&registro,pA2,data);
        confirma(pA2,data,registro,&confirmar);
        if(confirmar==1){
            fseek(pA2,0,SEEK_SET);
            fseek(pA2,(registro-1)*sizeof(struct competidor),SEEK_SET);
            fread(&v,sizeof(struct competidor),1,pA2);
            v.activo=0;
            fseek(pA2,-sizeof(struct competidor),SEEK_CUR);
            fwrite(&v,sizeof(struct competidor),1,pA2);
            printf("\nBaja logica realizada.");
            printf("\nLa cantidad de registros es %d",totalReg);
        }
        fclose(pA2);
    }
    else{printf("\nError en la apertura del archivo.");}
}

//OPCION 9:ELIMINA INACTIVOS(MIGRA ELIMINADOR A OTRO ARCHIVO) NO FUNCIONA CORRECTAMENTE
void eliminaInactivos(FILE *pA2,FILE *pA3,char *data,char *eliminados){
    struct competidor v;
    pA2=fopen(data,"rb+");
    pA3=fopen(eliminados,"wb+");
    fseek(pA2,0,SEEK_SET);
    if(pA2!=NULL && pA3!=NULL){
        while(!feof(pA2)){
            fread(&v,sizeof(struct competidor),1,pA2);
            if(v.orden==0){
                fwrite(&v,sizeof(struct competidor),1,pA3);
                //remueveInactivos()
            }
        }
        fclose(pA2);
        fclose(pA3);
        printf("\nRegistros inactivos migrados correctamente.");
    }
    else{printf("\nError en la apertura del archivo.");}
    remueveInactivos(pA2,data);
}


