#include "Image_Processing.h"


void process_image(FILE* input_stream, FILE* output_stream){

	Image* image = (Image*)calloc(1, sizeof(Image));
	Command* commands = (Command*)calloc(100, sizeof(Command));

	int commands_amount;

	readData(input_stream, image, &commands_amount);

	//дл€ удобства ранее добавили размер к каждой компоненте, установим его:
	image->component_A.comp_cols = image->cols;
	image->component_A.comp_rows = image->rows;
	image->component_B.comp_cols = image->cols;
	image->component_B.comp_rows = image->rows;
	image->component_C.comp_cols = image->cols;
	image->component_C.comp_rows = image->rows; 

	get_command(input_stream, commands, commands_amount);

	int operands_len; 

	int param_len;

	//вызов соответсвующей функции дл€ команды дл€ всех операнд с парметрами
	//сначала поиск имени команды, затем - проход по опреандам
	for (int i = 0; i < commands_amount; i++) {


		operands_len = strlen(commands[i].operands);
		param_len = strlen(commands[i].parametres);

		if (strstr(commands[i].operation, "rotate")) {

			//выт€жка параметра конкретно дл€ rotate
			int angle = 0;
			int pow_k = -1;
			int iterations = 0; 

			//выт€гиваем угол из строки

			bool found_angle = false;
			for (int j = 0; j < param_len && !found_angle; j++) {
				int temp_j = j;
				while (isdigit(commands[i].parametres[temp_j])) {
					pow_k++;

					temp_j++;
				}
				if (temp_j > j) {
					for (int k = j; k < temp_j; k++) {
						angle += (commands[i].parametres[k] - '0') * pow(10, pow_k);
						pow_k--;
					}
					j = temp_j;
					found_angle = true;
				}
			}

			//количество итераций функции поворота на 90 градусов
			iterations = (angle % 360) / 90;

			//проход по операндам
			for (int j = 0; j < operands_len && iterations; j++) {
				if (commands[i].operands[j] == 'A') {
					rotate(iterations, &image->component_A);
				}
				else if (commands[i].operands[j] == 'B') {
					rotate(iterations, &image->component_B);
				}
				else if (commands[i].operands[j] == 'C') {
					rotate(iterations, &image->component_C);
				}
			}
			
		}
		else if (strstr(commands[i].operation, "flip")) {

				if (strstr(commands[i].parametres, "V")) {

					for (int j = 0; j < operands_len; j++) {
						if (commands[i].operands[j] == 'A') {
							flip('V', &image->component_A);
						}
						else if (commands[i].operands[j] == 'B') {
							flip('V', &image->component_B);
						}
						else if (commands[i].operands[j] == 'C') {
							flip('V', &image->component_C);
						}
					}
				}
				if (strstr(commands[i].parametres, "H")) {
					for (int j = 0; j < operands_len; j++) {
						if (commands[i].operands[j] == 'A') {
							flip('H', &image->component_A);
						}
						else if (commands[i].operands[j] == 'B') {
							flip('H', &image->component_B);
						}
						else if (commands[i].operands[j] == 'C') {
							flip('H', &image->component_C);
						}
					}
				}
			
		}
		else if (strstr(commands[i].operation, "upscale")) {

			int pow_k = -1;
			int scales[2] = {0, 0};
			int scales_index = 0;

			//выт€гиваем числа из строки 
			for (int j = 0; j < param_len; j++) {
				int temp_j = j;
				while (isdigit(commands[i].parametres[temp_j])) {
					pow_k++;

					temp_j++;
				}
				if (temp_j > j) {
					for (int k = j; k < temp_j; k++) {
						scales[scales_index] += (commands[i].parametres[k] - '0') * pow(10, pow_k);
						pow_k--;
					}
					j = temp_j;
					scales_index++;
				}
			}

			
			bool execute_forA = true;
			bool execute_forB = true;
			bool execute_forC = true;
			bool execute = true;

			//проверка на возможность выполнени€ команды

			if ((image->component_A.comp_rows * scales[0] > 1000) || (image->component_A.comp_cols * scales[1] > 1000)) {
				execute_forA = false;
			}

			if ((image->component_B.comp_rows * scales[0] > 1000) || (image->component_B.comp_cols * scales[1] > 1000)) {
				execute_forB = false;
			}

			if ((image->component_C.comp_rows * scales[0] > 1000) || (image->component_C.comp_cols * scales[1] > 1000)) {
				execute_forC = false;
			}


			if (strstr(commands[i].operands, "A")) {
				execute = execute && execute_forA;
			}

			if (strstr(commands[i].operands, "B")) {
				execute = execute && execute_forB;
			}

			if (strstr(commands[i].operands, "C")) {
				execute = execute && execute_forC;
			}

			if (execute) {
				for (int j = 0; j < operands_len; j++) {
					if (commands[i].operands[j] == 'A') {
						upscale(scales[0], scales[1], &image->component_A);
					}
					else if (commands[i].operands[j] == 'B') {
						upscale(scales[0], scales[1], &image->component_B);
					}
					else if (commands[i].operands[j] == 'C') {
						upscale(scales[0], scales[1], &image->component_C);
					}
				}
			}
		}
		else if (strstr(commands[i].operation, "downscale")) {
			for (int j = 0; j < operands_len; j++) {

				int pow_k = -1;
				int scales[2] = { 0, 0 };
				int scales_index = 0;

				//выт€гиваем числа из строки 
				for (int j = 0; j < param_len; j++) {
					int temp_j = j;
					while (isdigit(commands[i].parametres[temp_j])) {
						pow_k++;

						temp_j++;
					}
					if (temp_j > j) {
						for (int k = j; k < temp_j; k++) {
							scales[scales_index] += (commands[i].parametres[k] - '0') * pow(10, pow_k);
							pow_k--;
						}
						j = temp_j;
						scales_index++;
					}
				}


				bool execute_forA = true;
				bool execute_forB = true;
				bool execute_forC = true;
				bool execute = true;


				if ((image->component_A.comp_rows % scales[0] != 0) || (image->component_A.comp_cols % scales[1] != 0)) {
					execute_forA = false;
				}

				if ((image->component_B.comp_rows % scales[0] != 0) || (image->component_B.comp_cols % scales[1] != 0)) {
					execute_forB = false;
				}

				if ((image->component_C.comp_rows % scales[0] != 0) || (image->component_C.comp_cols % scales[1] != 0)) {
					execute_forC = false;
				}

				if (strstr(commands[i].operands, "A")) {
					execute = execute && execute_forA;
				}

				if (strstr(commands[i].operands, "B")) {
					execute = execute && execute_forB;
				}

				if (strstr(commands[i].operands, "C")) {
					execute = execute && execute_forC;
				}


				if (execute) {
					for (int j = 0; j < operands_len; j++) {
						if (commands[i].operands[j] == 'A') {
							downscale(scales[0], scales[1], &image->component_A);
						}
						else if (commands[i].operands[j] == 'B') {
							downscale(scales[0], scales[1], &image->component_B);
						}
						else if (commands[i].operands[j] == 'C') {
							downscale(scales[0], scales[1], &image->component_C);
						}
					}
				}
			}
		}
		else if (strstr(commands[i].operation, "bright")) {
			
			int f = 0;
			int pow_k = -1;

			//выт€гиваем аргумент из строки

			bool found_f = false;
			for (int j = 0; j < param_len && !found_f; j++) {
				int temp_j = j;
				while (isdigit(commands[i].parametres[temp_j])) {
					pow_k++;

					temp_j++;
				}
				if (temp_j > j) {
					for (int k = j; k < temp_j; k++) {
						f += (commands[i].parametres[k] - '0') * pow(10, pow_k);
						pow_k--;
					}
					j = temp_j;
					found_f = true;
				}
			}
			for (int j = 0; j < operands_len; j++) {
				if (commands[i].operands[j] == 'A') {
					bright(&image->component_A.data[0][0], image->component_A.comp_rows, image->component_A.comp_cols, f);
				}
				else if (commands[i].operands[j] == 'B') {
					bright(&image->component_B.data[0][0], image->component_B.comp_rows, image->component_B.comp_cols, f);
				}
				else if (commands[i].operands[j] == 'C') {
					bright(&image->component_B.data[0][0], image->component_B.comp_rows, image->component_B.comp_cols, f);
				}
			}


		}else if (strstr(commands[i].operation, "grayscale")) {

			grayscale(&image->component_A.data[0][0], &image->component_B.data[0][0], &image->component_C.data[0][0], image->rows, image->cols);
		}

		save_imsize(image);
	}

	writeData(output_stream, image);

	free(image);
	image = NULL;

	free(commands);
	commands = NULL;
}

