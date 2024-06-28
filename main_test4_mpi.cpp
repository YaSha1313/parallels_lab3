#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

int main(int agrc, char* argv[]) {
    int m, n, start_time;    // Размеры матрицы и переменная для замера времени
    double *a, *b, *res_sub;   // Исходные и результирующая матрицы
    int proc_size;    // Количество процессов
    int proc_rank;   // Ранг процесса

    // Инициализация MPI
    MPI_Init(&agrc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == 0) {
        printf("Введите размеры матрицы (m, n): ");
        scanf("%d%d", &m, &n);
        printf("\n");

        // Выделение памяти под элементы матриц
        a = (double *) malloc(m * n * sizeof(double));
        b = (double *) malloc(m * n * sizeof(double));
        res_sub = (double *) malloc(m * n * sizeof(double));

        //заполняем матрицу a
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                // Вводим элемент с индексами i, j
                //a[i][j] = rand_num();
                int range_min=-20, range_max=20, rang_float=4;
                srand( time( NULL ) );
                a[i + j * n] = range_min+1+rand()%(range_max-range_min+1)-(rand()%((int)pow(10,rang_float))/pow(10,rang_float));
            }
        }
        
        //заполняем матрицу b
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                // Вводим элемент с индексами i, j
                //b[i][j] = rand_num();
                int range_min=-20, range_max=20, rang_float=4;
                srand( time( NULL ) );
                b[i + j * n] = range_min+1+rand()%(range_max-range_min+1)-(rand()%((int)pow(10,rang_float))/pow(10,rang_float));
            }
        }
    }

    start_time = MPI_Wtime();

    // Рассылка полученных значений на все процессы
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(a, m * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, m * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int chunk = m / proc_size;
    int begin = chunk * proc_rank;
    int end = chunk * (proc_rank + 1);

    if (proc_rank == proc_size-1) {
        end = m;
    }

    for(int i = begin; i < end; i++) {
        for(int j = 0; j < n; j++) {
            res_sub[i + j * n] = b[i + j * n] - a[i * n + j];
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    // Вывод результатов
    if (proc_rank == 0) {
        printf("Общее время вычитания параллельно: %5.5f s\n%d-process.\n", MPI_Wtime()-start_time, proc_size);
    }

    // Завершение MPI
    MPI_Finalize();

    return(0);
}
