#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>

std::string load_file(const std::string &path)
{
    std::ifstream file(path);
    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

std::string run_command(const std::string &command)
{
    std::string out;
    char buffer[256];
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        return out;
    }
    while (fgets(buffer, sizeof(buffer), pipe))
    {
        out += buffer;
    }
    pclose(pipe);
    return out;
}

bool is_binary(const std::string &path, size_t sample = 8192)
{
    std::ifstream f(path, std::ios::binary);
    if (!f)
        return false;

    char buf[8192];
    f.read(buf, std::min(sample, sizeof(buf)));
    std::streamsize n = f.gcount();

    for (std::streamsize i = 0; i < n; ++i)
        if (buf[i] == '\0')
            return true;
    return false;
}