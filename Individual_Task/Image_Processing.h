#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "StaticLib.h"


//добавляем в структуру компоненты еще и размер ее самой для удобства выполнения некоторых функций
typedef struct Component {
	int comp_rows, comp_cols;
	unsigned short int data[1000][1000];
};

//основная структура, хранящая всё изображение
typedef struct Image {
	int rows, cols;
	Component component_A;
	Component component_B;
	Component component_C;
};

//структура для "парсинга" команд
typedef struct Command {
	char operands[16];
	char operation[16];
	char parametres[16];
};

//главная функция процессинга - в main планируется наличие толко ее одной
void process_image(FILE* input_stream, FILE* output_stream);

//считывание из файла
void readData(FILE* stream, Image* image, int* commands_amount);

//запись в файл
void writeData(FILE* stream, Image* image);

//вращает компоненту на угол, кратный 90
void rotate(int iterations, Component* component);

//переворачивает компоненту по оси
void flip(char axis, Component* component);

//апскейл компоненты
void upscale(int scale_x, int scale_y, Component* component);

//даунскейл компоненты
void downscale(int scale_rows, int scale_cols, Component* component);

//получение команды из потока
void get_command(FILE* stream, Command* commands, int commands_amount);

//сохраняет размер изображения, исходя из размеров компонент
void save_imsize(Image* image);

#endif
