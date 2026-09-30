#pragma once

std::string load_file(const std::string &path);

std::string run_command(const std::string &command);

bool is_binary(const std::string &path, size_t sample = 8192);