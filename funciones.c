#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "funcionesAux.c"

//PROTOTIPO DE FUNCIONES
void emiteBinFile(FILE *pA2,char *data);
//

//AQUI SE ENCUENTRAN LAS FUNCIONES CON LAS QUE TRABAJA CADA OPCION DEL MENU
//EN ALGUNOS CASOS EL MENU UTILIZA FUNCIONES DE "funcionesAux.c"

//FUNCIONES OPCION 4. emiteBinFile tambien se usa en otras funciones
void emiteBinFile(FILE *pA2,char *data){
    struct competidor v;
    pA2=fopen(data,"rb");
    if(pA2!=NULL){
        fseek(pA2,0,SEEK_SET);
        printf("                          FECHA                      \n");
        printf("Orden\tCorredor\tDia-Mes-Anio\tEdad\tPais\tTiempo\t\tActivo\n");
        while(!feof(pA2)){
           fread(&v,sizeof(struct competidor),1,pA2);
           printf("(%d)\t%d\t\t%d-%3s-%d\t%d",v.orden,v.nroCorredor,v.date.dia,v.date.mes,v.date.anio,v.edad);
           printf("\t%3s\t%lf\t%d\n",v.pais,v.tiempo,v.activo);
        }
        fclose(pA2);
    }
    else{printf("\nError en la apertura del archivo.");}
}
void emiteActivos(FILE *pA2,char *data){
    struct competidor v;
    pA2=fopen(data,"rb");
    if(pA2!=NULL){
        fseek(pA2,0,SEEK_SET);
        printf("                          FECHA                      \n");
        printf("Orden\tCorredor\tDia-Mes-Anio\tEdad\tPais\tTiempo\t\tActivo\n");
        while(!feof(pA2)){
           fread(&v,sizeof(struct competidor),1,pA2);
           if(v.activo==1){
            printf("%d\t%d\t\t%d-%3s-%d\t%d",v.orden,v.nroCorredor,v.date.dia,v.date.mes,v.date.anio,v.edad);
            printf("\t%3s\t%lf\t%d\n",v.pais,v.tiempo,v.activo);
            }
        }
        fclose(pA2);
    }
    else{printf("\nError en la apertura del archivo.");}
}
void emitePais(FILE *pA2,char *data){
    //INGRESA PAIS POR TECLADO, LO CONVIERTE A MAYUS.
    //RECORRE Y COMPARA TODAS LAS CADENAS PAIS DEL ARCHIVO. SI ENCUENTRA COINCIDENCIA, EMITE DATOS
    char charAux[]="000";
    int i,noEncontro=0;
    struct competidor v;
    pA2=fopen(data,"rb");
    if(pA2!=NULL){
        fflush(stdin);
        printf("\nIngrese el pais que desea emitir:");
        scanf("%3s",&charAux);
        //CONVIERTE LA CADENA INGRESADA A MAYUS
        for( i=0 ; charAux[i]!='\0' ; i++ ){
            charAux[i]=toupper(charAux[i]);
        }
        fseek(pA2,0,SEEK_SET);
        while(!feof(pA2)){
           fread(&v,sizeof(struct competidor),1,pA2);
            if(strcmp(charAux,v.pais)==0){
                printf("                          FECHA                      \n");
                printf("Orden\tCorredor\tDia-Mes-Anio\tEdad\tPais\tTiempo\t\tActivo\n");
                printf("%d\t%d\t\t%d-%3s-%d\t%d",v.orden,v.nroCorredor,v.date.dia,v.date.mes,v.date.anio,v.edad);
                printf("\t%3s\t%lf\t%d\n",v.pais,v.tiempo,v.activo);
                noEncontro=1;
            }
        }
        if(noEncontro==0) printf("\nError 014: No se encontro ningun registro con nombre(%s)\n",charAux);
        fclose(pA2);
    }
    else{printf("\nError en la apertura del archivo.");}
}
void emiteTimeRange(FILE *pA2,char *data){
    //A PARTIR DE VALORES MAX Y MIN INGRESADO POR TECLADO, BUSCA LOS TIEMPOS DENTRO DEL RANGO
    double timeMax=-1,timeMin=-1;
    int i,noEncontro=0;
    struct competidor v;
    pA2=fopen(data,"rb");
    if(pA2!=NULL){
        fflush(stdin);
        printf("\nIngrese el rango de tiempo.{-X,+Y}\n");
        printf("Minimo:");
        scanf("%lf",&timeMin);
        printf("Maximo:");
        scanf("%lf",&timeMax);
        fseek(pA2,0,SEEK_SET);
        while(!feof(pA2)){
           fread(&v,sizeof(struct competidor),1,pA2);
            if(v.tiempo>=timeMin && v.tiempo<=timeMax){
                printf("                          FECHA                      \n");
                printf("Orden\tCorredor\tDia-Mes-Anio\tEdad\tPais\tTiempo\t\tActivo\n");
                printf("%d\t%d\t\t%d-%3s-%d\t%d",v.orden,v.nroCorredor,v.date.dia,v.date.mes,v.date.anio,v.edad);
                printf("\t%3s\t%lf\t%d\n",v.pais,v.tiempo,v.activo);
                noEncontro=1;
            }
        }
        if(noEncontro==0) printf("\nError 015: No se encontro ningun registro con entre los rangos {%lf;%lf}\n",timeMin, timeMax);
        fclose(pA2);
    }
    else{printf("\nError en la apertura del archivo.");}
}