void readData(FILE* stream, Image* image, int* commands_amount){
	fscanf(stream, "%d %d", &image->rows, &image->cols);

	for (int i = 0; i < (*image).rows; i++) {
		for (int j = 0; j < (*image).cols; j++) {
			fscanf(stream, "%02X%02X%02X", &image->component_C.data[i][j], &image->component_B.data[i][j], &image->component_A.data[i][j]);
		}
	}

	fscanf(stream, "%d", commands_amount);
}  

void writeData(FILE* stream, Image* image){

	fprintf(stream, "%d %d\n", (*image).rows, (*image).cols);

	for (int i = 0; i < (*image).rows; i++) {
		for (int j = 0; j < (*image).cols; j++) {
			fprintf(stream, "%02X%02X%02X", image->component_C.data[i][j], image->component_B.data[i][j], image->component_A.data[i][j]);
			
			if (j < (*image).cols - 1) {
				fprintf(stream, " ");
			}
		}
		fprintf(stream, "\n");
	}

}

void flip(char axis, Component* component) {

	unsigned short int temp;

	if (axis == 'V') {
		for (int i = 0; i < component->comp_rows; i++) {
			for (int j = 0; j < component->comp_cols / 2; j++) {
				temp = component->data[i][j];
				component->data[i][j] = component->data[i][component->comp_cols - j - 1];
				component->data[i][component->comp_cols - j - 1] = temp;
			}
		}
	}
	else if (axis == 'H') {
		for (int i = 0; i < component->comp_cols; i++) {
			for (int j = 0; j < component->comp_rows / 2; j++) {
				temp = component->data[j][i];
				component->data[j][i] = component->data[component->comp_rows - j - 1][i];
				component->data[component->comp_rows - j - 1][i] = temp;
			}
		}
	}
}

