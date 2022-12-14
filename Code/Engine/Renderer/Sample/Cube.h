#pragma once

const int CubeVertexCount = 24;
const int CubeIndexCount = 36;

const float CubePoints[120] =
{
    -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,     0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,     1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,     1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,     0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,     0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,     0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,     0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,     0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,     1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,     0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,     0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,     1.0f, 0.0f,
};

const float CubeVertices[72] =
{
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
};

const float CubeNormals[72] =
{
     0.0f, -1.0f,  0.0f,
     0.0f, -1.0f,  0.0f,
     0.0f, -1.0f,  0.0f,
     0.0f, -1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,
     0.0f,  0.0f, -1.0f,
     0.0f,  0.0f, -1.0f,
     0.0f,  0.0f, -1.0f,
     0.0f,  0.0f, -1.0f,
     0.0f,  0.0f,  1.0f,
     0.0f,  0.0f,  1.0f,
     0.0f,  0.0f,  1.0f,
     0.0f,  0.0f,  1.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,
};

const float CubeTexCoords[48] =
{
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
};

const unsigned int CubeIndices[36] =
{
     0,  2,  1,
     0,  3,  2,
     4,  5,  6,
     4,  6,  7,
     8,  9, 10,
     8, 10, 11,
    12, 15, 14,
    12, 14, 13,
    16, 17, 18,
    16, 18, 19,
    20, 23, 22,
    20, 22, 21
};