//FUNCIONES OPCION 5:ALTA COMPETIDORES
void creaRegsNulos(int nroOrdenAlta,FILE *pA2,char *data){
    //SI SE INGRESA UN NRO MAYOR A LA CANT DE REGISTROS, CREA NUEVOS REGISTROS VACIOS, HASTA EL NRO DE ORDEN INGRESADO
    int cantRegsNulos=0,i=0;
    int totalReg;
    struct competidor vNulo={0,0,{0,0,"000"},0,"000",0,0};
    char auxNulo[3]="000";
    pA2=fopen(data,"ab+");
    if(pA2!=NULL){
        fflush(stdin);
        fseek(pA2,0,SEEK_END);
        totalReg=ftell(pA2)/sizeof(struct competidor);
        cantRegsNulos=cantRegsNulos+(nroOrdenAlta-totalReg);
        if(nroOrdenAlta>totalReg){
            for(i=0;i<cantRegsNulos-1;i++){fwrite(&vNulo,sizeof(struct competidor),1,pA2);}
        }
        else{return;}
        fclose(pA2);
        }
    else{printf("\nError en la apertura del archivo.");}
}
void chequeaIDValido(char *nroIDChar,FILE *pA2,char *data){
    //CHEQUEA QUE EL NRO DE CORREDOR INGRESADO ESTE DISPONIBLE
    struct competidor v;
    int nroIDNum,nroOcupado;
    int encontro=0;
    verificaCharNumerico(nroIDChar,1);
    nroIDNum = (int) strtol(nroIDChar,NULL, 10);
    pA2=fopen(data,"rb+");
    if(pA2!=NULL){
        fflush(stdin);
        fseek(pA2,0,SEEK_SET);
        while(!feof(pA2)){
            fread(&v,sizeof(struct competidor),1,pA2);
            if(nroIDNum==v.nroCorredor){
                encontro=1;
                nroOcupado=v.nroCorredor;
            }
        }
        do{
            if(encontro==1){
                //SI ENCUENTRO==1, HUBO COINCIDENCIA
                //SI HUBO COINCIDENCIA, ESTA OCUPADO. PREVIENE LA FALLA 1 VEZ CON NUMEROS DIFERENTES
                fflush(stdin);
                printf("\nError 016:EL numero de ID %d esta ocupado",nroIDNum);
                printf("\nPor favor, ingrese otro numero:");
                scanf("%5s",nroIDChar);
                nroIDNum = (int) strtol(nroIDChar,NULL, 10);
                if(nroIDNum==nroOcupado){encontro==1;}
                else{encontro=0;}
                fflush(stdin);
            }
            else{encontro=0;}
        }while(encontro==1);
        fclose(pA2);
    }
    else{printf("\nError en la apertura del archivo.");}
}
void verificaOcupado(int *nroOrdenAlta,FILE *pA2,char *data){
    struct competidor v;
    pA2=fopen(data,"rb+");
    if(pA2!=NULL){
        fflush(stdin);
        fseek(pA2,0,SEEK_SET);
        fseek(pA2,(*nroOrdenAlta-1)*sizeof(struct competidor),SEEK_SET);
        fread(&v,sizeof(struct competidor),1,pA2);
        if(v.orden==0){return;}
        else{
            printf("\nError XXX:Numero de registro ya ocupado.");
            printf("Por favor, ingrese un numero de registro vacio:");
            scanf("%d",nroOrdenAlta);
            verificaNroAlta(nroOrdenAlta);
            verificaOcupado(nroOrdenAlta,pA2,data);
        }
        fclose(pA2);
    }
    else{printf("\nError en la apertura del archivo.");}
}
void ingresaVerificaFecha(char *dia,char *mes,char *anio,FILE *pA2, char *data){
    //RECIBE LAS FECHAS EN FORMATO CARACTER, PIDE INGRESO POR TECLADO, Y VERIFICA CHAR CON SUS CONDICIONES
    struct competidor v;
    int anioMin=3000;
    pA2=fopen(data,"rb+");
    //RECORRO EL ARCHIVO PARA OBTENER EL AÑO MINIMO
    if(pA2!=NULL){
        fflush(stdin);
        fseek(pA2,0,SEEK_SET);
        fread(&v,sizeof(struct competidor),1,pA2);
        while(!feof(pA2)){
            fread(&v,sizeof(struct competidor),1,pA2);
            if(v.date.anio<anioMin && v.date.anio!=0){anioMin=v.date.anio;}
        }
        fclose(pA2);
    }
    else{printf("\nError en la apertura del archivo.");}
    fflush(stdin);
    //DE ACA HACIA ABAJO INGRESO Y VERIFICO DIA, MES AÑO
    printf("\nIngrese mes:");
    scanf("%3s",mes);
    verificaMes(mes);
    fflush(stdin);
    printf("\nIngrese dia:");
    scanf("%2s",dia);
    fflush(stdin);
    verificaDia(dia,mes);
    printf("\nIngrese anio:");
    scanf("%4s",anio);
    verificaAnio(anio,anioMin);
}
void ingresaVerificaEdad(char *edad){
    int edadNum ,edadMin=16,edadMax=99;
    int edadValida=1;
    printf("\nIngrese edad:");
    scanf("%2s",edad);
    verificaCharNumerico(edad,2);
    edadNum = (int) strtol(edad, NULL, 10);
    do{
        if(edadNum<edadMin || edadNum>edadMax){
            printf("\nError 017:Edad ingresada fuera de rango");
            printf("\nPor favor, ingrese un valor entre 16 y 99:");
            scanf("%d",&edadNum);
            fflush(stdin);
        }
        else{edadValida=0;}
    }while(edadValida==1);
    sprintf(edad,"%d",edadNum);
}
void ingresaVerificaTiempo(char *tiempoChar){
    double tiempoNum;
    int tiempoValido=1;
    printf("\nIngrese tiempo:");
    scanf("%lf",&tiempoNum);
    do{
        if(tiempoNum<=0){
            printf("\nError 018:Tiempo ingresado fuera de rango.");
            printf("\nPor favor, ingrese valores mayores a 0:");
            scanf("%lf",&tiempoNum);
            fflush(stdin);
        }
        else{tiempoValido=0;}
    }while(tiempoValido==1);
    sprintf(tiempoChar,"%lf",tiempoNum);
}
void cargaDatosAlta(FILE *pA2,char *data,struct competidorAux v,int nroOrdenAlta){
    struct competidor vAlta;
    int totalReg,confirmar;
    pA2=fopen(data,"rb+");
    if(pA2!=NULL){
        vAlta.orden=nroOrdenAlta;
        vAlta.nroCorredor = (int) strtol(v.nroCorredor, NULL, 10);
        vAlta.date.dia = (int) strtol(v.date2.dia, NULL, 10);
        strcpy(vAlta.date.mes,v.date2.mes);
        vAlta.date.anio = (int) strtol(v.date2.anio, NULL, 10);
        vAlta.edad =(int) strtol(v.edad, NULL, 10);
        strcpy(vAlta.pais,v.pais);
        vAlta.tiempo = (double) strtol(v.tiempo, NULL, 10);
        vAlta.activo=1;
        printf("\nDesea dar de alta el siguiente registro?\n");
        printf("Orden\tCorredor\tDia-Mes-Anio\tEdad\tPais\tTiempo\t\tActivo\n");
        printf("(%d)\t%d\t\t%d-%3s-%d\t%d",vAlta.orden,vAlta.nroCorredor,vAlta.date.dia,vAlta.date.mes,vAlta.date.anio,vAlta.edad);
        printf("\t%3s\t%lf\t%d\n",vAlta.pais,vAlta.tiempo,vAlta.activo);
        printf("\n1)Confirmar.");
        printf("\nPresione cualquier tecla para cancelar.");
        printf("\nNro registro actual:%d",vAlta.orden);
        printf("\nIngrese:");
        scanf("%d",&confirmar);
        if(confirmar==1){
            fseek(pA2,0,SEEK_SET);
            fseek(pA2,(nroOrdenAlta-1)*sizeof(struct competidor),SEEK_SET);
            fwrite(&vAlta,sizeof(struct competidor),1,pA2);
            printf("\nAlta realizada.\n");
        }
        fclose(pA2);
    }
    else{printf("\nError en la apertura del archivo.");}
}


