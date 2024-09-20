// username: joe password: skrt
// Websites should NOT store the password just as you typed it
// username | password
//    joe   |   something else (not "skrt")
//
// Instead, store the hash of that string. (With a “salt” commonly)
// The hash of a string is another string that is:
// - deterministically generate
// - difficult to reverse (if you have hash("skrt"), hard to find "skrt")
//
// Why? If they get hacked, then it's hard for the hacker to find your password even if they get this database table
// Easy to check passwords, because if you type it in the site can hash it again and check it against the stored hashed value
//
// We will try this with the MD5 hash function (and see how to “reverse” it with a lot of work)

#include <openssl/md5.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // MD5 hashes are always 16 bytes long (independent of length)
    printf("in: %s, len: %ld \n", argv[1], strlen(argv[1]));

    unsigned char hash[16];
    MD5(argv[1], strlen(argv[1]), hash); // result stored in hash
    for(int i = 0; i < 16; i += 1) {
        printf("%02x", hash[i]); // %02x means print as a hex value
    }
    printf("\n");
    for(int i = 0; i < 16; i += 1) {
        printf("%d ", hash[i]);
    }
    printf("\n");
}