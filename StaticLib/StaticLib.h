#ifndef STATIC_LIB_H
#define STATIC_LIB_H
#pragma once
#include <math.h>

//���������� ��� ���������� �������
void bright(unsigned short int* component, int comp_rows, int comp_cols, int f);

//������ ������
void grayscale(unsigned short int* component_A, unsigned short int* component_B, unsigned short int* component_C, int image_rows, int image_cols);


#endif
