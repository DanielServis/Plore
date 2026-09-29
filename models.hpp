#pragma once
#include <vector>

struct Mesh
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

class Model
{
    public:
        Model(const Mesh &mesh);
        virtual ~Model();
        void draw() const;

        Model(const Model &) = delete;
        Model &operator=(const Model &) = delete;
        Model(Model &&) = default;
        Model &operator=(Model &&) = default;

    private:
        unsigned int VAO, VBO, EBO;
        int index_count;
};