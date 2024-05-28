#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

//struct competidor. usado para imprimir y migrar mayormente
struct fecha{
    int dia, anio;
    char mes[4];
};
struct competidor{
    int orden,nroCorredor;
    struct fecha date;
    int edad;
    char pais[4];
    double tiempo;
    int activo;
};
//struct competidorAux. usao mayormente para veirificar variables en OPCION 5.ALTA
struct fecha2{
    char dia[3],mes[4],anio[5];
};
struct competidorAux{
    int orden;
    char nroCorredor[6];
    struct fecha2 date2;
    char edad[3],pais[4],tiempo[9];
    int activo;
};
//PROTOTIPO DE FUNCIONES
void emiteBinFile(FILE *pA2,char *data);
void verificaOpcion(char *opc,int nroOpcion); //SEGUN EL MODO(nroOpcion) CON EL QUE TRABAJE, CUBRE UNOS ERRORES U OTROS
void verificaReg(int totalReg, int *nroReg, FILE *pa2, char *data); //VERIFICA QUE EL REGISTRO INGRESADO NO SEA NEGATIVO Y SE ENCUENTRE DENTRO DE LOS DISPONIBLES
void verificaNroAlta(int *nroOrdenAlta); //VERIFICA EL NRO DE REGISTRO PARA DAR ALTA.
void confirma(FILE *pA2,char *data,int registro,int *continua); //CONFIRMA LA MODIFICACION DE UN REGISTRO
void modificaTiempo(FILE *pA2,char *data,int *registro); //MODIFICA EL TIEMPO
void modificaEdad(FILE *pA2,char *data,int *registro); //MODIFICA LA EDAD
void verificaCharNumerico(char *nroChar,int opcion); //VERIFICA QUE UN CARACTER SEA NUMERICO. RECIBE EL CHAR Y LA OPCION A OPERAR
void verificaMes(char *mes); //VERIFICA QUE EL MES INGRESADO SEA VALIDO.TRABAJA CON ARRAY DE MESES
void verificaDia(char *dia,char *mes); //VERIFICA QUE EL MES INGRESADO SEA VALIDO.RECIBE MES Y DIA. TRABAJA CON ARRAY DE MESES Y ARRAY CANTIDAD DE DIAS
void verificaAnio(char *anio,int anioMin); //VERIFICA QUE AÑO INGRESADO SEA MAYOR AL MENOR AÑO DEL REGISTRO(2015 EN ESTE CASO)
//


void verificaOpcion(char *opc,int nroOpcion){
    char charAux=toupper(*opc);
    if(nroOpcion==4){
        if(charAux=='A'||charAux=='C'||charAux=='B'||charAux=='D') {return;}
        else{
            //CUBRE ERROR CON RECURSIVIDAD
            fflush(stdin);
            printf("\nERROR 001:La opcion ingresada no es ninguna de las disponibles.");
            printf("\n  a)Emite todos los datos.");
            printf("\n  b)Emite solo activos.");
            printf("\n  c)Emite segun pais.");
            printf("\n  d)Emite segun rango de tiempo.");
            printf("\nPor favor ingrese una opcion valida: ");
            scanf("%c",opc);
            fflush(stdin);
            verificaOpcion(opc,nroOpcion);
        }
    }
    if(nroOpcion==6){
       if(charAux=='A'||charAux=='B') return;
        else{
            //CUBRE ERROR CON RECURSIVIDAD
            fflush(stdin);
            printf("\nERROR 002:La opcion ingresada no es ninguna de las disponibles.");
            printf("\n  a)Por Nro.Orden.");
            printf("\n  b)Por Nro.Corredor/a.");
            printf("\nPor favor ingrese una opcion valida :");
            scanf("%c",opc);
            fflush(stdin);
            verificaOpcion(opc,nroOpcion);
        }
    }
    if(nroOpcion==7){
       if(charAux=='A'||charAux=='B') return;
        else{
            //CUBRE ERROR CON RECURSIVIDAD
            fflush(stdin);
            printf("\nERROR 003:La opcion ingresada no es ninguna de las disponibles.");
            printf("\n  a)Modifica tiempo.");
            printf("\n  b)Modifica edad.");
            printf("\nPor favor ingrese una opcion valida :");
            scanf("%c",opc);
            fflush(stdin);
            verificaOpcion(opc,nroOpcion);
        }
    }
}

