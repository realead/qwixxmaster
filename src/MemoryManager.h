#pragma once

#include <string>
#include <vector>

namespace MemoryManager{
   void load_memory(const std::string &filename, std::vector<float> &mem);
   void save_memory(const std::string &filename, const std::vector<float> &mem);
}


