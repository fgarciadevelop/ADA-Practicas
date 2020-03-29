#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;

const string INFO_ARGUMENTOS_CORRECTOS = "$maxsum [-t] [--ignore_naive] -f file";

struct opciones_t{
    bool iterativa = false;
    bool excluyeRecursivaSinAlmacen = false;
    bool nombreFichero = false;
}opcionesDeEntrada;


//Gestión de errores.
void mostrarArgumentosCorrectos(){
    cout << "Usage: " << endl << INFO_ARGUMENTOS_CORRECTOS << endl;
}

void mostrarErrorArgumentos(string opcion){
    cout << "ERROR: unknown option " << opcion << "." << endl;
    mostrarArgumentosCorrectos();
}

void mostrarErrorFichero(string nombreFichero){
    cout << "ERROR: can't open file: " << nombreFichero << "." << endl;
    mostrarArgumentosCorrectos();
}

void mostrarErrorNombreFicheroVacio(){
    cout << "ERROR: expected file name." << endl;
    mostrarArgumentosCorrectos();
}

bool processArguments(int argc, char *argv[], opciones_t &opcionesDeEntrada){
    bool argValidos = true;
    int err = 0;
    string fichero;
    if(argc > 2){
        for(int i = 1; i < argc; i++){
            if(!strcmp(argv[i],"-t")){
                cout << "recibido -t" << endl;
            }else if(!strcmp(argv[i],"--ignore_naive")){
                cout << "recibido --naive" << endl;
            }else if(!strcmp(argv[i],"-f")){
                cout << "recibido fichero" << endl;
            }
        }
    }else{
        argValidos = false;
        mostrarArgumentosCorrectos();
    }
    return argValidos;
}



int main(int argc, char *argv[], char *envp[]){
    string fileName;
    string opcion1 = "-t";
    string opcion2 = "--ignore_naive";
    string opcion3 = "-f";

    bool prArg = processArguments(argc, argv, opcionesDeEntrada);
/*
    if(argc >= 3){
        for(int i = 1; i < argc; i++){
            if(opcion1.compare(argv[i]) == 0){
                opcionesDeEntrada.iterativa = true;
            }else if(opcion3.compare(argv[i]) == 0){
                opcionesDeEntrada.excluyeRecursivaSinAlmacen = true;
            }else if (opcion3.compare(argv[i]) == 0){
                if(i+1 <= argc){
                    opcionesDeEntrada.nombreFichero = true;
                    fileName = argv[i+1];
                    i = argc+1;
                }else{
                    i = argc + 1;
                    mostrarErrorNombreFicheroVacio();
                }
            }else{
                i = argc+1;
                mostrarErrorArgumentos(argv[i]);
            }
        }
    }else{
        mostrarArgumentosCorrectos();
    }*/
}