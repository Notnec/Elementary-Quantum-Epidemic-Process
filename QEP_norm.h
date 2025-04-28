#ifndef QEPNORM_H
#define QEPNORM_H


double square_two_norm(double vec[2]){
    return vec[1]*vec[1] + vec[0]*vec[0];
}

double two_norm(double vec[2]){
    return sqrt(square_two_norm(vec));
}

void normalisation2(double vec[2]){
    double aux_n = two_norm(vec);
    vec[0] = vec[0] * sqrt(2) / aux_n;
    vec[1] = vec[1] * sqrt(2) / aux_n;
}




#endif // QEPNORM_H

