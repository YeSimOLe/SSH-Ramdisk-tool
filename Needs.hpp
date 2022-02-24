#ifndef Needs_hpp
#define Needs_hpp

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <array>
#define RESET   "\033[0m"
#define RED     "\033[31m"

const char* identifier = NULL;
const char* version = NULL;
const char* board = NULL;
const char* blob = NULL;
std::string input1, input2;

namespace Check {
  int Needs(){
    std::ifstream img4("/usr/local/bin/img4");
    if(!img4){
      std::cerr << "[!] Img4 not found." << std::endl;
      return -1;
    }
    std::ifstream img4tool("/usr/local/bin/img4tool");
    if(!img4tool){
      std::cerr << "[!] Img4tool not found." << std::endl;
      return -1;
    }
    std::ifstream kpatcher("/usr/local/bin/Kernel64Patcher");
    if(!kpatcher){
      std::cerr << "[!] Kernel64Patcher not found." << std::endl;
      return -1;
    }
    std::ifstream kairos("/usr/local/bin/kairos");
    if(!kairos){
      std::cerr << "[!] Kairos not found." << std::endl;
      return -1;
    }
    std::ifstream irecv("/usr/local/Cellar/libirecovery/1.0.0/bin/irecovery");
    std::ifstream irecovery("/usr/local/bin/irecovery");
    if(!irecv || !irecovery){
      std::cerr << "[!] Irecovery not found." << std::endl;
      return -1;
    }
    std::ifstream pzb("/usr/local/bin/pzb");
    if(!pzb){
      std::cerr << "[!] Partialzipbrowser not found." << std::endl;
      return -1;
    }
  return 0;
}
};

#endif /* Needs_hpp */