//FUNCIONES OPCION 6: BUSCA SEGUN OPCION
void emiteCorredorPorOrden(FILE *pA2, char *data){
    //INGRESA EL NRO DE ORDEN Y SE POSICIONA DIRECTAMENTE SOBRE EL REGISTRO CON FSEEK
    //LUEGO EMITE DONDE SE POSICIONE EL PUNTERO
    struct competidor v;
    int nroOrden,totalReg;
    pA2=fopen(data,"rb");
    if(pA2!=NULL){
        fflush(stdin);
        fseek(pA2,0,SEEK_END);
        totalReg=ftell(pA2)/sizeof(struct competidor);
        printf("\nIngrese el numero de orden del corredor:\n");
        scanf("%d",&nroOrden);
        verificaReg(totalReg,&nroOrden,pA2,data);
        fseek(pA2,0,SEEK_SET);
        fseek(pA2,(nroOrden-1)*sizeof(struct competidor),SEEK_SET);
        fread(&v,sizeof(struct competidor),1,pA2);
        printf("                          FECHA                      \n");
        printf("Orden\tCorredor\tDia-Mes-Anio\tEdad\tPais\tTiempo\t\tActivo\n");
        printf("%d\t%d\t\t%d-%3s-%d\t%d",v.orden,v.nroCorredor,v.date.dia,v.date.mes,v.date.anio,v.edad);
        printf("\t%3s\t%lf\t%d\n",v.pais,v.tiempo,v.activo);
        fclose(pA2);
    }
    else{printf("\nError en la apertura del archivo.");}
}
void emiteCorredorPorID(FILE *pA2, char *data){
    //RECORRE TODOS LOS NRO DE CORREDOR HASTA EL FIN DE ARCHIVO
    //SI NO ENCUENTRA, ACTIVA FLAG Y EMITE ERROR
    int nroID,noEncontro=0;
    struct competidor v;
    pA2=fopen(data,"rb");
    if(pA2!=NULL){
        fflush(stdin);
        printf("\nIngrese el numero ID:\n");
        scanf("%d",&nroID);
        fseek(pA2,0,SEEK_SET);
        while(!feof(pA2)){
           fread(&v,sizeof(struct competidor),1,pA2);
            if(nroID==v.nroCorredor){
                printf("                          FECHA                      \n");
                printf("Orden\tCorredor\tDia-Mes-Anio\tEdad\tPais\tTiempo\t\tActivo\n");
                printf("%d\t%d\t\t%d-%3s-%d\t%d",v.orden,v.nroCorredor,v.date.dia,v.date.mes,v.date.anio,v.edad);
                printf("\t%3s\t%lf\t%d\n",v.pais,v.tiempo,v.activo);
                noEncontro=1;
            }
        }
        if(noEncontro==0) printf("\nError 019: No se encontro ningun registro con numero de ID (%d)\n",nroID);
        fclose(pA2);
    }
    else{printf("\nError en la apertura del archivo.");}
}


