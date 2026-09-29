#include "objects.hpp"
#include <GL/glew.h>
#include <cmath>



Object::Object(float x, float y, float z, const Mesh &mesh, int fragment_id)
    : model(mesh), fragment_id(fragment_id)
{
    transform.x = x;
    transform.y = y;
    transform.z = z;
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



Binary::Binary(float x, float y, float z) : Object(x, y, z, create_mesh(), 0) {}

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




File::File(float x, float y, float z) : Object(x, y, z, create_mesh(), 1) {}

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



Directory::Directory(float x, float y, float z) : Object(x, y, z, create_mesh(), 2) {}

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