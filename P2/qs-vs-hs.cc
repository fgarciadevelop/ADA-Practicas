/*
ADA. 2019-20
Pràctica 2 de laboratori: Empirical analysis by means of program-steps account of two sorting algorithms: Middle-Quicksort and Heapsort.

Objectiu de la pràctica (a més dels especificats en la pràctica anterior):
- Conèixer els algorismes d'ordenació quicksort i heapsort
- Utilitzar el mètode del compte de passos de manera empírica per a obtenir la complexitat temporal d'un algorisme.
- Comprender la necesidad de generar un número (significativo desde el punto de vista estadístico) de instancias aleatorias para el cálculo de la complejidad temporal promedio cuando el algoritmo presenta caso mejor y peor.
- Entender que la complejidad temporal (y espacial) de un algoritmo puede depender del tipo de instancias del problema que se le presentan.
*/

#include <unistd.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

size_t pasos;

void rellenaVector(int* &v, size_t size){
    for(size_t j = 0; j < size; j++)
        v[j] = rand();
}

//--------------------------------------------------------------
// Middle Quick Sort
void 
middle_QuickSort(int * v, long left, long right){
long i,j;
int pivot; 
	if (left<right){
		i=left; j=right;
		pivot=v[(i+j)/2];
		do{
			while (v[i]<pivot) {
                i++;
                pasos++;
            }
			while (v[j]>pivot) {
                j--;
                pasos++;
            }
			if (i<=j){ 
                pasos++;
				swap(v[i], v[j]);
				i++; j--;
			}
		}while (i<=j);
		if (left < j)  middle_QuickSort(v,left,j);
		if (i < right) middle_QuickSort(v,i,right);
	}
}

//--------------------------------------------------------------                
//HEAPSORT
// Procedure sink used by Heapsort algorith
// Sink an element (indexed by i) in a tree to keep the heap property. n is the size of the heap. 

void sink(int *v, size_t n, size_t i)
{
size_t largest;
size_t l, r; //left and right child

do{

   	largest = i;  // Initialize largest as root
    l = 2*i + 1;  // left = 2*i + 1
    r = 2*i + 2;  // right = 2*i + 2
 
    // Is left child larger than root?
    if (l < n && v[l] > v[largest])
        largest = l;
 
    // Is right child larger than largest so far?
    if (r < n && v[r] > v[largest])
        largest = r;
 
    // If largest is still root then the process is done
    if (largest == i) break;
    
	// If not, swap new largest with current node i and repeat the process with childs. 
    swap(v[i], v[largest]);
 	i=largest;  
    pasos++;
   } while (true);
}
 
//--------------------------------------------------------------                
// Heapsort algorithm (ascending sorting)
void heapSort(int *v, size_t n)
{
    // Build a MAX-HEAP with the input array
    for (size_t i = n / 2 - 1; ; i--){
        sink(v, n, i);
        pasos++;
		if (i==0) break; //as size_t is unsigned type
	}	

	// A this point we have a HEAP on the input array, let's take advantage of this to sort the array:
    // One by one swap the first element, which is the largest (max-heap), with the last element of the vector and rebuild heap by sinking the new placed element on the begin of vector.  
    for (size_t i=n-1; i>0; i--)
    {
		// Move current root to the end.
        swap(v[0], v[i]);
 		// Now largest element is at the end but do not know if the element just moved to the beginning is well placed, so a sink process is required.
		// Note that, at the same time, the HEAP size is reduced one element by one.
        sink(v, i, 0);
        pasos++;
		// The procedure ends when HEAP has only one element, the lower of the input array
    }
}

int main(void){
    srand(getpid());
    size_t tiempoRandHeap = 0;
    size_t tiempoRandQuick = 0;
    size_t tiempoSortedHeap = 0;
    size_t tiempoSortedQuick = 0;
    size_t tiempoRevHeap = 0;
    size_t tiempoRevQuick = 0;

    cout 
    << "# QUICKSORT VERSUS HEAPSORT."
    << endl
    << "# Average processing Msteps (millions of program steps)"
    << endl
    << "# Number of samples (arrays of integer): 30"
    << endl << endl << endl
    << "========================================================================================================="
    << endl
    << "|\t\tRANDOM ARRAYS\t\t\tSORTED ARRAYS\t\t\tREVERSE SORTED ARRAYS   |"
    << endl
    << "|\t\t-------------\t\t\t-------------\t\t\t---------------------   |"
    << endl
    << "|\tSize\tQuickSort\tHeapSort\tQuickSort\tHeapSort\tQuickSort\tHeapSort|"
    << endl
    << "========================================================================================================="
    << endl;

    for (int exp = 15; exp <= 22; exp++){
        //tamaño vector 2^exp
        size_t size = size_t(pow(2,exp));

        size_t c = 0;
        //vector con tamaño "size"
        int* v = new int[size];
        int* v2 = new int[size];

        if(!v || !v2){
            cerr << "Error, no hay memoria suficiente!" << endl;
            exit (EXIT_FAILURE);
        }

        cout << size << "\t\t" << std::flush;

        for(int k = 0; k < 30; k++){
            //rellena el vector con números aleatorios.
            rellenaVector(v, size);
            v2 = v;

            pasos = 0;
            middle_QuickSort(v, 0, size-1);
            tiempoRandQuick += pasos;
            pasos = 0;

            heapSort(v2, size);
            tiempoRandHeap += pasos;
            pasos = 0;

            for (size_t i = 1; i < size; i++)
                if(v[i] < v[i-1] || v2[i] < v2[i-1]){
                    cerr << "Panic, array not sorted! " << i << endl;
                    exit(EXIT_FAILURE);
                }

            if(v != v2){
                cerr << "ERRRRRRRROR"<< endl;
                exit(EXIT_FAILURE);
            }


            middle_QuickSort(v, 0, size-1);
            tiempoSortedQuick += pasos;
            pasos = 0;

            heapSort(v2, size);
            tiempoSortedHeap += pasos;
            pasos = 0;

            c = size-1;
            for(size_t i = 0; i < size; i++){
                v2[i] = v[c];
                c--;
            }
            pasos = 0;
            middle_QuickSort(v2, 0, size-1);
            tiempoRevQuick += pasos;
            pasos = 0;

            c = size-1;
            for(size_t i = 0; i < size; i++){
                v2[i] = v[c];
                c--;
            }
            heapSort(v2, size);
            tiempoRevHeap += pasos;
            pasos = 0;
        }
        cout << setprecision(3) << fixed << (tiempoRandQuick/30)/1000000. << "\t\t" << (tiempoRandHeap/30)/1000000. << "\t\t"
        << (tiempoSortedQuick/30)/1000000. << "\t\t" << (tiempoSortedHeap/30)/1000000. << "\t\t"
        << (tiempoRevQuick/30)/1000000. << "\t\t" << (tiempoRevHeap/30)/1000000.
        << endl;

        tiempoRandQuick = 0;
        tiempoRandHeap = 0;
        tiempoSortedHeap = 0;
        tiempoSortedQuick = 0;
        tiempoRevQuick = 0;
        tiempoRevHeap = 0;
        delete[] v;
    }
}

