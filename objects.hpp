#pragma once
#include "models.hpp"

struct Transform
{
    float x = 0, y = 0, z = 0;
    float pitch = 0, yaw = 0, roll = 0;
};

class Object
{
    public:
        void draw(unsigned int modelLoc, unsigned int modelTypeLoc) const;
        virtual ~Object() = default;
        int fragment_id = 0;
        Transform *get_transform();

    protected:
        Transform transform;
        Model model;
        Object(float x, float y, float z, float pitch, float yaw, float roll, const Mesh &mesh, int fragment_id);
};

class Center : public Object
{
    public:
        Center(float x, float y, float z, float pitch, float yaw, float roll);
        ~Center();

    private:
        static Mesh create_mesh();
};

class Binary : public Object
{
    public:
        Binary(float x, float y, float z, float pitch, float yaw, float roll);
        ~Binary();

    private:
        static Mesh create_mesh();
};

class File : public Object
{
    public:
        File(float x, float y, float z, float pitch, float yaw, float roll);
        ~File();

    private:
        static Mesh create_mesh();
};

class Directory : public Object
{
    public:
        Directory(float x, float y, float z, float pitch, float yaw, float roll);
        ~Directory();

    private:
        static Mesh create_mesh();
};