//FUNCION OPCION 7:MODIFICA SEGUN OPCION
void modificaPorOrden(FILE *pA2, char *data){
    //EL FLAG nroOpcion=7 ES UNICAMENTE PARA LA FUNCION VERIFICAR, QUE RECIBE FLAG Y TRABAJA SEGUN VALOR QUE RECIBA
    struct competidor v;
    int totalReg,registro;              //variables
    int continua=0,nroOpcion=7;        //flags
    char opcion;
    pA2=fopen(data,"rb+");
    if(pA2!=NULL){
        fflush(stdin);
        fseek(pA2,0,SEEK_END);
        totalReg=ftell(pA2)/sizeof(struct competidor);
        printf("\nIngrese el numero de orden para modificar:");
        scanf("%d",&registro);
        fflush(stdin);
        verificaReg(totalReg,&registro,pA2,data);
        confirma(pA2,data,registro,&continua);
        if(continua==1){
            printf("\nDesea modificar tiempo o edad?");
            printf("\na)Tiempo.");
            printf("\nb)Edad.");
            printf("\nIngrese:");
            scanf("%c",&opcion);
            fflush(stdin);
            verificaOpcion(&opcion,nroOpcion);
            if(toupper(opcion)=='A'){modificaTiempo(pA2,data,&registro);}
            if(toupper(opcion)=='B'){modificaEdad(pA2,data,&registro);}
        }
    fclose(pA2);
    }
    else{printf("\nError en la apertura del archivo.");}
}
void modificaPorID(FILE *pA2, char *data){
    //EL FLAG nroOpcion=7 ES UNICAMENTE PARA LA FUNCION VERIFICAR, QUE RECIBE FLAG Y TRABAJA SEGUN VALOR QUE RECIBA
    //FLASG CONTINUA UNICAMENTE PARA CONFIRMAR LA MODIFICACION(trabaja con la funcion confirma()
    struct competidor v;
    int nroID,totalReg;//variables
    int registro=0;//contadores
    int continua,nroOpcion=7,recorre=1;//flags
    char opcion;
    pA2=fopen(data,"rb+");
    if(pA2!=NULL){
        fseek(pA2,0,SEEK_END);
        totalReg=ftell(pA2)/sizeof(struct competidor);
        printf("\nIngrese el numero de ID para modificar:");
        scanf("%d",&nroID);
        fflush(stdin);
        while(recorre==1 && registro<totalReg){
            fseek(pA2,0,SEEK_SET);
            fseek(pA2,(registro-1)*sizeof(struct competidor),SEEK_SET);
            fread(&v,sizeof(struct competidor),1,pA2);
            if(nroID==v.nroCorredor){
                confirma(pA2,data,registro,&continua);
                if(continua==1){
                    printf("\nDesea modificar tiempo o edad?");
                    printf("\na)Tiempo.");
                    printf("\nb)Edad.");
                    printf("\nIngrese:");
                    scanf("%c",&opcion);
                    fflush(stdin);
                    verificaOpcion(&opcion,nroOpcion);
                    verificaOpcion(&opcion,nroOpcion);
                    if(toupper(opcion)=='A'){modificaTiempo(pA2,data,&registro);}
                    if(toupper(opcion)=='B'){modificaEdad(pA2,data,&registro);}
                }
                recorre=0;
            }
            fflush(stdin);
            registro++;
        }
        if(recorre==1){printf("\nError: No existen ningun registro con el numero de ID (%d)",nroID);}
    fclose(pA2);
    }
    else {printf("\nError en la apertura del archivo.");}

}


//FUNCION OPCION 9:
//ESTA FUNCION PRESENTA ERRORES. NO SE COPIA NI SE ELIMINAN LOS INACTIVOS
void remueveInactivos(FILE *pA2, char *data){
    struct competidor vNulo={0,0,{0,0,"000"},0,"000",0,0};
    struct competidor v;
    pA2=fopen(data,"rb+");
    if(pA2!=NULL){
        fseek(pA2,0,SEEK_SET);
        while(!feof(pA2)){
            fread(&v,sizeof(struct competidor),1,pA2);
            if(v.orden==0){
                fwrite(&vNulo,sizeof(struct competidor),1,pA2);
            }
        }
        fclose(pA2);
    }
    else{printf("\nError en la apertura del archivo.");}
}
