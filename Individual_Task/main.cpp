#include "Image_Processing.h"


int main() {
	FILE* input = fopen("input16.txt","r");
	FILE* output = fopen("output16.txt","w");
	

	process_image(input, output);
	


	fclose(input);
	fclose(output);
	return 0;
}
