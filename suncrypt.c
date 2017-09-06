#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <gcrypt.h>
#include "cryptutil.c"

int main(int argc, char* argv[]) {


   /***** Read in all user input *****/

   // input file name
   char* input_file;
   bool is_local = false;
   int ip = -1;
   int port = -1;

   parseUserInput(argc, argv, &input_file, &is_local, &ip, &port);


   /***** Check that input is valid ****/

   FILE* input;

   input = fopen(input_file, "r");

   if (input == NULL) {

      printf("Invalid file name. The program will exit.\n");

      exit(EXIT_FAILURE);
   }

   if ((!is_local) && ((port == -1) || (ip == -1))) {

      printf("Valid input is the following: suncrypt <input file> [-d < IP-addr:port >][-l]\n");
      printf("The program will exit.\n");

      exit(EXIT_FAILURE);
   }


   /***** Get user password *****/

   printf("Password (maximum of 100 characters): ");
   char password [100];
   scanf("%s", password);


   // now generate a key using that password

   char salt [5] = "NaCl";
   unsigned char key [16];

   gcry_kdf_derive(password, strlen(password), GCRY_KDF_PBKDF2, GCRY_MD_SHA512, salt, 4, 4096, 16, key);

   printf("Key: ");

   for (int i = 0; i < 16; i++) {

      printf("%X ", key[i]);
   }
   
   printf("\n");


   // read in the file
   
   char c;
   int text_size = 0;

   while ((c = getc(input)) != EOF) {

      text_size++;
   }

   char text [text_size];

   rewind(input);

   int i = 0;

   while ((c = getc(input)) != EOF) {

      text[i] = c;
      i++;
   }

   // encrypt the file

   gcry_cipher_hd_t handle;
   gcry_cipher_open(&handle, GCRY_CIPHER_RIJNDAEL128, GCRY_CIPHER_MODE_CFB, 0);
   gcry_cipher_setkey(handle, key, 16);

   char iv [16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 212};
   //char iv [16] = "just for testing";
   //unsigned short iv = 5844; 
   gcry_cipher_setiv(handle, &iv, 16);

   for (int i = 0; i < text_size; i++) {

      printf("%c,", text[i]);
   }

   gcry_cipher_encrypt(handle, text, text_size, NULL, 0);

   for (int i = 0; i < text_size; i++) {

      printf("%c,", text[i]);
   }

   printf("\n");

   gcry_cipher_setiv(handle, &iv, 16);
   gcry_cipher_decrypt(handle, text, text_size, NULL, 0);

   for (int i = 0; i < text_size; i++) {

      printf("%c,", text[i]);
   }

   printf("\n");

}

