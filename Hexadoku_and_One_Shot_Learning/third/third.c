#include<stdio.h>
#include<stdlib.h>

double** transpose(double** matrix, int m, int n){
	
	double** answer = (double**) malloc(n*sizeof(double));
	for (int i = 0; i < n; i++){
		answer[i] = (double*) malloc (m*sizeof(double));
		for (int j = 0; j < m; j++){
			answer[i][j] = matrix[j][i];
			//printf("%lf\t", answer[i][j]);
		}
		//printf("\n");
	}

	return answer;
}

double** multiply(double** matrix1, double** matrix2, int rows1, int cols1, int rows2, int cols2){
	////printf("yee\n");
	double ** answer =  (double**) malloc (rows1*sizeof(double*));
	
	if (cols1 == rows2){
		////printf("yee1\n");
		for (int m = 0; m < rows1; m++){
			////printf("yee2\n");
			answer[m] = (double*) malloc (cols2*sizeof(double));
			////printf("yee3\n");
			for (int n = 0; n < cols2; n++){
				////printf("yee4\n");
				for (int z = 0; z < rows2; z++){
					////printf("yee5\n");
					answer[m][n] += (matrix1[m][z] * matrix2[z][n]);
				}
				//printf("%lf\t", answer[m][n]);
			}
			//printf("\n");
		}
	}

	return answer;
	
}

double** inverse(double** matrix, int n){
	
	//MEMORY ALLOCATION
	double ** answer =  (double**) malloc (n*sizeof(double*));	
	for (int x = 0; x < n; x++){
		answer[x] = (double*) malloc (n*sizeof(double));
		for (int y = 0; y < n; y++){
			if (x == y){
				answer[x][y] = 1;
			}
			else{
				answer[x][y] = 0;
			}
		}
	}

	//FORWARD ELIMINATION
	for (int i=0; i<n; i++){
		for (int j = 0; j < n; j++){
			if (i == j){
				if (matrix[i][j] == 0){
					double mult = 1/matrix[i+1][j];
					for (int k = 0; k < n; k++){
						matrix[i][k] = matrix[i][k]+mult*matrix[i+1][k];
						answer[i][k] = answer[i][k]+mult*answer[i+1][k];
					}
					for (int r = i+1; r < n; r++){
						if (matrix[r][j] != 0){
							mult = matrix[i][j]/matrix[r][j];
							for (int c = 0; c < n; c++){
								matrix[r][c] = matrix[r][c]-mult*matrix[i][c];
								answer[r][c] = matrix[r][c]-mult*matrix[i][c];
							}
						}
					}
				}
				else if (matrix[i][j] != 1){
					double mult = 1/matrix[i][j];
					for (int k = 0; k < n; k++){
						matrix[i][k] = mult*matrix[i][k];
						answer[i][k] = mult*answer[i][k];
					}
					for (int r = i+1; r < n; r++){
						mult = matrix[r][j]/matrix[i][j];
						//printf("mult: %lf\n", mult);
						for (int c = 0; c < n; c++){
							matrix[r][c] = matrix[r][c]-mult*matrix[i][c];
							answer[r][c] = answer[r][c]-mult*answer[i][c];
						}
					}
				} else {
					for (int r = i+1; r < n; r++){
						if (matrix[r][j] != 0){
							double mult = matrix[i][j]/matrix[r][j];
							for (int c = 0; c < n; c++){
								matrix[r][c] = matrix[r][c]-mult*matrix[i][c];
								answer[r][c] = answer[r][c]-mult*answer[i][c];
							}
						}
					}
				}
			}
		}
	}
	
	for (int x = 0; x < n; x++){
		for (int y = 0; y < n; y++){
			//printf("%lf\t", answer[x][y]);
		}//printf("\n");
	}

	//printf("\n\n");
	
	//BACK ELIMINATION
	
	for (int i = n-1; i >= 0; i--){
		for (int j  = n-1; j >= i; j--){
			if (i == j){
				continue;
			}
			else{
				double mult = matrix[i][j]/matrix[j][j];
				for (int q  = 0; q < n; q++){
					matrix[i][q] = matrix[i][q] - mult*matrix[j][q];
					answer[i][q] = answer[i][q] - mult*answer[j][q];
				}
			}
		}
	}
	/**/
	for (int x = 0; x < n; x++){
		for (int y = 0; y < n; y++){
			//printf("%lf\t", answer[x][y]);
		}//printf("\n");
	}
	/**/
	return answer;
}