void upscale(int scale_rows, int scale_cols, Component* component){
	unsigned short int* temp_arr = (unsigned short int*)calloc(1000, sizeof(unsigned short int));
	int temp_arr_size = 0;

	int scaled_index = 0;

	//копируем строку во врменнный массив и перезаписываем каждый элемент столко раз, сколько требует масштаб
	for (int row = 0; row < component->comp_rows; row++) {
		for (int col = 0; col < component->comp_cols; col++) {
			temp_arr[col] = component->data[row][col];
			temp_arr_size++;
		}
		for (int col = 0; col < temp_arr_size; col++) {
			for (int scaled_cols = 0; scaled_cols < scale_cols; scaled_cols++) {
				component->data[row][scaled_index] = temp_arr[col];
				scaled_index++;
			}
		}

		scaled_index = 0;
		temp_arr_size = 0;
	}

	component->comp_cols *= scale_cols;

	//копирууем столбец во временный массив и перезаписываем каждый элемент столько раз, сколько требует масштаб
	for (int col = 0; col < component->comp_cols; col++) {
		for (int row = 0; row < component->comp_rows; row++) {
			temp_arr[row] = component->data[row][col];
			temp_arr_size++;
		}
		for (int row = 0; row < temp_arr_size; row++) {
			for (int scaled_row = 0; scaled_row < scale_rows; scaled_row++) {
				component->data[scaled_index][col] = temp_arr[row];
				scaled_index++;
			}
		}

		scaled_index = 0;
		temp_arr_size = 0;
	}

	component->comp_rows *= scale_rows;

	free(temp_arr);
	temp_arr = NULL;
}