void verificaReg(int totalReg, int *nroReg, FILE *pa2, char *data){
    if(*nroReg<=0){
        //CUBRE ERROR CON RECURSIVIDAD
        fflush(stdin);
        printf("\nError 004:Numero negativo o cero ingresado.",*nroReg);
        printf("\nPor favor, ingrese un numero de orden mayor a 0(1,2,3...):");
        scanf("%d",nroReg);
        fflush(stdin);
        verificaReg(totalReg,nroReg,pa2,data);
    }
    if(*nroReg>totalReg){
        //CUBRE ERROR CON RECURSIVIDAD
        fflush(stdin);
        emiteBinFile(pa2,data);
        printf("\nError 005:El numero de registro ingresado(%d) no se encuentra dentro de los disponibles.",*nroReg);
        printf("\nPor favor, ingrese un numero de registro dentro de los disponibles:");
        scanf("%d",nroReg);
        fflush(stdin);
        verificaReg(totalReg,nroReg,pa2,data);
    }
    else {return;}
}

void verificaNroAlta(int *nroOrdenAlta){
    if(*nroOrdenAlta>0){return;}
    else {
        //CUBRE ERROR CON RECURSIVIDAD
        fflush(stdin);
        printf("\nError 006:Numero negativo ingresado.",*nroOrdenAlta);
        printf("\nPor favor, ingrese un numero de orden mayor a 0(1,2,3...):");
        scanf("%d",nroOrdenAlta);
        verificaNroAlta(nroOrdenAlta);
        fflush(stdin);
    }
}

void confirma(FILE *pA2,char *data,int registro,int *continua){
        struct competidor v;
        fseek(pA2,0,SEEK_SET);
        fseek(pA2,(registro-1)*sizeof(struct competidor),SEEK_SET);
        fread(&v,sizeof(struct competidor),1,pA2);
        printf("\nConfirmar la modificacion en el siguiente registro?\n");
        printf("                          FECHA                      \n");
        printf("Orden\tCorredor\tDia-Mes-Anio\tEdad\tPais\tTiempo\t\tActivo\n");
        printf("%d\t%d\t\t%d-%3s-%d\t%d",v.orden,v.nroCorredor,v.date.dia,v.date.mes,v.date.anio,v.edad);
        printf("\t%3s\t%lf\t%d\n",v.pais,v.tiempo,v.activo);
        printf("\n1)Confirmar.");
        printf("\nPresione cualquier tecla para cancelar.");
        printf("\nIngrese:");
        scanf("%d",continua);
        fflush(stdin);
}

void modificaTiempo(FILE *pA2,char *data,int *registro){
        struct competidor v;
        fseek(pA2,0,SEEK_SET);
        fseek(pA2,(*registro-1)*sizeof(struct competidor),SEEK_SET);
        fread(&v,sizeof(struct competidor),1,pA2);
        printf("\nIngrese tiempo:");
        scanf("%lf",&v.tiempo);
        fflush(stdin);
        fseek(pA2,-sizeof(struct competidor),SEEK_CUR);
        fwrite(&v,sizeof(struct competidor),1,pA2);
        printf("\nTiempo modificado.");
        fflush(stdin);
}

void modificaEdad(FILE *pA2,char *data,int *registro){
        struct competidor v;
        fseek(pA2,0,SEEK_SET);
        fseek(pA2,(*registro-1)*sizeof(struct competidor),SEEK_SET);
        fread(&v,sizeof(struct competidor),1,pA2);
        printf("\nIngrese edad:");
        scanf("%d",&v.edad);
        fflush(stdin);
        fseek(pA2,-sizeof(struct competidor),SEEK_CUR);
        fwrite(&v,sizeof(struct competidor),1,pA2);
        printf("\nEdad modificada.");
        fflush(stdin);
}

