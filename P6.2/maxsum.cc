//Francisco García Mora 48780988N
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <limits>

using namespace std;

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
    long naive;
    long memo;
    long itTable;
    long itVector;
    vector<long> selection;
    long selectionValue;
    vector<vector<long>> iterativeTable;
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

long solucionSinAlmacen(const vector<long> &v, int n, unsigned W){
    if(n == 0){
        return 0;
    }else{
        long S1 = solucionSinAlmacen(v, n-1, W);
        long S2 = numeric_limits<long>::lowest();
        if(v[n-1] <= W)
            S2 = v[n-1] + solucionSinAlmacen(v, n-1, W-v[n-1]);
        return max(S1, S2);
    }
}

long solucionConAlmacen(vector<vector<long>> &M, const vector<long> &v, int n, unsigned W){
    if(M[n][W] != SENTINEL)
        return M[n][W];
    if(n == 0)
        return M[n][W] = 0.0;

    long S1 = solucionConAlmacen(M, v, n-1, W);
    long S2 = numeric_limits<long>::lowest();

    if(v[n-1] <= W)
        S2 = v[n-1] + solucionConAlmacen(M, v, n-1, W - v[n-1]);

    M[n][W] = max(S1, S2);
    
    return M[n][W];
}

long solucionIterativaConAlmacen(vector<vector<long>> &M, const vector<long> &v, int lastn, unsigned lastW){
	vector<vector<long>> R(lastn+1, vector<long>(lastW+1));

	for(unsigned W = 0; W <= lastW; W++) R[0][W] = 0;

	for(int n = 1; n <= lastn; n++){
		for(unsigned W = 1; W <= lastW; W++){
			long S1 = R[n-1][W];
			long S2 = numeric_limits<long>::lowest();
			if(W >= v[n-1]){
				S2 = v[n-1] + R[n-1][W-v[n-1]];
			}
			R[n][W] = max(S1, S2);
		}
	}
	M = R;

	return R[lastn][lastW];
}

long solucionVector(const vector<long> &v, int lastn, unsigned lastW){
	vector<long> v0(lastW+1);
	vector<long> v1(lastW+1);

	for(unsigned W = 0; W <= lastW; W++){
		v0[W] = 0;
	}

	for(long n = 1; n <= lastn; n++){
		for(unsigned W = 1; W <= lastW; W++){
			long S1 = v0[W];
			long S2 = numeric_limits<long>::lowest();
			if(W >= v[n-1]){
				S2 = v[n-1] + v0[W-v[n-1]];
			}
			v1[W] = max(S1,S2);
		}
		swap(v0,v1);
	}
	return v0[lastW];
}

void sacaSelection(const vector<vector<long>> &M, const vector<long> &v, int n, unsigned W, vector<long> &sol){
	if(n == 0) return;

	long S1 = M[n-1][W];
	long S2 = numeric_limits<long>::lowest();
	if(W >= v[n-1])
		S2 = v[n-1] + M[n-1][W-v[n-1]];

	if(S1 >= S2){
		sacaSelection(M, v, n-1, W, sol);
	}else{
		sol.insert(sol.begin(),v[n-1]);
		sacaSelection(M, v, n-1, W - v[n-1], sol);
	}
}


resultados_t calcularResultados(opciones_t opciones, valores_t v){
	/*    long naive;
    long memo;
    long itTable;
    long itVector;
    vector<long> selection;
    long selectionValue;
    vector<vector<long>> iterativeTable;*/

    resultados_t devuelto;
   	if(v.valorN == 0 || v.valorT == 0 || v.entradaN.size() < 1){
        devuelto.naive = 0;
        devuelto.memo = 0;
        devuelto.itTable = 0;
        devuelto.itVector = 0;
        for(unsigned long i = 0; i < devuelto.selection.size(); i++)
        	devuelto.selection[i] = 0;
        devuelto.selectionValue = 0;

        vector<vector<long>> vacio;
        vector<long> lvacio;

        if(v.valorN != 0 && v.valorT == 0){
        	for(long i = 0; i <= v.valorN; i++){
				lvacio.push_back(0);
				vacio.push_back(lvacio);
				lvacio.pop_back();
        	}
        }else if(v.valorN == 0 && v.valorT != 0){
			for(unsigned long i = 0; i <= v.valorT; i++){
				lvacio.push_back(0);
        	}
        	vacio.push_back(lvacio);
        }else{
        	lvacio.push_back(0);
			vacio.push_back(lvacio);
        }
        devuelto.iterativeTable = vacio;

    }else{
        if(!opciones.excluyeRecursivaSinAlmacen){
            devuelto.naive = solucionSinAlmacen(v.entradaN, v.valorN, v.valorT);
        }
        vector<vector<long>> M(v.valorN+1, vector<long>(v.valorT+1, SENTINEL));
        devuelto.memo = solucionConAlmacen(M,v.entradaN, v.valorN, v.valorT);

		devuelto.itTable = solucionIterativaConAlmacen(M, v.entradaN, v.valorN, v.valorT);
        if(opciones.iterativa){	
        	devuelto.iterativeTable = M;
        }

        devuelto.itVector = solucionVector(v.entradaN, v.valorN, v.valorT);
        sacaSelection(M, v.entradaN, v.valorN, v.valorT, devuelto.selection);
        devuelto.selectionValue = 0;
        for(unsigned long i = 0; i < devuelto.selection.size(); i++){
        	devuelto.selectionValue += devuelto.selection[i];
        }
    }

    return devuelto;
}

void mostrarResultados(resultados_t resultadosCalculados, opciones_t opcionesDeEntrada){
    if(!opcionesDeEntrada.excluyeRecursivaSinAlmacen)
        cout << "Naive: " << resultadosCalculados.naive << endl;

    cout << "Memoization: " << resultadosCalculados.memo << endl;
	cout << "Iterative (table): " << resultadosCalculados.itTable << endl;   

    cout << "Iterative (vector): " << resultadosCalculados.itVector << endl;
    cout << "Selection: ";
    if(resultadosCalculados.selection.size() == 0)
    	cout << endl;
    else{
	    for(unsigned int i = 0; i < resultadosCalculados.selection.size(); i++){
	        if(i == resultadosCalculados.selection.size()-1)
	            cout << resultadosCalculados.selection[i] << " " << endl;
	        else
	            cout << resultadosCalculados.selection[i] << " ";
	    }
	}
    cout << "Selection value: " << resultadosCalculados.selectionValue << endl;

    if(opcionesDeEntrada.iterativa){
    	cout << "Iterative table:" << endl;
        for(unsigned long i = 0; i < resultadosCalculados.iterativeTable.size(); i++){
        	for(unsigned long j = 0; j < resultadosCalculados.iterativeTable[i].size(); j++)
        		cout << resultadosCalculados.iterativeTable[i][j] << " ";
        	if(i != resultadosCalculados.iterativeTable.size()-1)
        		cout << endl;
        }
        cout << endl;
    }
    
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