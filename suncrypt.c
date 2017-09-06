#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <gcrypt.h>

int main(int argc, char* argv[]) {

   // First, read in all user input

   // whether there has been an error in the user input and the program can't run
   bool input_error = false;

   // get the file that we are encrypting
   char* plntxt_nm;
   FILE* plntxt;

   if (argc < 3) {

      // can't possible put less than 3 inputs
      input_error = true;
   }
   else {

      plntxt_nm = argv[1];

      plntxt = fopen(plntxt_nm, "r");

      if (plntxt == NULL) {
         printf("There does not exist a file named %s.\n", plntxt_nm);
         input_error = true;
      }
   }

   // get the options
   
   bool is_local = false;
   char* port_str;
   char* ip_str;   
   int ip = -1;
   int port = -1;

   int opt;

   while ((opt = getopt(argc, argv, "d:l")) != -1) {
      
      switch (opt) {
         
         case 'd':
        
            is_local = false;

            // split the option argument at colon to get IP and port

            ip_str = strtok(optarg, ":");

            if (ip_str == NULL) {
               
               input_error = true;
            }
            else {
               
               port_str = strtok(NULL, ":");

               if (port_str == NULL) {

                  input_error = true;
               }
            }

            if ((ip_str != NULL) && (port_str != NULL)) {
               
               ip = atoi(ip_str);
               port = atoi(port_str);
            }

            break;
	 
         case 'l':
            
            is_local = true;
               
            break;

         case '?':

            if (optopt == 'd') {

               // -d was input but without ip:port argument

               input_error = true;
            }
            else {

               printf("%d is an unknown option.\n", optopt);
            }

            break;
      }
   }
   

   // make sure that we have all needed options

   if ((!is_local) && ((port == -1) || (ip == -1))) {

      // if it isn't local, we should have a valid port and ip
      input_error = true;
   }

   if (input_error) {

      printf("Valid input is the following: suncrypt <input file> [-d < IP-addr:port >][-l]\n");
      printf("Terminating the program due to invalid input.\n");

      exit(EXIT_FAILURE);
   }


   // take in the user's password

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
   int file_size = 0;

   while ((c = getc(plntxt)) != EOF) {

      file_size++;
   }

   char input [file_size];

   rewind(plntxt);

   int i = 0;

   while ((c = getc(plntxt)) != EOF) {

      input[i] = c;
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

   for (int i = 0; i < file_size; i++) {

      printf("%c,", input[i]);
   }

   gcry_cipher_encrypt(handle, input, file_size, NULL, 0);

   for (int i = 0; i < file_size; i++) {

      printf("%c,", input[i]);
   }

   printf("\n");

   gcry_cipher_setiv(handle, &iv, 16);
   gcry_cipher_decrypt(handle, input, file_size, NULL, 0);

   for (int i = 0; i < file_size; i++) {

      printf("%c,", input[i]);
   }

   printf("\n");

}