void verificaCharNumerico(char *nroChar,int opcion){
    //CUBRE ERRORRES CON RECURSIVIDAD, CADA OPCION ES PARA LEER UN LARGO ESPECIFICO DE CARACTERES
    int i=0,j=0;
    for(i=0;i<strlen(nroChar);i++){
        if(opcion==5){
            if(isdigit(nroChar[i])==0){
                //VERIFICA QUE NO SE INGRESA CARACTER EN EL NRO DE ID
                fflush(stdin);
                printf("\nError 007: Ingreso un caracter, ingrese SOLO NUMEROS por favor.");
                printf("\nIngrese numero de ID:");
                scanf("%5s",nroChar);
                fflush(stdin);
                verificaCharNumerico(nroChar,5);
            }
        }
        if(opcion==2){
            if(isdigit(nroChar[i])==0){
                //VERIFICA QUE NO SE INGRESA CARACTER EN LA EDAD
                fflush(stdin);
                printf("\nError008: Ingreso un caracter, ingrese SOLO NUMEROS por favor.");
                printf("\nIngrese edad:");
                scanf("%2s",nroChar);
                fflush(stdin);
                verificaCharNumerico(nroChar,2);
            }
        }
        if(opcion==3){
            if(isdigit(nroChar[i])==1){
                //VERIFICA QUE NO SE INGRESA NUMERO EN EL PAIS
                fflush(stdin);
                printf("\nError009: Ingreso un numero, ingrese SOLO CARACTER por favor.");
                printf("\nIngrese pais:");
                scanf("%3s",nroChar);
                fflush(stdin);
                verificaCharNumerico(nroChar,3);
            }
        }
        if(opcion==9){
            if(isdigit(nroChar[i])==0){
                //VERIFICA QUE NO SE INGRESA CARACTER EN EL TIEMPO
                fflush(stdin);
                printf("\nError010: Ingreso un caracter, ingrese SOLO NUMEROS por favor.");
                printf("\nIngrese tiempo:");
                scanf("%9s",nroChar);
                fflush(stdin);
                verificaCharNumerico(nroChar,9);
            }
        }
    }
}

void verificaMes(char *mes){
    char *meses[]={"ene","feb","mar","abr","may","jun","jul","ago","sep","oct","nov","dic"};
    int i,tam = sizeof(meses)/sizeof(char *);
    int repite=0;
    for( i=0 ; mes[i]!='\0' ; i++ ){
        mes[i]=tolower(mes[i]);
    }
    //SI HAY COINCIDENCIA VUELVE
    for(i=0;i<tam;i++){
        if(strcmp(mes,meses[i])==0){return;}
        else{repite=1;}
    //CASO CONTRARIO ACTIVA FLAG REPITE Y VUELVE A SOLICITAR EL MES, HASTA QUE SE INGRESE UNO VALIDO
    }
    if(repite==1){
        fflush(stdin);
        printf("\nError 011:El mes ingresado no es ninguno de los 12 meses.");
        printf("\nMes ingresado:%3s",mes);
        printf("\nPor favor, ingrese un mes valido:");
        scanf("%3s",mes);
        fflush(stdin);
        verificaMes(mes);
    }
}

void verificaDia(char *dia,char *mes){
    char *meses[]={"ene","feb","mar","abr","may","jun","jul","ago","sep","oct","nov","dic"};
    int  cantDias[]={31,28,31,30,31,30,31,31,30,31,30,31};
    int i,tam = sizeof(meses)/sizeof(char *);
    int tam2 = sizeof(meses)/sizeof(int *);
    int diaNum = (int) strtol(dia, NULL, 10);
    int diaMax;
    int diaInvalido;
    //RECORRE PARA SABER LA CANTIDAD MAXIMA DE DIAS SEGUN EL MES INGRESADO
    for(i=0;i<tam;i++){
        if(strcmp(mes,meses[i])==0){diaMax=cantDias[i];}
    }
    //VERIFICA QUE LA CANTIDAD DE DIAS NO SUPERA AL MAXIMO SEGUN CADA MES
    if(diaNum>diaMax){
        do{
            fflush(stdin);
            printf("\nError 012:Para el mes ingresado puede elegir de 1 a %d dias",diaMax);
            printf("\nMes ingresado:%3s",mes);
            printf("\nNumero de dia ingresado:%2s",dia);
            printf("\nPor favor, ingrese un dia valido:");
            scanf("%d",&diaNum);
            if(diaNum>0 && diaNum<=diaMax){diaInvalido=0;}
            else{diaInvalido=1;}
            fflush(stdin);
        }while(diaInvalido==1);
        fflush(stdin);
    }
    sprintf(dia,"%d",diaNum);
}

void verificaAnio(char *anio,int anioMin){
    //RECIBE EL AÑO MINIMO DEL ARCHIVO BINARIO
    //SOLICITA UN VALOR MAYOR AL MINIMO SIEMPRE
    int anioNum = (int) strtol(anio, NULL, 10);
    int anioValido=1;
    do{
        if(anioNum<anioMin){
            //CUBRE ERROR
            printf("\nError 013:Anio ingresado es menor al valor minimo permitido(%d)",anioMin);
            printf("\nPor favor, ingrese un valor mayor a (%d):",anioMin);
            scanf("%d",&anioNum);
            fflush(stdin);
        }
        else{anioValido=0;}
    }while(anioValido==1);
    sprintf(anio,"%d",anioNum);
}

