#include <iostream>
#include <spawn.h>
#include <unistd.h>

// sshd start for starting ssh daemon ig?

int main(int argc, char const *argv[]) {
  std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
  std::cout << "@                            @" << std::endl;
  std::cout << "@    Usb and mounting shit   @" << std::endl; // if you look closely this shit spawns in the fucking verbose messages
  std::cout << "@                            @" << std::endl;
  std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
  int pid, i;
   char *arg[] = {"mount_apfs", "/dev/disk0s1", "/mnt1", NULL}; // /dev/disk0s1s1 == rootfs
   posix_spawn(&pid, "/System/Library/Filesystems/apfs.fs/mount_apfs", NULL, NULL, (char* const*)arg, NULL);
   waitpid(pid, &i, 0);
   //
   char *arg3[] = {"bash","-c","/System/Library/Filesystems/apfs.fs/mount_apfs /dev/disk0s2 /mnt2", NULL}; // /private/var is mounted on this one
   posix_spawn(&pid, "/bin/bash",NULL, NULL, (char* const*)arg3, NULL);
   waitpid(pid, &i, 0);
   sleep(1);
   //
   char *arg4[] = {"restored_external", "-server", NULL};
   posix_spawn(&pid, "/usr/local/bin/restored_external_original", NULL, NULL, (char* const*)arg4, NULL);
   waitpid(pid, &i, 0);
   sleep(1);
   //
   sleep(99999);
  return 0;
}
