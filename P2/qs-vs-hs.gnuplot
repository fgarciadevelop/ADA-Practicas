#Francisco García Mora 48780988N

set terminal png

set xtics ("2¹⁵" 32768, "2²⁰" 1048576, "2²¹" 2097152, "2²²" 4194304)
  
set ylabel "Millions of program steps"
set xlabel "Size of arrays"

set yrange [0:]

#QuickSort gnuplot
set output "quicksort.png"
set title  "Quicksort"
plot "qs-vs-hs.Msteps" using 1:2 with lines title "Random arrays", "qs-vs-hs.Msteps" using 1:4 with lines title "Sorted arrays", "qs-vs-hs.Msteps" using 1:6 with lines title "Reverse sorted arrays"

#HeapSort gnuplot
set output "heapsort.png"
set title "Heapsort"
plot "qs-vs-hs.Msteps" using 1:3 with lines title "Random arrays","qs-vs-hs.Msteps" using 1:5 with lines title "Sorted arrays","qs-vs-hs.Msteps" using 1:7 with lines title "Reverse sorted arrays"

##Functions to fit our algorithms
y(x) = a1 * x * log(x) + b1 * x + c1
z(x) = a2 * x * log(x) + b2 * x + c2


## Comparision between Quicksort and Heapsort on Random array.
set output "qs-vs-hs-RA.png"
set title "Quicksort versus Heapsort (random arrays)"

##Fit the function to the dates of 2 and 3.
fit y(x) "qs-vs-hs.Msteps" using 1:2 via a1,b1,c1
fit z(x) "qs-vs-hs.Msteps" using 1:3 via a2,b2,c2

plot "qs-vs-hs.Msteps" using 1:2 with lines title "Quicksort","qs-vs-hs.Msteps" using 1:3 with lines title "Heapsort", y(x) with lines title "fitting Quicksort time values to n*log(n)+n", z(x) with lines title "fitting Heapsort time values to n*log(n)+n"


## Comparision between Quicksort and Heapsort on Sorted array.
set output "qs-vs-hs-SA.png"
set title "Quicksort versus Heapsort (sorted arrays)"

##Fit the function to the dates of 4 and 5.
fit y(x) "qs-vs-hs.Msteps" using 1:4 via a1,b1,c1
fit z(x) "qs-vs-hs.Msteps" using 1:5 via a2,b2,c2

plot "qs-vs-hs.Msteps" using 1:4 with lines title "Quicksort","qs-vs-hs.Msteps" using 1:5 with lines title "Heapsort", y(x) with lines title "fitting Quicksort time values to n*log(n)+n", z(x) with lines title "fitting Heapsort time values to n*log(n)+n"


## Comparision between Quicksort and Heapsort on Reverse Sorted array.
set output "qs-vs-hs-RSA.png"
set title "Quicksort versus Heapsort (reverse-sorted arrays)"

##Fit the function to the dates of 6 and 7.
fit y(x) "qs-vs-hs.Msteps" using 1:6 via a1,b1,c1
fit z(x) "qs-vs-hs.Msteps" using 1:7 via a2,b2,c2

plot "qs-vs-hs.Msteps" using 1:6 with lines title "Quicksort","qs-vs-hs.Msteps" using 1:7 with lines title "Heapsort", y(x) with lines title "fitting Quicksort time values to n*log(n)+n", z(x) with lines title "fitting Heapsort time values to n*log(n)+n"




