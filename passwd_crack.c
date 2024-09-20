#include <openssl/md5.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t char_to_num(unsigned char c) {
    // for example, 'f' as input should return 15
    // '0' as input should return 0
    // 'a' as input should return 10;
    if(c >= 'a' && c <= 'f') { return c - 96 + 9; }
    if(c >= '0' && c <= '9') { return c - 48; }
}
void easy (unsigned char* input);
void hard (unsigned char* input);

int main(int argc, char** argv) {
    unsigned char* user_input = argv[1]; 

    printf("Now trying the Hard one: \n");
    hard(user_input);
    return 0;
}


void init_int_arr(int* arr, int value, int size)
{
    for(int i = 0; i < size; ++i)
        arr[i] = value;
}
void init_unchar_arr(unsigned char* arr, unsigned char value, int size)
{
    for(int i = 0; i < size; ++i)
        arr[i] = value;
}

void easy(unsigned char* user_input)
{
        // Big assumption – only finds 4-character passwords
    unsigned char potential_passwd[4];
    unsigned char tocheck[16];
    // unsigned char* user_input = argv[1]; //assume input is the hashed value in hexdecimal 
    //turning the hex value back to char
    for(int i = 0; i < 16; i += 1) {
        uint8_t char1v = char_to_num(user_input[i * 2]);
        uint8_t char2v = char_to_num(user_input[(i * 2) + 1]);
        tocheck[i] = char1v * 16 + char2v;
    }
    unsigned char hash[16];
    unsigned long attempts = 0;
    // Make this work for all keyboard characters (a-zA-Z0-9!@#$%^&*();',./:"<>?")
    // Verify that you can find 4-length password with other characters
    for(char c1 = 0; c1 <= 126; c1 += 1) {
        potential_passwd[0] = c1;
        for(char c2 = 0; c2 <= 126; c2 += 1) {
            potential_passwd[1] = c2;
            for(char c3 = 0; c3 <= 126; c3 += 1) {
                potential_passwd[2] = c3;
                for(char c4 = 0; c4 <= 126; c4 += 1) {
                    potential_passwd[3] = c4;
                    // we have a potential password to check
                    MD5(potential_passwd, 4, hash); // result stored in hash
                    attempts += 1;
                    if(strncmp(hash, tocheck, 16) == 0) {
                        printf("Found it! MD5(%.4s) = %s\n", potential_passwd, user_input);
                        printf("It took %ld attempts\n", attempts);
                        return;
                    }
                }
            }
        }
    }
    printf("Did not find a matching hash");
}

void hard(unsigned char* user_input)    //assume input is the hashed value in hexdecimal 
{
    unsigned char tocheck[16];
    //turning the hex value back to char
    for(int i = 0; i < 16; i += 1) {
        uint8_t char1v = char_to_num(user_input[i * 2]);
        uint8_t char2v = char_to_num(user_input[(i * 2) + 1]);
        tocheck[i] = char1v * 16 + char2v;
    }

    unsigned char potential_passwd[20]; //len: 4 - 20
    unsigned char hash[16];
    unsigned char loop[20];
    unsigned long attempts = 0;
    unsigned int potential_len = 1;
    while(potential_len <= 20)
    {   
        //reset the loop
        init_unchar_arr(loop, 0, 20); 
        init_unchar_arr(potential_passwd, 0, 20);
        unsigned int index = potential_len - 1;
        unsigned long this_attempts = 0;

        //the For loop
        while(loop[0] <= 127) 
        {
            //set the last 
            potential_passwd[index] = loop[index];
            MD5(potential_passwd, potential_len, hash);
            //attempts to solve hash
            attempts += 1;
            this_attempts += 1;
            if(strncmp(hash, tocheck, 16) == 0)
            {
                printf("Found it! MD5(%.20s) = %s, len = %d\n", potential_passwd, user_input, potential_len);
                printf("It took %ld attempts\n", attempts);
                return;  
            }
            
            //end of most inner loop
            loop[index] += 1;
            //while one of the loop is ended
            while(loop[index] > 127 && index > 0)
            {
                //reset the inner loop
                loop[index] = 0;
                index--;
                loop[index] += 1;
                //set char in the outter loop
                potential_passwd[index] = loop[index];
            }
            index = potential_len - 1;
        }
        printf("It took %ld attempts for n = %d\n", this_attempts, potential_len);
        // printf("\n\n");
        potential_len += 1;
    }
    printf("Did not find a matching hash\n");
    printf("It took %ld attempts\n", attempts);
}
// Bigger challenge: make this work for passwords of length 4 to 20
// “Easy” version is 20 nested loops. But can you do it without 20 nested loops?