#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>

double random_range(const double min, const double max){
    
    assert(min < max);
    return (max - min) * ((double)random() / (double)RAND_MAX) + min;
       
}

double gaussian(double x, double mi, double sigma){

    double raise = -1.0/2.0*powl((x-mi)/sigma, 2.0);
    return 1.0/(sqrt(2.0*M_PI)*sigma)*exp(raise);

}

double gen_point(double mi, double sigma){
   
    double lbound = mi - 5.0 * sigma;
    double rbound = mi + 5.0 * sigma;
    double tbound = 1.0 / (sqrt(2.0 * M_PI) * sigma);
    
    #ifdef DEB
    printf("top bound is %f \n", tbound);
    printf("right bound is %f \n", rbound);
    printf("left bound is %f \n", lbound);
    #endif
      
    double x, y;
               
    do{
        x = random_range(lbound, rbound);
        y = random_range(0.0, tbound);
        #ifdef DEBUG
        printf("x is %f \n", x);
        printf("y is %f \n", y);
        printf("normal in x is %f \n", gaussian(x, mi, sigma));
        #endif
    
    
    #ifdef DEBUG
    printf("ACCEPTED\n\n");
    #endif
    return x;
}

void gen_class(int id, FILE *fd, int d, int n, double sigma, double mi){

    #ifdef DEBUG
    printf("GENERATING CLASS %d\n", id);
    #endif
    for(int i=0; i<n;i++){
        for(int j=0; j < d; j++)
            fprintf(fd, "%f, ", gen_point(mi, sigma));
        fprintf(fd, "%d\n", id);
    }
    
    return;

}

int main(int argc, char **argv){

    assert(argc > 1);
    
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
    double sigma = c * sqrt(d);  //1,060660172
    #ifdef DEBUG
    printf("Sigma is %f \n", sigma);
    #endif    
        
    //Data file setup
    FILE *fd = NULL;
    char filedata[20];
    sprintf(filedata, "%ld.data", tid);
    fd = fopen(filedata, "w");
    //assert
    
    //Class generator -ver impar
    gen_class(0, fd, d, n/2, sigma, -1);
    gen_class(1, fd, d, n/2, sigma, 1);
    
    fclose(fd);
    
    //Names file
    FILE *fn = NULL;
    char filenames[20];
    sprintf(filenames, "%ld.names", tid);
    fn = fopen(filenames, "w");
    fprintf(fn, "0, 1.\n\n");
    for(int i=0; i<d; i++)
        fprintf(fn, "X%d: continuous.\n", i+1);
    fclose(fn);
    
    return 0;
 
}
