#include<stdio.h>
#include<stdlib.h>


char* getColumn(char** matrix, int j){
	char* column = (char*) malloc (16*sizeof(char));
	for (int i = 0;  i < 16; i++){
		column[i] = matrix[i][j];
	}
	return column;
}

char** getSubGrid(char** matrix, int i, int j){
	char** subGrid = (char**) malloc (4*sizeof(char*));
	
	int start_row = i / 4 * 4;
    int start_col = j / 4 * 4;

	int row = 0;
    for (int m = start_row; m < start_row + 4; m++){
    	subGrid[row] = (char*) malloc (4*sizeof(char));
    	int col = 0;
        for (int n = start_col; n < start_col + 4; n++){
        	subGrid[row] [col] = matrix[m][n];
        	col++;
        }
        row++;
    }
    return subGrid;
	
}

char* findOptions(char** matrix, int i, int j){
	char opt[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	char* options = (char*) malloc(16 * sizeof(char));
	
	for (int v = 0; v < 16; v++){
		options[v] = opt[v];
	}
	
	char* row = matrix[i];
	char* column = getColumn(matrix, j);
	char** box = getSubGrid(matrix, i, j);
	
	for (int p = 0; p < 16; p++ ){
		for (int k = 0; k < 16; k++ ){
			if ((row[p]==options[k])&&(options[k] != '-')){
				options[k] = '-';
			}if ((column[p]==options[k])&&(options[k] != '-')){
				options[k] = '-';
			}
		}
	}
	
	for (int m = 0; m < 4; m++ ){
		for (int n = 0; n < 4; n++ ){
			for (int k = 0; k < 16; k++ ){
				if ((box[m][n]==options[k])&&(options[k] != '-')){
					options[k] = '-';
				}
			}
		}
	}
	
	return options;
}

char** findSpot(char** matrix){
	int solved = -1;
	while (solved == -1){
		solved = 1;
		for (int m = 0; m < 16; m++ ){
			for (int n = 0; n < 16; n++ ){
				if (matrix[m][n] == '-'){
					
					solved = -1;
					char* options = findOptions(matrix, m, n);
					
					int count = 0;
					for (int i = 0; i < 16; i++){
						if (options[i] != '-'){
							count++;
						}
					}
					
					if (count == 0) {
						printf("no-solution\n");
						return NULL;
					}
					
					else if (count == 1) {
						for (int i = 0; i < 16; i++){
							if (options[i] != '-'){
								matrix[m][n] = options[i];
								break;
							}
						}
					}
				}
			}
			//printf("%d\n", solved);
		}
	}
	return matrix;
}

int rowCheck(char** matrix, int i){
	for (int j = 0; j < 16; j++){
		for (int k = 0; k < 16; k++){
			if ((k != j)&&(matrix[i][j]==matrix[i][k])){
				//printf("%c, %c\n", matrix[i][j], matrix[i][k]);
				return -1;
			}
		}
	}
	return 0;
}

int columnCheck(char** matrix, int j){
	char* column = getColumn(matrix, j);
	for (int i = 0; i < 16; i++){
		for (int k = 0; k < 16; k++){
			if ((k != i)&&(matrix[i][j]==column[k])){
				//printf("%c, %c\n", matrix[i][j], matrix[i][k]);
				return -1;
			}
		}
	}
	return 0;
}

int subGridCheck(char** matrix, int row, int col){
	char ** subGrid = getSubGrid(matrix, row, col);
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			for (int m = 0; m < 4; m++){
				for (int n = 0; n < 4; n++){
					if ((m != i)&&(n != j)&&(subGrid[i][j]==subGrid[m][n])){
						//printf("%c, %c\n", subGrid[i][j], subGrid[m][n]);
						return -1;
					}
				}
			}
		}
	}
	return 0;
}

int doublecheck(char**matrix){
	for (int i = 0; i < 16; i++){
		for (int j = 0; j < 16; j++){
			if ((rowCheck(matrix, i) == -1)||(columnCheck(matrix, j) == -1) || (subGridCheck(matrix, i, j) == -1)){
				return -1;
			}
		}
		
	}
	return 0;
}

int main(int argc, char** argv){
	if (argc != 2){
		printf("insufficient arguments\n");
		exit(0);
	}
	FILE* fp = fopen(argv[1], "r");
	if(fp == NULL){
		printf("file does not exist\n");
		exit(0);
	}
	
	char ** matrix = (char**) malloc (16*sizeof(char*));
	
	for (int i = 0; i < 16; i++){
		matrix[i] = (char*) malloc (16*sizeof(char));
		for (int j = 0; j < 16; j++){
			fscanf(fp, "%c\t", &matrix[i][j]);
		}
	}
	
	char ** answer = (char**) malloc (16*sizeof(char*));
	for (int i = 0; i < 16; i++){
		answer[i] = (char*) malloc (16*sizeof(char));
	}
	
	answer = findSpot(matrix);
	if (answer == NULL){
		return 0;
	}
	
	if (doublecheck(matrix)==-1){
		printf("no-solution\n");
		return 0;
	}
	
	for (int i = 0; i < 16; i++){
		for (int j = 0; j < 16; j++){
			printf("%c\t", answer[i][j]);
		}
		printf("\n");
	}
	
	free(answer);
	
	return 0;
}