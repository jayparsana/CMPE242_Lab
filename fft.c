#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE_OF_INPUT 256
#define TWO_TO_THE_POWER 8

struct Complex
{	double a;        //Real Part
	double b;        //Imaginary Part
}          X[SIZE_OF_INPUT], U, W, T, Tmp;

void FFT(void)
{
	int M = TWO_TO_THE_POWER;
	int N = pow(2, M);

	int i = 1, j = 1, k = 1;
	int LE = 0, LE1 = 0;
	int IP = 0;

	for (k = 1; k <= M; k++)
	{
		LE = pow(2, M + 1 - k);
		LE1 = LE / 2;

		U.a = 1.0;
		U.b = 0.0;

		W.a = cos(M_PI / (double)LE1);
		W.b = -sin(M_PI/ (double)LE1);

		for (j = 1; j <= LE1; j++)
		{
			for (i = j; i <= N; i = i + LE)
			{
				IP = i + LE1;
				T.a = X[i].a + X[IP].a;
				T.b = X[i].b + X[IP].b;
				Tmp.a = X[i].a - X[IP].a;
				Tmp.b = X[i].b - X[IP].b;
				X[IP].a = (Tmp.a * U.a) - (Tmp.b * U.b);
				X[IP].b = (Tmp.a * U.b) + (Tmp.b * U.a);
				X[i].a = T.a;
				X[i].b = T.b;
			}
			Tmp.a = (U.a * W.a) - (U.b * W.b);
			Tmp.b = (U.a * W.b) + (U.b * W.a);
			U.a = Tmp.a;
			U.b = Tmp.b;
		}
	}

	int NV2 = N / 2;
	int NM1 = N - 1;
	int K = 0;

	j = 1;
	for (i = 1; i <= NM1; i++)
	{
		if (i >= j) goto TAG25;
		T.a = X[j].a;
		T.b = X[j].b;

		X[j].a = X[i].a;
		X[j].b = X[i].b;
		X[i].a = T.a;
		X[i].b = T.b;
TAG25:	K = NV2;
TAG26:	if (K >= j) goto TAG30;
		j = j - K;
		K = K / 2;
		goto TAG26;
TAG30:	j = j + K;
	}
}

int main(void)
{
	  FILE *in_file  = fopen("adc_output_256_points.dat", "r"); // read only 
	 
          // test for files not existing. 
          if (in_file == NULL) 
            {   
              printf("Error! Could not open file\n"); 
              exit(-1); // must include stdlib.h 
            } 
           float ab[256];
          // write to file vs write to screen
	for(int j = 0; j < SIZE_OF_INPUT; j++){ 
        	fscanf(in_file, "%f", &ab[j]); // write to file  
	}

	int i;
	for (i = 0; i < SIZE_OF_INPUT; i++)
	{
		X[i].a = ab[i];
		X[i].b = 0.0;
	}

	printf ("*********Before*********\n");
	for (i = 1; i <= SIZE_OF_INPUT - 1; i++)
		printf ("X[%d]:real == %f  imaginary == %f\n", i, X[i].a, X[i].b);
	FFT();

	printf ("\n\n**********After*********\n");
	for (i = 1; i <= SIZE_OF_INPUT - 1; i++)
		printf ("X[%d]:real == %f  imaginary == %f\n", i, X[i].a, X[i].b);

	for(i = 1; i <= SIZE_OF_INPUT - 1; i++){
		X[i].a = pow(X[i].a, 2);
		X[i].b = pow(X[i].b, 2);
		ab[i] = X[i].a + X[i].b;
		ab[i] = sqrt(ab[i]);
		printf("Squared value [%d] = %f\n", i, ab[i]);
//		printf("%f\n", ab[i]); 
	}
	return 0;
}
