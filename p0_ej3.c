#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <string.h>

struct Point{
    double x_coord;
    double y_coord;
} Point;

//Generador de flotante aleatorio dentro de un rango
double random_range(const double min, const double max){
    
    assert(min < max); //ver 0
    return (max - min) * ((double)random() / (double)RAND_MAX) + min;
       
}

int is_class_0(double theta, double ro){

int i = -2;
while (i < 5){
    if((theta + (i*M_PI))/(4.0*M_PI) <= ro && ro<= (theta+(i+1)*M_PI)/(4.0*M_PI))
        return 1;
    i = i + 2;
}

return 0;

}

//Generador de puntos de acuerdo a la clase
struct Point gen_point(int c){
      
    struct Point p;
    double x, y, ro, theta;
    
    do{           
        do{
            x = random_range(-1.0, 1.0);
            y = random_range(-1.0, 1.0);
            ro = sqrt(x*x + y*y);
        } while(ro > 1);
        theta = atan2(y,x);
//        if(theta < 0)
//            theta = theta + 2*M_PI;
    } while((c==0 &&  is_class_0(theta, ro)) || (c==1 && !(is_class_0(theta, ro))));
   
    #ifdef DEEP_DEBUG
    printf("x is %f \n", x);
    printf("y is %f \n", y);
    printf("ro is %f \n", ro);
    printf("theta is %f \n", theta);
    printf("ACCEPTED\n\n");
    #endif
    p.x_coord = x;
    p.y_coord = y;

    return p;
}

//Generador de clase
void gen_class(int id, FILE *fd, int n){

    assert(id == 0 || id == 1);
    #ifdef DEBUG
    printf("GENERATING CLASS %d\n", id);
    #endif
    int i,j;
    struct Point p;
    for(i=0; i<n;i++){
        p = gen_point(id);
        fprintf(fd, "%f, %f, ", p.x_coord, p.y_coord);
        fprintf(fd, "%d\n", id);
    }
    
    return;

}

int main(int argc, char **argv){

    if(argc < 2){
        printf("Uso: ./ej3 [cantidad de puntos]");
        return 0;
    }
    
    //Uso time como seed de random y para generar nombres de archivo unicos
    time_t tid = time(NULL);
    srand(tid);
    
    #ifdef DEBUG
    printf("Dataset ID is %ld\n", tid);
    #endif
    
    //Parse inputs
    int n = atoi(argv[1]);
    #ifdef DEB
    printf("n is %d \n", n);
    #endif
    
    //Data file setup
    FILE *fd = NULL;
    char filedata[20];
    sprintf(filedata, "%ld.data", tid);
    assert(fd = fopen(filedata, "w"));
   
    //Class generator
    if(n%2 == 0){
        gen_class(0, fd, n/2);
        gen_class(1, fd, n/2);
    } else {
        gen_class(0, fd, n/2);
        gen_class(1, fd, n/2+1);
    }
    
    fclose(fd);
    
    //Names file
    FILE *fn = NULL;
    char filenames[20];
    sprintf(filenames, "%ld.names", tid);
    assert(fn = fopen(filenames, "w"));
    fprintf(fn, "0, 1.\n\n");
    fprintf(fn, "X: continuous.\n");    
    fprintf(fn, "Y: continuous.");
    fclose(fn);
    
    return 0;
 
}
