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
const std::string Board;
const char* blob = NULL;
std::string input1, input2;

namespace Apple {
  int Needs(){

    std::cout << "[i] Quick dependency check first.." << '\n';
    sleep(1);

    std::ifstream img4("/usr/local/bin/img4");
    if(!img4){
      std::cout << "[i] Img4 not found. Installing it for you.." << '\n';
      system("curl -LO https://github.com/xerub/img4lib/releases/download/1.0/img4lib-2020-10-27.tar.gz");
      system("tar -xzf img4lib-2020-10-27.tar.gz");
      chdir("img4lib-2020-10-27/apple");
      system("cp -v img4 /usr/local/bin");
      chdir("../..");
      system("chmod +x /usr/local/bin/img4");
      system("rm -rf img4lib-2020-10-27 && rm -rf img4lib-2020-10-27.tar.gz");
    }
    std::ifstream img4tool("/usr/local/bin/img4tool");
    if(!img4tool){
      std::cerr << "[!] Img4tool not found. Installing it for you..." << '\n';
      system("curl -LO https://github.com/tihmstar/img4tool/releases/download/197/buildroot_macos-latest.zip");
      system("unzip buildroot_macos-latest.zip");
      chdir("buildroot_macos-latest");
      system("rsync --ignore-existing -avhuK ./usr \"/\"");
      chdir("..");
      system("chmod +x /usr/local/bin/img4tool");
      system("rm -rf buildroot_macos-latest.zip && rm -rf buildroot_macos-latest");
    }
    std::ifstream kpatcher("/usr/local/bin/Kernel64Patcher");
    if(!kpatcher){
      std::cerr << "[!] Kernel64Patcher not found. Installing it for you.." << '\n';
      system("git clone https://github.com/Ralph0045/Kernel64Patcher.git");
      chdir("Kernel64Patcher");
      system("gcc Kernel64Patcher.c -o Kernel64Patcher");
      system("cp -v Kernel64Patcher /usr/local/bin");
      system("rm -rf Kernel64Patcher");
    }
    std::ifstream kairos("/usr/local/bin/kairos");
    if(!kairos){
      std::cerr << "[!] Kairos not found. Installing it for you.." << '\n';
      system("git clone https://github.com/dayt0n/kairos.git");
      chdir("kairos");
      system("make");
      system("cp -v kairos /usr/local/bin");
      chdir("..");
      system("rm -rf kairos");
    }
    std::ifstream irecv("/usr/local/Cellar/libirecovery/1.0.0/bin/irecovery");
    std::ifstream irecovery("/usr/local/bin/irecovery");
    if(!irecv || !irecovery){
      std::cerr << "[!] Irecovery not found. Installing it for you.." << '\n';
      system("brew install libirecovery");
    }
    std::ifstream pzb("/usr/local/bin/pzb");
    if(!pzb){
      std::cerr << "[!] Partialzipbrowser not found. Installing it for you.." << std::endl;
      system("curl -LO https://github.com/tihmstar/partialZipBrowser/releases/download/36/buildroot_macos-latest.zip");
      system("unzip buildroot_macos-latest.zip");
      chdir("buildroot_macos-latest/usr/local/bin");
      system("cp -v pzb /usr/local/bin");
      chdir("../../../..");
      system("chmod +x /usr/local/bin/pzb && rm -rf buildroot_macos-latest.zip && rm -rf buildroot_macos-latest");
    }
    std::cout << "[i] Done!" << '\n';
    system("clear");
  return 0;
}
};

namespace linux {

};

#endif
