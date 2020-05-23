//Francisco García Mora 48780988N

#include <unistd.h>
#include <iostream>
#include <math.h>
#include <chrono>

using namespace std;

void rellenaVector(int* &v, size_t size){
    for(size_t j = 0; j < size; j++)
        v[j] = rand();
}

//--------------------------------------------------------------
// Middle QuickSort
void 
middle_QuickSort(int * v, long left, long right){

    long i,j;
    int pivot,aux; 
    if (left<right)
    {
        i=left; j=right;
        pivot=v[(i+j)/2];
        do
        {
            while (v[i]<pivot) i++;
            while (v[j]>pivot) j--;
            if (i<=j)
            {
                aux=v[i]; v[i]=v[j]; v[j]=aux;
                i++; j--;
            }
       } while (i<=j);
       if (left<j)  middle_QuickSort(v,left,j);
       if (i<right) middle_QuickSort(v,i,right);
    }
}

int main(void){
    srand(0);
    int tiempo = 0;

    cout << "# Quicksort CPU times in milliseconds:"
    << endl
    << "#Size \t CPU time (ms.)"
    << endl
    << "# ----------------------------"
    << endl;

    for (int exp = 15; exp <= 22; exp++){
        //tamaño vector 2^exp
        size_t size = size_t(pow(2,exp));
        //vector con tamaño "size"
        int* v = new int[size];

        if(!v){
            cerr << "Error, no hay memoria suficiente!" << endl;
            exit (EXIT_FAILURE);
        }

        cout << size << "\t\t" << std::flush;

        for(int k = 0; k < 30; k++){
            //rellena el vector con números aleatorios.
            rellenaVector(v, size);

            auto start = high_resolution_clock::now();
            middle_QuickSort(v, 0, size-1);
            auto end = high_resolution_clock::now();
            tiempo += duration_cast<milliseconds>(end-start).count();

/*
            auto start = clock();
            middle_QuickSort(v, 0, size-1);
            auto end = clock();
            tiempo += end-start;*/

            for (size_t i = 1; i < size; i++)
                if(v[i] < v[i-1]){
                    cerr << "Panic, array not sorted! " << i << endl;
                    exit(EXIT_FAILURE);
                }
        }
        cout << tiempo/30. << endl;
        tiempo = 0;
        delete[] v;
    }
}