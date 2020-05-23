//Francisco García Mora 48780988N
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <limits>
#include <unistd.h>
#include <chrono>
#include <algorithm>


using namespace std;
using namespace chrono;

const string INFO_ARGUMENTOS_CORRECTOS = "$maxsum [-t] [--ignore_naive] -f file";
const long SENTINEL = -1.0;

struct opciones_t{
    bool iterativa;
    bool excluyeRecursivaSinAlmacen;
    bool nombreFichero;
}opcionesDeEntrada;

struct argumentos_t{
    opciones_t opcionesDeEntrada;
    bool argumentosOK = false;
    string fichero = "";
}argumentos;

struct valores_t{
    int valorN;
    unsigned valorT;
    bool error = false;
    vector<long> entradaN;
}valoresDeEntrada;

struct resultados_t{
    long backtracking;
    vector<unsigned> selection;
    long expanded_nodes;
    long discarded_nodes;
    long cpu_time;
}resultadosCalculados;

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

bool procesaPrimeraEntrada(string entrada, valores_t &devuelto){
    bool leeT = false;
    string valorNs = "";
    string valorTs = "";

    for(unsigned int i = 0; i < entrada.size(); i++){
        if(entrada[i] == ' '){
            leeT = true;
        }else if(leeT && (entrada[i] != ' ')){
            valorTs += entrada[i];
        }else if(!leeT && (entrada[i] != ' ')){
            valorNs += entrada[i];
        }
    }

    devuelto.valorN = stoi(valorNs);
    devuelto.valorT = stoi(valorTs);

    return true;
}

bool procesaRestoEntradas(string entrada, valores_t &devuelto){
    string lecturaS = "";

    if(entrada.size() > 1){
        for(unsigned int i = 0; i <= entrada.size(); i++){
            if(entrada[i] != ' ' && entrada[i] != '\0'){
                lecturaS += entrada[i];
            }else if(entrada[i] == ' ' || entrada[i] == '\0'){
                devuelto.entradaN.push_back(stoi(lecturaS));
                lecturaS = "";
            }
        }
        return true;
    }else{
        return true;
    }
}

bool procesaEntrada(string entrada, int linea, valores_t &devuelto){
    if(linea == 0){
        if(!procesaPrimeraEntrada(entrada, devuelto))
            return false;
    }else if(linea == 1){
        if(!procesaRestoEntradas(entrada, devuelto))
            return false;
    }
    return true;
}

valores_t procesaFichero(string fichero){
    valores_t devuelto;
    ifstream file(fichero);
    string entrada;
    int linea = 0;
    if(file.is_open()){
        while(getline(file, entrada) || linea == 1){
            if(procesaEntrada(entrada, linea, devuelto)){
                devuelto.error = false;
                linea++;
            }else{
                devuelto.error = true;
                return devuelto;
            }
        }
        file.close();
    }else{
        devuelto.error = true;
        mostrarErrorFichero(fichero);
    }
    return devuelto;
}

bool compruebaFichero(string fichero){
    ifstream file(fichero);
    if(file.is_open()){
        file.close();
        return true;
    }else
    return false;
}

argumentos_t processArguments(int argc, char *argv[], opciones_t opcionesDeEntrada){
    argumentos_t devuelto;
    bool argValidos = true;
    string fichero;
    string opcion;
    bool evaluaFichero = false;

    if(argc > 2){
        for(int i = 1; i < argc; i++){
            opcion = argv[i];
            if(evaluaFichero){
                fichero = argv[i];
                evaluaFichero = false;
                if(!compruebaFichero(fichero))
                    i = argc + 1;
            }else{
                if(opcion.compare("-t") == 0){
                    opcionesDeEntrada.iterativa = true;
                }else if(opcion.compare("--ignore_naive") == 0){
                    opcionesDeEntrada.excluyeRecursivaSinAlmacen = true;
                }else if(opcion.compare("-f") == 0){
                    if(i+1 < argc){
                        opcionesDeEntrada.nombreFichero = true;
                        evaluaFichero = true;
                    }else{
                        i = argc+1;
                        argValidos = false;
                        mostrarErrorNombreFicheroVacio();
                    }
                }else{
                    i = argc+1;
                    argValidos = false;
                    mostrarErrorArgumentos(opcion);
                }
            }
        }
        devuelto.opcionesDeEntrada = opcionesDeEntrada;
        devuelto.argumentosOK = argValidos;
        devuelto.fichero = fichero;
    }else{
        argValidos = false;
        mostrarArgumentosCorrectos();
    }
    return devuelto;
}

