#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec3 ModelPos;

uniform int ID;

void main()
{
    vec3 dx = dFdx(ModelPos);
    vec3 dy = dFdy(ModelPos);
    vec3 N = normalize(cross(dx, dy));
    vec3 absN = abs(N);
    vec3 colour;

    switch (ID)
    {
        case 0:
            colour = vec3(0.75, 0.75, 0.75);
            break;
        case 1:
            colour = vec3(0.75, 0.5, 0.5);
            break;
        case 2:
            colour = vec3(0.5, 0.5, 0.75);
            break;
        case 3:
            colour = vec3(0.5, 0.75, 0.5);
            break;
        default:
            colour = vec3(0.75, 0.75, 0.75);
    }

    vec3 viewDir = normalize(-FragPos);
    float fresnel = pow(1.0 - abs(dot(viewDir, N)), 3.0);
    colour += fresnel * 0.15;
    FragColor = vec4(colour, 1.0);
}