#include "StaticLib.h"

void bright(unsigned short int* component, int comp_rows, int comp_cols, int f) {
	if (f > 0) {
		for (int counter = 0; counter < f; counter++) {
			for (int row = 0; row < comp_rows; row++) {
				for (int col = 0; col < comp_cols; col++) {
					*(component + row * comp_rows + col) *= f;
				}
			}
		}
	}
	else if (f < 0) {
		f = abs(f);
		for (int counter = 0; counter < f; counter++) {
			for (int row = 0; row < comp_rows; row++) {
				for (int col = 0; col < comp_cols; col++) {
					*(component + row * comp_rows + col) /= f;
				}
			}
		}
	}
}

void grayscale(unsigned short int* component_A, unsigned short int* component_B, unsigned short int* component_C, int image_rows, int image_cols) {
	int average = 0;
	for (int row = 0; row < image_rows; row++) {
		for (int col = 0; col < image_cols; col++) {
			average = (*(component_A + row * image_rows + col) + *(component_B + row * image_rows + col) + *(component_C + row * image_rows + col))/ 3;
			*(component_A + row * image_rows + col) = average;
			*(component_B + row * image_rows + col) = average;
			*(component_C + row * image_rows + col) = average;
		}
	}
}