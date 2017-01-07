#include "MemoryManager.h"

#include <fstream>


#include "QwixxException.h"

namespace{
   const size_t MAGIC_NUMBER=422413;
   const size_t VERSION=1;
}

void MemoryManager::load_memory(const std::string &filename, std::vector<float> &mem){
    std::ifstream file(filename.c_str(), std::ios::binary);
    
    //magic number:
    size_t magic_number;
    file.read((char *)&magic_number, sizeof(magic_number));
    if(magic_number!=MAGIC_NUMBER){
        THROW_QUIXX("wrong magic number ("<<magic_number<<") in file '"<<filename<<"'");
    }
    
    size_t version;
    file.read((char *)&version, sizeof(version));
    if(version!=VERSION){
        THROW_QUIXX("my version is "<<VERSION<<", cannot read version "<<version<<" of file '"<<filename<<"'");
    }
    
    
    //number if elements:
    size_t size;
    file.read((char *)&size, sizeof(size));
    
    mem.resize(size);
    
    if(size!=0){
       file.read((char *)&(mem.front()), size*sizeof(float));
    }
    
    if(!file.good())
        THROW_QUIXX("could not read file '"<<filename<<"'");
}



void MemoryManager::save_memory(const std::string &filename, const std::vector<float> &mem){
    std::ofstream file(filename.c_str(), std::ios::binary);
    
    //magic number:
    file.write((const char *)&MAGIC_NUMBER, sizeof(MAGIC_NUMBER));
    
    //version number:
    file.write((const char *)&VERSION, sizeof(VERSION));
    
    //number of elements:
    const size_t size=mem.size();
    file.write((const char *)&size, sizeof(size));
    
    if(size!=0){
       file.write((const char *)&(mem.front()), size*sizeof(float));
    }
    
    if(!file.good())
        THROW_QUIXX("could not write file '"<<filename<<"'");
}



