#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <unistd.h>

main(int argc, char* argv[]) {

   // First, read in all user input

   // whether there has been an error in the user input and the program can't run
   bool input_error = false;

   // get the file that we are encrypting
   FILE* plntxt;

   if (argc < 2) {
      printf("You must input the name of the file to be encrypted.\n");
      input_error = true;
   }
   else {
      plntxt = fopen(argv[1], "r");

      if (plntxt == NULL) {
	 printf("There does not exist a file named %s.\n", argv[1]);
	 input_error = true;
      }
   }

   // get the options
   int opt;
   
   bool is_local;
   int ip;
   int port;
   char* port_str;
   char* ip_str;

   while ((opt = getopt(argc, argv, "d:l")) != -1) {
      switch (opt) {
	 case 'd':
	    is_local = false;

	    // split at colon to get IP and port
	    ip_str = strtok(optarg, ":");

	    if (ip_str == NULL) {
	       printf("The IP and port number should be entered as \"ip:port\".\n");
	       input_error = true;
	    }
	    else {
	       port_str = strtok(NULL, ":");

	       if (port_str == NULL) {
		  printf("The IP and port number should be entered as \"ip:port\".\n");
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
	       printf("The -d command must have an ip and port number.\n");
	       input_error = true;
	    }
	    else {
	       printf("%d is an unknown option.\n", optopt);
	    }
      }
   }
   

}

