#include "common.h"

int factorial(int n){
	if (n == 0){
		return 1;
	}
	else{
		return(n*factorial(n-1));
	}
}

int main(int argc, char **argv)
{
	int int_num = (int)atof(argv[1]);
	float float_num = atof(argv[1]); 

	if (int_num != float_num){ //this is float
		printf("Huh?\n");
	}
	else{
		if (int_num == 0){ //not number
			printf("Huh?\n");
		}
		
		else if (int_num > 12){
			printf("Overflow\n");
		}

		else{
			printf("%d\n", factorial(int_num));
		}
	}

	return 0;
}
