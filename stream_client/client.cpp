#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <cstring>
#include <netinet/in.h>


int main(int argc,char *argv[]){
    
    if(argc < 4){
        std::cout << "Not all arguments are entered" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    const char * server_ip = argv[2];
    int port = std::stoi(argv[3]);
    

    int client_socket = socket(AF_INET,SOCK_STREAM,0);

    if(client_socket == -1){

        std::cout << "Error socket" << std::endl;
        return 1;
    }

    struct sockaddr_in server_address;

    memset(&server_address,0,sizeof(server_address));

    server_address.sin_port = htons(port);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);

    std::ifstream readfile(filename,std::ios::binary);

    if(!readfile.is_open()){
        std::cout << "Error open file" << std::endl;
        close(client_socket);
        return 1;
    }

    int result_connect = connect(client_socket,(struct sockaddr *)&server_address,sizeof(server_address));

    if(result_connect < 0){
        std::cout << "Error connect" << std::endl;
        close(client_socket);
        return 1;
    }

    char buffer[4096];
    int counter = 1;
    int finish_counter = 0;
    while(readfile.read(buffer,sizeof(buffer)) || readfile.gcount() > 0){
        std::streamsize count_result = readfile.gcount();
        if(count_result > 0){
            ssize_t result_send = send(client_socket,buffer,count_result,0);
            if(result_send < 0){
                std::cout << "Error send" << std::endl;
                close(client_socket);
                return 1;
            }
        std::cout << "Chunk " << counter << ": ";
		counter++;
		std::cout << readfile.gcount() << std::endl;
        finish_counter = finish_counter + readfile.gcount();
        }
    }
 std::cout << "Total amount: " << finish_counter << std::endl;

 close(client_socket);
 readfile.close();
    
return 0;
}