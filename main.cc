
#include "include/Needs.hpp"
#include "include/Components.hpp"

int main(int argc, char **argv){
 
 if(argc < 2){
  std::cout << "Example: " << argv[0] << " -d iPad7,5 -i 14.5.1 -b j71bap -s bla/blob.shsh2" << std::endl;
   return -1;
 }
 while(--argc > 0){
  const char *args = *++argv;
   if(*args == '-') switch(args[1]){
    case 'h':
     load::Help();
     return -1;
    case 'd':
     if(argc >= 2){ identifier = *++argv; argc--; continue; }
    case 'i':
     if(argc >= 2){ version = *++argv; argc--; continue; }
    case 'b':
     if(argc >= 2){ board = *++argv; argc--; continue; }
    case 's':
     if(argc >= 2){ blob = *++argv; argc--; continue; }
    std::cerr << "'" << args << "'" << " And... apples, bananas?\n Use '-h' for options" << std::endl;
    return -1;
    default:
     std::cerr << "'" << args << "'" << "? " << "There's just 4 damn options." << std::endl;
     return -1;
   }
 }
 
Apple::Needs(); // dependency check apple
 
std::ifstream dirchk((std::string("Patched") + "_" + version + "/ramdisk").c_str());
if(dirchk){
  std::cout << "[i] Patched dir already exists. Going straight to booting.." << std::endl;
  std::cout << RED << "You ready to boot? y/n " << RESET;
  std::cin >> load::inpt.usrin;
  if(load::inpt.usrin == "y"){
    load::Ramdisk(version);
    return 0;
  }
  else {
    std::cout << "Alright. Run it when you're ready :)" << std::endl;
    return 0;
  }
}
std::cout << "   @@@@@ " << RED <<  "SSH Ramdisk Maker & Loader" << RESET << " @@@@@" << std::endl;
sleep(2);

std::cout << RED << load::Time() << RESET << " [1] Getting all the components.." << std::endl;
load::GetManifest(identifier, version);
load::Getkeyspage(identifier, version);
load::iBSS(load::tolower(board));
load::iBEC(load::tolower(board));
load::kernel(load::tolower(board));
load::DeviceTree(load::tolower(board));
load::ramdisk(identifier, version);
load::trsc(identifier, version);
std::ifstream check(load::ibss.name);
if(!check) {
  std::cerr << "[!] Something went wrong. Maybe your network connection? Exiting..." << std::endl;
  return -1;
}
else {
  std::cout << "[i] Done!" << std::endl << std::endl;
  sleep(2);
}

std::cout << RED << load::Time() << RESET << " [2] Converting blob to IM4M" << std::endl;
sleep(1);
system((std::string("img4tool -e -s ") + blob + " -m IM4M").c_str());
std::cout << "[!] Done!" << std::endl << std::endl;

std::cout << RED << load::Time() << RESET << " [3] Decrypting " << load::ipsw.ramdisk << "..." << std::endl;
system((std::string("img4 -i ") + load::ipsw.ramdisk + " -o ramdisk.dmg").c_str());
std::cout << "[i] Done!" << std::endl << std::endl;
sleep(1);

std::cout << RED << " [i] Beginning to patch and sign the components.." << RESET << std::endl << std::endl;

std::cout << RED << load::Time() << RESET << " [4] Decrypting & Patching iBSS.." << RESET << std::endl;
system((std::string("img4 -i ") + load::ibss.name + " -o ibss.raw -k " + load::iBSSIV() + load::iBSSKEY()).c_str());
system("kairos ibss.raw ibss.pwn");
system("img4 -i ibss.pwn -o iBSS.img4 -M IM4M -A -T ibss");
std::cout << "[!] Done!" << std::endl << std::endl;
sleep(1);

std::cout << RED << load::Time() << RESET << " [5] Decrypting & Patching iBEC.." << RESET << std::endl; // 'RED' and 'RESET' s definition is located in "Needs.hpp"
system((std::string("img4 -i ") + load::ibec.name + " -o ibec.raw -k " + load::iBECIV() + load::iBECKEY()).c_str());
system("kairos ibec.raw ibec.pwn -b \"rd=md0 -restore serial=3 -v\"");
system("img4 -i ibec.pwn -o iBEC.img4 -M IM4M -A -T ibec");
std::cout << "[!] Done!" << std::endl << std::endl;
sleep(1);

std::cout << RED << load::Time() << RESET << " [6] Decrypting & Patching kernel.." << RESET << std::endl;
system((std::string("img4 -i ") + load::ipsw.Kernel + " -o kernel.raw").c_str());
load::check(version); // if the version is above 14.8 it will not only patch amfi
sleep(1);

std::cout << RED << load::Time() << RESET << " [7] Converting DeviceTree to rdtr.." << RESET << std::endl; // rdtr == restore devicetree
system((std::string("img4 -i ") + load::ipsw.Devicetree + " -o DeviceTree.img4 -M IM4M -T rdtr").c_str());
std::cout << "[!] Done!" << std::endl << std::endl;
sleep(1);

std::cout << RED << load::Time() << RESET << " [8] stitching IM4M to trustcache.." << RESET << std::endl;
system((std::string("img4 -i ") + load::ipsw.trustcache + " -o Trustcache.img4 -M IM4M").c_str());
std::cout << "[!] Done!" << std::endl << std::endl;
sleep(1);
chdir("..");
mkdir((std::string("Patched") + "_" + version).c_str(), S_IRWXU);
chdir((std::string("WD_") + identifier + "_" + version).c_str());
system((std::string("mv Trustcache.img4 ../Patched") + "_" + version + "; mv iBEC.img4 ../Patched" + "_" + version + "; mv iBSS.img4 ../Patched" + "_" + version + "; mv KernelCache.img4 ../Patched" + "_" + version + "; mv DeviceTree.img4 ../Patched" + "_" + version).c_str());
std::cout << "[i] Done!" << std::endl << std::endl;
sleep(1);
chdir("..");
system("clear");

std::cout << "[i] Patched bootchain now only the ramdisk is left to customise" << std::endl << "Wanna customise the ramdisk yourself or should i? type n to exit & y to proceed: ";
std::cin >> load::ipsw.usrin;
if(load::ipsw.usrin == "n"){
  return 0;
}
else if(load::ipsw.usrin == "y"){
chdir((std::string("WD_") + identifier + "_" + version).c_str());
system("hdiutil resize -size 160mb ramdisk.dmg");
load::mount();
chdir("..");
system((std::string("xcrun -sdk iphoneos clang++ -arch arm64 ") + "Stuff/restored_external.cc -o restored_external" + " -std=c++11 -Wno-write-strings").c_str());
system("ldid2 -S restored_external");
system((std::string("mv -v ") + load::ipsw.rdpath + "/usr/local/bin/restored_external " + load::ipsw.rdpath + "/usr/local/bin/restored_external_original").c_str());
system((std::string("mv -v restored_external ") + load::ipsw.rdpath + "/usr/local/bin/restored_external").c_str());
chdir((std::string("WD_") + identifier + "_" + version).c_str());


std::string debs[] = {
"https://apt.bingner.com/debs/550.58/shell-cmds_118-8_iphoneos-arm.deb",
"https://apt.bingner.com/debs/550.58/bash_5.0.3-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/ldid_2:2.0.1-2_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/make_4.2.1-2_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/nano_4.5-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/sed_4.5-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/system-cmds_790.30.1-2_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/tar_1.33-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/unzip_6.0-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/tree_1.8.0-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/grep_3.1-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/img4tool_197-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/550.58/coreutils-bin_8.31-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/550.58/coreutils_8.31-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/com.bingner.plutil_0.2.1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/firmware-sbin_0-1_iphoneos-all.deb",
"https://apt.bingner.com/debs/1443.00/launchctl-25_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/openssh-global-listener_8.4-2_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/openssh-server_8.4-2_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/openssh_8.4-2_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/openssl_1.1.1i-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/openssh-client_8.4-2_iphoneos-arm.deb"
};
// if you want to add more debs just put the link in the array above and the deb filename in the array below :)
std::string deb[] = {
"shell-cmds_118-8_iphoneos-arm.deb",
"bash_5.0.3-1_iphoneos-arm.deb",
"ldid_2:2.0.1-2_iphoneos-arm.deb",
"make_4.2.1-2_iphoneos-arm.deb",
"nano_4.5-1_iphoneos-arm.deb",
"sed_4.5-1_iphoneos-arm.deb",
"system-cmds_790.30.1-2_iphoneos-arm.deb",
"tar_1.33-1_iphoneos-arm.deb",
"unzip_6.0-1_iphoneos-arm.deb",
"tree_1.8.0-1_iphoneos-arm.deb",
"grep_3.1-1_iphoneos-arm.deb",
"img4tool_197-1_iphoneos-arm.deb",
"coreutils-bin_8.31-1_iphoneos-arm.deb",
"coreutils_8.31-1_iphoneos-arm.deb",
"com.bingner.plutil_0.2.1_iphoneos-arm.deb",
"firmware-sbin_0-1_iphoneos-all.deb",
"launchctl-25_iphoneos-arm.deb",
"openssh-global-listener_8.4-2_iphoneos-arm.deb",
"openssh-server_8.4-2_iphoneos-arm.deb",
"openssh_8.4-2_iphoneos-arm.deb",
"openssl_1.1.1i-1_iphoneos-arm.deb",
"openssh-client_8.4-2_iphoneos-arm.deb"
};
mkdir("bins", S_IRWXU);
mkdir("temp", S_IRWXU);
chdir("temp");

for(auto &i : debs) {
  system((std::string("curl -C - -s -LO ") + i).c_str());
}
for(auto &i : deb) {
std::ifstream dir((i + ".dir"));
if(!dir){
  mkdir((std::string(i) + ".dir").c_str(), S_IRWXU);
}
chdir(((std::string(i) + ".dir").c_str()));
system((std::string("ar -x ../") + i).c_str());
chdir("..");
}
for(auto &i : deb) {
  chdir(((std::string(i) + ".dir").c_str()));
  system((std::string("tar -C ../../bins -xzvkf data.*")).c_str());
  chdir("..");
}
chdir("..");
std::cout << RED << load::Time() << RESET << "[i] I've put sudo before the rsync command so it copys everything and nothing is left behind so it will ask for your password." << std::endl << "If your not comfortable with it just hit n it will run without sudo and y with sudo: ";
std::cin >> input1;
if(input1 == "y"){
system((std::string("sudo rsync --ignore-existing -avhuK --progress ./bins/ \"") + load::ipsw.rdpath + "/\"").c_str());
}
else if(input1 == "n"){
system((std::string("rsync --ignore-existing -avhuK --progress ./bins/ \"") + load::ipsw.rdpath + "/\"").c_str());
}
system((std::string("hdiutil detach ") + load::ipsw.rdpath).c_str());
std::cout << "[!] Done!" << std::endl << std::endl;
sleep(1);
std::cout << RED << load::Time() << RESET << "[9] Packing ramdisk back.." << std::endl;
system("img4 -i ramdisk.dmg -o ramdisk -M IM4M -A -T rdsk");
system("clear");
system((std::string("mv -v ramdisk ") + "../Patched" + "_" + version).c_str());
chdir("..");
}
else {
  return 0;
} //
 // end else statement
std::cout << "[i] Would you like to boot the ramdisk now? y/n: ";
std::cin >> input2;
if(input2 == "y"){
  load::Ramdisk(version);
}
else {
  return 0;
}
} // end main function
