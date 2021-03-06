#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>

//Generador de flotante aleatorio dentro de un rango
double random_range(const double min, const double max){
    
    assert(min < max);
    return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
       
}

//Función de densidad de la distribución normal en un punto
double gaussian(double x, double mi, double sigma){

    double raise = -1.0/2.0*powl((x-mi)/sigma, 2.0);
    return 1.0/(sqrt(2.0*M_PI)*sigma)*exp(raise);

}

//Generador de puntos con distribución normal
double gen_point(double mi, double sigma){
   
    double lbound = mi - 5.0 * sigma;
    double rbound = mi + 5.0 * sigma;
    double tbound = 1.0 / (sqrt(2.0 * M_PI) * sigma);
    
    #ifdef DEEP_DEBUG
    printf("top bound is %f \n", tbound);
    printf("right bound is %f \n", rbound);
    printf("left bound is %f \n", lbound);
    #endif
      
    double x, y;
               
    do{
        x = random_range(lbound, rbound);
        y = random_range(0.0, tbound);
        #ifdef DEEP_DEBUG
        printf("x is %f \n", x);
        printf("y is %f \n", y);
        printf("normal in x is %f \n", gaussian(x, mi, sigma));
        #endif
    } while( y > gaussian(x, mi, sigma));
    
    #ifdef DEEP_DEBUG
    printf("ACCEPTED\n\n");
    #endif
    return x;
}

//Generador de clase
void gen_class(int id, FILE *fd, int d, int n, double sigma, double mi){

    #ifdef DEBUG
    printf("GENERATING CLASS %d\n", id);
    #endif
    int i,j;
    for(i=0; i<n;i++){
        for(j=0; j < d; j++)
            fprintf(fd, "%f, ", gen_point(mi, sigma));
        fprintf(fd, "%d\n", id);
    }
    
    return;

}

int main(int argc, char **argv){

    if(argc < 4){
        printf("Uso: ./ej1 [dimensión][cantidad de puntos][valor de c]\n");
        return 0;
    }
    
    //Uso time como seed de random y para generar nombres de archivo unicos
    time_t tid = time(NULL);
    srand(tid);
    
    #ifdef DEBUG
    printf("Dataset ID is %ld\n", tid);
    #endif
    
    //Parse inputs
    int d = atoi(argv[1]);
    int n = atoi(argv[2]);
    double c = atof(argv[3]);
    #ifdef DEB
    printf("d is %d \n", d);
    printf("n is %d \n", n);
    printf("c is %f \n", c);
    #endif
    
    //Sigma 
    double sigma = c * sqrt(d);
    #ifdef DEBUG
    printf("Sigma is %f \n", sigma);
    #endif    
        
    //Data file setup
    FILE *fd = NULL;
    char filedata[20];
    sprintf(filedata, "%ld.data", tid);
    assert(fd = fopen(filedata, "w"));
   
    //Class generator
    if(n%2 == 0){
        gen_class(0, fd, d, n/2, sigma, -1);
        gen_class(1, fd, d, n/2, sigma, 1);
    } else {
        gen_class(0, fd, d, n/2, sigma, -1);
        gen_class(1, fd, d, n/2+1, sigma, 1);
    }
    
    fclose(fd);
    
    //Names file
    FILE *fn = NULL;
    int i;
    char filenames[20];
    sprintf(filenames, "%ld.names", tid);
    assert(fn = fopen(filenames, "w"));
    fprintf(fn, "0, 1.\n\n");
    for(i=0; i<d; i++)
        fprintf(fn, "X%d: continuous.\n", i+1);
    fclose(fn);
    
    return 0;
 
}
