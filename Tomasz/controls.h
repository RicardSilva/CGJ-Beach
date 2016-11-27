#ifndef CONTROLS_H
#define CONTROLS_H

#include "matrix4d.h"
#include "matrixFactory.h"

void computeMatricesFromInputs();
matrix4d getViewMatrix();
matrix4d getProjectionMatrix();

#endif