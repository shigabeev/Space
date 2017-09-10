#include <vector>
#include "tgaimage.hpp"
#include <iostream>
#include <math.h>
using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor gray = TGAColor(32, 32, 32, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
const int width  = 1000;
const int height = 1000;
float origin[] = {width/2, height/2};

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (abs(x0-x1)<abs(y0-y1)) {
        swap(x0, y0);
        swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;

        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}
float sqr(float a){
    return a*a;
}

float* sum(float P[], float Q[], int size){
    float* result = new float[size];
    for (int i=0; i<size; i++){
        result[i] = P[i] + Q[i];
    }
    return result;
}

float* neg(float P[], int size){
    float *Q = new float[size];
    for (int i=0; i<size; i++){
        Q[i] = - P[i];
    }
    return Q;
}

float dot_product(float P[], float Q[],int size){
    float sum = 0;
    for (int i=0; i<size; i++)
        sum += P[i]*Q[i];
    return sum;
}

float magnitude(float P[], int size) {
    float sum = 0;
    for(int i=0; i<size; i++)
        sum += P[i]*P[i];
    return sqrt(sum);
}

float* mult(float a, float P[], int size){
    float *Q = new float[size];
    for (int i=0; i<size; i++)
        Q[i] = P[i] * a;
    return Q;
}

float* proj(float P[], float Q[], int size){
    return mult(dot_product(P, Q, size)/sqr(magnitude(Q, size)), Q, size);
}

float* perp(float P[], float Q[], int size){
    //Under construction
    float* R = new float [size];
    return sum(P, neg(proj(P, Q, size), size), size);
}

void drawvec2(float tail[], float head[], TGAImage &image, TGAColor color){
    line(origin[0] + tail[0], origin[1] + tail[1], origin[0] + head[0], origin[1] + head[1], image, color);
}

void drawline(float tail[], float head[], TGAImage &image, TGAColor color){
    line(tail[0],tail[1],head[0],head[1], image, color);
}

void triangle(float t0[], float t1[], float t2[], TGAImage &image, TGAColor color){
    // sort so top point is t0, bottom point is t2
    if (t0[1] < t1[1]) swap(t0, t1);
    if (t0[1] < t2[1]) swap(t0, t2);
    if (t1[1] < t2[1]) swap(t1, t2);
    
    drawline(t0, t1, image, color); // l0
    drawline(t1, t2, image, color);
    drawline(t2, t0, image, color); // l2


    int height = t0[1]-t2[1];
    // calculate tan of 3 lines
    int dx0 = (t0[0] - t1[0]); // for l0
    int dx1 = (t1[0] - t2[0]); // for l1
    int dx2 = (t2[0] - t0[0]); // for l2
    
    int dy0 = (t0[1] - t1[1]);
    int dy1 = (t1[1] - t2[1]);
    int dy2 = (t2[1] - t0[1]);

    //for l0 and top part of l2
    int i = 0;
    for (int y=t0[1]; y>t1[1];y--, i++){
        int lbound = t0[0] - dx0*i/dy0;
        int rbound = t0[0] - dx2*i/dy2;
        if (lbound > rbound){
            swap(lbound, rbound);
            lbound++;
        }
        for (int x=lbound; x<=rbound; x++)
            image.set(x, y, color);
    }
    // for l1 and bottom part of l2
    for (int y=t2[1], i=0; y<=t1[1];y++, i++){
        int lbound = t2[0] + dx1*i/dy1;
        int rbound = t2[0] + dx2*i/dy2;
        if (lbound > rbound){
            swap(lbound, rbound);
            lbound++;
        }
        for (int x=lbound; x<rbound; x++)
            image.set(x, y, color);
    }
}

int main () {
    TGAImage image(width, height, TGAImage::RGB);
    float zero[] = {0, 0};
    // //draw coordinate origin
    // float i[] = {100, 0};
    // float j[] = {0, 100};
    // drawvec2(zero, i, image, red);
    // drawvec2(zero, j, image, green);

    // //these are more useful for vector operations that start from the center of screen 
    // float P[] = {200, 300};
    // float Q[] = {-100, 200};

    // // draw vectors
    // drawvec2(zero, P, image, white);
    // drawvec2(zero, Q, image, white);
    
    // // draw their projections on each other
    // drawvec2(zero, proj(Q, P, 2), image, green);
    // drawvec2(zero, proj(P, Q, 2), image, red);
    
    // // draw perpendiculars
    // drawvec2(proj(P, Q, 2), P, image, red);
    // drawvec2(proj(Q, P, 2), Q, image, green);
    
    //draw triangle. I draw it from bottom-left, so these variables more useful in that case
    float P[] = {200, 800};
    float Q[] = {500, 700};
    float R[] = {600, 400};
    triangle(P, Q, R, image, white);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}