long voraz(const vector<long> &v, long W){
    vector<long> idx(v.size());
    for(unsigned long i = 0; i < idx.size(); i++){
        idx[i] = i;
    }
    sort(idx.begin(), idx.end(), [&v](long x, long y){
        return v[x] > v[y];
    });

    long acc_v = 0.0;
    for(auto i : idx){
        if(v[i] < W){
            acc_v += v[i];
            W -= v[i];
        }
    }

    return acc_v;
}


long add_rest( const vector<long> &v, size_t k){
	long r = 0.0;
	for(size_t i = k; i < v.size(); i++) {
        r+= v[i];
    }
	return r;
}

void calculaSolucion2(const vector<long> &v, long W, size_t k, vector<unsigned> &x, long acc_v, long &best_v){
	if(k == x.size()){
        resultadosCalculados.selection = x;
		best_v = max(best_v, acc_v);
		return;
	}

	for (unsigned j = 0; j < 2; j++){
		x[k] = j;
		long present_v = acc_v + x[k] * v[k];
		if(present_v <= W && present_v + add_rest(v, k+1)>=best_v){
            resultadosCalculados.expanded_nodes++;
			calculaSolucion2(v, W, k+1, x, present_v, best_v);
		}else{
            resultadosCalculados.discarded_nodes++;
        }
	}
}

long calculaSolucion1(const vector<long> &v, long W){
    vector<unsigned> x(v.size());
    long best_v = voraz(v, W);
    calculaSolucion2(v, W, 0, x, 0, best_v);
    return best_v;
}

resultados_t calcularResultados(opciones_t opciones, valores_t v){

    resultados_t devuelto;
    vector<unsigned> y(v.entradaN.size());
    long best_v = -1.0;
    
    auto start = clock(); 
    best_v = calculaSolucion1(v.entradaN, v.valorT);
    auto end = clock();
    auto tiempo = 1000.0 * (end-start) / CLOCKS_PER_SEC;

	devuelto.backtracking = best_v;
	devuelto.expanded_nodes = resultadosCalculados.expanded_nodes;
	devuelto.discarded_nodes = resultadosCalculados.discarded_nodes;
	devuelto.cpu_time = tiempo;

    for(unsigned long i = 0; i < resultadosCalculados.selection.size(); i++){
        if(resultadosCalculados.selection[i] == 1){
            devuelto.selection.push_back(v.entradaN[i]);
        }
    }

    return devuelto;
}

void mostrarResultados(resultados_t resultadosCalculados, opciones_t opcionesDeEntrada){

	cout << "Backtracking: " << resultadosCalculados.backtracking << endl;
	cout << "Selection: ";
	for(unsigned long i = 0; i < resultadosCalculados.selection.size(); i++){
		cout << " " << resultadosCalculados.selection[i];
	}
	cout << endl;
	cout << "Expanded nodes: " << resultadosCalculados.expanded_nodes << endl;
	cout << "Discarded nodes: " << resultadosCalculados.discarded_nodes << endl;
	cout << "CPU time(ms): " << resultadosCalculados.cpu_time << endl;
}


int main(int argc, char *argv[], char *envp[]){
    string fileName;

    argumentos = processArguments(argc, argv, opcionesDeEntrada);
    valoresDeEntrada = procesaFichero(argumentos.fichero);
    if(!valoresDeEntrada.error){
        resultadosCalculados = calcularResultados(argumentos.opcionesDeEntrada, valoresDeEntrada);
        mostrarResultados(resultadosCalculados, argumentos.opcionesDeEntrada);
        return 0;
    }else{
        return -1;
    }
}