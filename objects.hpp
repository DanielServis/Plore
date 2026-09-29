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
        Object(float x, float y, float z, const Mesh &mesh, int fragment_id);
};

class Binary : public Object
{
    public:
        Binary(float x, float y, float z);
        ~Binary();

    private:
        static Mesh create_mesh();
};

class File : public Object
{
    public:
        File(float x, float y, float z);
        ~File();

    private:
        static Mesh create_mesh();
};

class Directory : public Object
{
    public:
        Directory(float x, float y, float z);
        ~Directory();

    private:
        static Mesh create_mesh();
};