void downscale(int scale_rows, int scale_cols, Component* component){
	unsigned short int* temp_arr = (unsigned short int*)calloc(1000, sizeof(int));

	int r_c, w_c;

	for (int row = 0; row < component->comp_rows; row++) {
		for (r_c = 0; r_c < component->comp_cols / scale_cols; r_c++) {
			temp_arr[r_c] = component->data[row][r_c * scale_cols];
		}

		for (w_c = 0; w_c < component->comp_cols / scale_cols; w_c++) {
			component->data[row][w_c] = temp_arr[w_c];
		}

		while (w_c < component->comp_cols) {
			component->data[row][w_c] = 0;
			w_c++;
		}
	}

	component->comp_cols /= scale_cols;

	for (int col = 0; col < component->comp_cols; col++) {
		for (r_c = 0; r_c < component->comp_rows / scale_rows; r_c++) {
			temp_arr[r_c] = component->data[r_c * scale_rows][col];
		}

		for (w_c = 0; w_c < component->comp_rows / scale_rows; w_c++) {
			component->data[w_c][col] = temp_arr[w_c];
		}

		while (w_c < component->comp_rows) {
			component->data[w_c][col] = 0;
			w_c++;
		}
	}

	component->comp_rows /= scale_rows;



	free(temp_arr);
	temp_arr = NULL;
}

void get_command(FILE* stream, Command* commands, int commands_amount){
	

	for (int counter = 0; counter < commands_amount; counter++) {
		fscanf(stream, "%s", commands[counter].operands);
		fscanf(stream, " : %s", commands[counter].operation);
		fgets(commands[counter].parametres, 16, stream);
	}

}

void rotate(int iterations, Component* component){
	
	int temp, max, temp_max;

	for (int turn = 0; turn < iterations; turn++){

		if (component->comp_rows < component->comp_cols) {
			max = component->comp_cols;
		}
		else {
			max = component->comp_rows;
		}

		temp_max = max - 1;

		for (int row = 0; row < max / 2; row++){
			for (int col = row; col < temp_max; col++){
				temp = component->data[row][col];

				component->data[row][col] = component->data[temp_max - (col - row)][row];
				component->data[temp_max - (col - row)][row] = component->data[temp_max][temp_max - (col - row)];
				component->data[temp_max][temp_max - (col - row)] = component->data[col][temp_max];
				component->data[col][temp_max] = temp;
			}

			temp_max--;
		}


		temp = component->comp_rows;
		component->comp_rows = component->comp_cols;
		component->comp_cols = temp;

		if (component->comp_rows > component->comp_cols){
			for (int row = 0; row < component->comp_rows; row++){
				for (int col = 0; col < component->comp_cols; col++){
					temp = component->data[row][col];
					component->data[row][col] = component->data[row][col + (component->comp_rows - component->comp_cols)];
					component->data[row][col + (component->comp_rows - component->comp_cols)] = temp;
				}
			}
		}
	}
}

void save_imsize(Image* image) {
	int max_comp_rows = 0;
	int max_comp_cols = 0;
	
	if (image->component_A.comp_rows > max_comp_rows) {
		max_comp_rows = image->component_A.comp_rows;
	}

	if (image->component_B.comp_rows > max_comp_rows) {
		max_comp_rows = image->component_B.comp_rows;
	}

	if (image->component_C.comp_rows > max_comp_rows) {
		max_comp_rows = image->component_C.comp_rows;
	}


	if (image->component_A.comp_cols > max_comp_cols) {
		max_comp_cols = image->component_A.comp_cols;
	}

	if (image->component_B.comp_cols > max_comp_cols) {
		max_comp_cols = image->component_B.comp_cols;
	}

	if (image->component_C.comp_cols > max_comp_cols) {
		max_comp_cols = image->component_C.comp_cols;
	}


	image->rows = max_comp_rows;
	image->cols = max_comp_cols;
}