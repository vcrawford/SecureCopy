#include <getopt.h>
#include <gcrypt.h>

// parse user input in order to get input file, local or not, ip address, and port
// Sets input_file, is_local, ip, and port to point to input values
void parseUserInput(int argc, char* argv[], char** input_file, bool* is_local,
	int* ip, int* port) {

	// nothing to parse
	if (argc < 2) return;

	// get file name

   *input_file = argv[1];

   // get all other input

   char* port_str;
   char* ip_str;

   int opt;

   while ((opt = getopt(argc, argv, "d:l")) != -1) {
      
      switch (opt) {
         
         case 'd':
        
            *is_local = false;

            // split the option argument at colon to get IP and port

            ip_str = strtok(optarg, ":");

            if (ip_str != NULL) {
               
               port_str = strtok(NULL, " ");

            	if (port_str != NULL) {
               
               	*ip = atoi(ip_str);
               	*port = atoi(port_str);
            	}
            }

            break;
	 
         case 'l':
            
            *is_local = true;
               
            break;
      }
   }

}