int main(int argc, char** argv){
	
	if (argc != 3){
		//printf("insufficient arguments\n");
		exit(0);
	}
	
	FILE* fp1 = fopen(argv[1], "r");
	FILE* fp2 = fopen(argv[2], "r");
	
	if(fp1 == NULL){
		//printf("file does not exist\n");
		exit(0);
	}
	if(fp2 == NULL){
		//printf("file does not exist\n");
		exit(0);
	}
	
	//TRAINING
	int n = 0;
	int m = 0;
	fscanf(fp1, "%d\n%d", &n, &m);
	
	double ** y_train = (double**) malloc (m*sizeof(double));
	double ** x_train = (double**) malloc (m*sizeof(double*));
	
	for (int i = 0; i < m; i++){
		x_train[i] = (double*) malloc ((n+1)*sizeof(double));
		y_train[i] = (double*) malloc (sizeof(double));
				
		x_train[i][0] = 1;
		for (int j = 1; j < n+1; j++){
			fscanf(fp1, "%lf, ", &x_train[i][j]);
			//printf("%lf\t", x_train[i][j]);
		}
		fscanf(fp1, "%lf\n", &y_train[i][0]);
		//printf("%lf\n", y_train[i][0]);
	}
	
	//printf("\n\n");
	
	//TRANSPOSE OF X_TRAIN
	double ** trans = (double**) malloc ((n+1)*sizeof(double*));
	for (int i = 0; i < (n+1); i++){
		trans[i] = (double*) malloc ((m)*sizeof(double));
	}
	trans = transpose(x_train, m, (n+1));
	
	//printf("\n\n");
	
	//MULTIPLICATION OF TRANS AND X_TRAIN
	double ** mult1 = (double**) malloc (((n+1))*sizeof(double*));
	////printf("yerr2\n");
	for (int i = 0; i < (n+1); i++){
		////printf("%d, %d\n", i, n+1);
		mult1[i] = (double*) malloc (((n+1))*sizeof(double));
	}
	
	mult1 = multiply(trans, x_train, (n+1), m, m, (n+1));

	//printf("\n\n");

	//INVERSE OF MULT1
	double ** invMult1 = (double**) malloc (((n+1))*sizeof(double*));
	for (int i = 0; i < (n+1); i++){
		invMult1[i] = (double*) malloc (((n+1))*sizeof(double));
	}
	invMult1 = inverse(mult1, (n+1));
	
	//printf("\n\n");
	
	//MULTIPLICATION OF INVMULT1 AND TRANS
	double ** mult2 = (double**) malloc (((n+1))*sizeof(double*));
	for (int i = 0; i < (n+1); i++){
		mult2[i] = (double*) malloc ((m)*sizeof(double));
	}
	mult2 = multiply(invMult1, trans, (n+1), (n+1), (n+1), m);

	//printf("\n\n");

	//MULTIPLICATION OF MULT2 AND Y_TRAIN
	for (int i = 0; i < m; i++){
		//printf("%lf\n", y_train[i][0]);
	}
	//printf("\n\n");
	
	double ** weight = (double**) malloc (((n+1))*sizeof(double*));
	for (int i = 0; i < (n+1); i++){
		weight[i] = (double*) malloc (sizeof(double));
	}
	weight = multiply(mult2, y_train, (n+1), m, m, 1);

	//printf("\n\n");

	//TESTING
	int testNum = 0;
	fscanf(fp2, "%d\n", &testNum);
	double ** y =  (double**) malloc (testNum*sizeof(double*));
	double ** x_test = (double**) malloc (testNum*sizeof(double*));
	
	for (int i = 0; i < testNum; i++){
		x_test[i] = (double*) malloc (((n+1))*sizeof(double));
		y[i] = (double*) malloc (sizeof(double));
		
		x_test[i][0] = 1;
		for (int j = 1; j < (n+1); j++){
			fscanf(fp2, "%lf, ", &x_test[i][j]);
			//printf("%lf\t", x_test[i][j]);
		}
		//printf("\n");
	}
	

	//printf("\n\n");

	y = multiply(x_test, weight, testNum, (n+1), (n+1), 1);
	for (int i = 0; i < testNum; i++){
		printf("%0.0lf\n", y[i][0]);
	}
	
	free(x_train);
	free(trans);
	free(mult1);
	free(invMult1);
	free(mult2);
	free(weight);
	free(y_train);
	free(x_test);
	free(y);
	
	//printf("\n\n");
	
	return 0;
}
