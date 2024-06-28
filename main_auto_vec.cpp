
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


void MatrixSum_auto_vec(int m, int n, float **a, float **b, float **res);

void MatrixSub_auto_vec(int m, int n, float **a, float **b, float **res);

int main() {
    
    int num = 100;
    double start, finish;
    double average_time;
    
    int m, n, i, j;
    float **a;
    float **b;
    float **res_sum;
    float **res_sub;

    printf("Введите размеры матрицы m, n: ");
    scanf("%d%d", &m, &n);
    printf("\n");
    printf("Размеры матрицы m = %d, n = %d\n", m, n);

    // Захватываем память под элементы матриц
    //a = (float *) malloc(m * n * sizeof(float));
    
    a = (float**)malloc(sizeof(float*)*m);
    for (i = 0; i < m; i++) {
        a[i]=(float*)malloc(sizeof(float)*n);
    }
    
    //b = (float *) malloc(m * n * sizeof(float));
    
    b = (float**)malloc(sizeof(float*)*m);
    for (i = 0; i < m; i++) {
        b[i]=(float*)malloc(sizeof(float)*n);
    }
    
    //res_sum = (float *) malloc(m * n * sizeof(float));
    
    res_sum = (float**)malloc(sizeof(float*)*m);
    for (i = 0; i < m; i++) {
        res_sum[i]=(float*)malloc(sizeof(float)*n);
    }
    for(i = 0 ; i < m ; i++) {
        for(j = 0; j < n; j++) {
            res_sum[i][j] = 0;
        }
    }
    
    //res_sub = (float *) malloc(m * n * sizeof(float));
    
    res_sub = (float**)malloc(sizeof(float*)*m);
    for (i = 0; i < m; i++) {
        res_sub[i]=(float*)malloc(sizeof(float)*n);
    }
    for(i = 0 ; i < m ; i++) {
        for(j = 0; j < n; j++) {
            res_sub[i][j] = 0;
        }
    }
    

    //заполняем матрицу a
    for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
            // Вводим элемент с индексами i, j
            a[i][j] = rand_num();
        }
    }
    
    //заполняем матрицу b
    for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
            // Вводим элемент с индексами i, j
            b[i][j] = rand_num();
        }
    }
    

    printf("_________________________________________\n");
    printf("Тест функции... Число запусков: %d\n", num);
   
    average_time = 0;
    for (int i = 0; i < num; i++) {
        
        start = omp_get_wtime();
        MatrixSum_auto_vec(m, n, a, b, res_sum);
        average_time += omp_get_wtime() - start;
    }
    printf("Общее время сложения параллельно: %f\n", average_time);
    printf("Среднее время сложения параллельно: %f\n", average_time / num);
    
    average_time = 0;
    for (int i = 0; i < num; i++) {
        
        start = omp_get_wtime();
        MatrixSub_auto_vec(m, n, a, b, res_sub);
        finish = omp_get_wtime();
        average_time += (finish - start);
    }
    printf("Общее время вычитания параллельно: %f\n", average_time);
    printf("Среднее время вычитания параллельно: %f\n", average_time / num);
    
    printf("_________________________________________\n");

    free(a);
    free(b);
    free(res_sum);
    free(res_sub);  // Освобождаем память
    return 0;
}

#pragma ompenmp simd
void MatrixSum_auto_vec(int m, int n, float **a, float **b, float **res) {
    
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            // Вводим элемент с индексами i, j
            res[i][j] = a[i][j] + b[i][j];
           
        }
    }
}


#pragma ompenmp simd
void MatrixSub_auto_vec(int m, int n, float **a, float **b, float **res) {
    
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            // Вводим элемент с индексами i, j
            res[i][j] = a[i][j] - b[i][j];
           
        }
    }
}
