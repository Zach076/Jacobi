gcc -pthread -g -o jacobi -Wall jacobi.c

rm results

(time ./jacobi 1) >> results.txt 2>&1
(time ./jacobi 2) >> results.txt 2>&1
(time ./jacobi 3) >> results.txt 2>&1
(time ./jacobi 4) >> results.txt 2>&1
(time ./jacobi 5) >> results.txt 2>&1
(time ./jacobi 6) >> results.txt 2>&1
(time ./jacobi 7) >> results.txt 2>&1
(time ./jacobi 8) >> results.txt 2>&1
(time ./jacobi 9) >> results.txt 2>&1
(time ./jacobi 10) >> results.txt 2>&1
