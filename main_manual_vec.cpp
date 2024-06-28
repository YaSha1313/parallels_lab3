
//  main.cpp
//  Parallel_Laba_2
//
//  Created by Яна Шарандак on 26.12.2023.
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <xmmintrin.h>
#include "omp.h"
#include <time.h>

float rand_num(){
    int range_min=-20, range_max=20, rang_float=4;
    srand( time( NULL ) );
    float a=range_min+1+rand()%(range_max-range_min+1)-(rand()%((int)pow(10,rang_float))/pow(10,rang_float));
    return a;
}

void MatrixSum_vec(int m, int n, float *a, float *b, float *res);


int main() {
    
    int num = 100;
    float start, average_time;
    
    int m, n, i, j;
    float *a;
    float *b;
    float *res;

    printf("Введите размеры матрицы m, n: ");
    scanf("%d%d", &m, &n);
    printf("\n");
    printf("Размеры матрицы m = %d, n = %d\n", m, n);

    // Захватываем память под элементы матриц
    a = (float *) malloc(m * n * sizeof(float));
    b = (float *) malloc(m * n * sizeof(float));
    res = (float *) malloc(m * n * sizeof(float));

    //заполняем матрицу a
    for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
            // Вводим элемент с индексами i, j
            a[i*n + j] = rand_num();
        }
    }
    
    //заполняем матрицу b
    for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
            // Вводим элемент с индексами i, j
            b[i*n + j] = rand_num();
        }
    }
    

    printf("_________________________________________\n");
    printf("Тест функции... Число запусков: %d\n", num);
    
    average_time = 0;
    for (int i = 0; i < num; i++) {
        
        start = omp_get_wtime();
        MatrixSum_vec(m, n, a, b, res);
        average_time += omp_get_wtime() - start;
        
    }
    printf("Общее время выполнения c векторизацией: %f\n", average_time);
    printf("Среднее время выполнения с векторизацией: %f\n", average_time / num);
    printf("_________________________________________\n");

    free(a);
    free(b);
    free(res);  // Освобождаем память
    return 0;
}

void MatrixSum_vec(int m, int n, float *a, float *b, float *res) {
    
    
    size_t len = m*n;
    size_t len4 = len & ~0x03; // Обрезаем вектор до длины кратной 4, остаток вычисляем обычным способом
    __m128 x0, x1;
   
    for (size_t i = 0; i < len4; i += 4) {
        x0 = _mm_loadu_ps(a + i);
        x1 = _mm_loadu_ps(b + i);
        x0 = _mm_add_ps(x0, x1); // векторное сложение и запись результата в регистр x0
        _mm_storeu_ps(res + i, x0); // загрузка результата из регистра x0 в R
    }
    for(size_t i = len4; i < len; i++) { // считаем остаток без векторизации
       res[i] = a[i] * b[i];
    }
}
