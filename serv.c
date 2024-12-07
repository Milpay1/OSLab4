#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include <signal.h> // for signal handling
#include <unistd.h>     // For close() function
#include "list.h"

#define PORT 9001
#define ACK "ACK"

// Global variables for cleanup
int servSockD = -1;
int clientSocket = -1;
list_t *mylist = NULL;

// Signal handler for graceful termination
void handle_termination(int sig) {
    if (mylist) {
        list_free(mylist);
        mylist = NULL;
    }
    if (clientSocket != -1) {
        close(clientSocket);
        clientSocket = -1;
    }
    if (servSockD != -1) {
        close(servSockD);
        servSockD = -1;
    }
    printf("\nServer terminated gracefully.\n");
    exit(0);
}

int main(int argc, char const* argv[]) 
{ 
    // Register signal handlers
    signal(SIGINT, handle_termination);
    signal(SIGTERM, handle_termination);

    int n, val, idx;

    // create server socket
    servSockD = socket(AF_INET, SOCK_STREAM, 0); 
  
    // string store data to recv/send to/from client 
    char buf[1024];
    char sbuf[1024];
    char* token;

    // define server address 
    struct sockaddr_in servAddr; 
  
    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(PORT); 
    servAddr.sin_addr.s_addr = INADDR_ANY; 
  
    // bind socket to the specified IP and port 
    bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr)); 
  
    // listen for connections 
    listen(servSockD, 1); 
  
    // accept client connection
    clientSocket = accept(servSockD, NULL, NULL); 

    // Initialize the list
    mylist = list_alloc();

    while (1) {
        // Receive messages from client socket 
        n = recv(clientSocket, buf, sizeof(buf), 0);
        buf[n] = '\0';

        if (n > 0) {
            token = strtok(buf, " ");
    
            if (strcmp(token, "exit") == 0) {
                handle_termination(SIGINT); // Gracefully terminate
            }
            else if (strcmp(token, "get_length") == 0) {
                // Call list_length
                val = list_length(mylist);
                sprintf(sbuf, "Length = %d", val);
            }
            else if (strcmp(token, "add_front") == 0) {
                token = strtok(NULL, " ");  // get next token (value)
                val = atoi(token);
                list_add_to_front(mylist, val);
                sprintf(sbuf, "%s%d", ACK, val);
            }
            else if (strcmp(token, "remove_position") == 0) {
                token = strtok(NULL, " ");
                idx = atoi(token);
                val = list_remove_at_index(mylist, idx);
                sprintf(sbuf, "%s%d", ACK, val);
            }
            else if (strcmp(token, "print") == 0) {
                sprintf(sbuf, "%s", listToString(mylist));
            }
            else if (strcmp(token, "add_back") == 0) {
                token = strtok(NULL, " "); // get the value
                val = atoi(token);
                list_add_to_back(mylist, val);
                sprintf(sbuf, "%s%d", ACK, val);
            }
            else if (strcmp(token, "add_position") == 0) {
                token = strtok(NULL, " "); // get index
                idx = atoi(token);

                token = strtok(NULL, " "); // get value
                val = atoi(token);

                list_add_at_index(mylist, val, idx);
                sprintf(sbuf, "%s%d", ACK, val);
            }
            else if (strcmp(token, "remove_back") == 0) {
                val = list_remove_from_back(mylist);
                sprintf(sbuf, "%s%d", ACK, val);
            }
            else if (strcmp(token, "remove_front") == 0) {
                val = list_remove_from_front(mylist);
                sprintf(sbuf, "%s%d", ACK, val);
            }
            else if (strcmp(token, "get") == 0) {
                token = strtok(NULL, " "); // Get the index
                idx = atoi(token);

                int elem = list_get_elem_at(mylist, idx);

                if (elem == -1) {
                    sprintf(sbuf, "Error: Invalid index");
                } else {
                    sprintf(sbuf, "Element at index %d: %d", idx, elem);
                }
            }
            else {
                sprintf(sbuf, "Error: Unknown command");
            }
            // Send message to client socket 
            send(clientSocket, sbuf, sizeof(sbuf), 0);
        }
        memset(buf, '\0', 1024);
    }
  
    return 0; 
}
