#include "anomaly_detection_util.h"
#include <cmath>

using namespace std;

float meanCalculate(float* x, int size) {
    float sum = 0;
    for(int i = 0; i < size; i++) {
        sum += x[i];
    }
    float u = (sum / (float)size);
    return u;
}

float var(float* x, int size) {
    if(size == 0) {
        return 0.0f;
    }
    float sum = 0;
    float u = meanCalculate(x, size);
    for(int i = 0; i < size; i++) {
        sum += pow(x[i], 2.0f);
    }
    sum = sum / (float) size;
    float variance = sum - pow(u, 2.0f);
    return variance;
}

float cov(float* x, float* y, int size) {
    if(size == 0) {
        return 0.0f;
    }
    float sum = 0;
    for(int i = 0; i < size; i++) {
        sum += x[i] * y[i];
    }
    sum /= (float)size;
    float covariance = sum - (meanCalculate(x,size) * meanCalculate(y, size));
    return covariance;
}

float pearson(float* x, float* y, int size) {
    float numerator = cov(x, y, size);
    float denominator = sqrt(var(x, size)) * sqrt(var(y, size));
//    if(denominator == 0) {
//        throw "dividing in zero";
//    }
    return numerator / denominator;
}

Line linear_reg(Point** points, int size) {
    float px[size], py[size];
    for(int i = 0; i < size; i++) {
        px[i] = points[i]->x;
        py[i] = points[i]->y;
    }
    float covariance = cov(px, py, size);
    float variance = var(px, size);
    float averageX = meanCalculate(px,size);
    float averageY = meanCalculate(py,size);
    float a = covariance / variance;
    float b = averageY - (a * averageX);
    Line l = Line(a, b);
    return l;
}

float dev(Point p,Point** points, int size) {
    Line l = linear_reg(points, size);
    float yOnLine = (l.a * p.x) + l.b;
    float distance = p.y - yOnLine;
    if(distance < 0) {
        return distance * -1;
    }
    return distance;
}

float dev(Point p,Line l) {
    float yOnLine = (l.a * p.x) + l.b;
    float distance = p.y - yOnLine;
    if(distance < 0) {
        return distance * -1;
    }
    return distance;
}