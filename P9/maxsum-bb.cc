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
#include <queue>
#include <tuple>


using namespace std;
using namespace chrono;

//************************
//* Definiciones previas *
//************************

const string INFO_ARGUMENTOS_CORRECTOS = "$maxsum [-t] [--ignore_naive] -f file";
const long SENTINEL = -1.0;

struct argumentos_t{
    bool argumentosOK = false;
    string fichero = "";
}argumentos;

struct valores_t{
    int valorN;
    unsigned valorT;
    bool error = false;
    vector<long> entradaN;
}valoresDeEntrada;

struct nodos_t{
	long expandidos;
	long added;
	long noFactibles;
	long noPrometedores;
	long prometedoresDescartados;
	long completados;
	long actualizacionNodoCompletado;
	long actualizacionNodoNoCompletado;
}nodosResultado;

struct resultados_t{
    long suma;
    nodos_t nodos;
    long tiempo;
}resultadosCalculados;

//**********************
//* Gestión de errores *
//**********************

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

//******************************************
//* Procesamiento de argumentos y entradas *
//******************************************

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

valores_t procesaFichero(){
    valores_t devuelto;
    ifstream file(argumentos.fichero);
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
        mostrarErrorFichero(argumentos.fichero);
    }
    return devuelto;
}

bool compruebaFichero(string fichero){
    ifstream file(fichero);
    if(file.is_open()){
        file.close();
        argumentos.fichero = fichero;
        return true;
    }else
    return false;
}

bool processArguments(int argc, char *argv[]){
    string fichero;
    string opcion;
    bool evaluaFichero = false;

    if(argc > 2){
        for(int i = 1; i < argc; i++){
            opcion = argv[i];
            if(evaluaFichero){
                fichero = argv[i];
                evaluaFichero = false;
                return compruebaFichero(fichero);
            }else{
                if(opcion.compare("-f") == 0){
                    if(i+1 < argc){
                        evaluaFichero = true;
                    }else{
                        mostrarErrorNombreFicheroVacio();
                        mostrarArgumentosCorrectos();
                        return false;
                    }
                }else{
                    mostrarErrorArgumentos(opcion);
                    mostrarArgumentosCorrectos();
                    return false;
                }
            }
        }
    }else{
        mostrarArgumentosCorrectos();
        return false;
    }
    return false;
}

//*************
//* Funciones *
//*************

long memoization(int n, int t, vector<vector<long>> &M, const vector<long> &v, long k){
    if(M[n][t] != SENTINEL)
        return M[n][t];
    if(n == k)
        return M[n][t] = 0.0;

    long S1 = memoization(n-1, t, M, v, k);
    long S2 = numeric_limits<long>::lowest();

    if(v[n-1] <= t)
        S2 = v[n-1] + memoization(n-1, t - v[n-1], M, v, k);

    M[n][t] = max(S1, S2);
    
    return M[n][t];
}

long add_rest( const vector<long> &v, size_t k){
	long r = 0.0;
	for(size_t i = k; i < v.size(); i++) {
        r+= v[i];
    }
	return r;
}

long voraz(const vector<long> &v, long t, long k){
    vector<long> idx(v.size());
    for(unsigned long i = 0; i < idx.size(); i++){
        idx[i] = i;
    }
    sort(idx.begin(), idx.end(), [&v](long x, long y){
        return v[x] > v[y];
    });

    long acc_v = 0.0;

    for(unsigned int i = k; i < idx.size(); i++){
    	int pos = idx[i];
    	if(v[pos] < t){
    		acc_v += v[pos];
    		t -= v[pos];
    	}
    }

    return acc_v;
}

bool is_better(long v1, long v2){
	return v1 > v2;
}

long funcion1(const vector<long> &v, long t, vector<vector<long>> &M){
	typedef vector<short> Sol;
	typedef tuple<long, long, Sol, long> Node; //Añade  opt_bound
	//typedef tuple<long, Sol, long> Node; //Añade  opt_bound
	priority_queue<Node> pq;

	long best_val = voraz(v, t, 0);
	//long opt_bound = add_rest(v, 0);
	long opt_bound = memoization(valoresDeEntrada.valorN, t, M, v, 0);

	pq.emplace(opt_bound, 0, Sol(v.size()), 0);
	//pq.emplace(0, Sol(v.size()), 0);
	while(!pq.empty()){
		

		long value; unsigned k; Sol x;
		tie(ignore, value, x, k) = pq.top();
		//tie(value, x, k) = pq.top();
		pq.pop();

//no fiarse de esto
		/*if(opt_bound > best_val){
			resultadosCalculados.nodos.prometedoresDescartados++;
		}
		cout << k << "   " << v.size() << endl;*/

		if(k == v.size()){
			resultadosCalculados.nodos.completados++;
			if(value >= best_val){
				best_val = value;
				resultadosCalculados.nodos.actualizacionNodoCompletado++;
			}
			continue;
		}
		resultadosCalculados.nodos.expandidos++;
		for(int j = 1; j >= 0; j--){
		//for(int j = 0; j < 2; j++){
			x[k] = j;

			long new_value = value + x[k] * v[k];

			if(new_value <= t){
				long pessimist = new_value + voraz(v, t - new_value, k+1);
				if(pessimist >= best_val){
					resultadosCalculados.nodos.actualizacionNodoNoCompletado++;
					best_val = pessimist;
				}
				//best_val = max(best_val, new_value + voraz(v, t - new_value, k+1));

				double opt_bound = new_value + memoization(valoresDeEntrada.valorN, t-new_value, M, v, k+1);
				if(opt_bound > best_val){
					resultadosCalculados.nodos.added++;
					cout << k << endl;
					pq.emplace(opt_bound, new_value, x, k+1);
					//pq.emplace(new_value, x, k+1);
				}else{
					resultadosCalculados.nodos.noPrometedores++;
				}
			}else{
				resultadosCalculados.nodos.noFactibles++;
			}
		}
	}

	return best_val;
}


//**************
//* Resultados *
//**************

void calcularResultados(){
	vector<vector<long>> M(valoresDeEntrada.valorN+1, vector<long>(valoresDeEntrada.valorT+1, SENTINEL));
    resultadosCalculados.suma = funcion1(valoresDeEntrada.entradaN, valoresDeEntrada.valorT, M);
}

void mostrarResultados(){
	auto start = clock();
    calcularResultados();
    auto end = clock();
    auto total = (end-start);
    auto tiempo = 1000.0 * total / CLOCKS_PER_SEC;
	
	cout << resultadosCalculados.suma << endl;
	cout << resultadosCalculados.nodos.expandidos << " ";
	cout << resultadosCalculados.nodos.added << " ";
	cout << resultadosCalculados.nodos.noFactibles << " ";
	cout << resultadosCalculados.nodos.noPrometedores << " ";
	cout << resultadosCalculados.nodos.prometedoresDescartados << " ";
	cout << resultadosCalculados.nodos.completados << " ";
	cout << resultadosCalculados.nodos.actualizacionNodoCompletado << " ";
	cout << resultadosCalculados.nodos.actualizacionNodoNoCompletado << endl;
	cout << tiempo << endl;
}

int main(int argc, char *argv[], char *envp[]){
    string fileName;

    argumentos.argumentosOK = processArguments(argc, argv);
    valoresDeEntrada = procesaFichero();
    if(argumentos.argumentosOK){
        mostrarResultados();
        return 0;
    }else{
        return -1;
    }
}