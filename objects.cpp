#include "objects.hpp"
#include <GL/glew.h>
#include <cmath>

Object::Object(float x, float y, float z, float pitch, float yaw, float roll, const Mesh &mesh, int fragment_id)
    : model(mesh), fragment_id(fragment_id)
{
    transform.x = x;
    transform.y = y;
    transform.z = z;
    transform.pitch = pitch;
    transform.yaw = yaw;
    transform.roll = roll;
}

void Object::draw(unsigned int modelLoc, unsigned int objectTypeLoc) const
{
    float Cy = cosf(transform.yaw), Sy = sinf(transform.yaw);
    float Cp = cosf(transform.pitch), Sp = sinf(transform.pitch);
    float Cr = cosf(transform.roll), Sr = sinf(transform.roll);

    float mat[16] = {
        Cy * Cr + Sy * Sp * Sr, Cp * Sr, -Sy * Cr + Cy * Sp * Sr, 0,
        -Cy * Sr + Sy * Sp * Cr, Cp * Cr, Sy * Sr + Cy * Sp * Cr, 0,
        Sy * Cp, -Sp, Cy * Cp, 0,
        transform.x, transform.y, transform.z, 1};

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, mat);
    glUniform1i(objectTypeLoc, fragment_id);
    model.draw();
}

Transform *Object::get_transform()
{
    return &transform;
}

Center::Center(float x, float y, float z, float pitch, float yaw, float roll) : Object(x, y, z, pitch, yaw, roll, create_mesh(), 0) {}

Center::~Center() {}

Mesh Center::create_mesh()
{
    Mesh mesh;

    mesh.vertices = {
        -0.5f, 0.5f, -1.0f, // front square
        0.5f, 0.5f, -1.0f,
        -0.5f, -0.5f, -1.0f,
        0.5f, -0.5f, -1.0f,
        -0.5f, 0.5f, 1.0f, // back square
        0.5f, 0.5f, 1.0f,
        -0.5f, -0.5f, 1.0f,
        0.5f, -0.5f, 1.0f,
        -1.0f, 0.5f, -0.5f, // left square
        -1.0f, 0.5f, 0.5f,
        -1.0f, -0.5f, -0.5f,
        -1.0f, -0.5f, 0.5f,
        1.0f, 0.5f, -0.5f, // right square
        1.0f, 0.5f, 0.5f,
        1.0f, -0.5f, -0.5f,
        1.0f, -0.5f, 0.5f,
        -0.5f, 1.0f, -0.5f, // top square
        0.5f, 1.0f, -0.5f,
        -0.5f, 1.0f, 0.5f,
        0.5f, 1.0f, 0.5f,
        -0.5f, -1.0f, -0.5f, // bottom square
        0.5f, -1.0f, -0.5f,
        -0.5f, -1.0f, 0.5f,
        0.5f, -1.0f, 0.5f
    };

    mesh.indices = {
        0, 1, 2, // squares
        2, 3, 1,
        4, 5, 6,
        6, 7, 5,
        8, 9, 10,
        10, 11, 9,
        12, 13, 14,
        14, 15, 13,
        16, 17, 18,
        18, 19, 17,
        20, 21, 22,
        22, 23, 21,
        0, 2, 8, // connecting squares
        2, 8, 10,
        1, 3, 12,
        3, 12, 14,
        9, 11, 6,
        4, 6, 9,
        13, 15, 5,
        15, 5, 7,
        16, 17, 0,
        0, 1, 17,
        18, 19, 4,
        19, 4, 5,
        16, 18, 8,
        18, 8, 9,
        17, 19, 12,
        19, 12, 13,
        20, 21, 2,
        2, 3, 21,
        22, 23, 6,
        6, 7, 23,
        20, 22, 10,
        10, 11, 22,
        21, 23, 14,
        14, 15, 23,
        0, 8, 16, // triangles front
        1, 12, 17,
        2, 10, 20,
        3, 14, 21,
        4, 9, 18, // triangles back
        5, 13, 19,
        6, 11, 22,
        7, 15, 23
    };

    return mesh;
}

Binary::Binary(float x, float y, float z, float pitch, float yaw, float roll) : Object(x, y, z, pitch, yaw, roll, create_mesh(), 1) {}

Binary::~Binary() {}

Mesh Binary::create_mesh()
{
    Mesh mesh;

    mesh.vertices = {
        0.5f, 0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, 
        -0.5f, 0.5f, -0.5f, 
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f, 
        -0.5f, -0.5f, 0.5f, 
        -0.5f, 0.5f, 0.5f 
    };

    mesh.indices = {
        0, 1, 2,    2, 3, 0, 
        4, 5, 6,    6, 7, 4,
        4, 0, 3,    3, 7, 4, 
        1, 5, 6,    6, 2, 1, 
        3, 2, 6,    6, 7, 3,
        4, 5, 1,    1, 0, 4,
    };

    return mesh;
}

File::File(float x, float y, float z, float pitch, float yaw, float roll) : Object(x, y, z, pitch, yaw, roll, create_mesh(), 2) {}

File::~File() {}

Mesh File::create_mesh()
{
    Mesh mesh;

    mesh.vertices = {
        0.5f, 0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, 
        -0.5f, 0.5f, -0.5f, 
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f, 
        -0.5f, -0.5f, 0.5f, 
        -0.5f, 0.5f, 0.5f 
    };

    mesh.indices = {
        0, 1, 2,    2, 3, 0, 
        4, 5, 6,    6, 7, 4,
        4, 0, 3,    3, 7, 4, 
        1, 5, 6,    6, 2, 1, 
        3, 2, 6,    6, 7, 3,
        4, 5, 1,    1, 0, 4,
    };

    return mesh;
}

Directory::Directory(float x, float y, float z, float pitch, float yaw, float roll) : Object(x, y, z, pitch, yaw, roll, create_mesh(), 3) {}

Directory::~Directory() {}

Mesh Directory::create_mesh()
{
    Mesh mesh;

    mesh.vertices = {
        0.5f, 0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, 
        -0.5f, 0.5f, -0.5f, 
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f, 
        -0.5f, -0.5f, 0.5f, 
        -0.5f, 0.5f, 0.5f 
    };

    mesh.indices = {
        0, 1, 2,    2, 3, 0, 
        4, 5, 6,    6, 7, 4,
        4, 0, 3,    3, 7, 4, 
        1, 5, 6,    6, 2, 1, 
        3, 2, 6,    6, 7, 3,
        4, 5, 1,    1, 0, 4,
    };

    return mesh;
}