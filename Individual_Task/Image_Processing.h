#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "StaticLib.h"


//��������� � ��������� ���������� ��� � ������ �� ����� ��� �������� ���������� ��������� �������
typedef struct Component {
	int comp_rows, comp_cols;
	unsigned short int data[1000][1000];
};

//�������� ���������, �������� �� �����������
typedef struct Image {
	int rows, cols;
	Component component_A;
	Component component_B;
	Component component_C;
};

//��������� ��� "��������" ������
typedef struct Command {
	char operands[16];
	char operation[16];
	char parametres[16];
};

//������� ������� ����������� - � main ����������� ������� ����� �� �����
void process_image(FILE* input_stream, FILE* output_stream);

//���������� �� �����
void readData(FILE* stream, Image* image, int* commands_amount);

//������ � ����
void writeData(FILE* stream, Image* image);

//������� ���������� �� ����, ������� 90
void rotate(int iterations, Component* component);

//�������������� ���������� �� ���
void flip(char axis, Component* component);

//������� ����������
void upscale(int scale_x, int scale_y, Component* component);

//��������� ����������
void downscale(int scale_rows, int scale_cols, Component* component);

//��������� ������� �� ������
void get_command(FILE* stream, Command* commands, int commands_amount);

//��������� ������ �����������, ������ �� �������� ���������
void save_imsize(Image* image);

#endif
