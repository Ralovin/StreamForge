#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <cstring>

int main(int argc,char *argv[]){
    if(argc < 3){
        std::cout << "Not all arguments are entered" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    int port = std::stoi(argv[2]);

    int server_socket = socket(AF_INET,SOCK_STREAM,0);

    if(server_socket == -1){
        std::cout << "Error server_socket";
        return 1;
    }

    struct sockaddr_in address;

    memset(&address,0,sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int result_bind = bind(server_socket,(struct sockaddr *)&address,sizeof(address));

    if(result_bind < 0){
        std::cout << "Error bind";
        close(server_socket);
        return 1;
    }

    int result_listen = listen(server_socket,5);

    if(result_listen < 0){
        std::cout << "Error listen";
        close(server_socket);
        return 1;
    }

    std::ofstream writefile(filename,std::ios::binary);

    if(!writefile.is_open()){
        std::cout << "Error open file" << std::endl;
        close(server_socket);
        return 1;
    }

    struct sockaddr_in client_address;
    socklen_t client_address_sizeof = sizeof(client_address);

    int client_socket = accept(server_socket,(struct sockaddr *)&address,&client_address_sizeof);

    char buffer[4096];
    int counter = 1;
    int finish_counter = 0;
    ssize_t  result_recv;


    while((result_recv = recv(client_socket,buffer,sizeof(buffer),0)) > 0){
        writefile.write(buffer,result_recv);
        std::cout << "Chunk " << counter << ": ";
		counter++;
		std::cout << result_recv << std::endl;
        finish_counter = finish_counter + result_recv;
    }

    std::cout << "Total amount: " << finish_counter << std::endl;

    close(client_socket);
    close(server_socket);
    writefile.close();


    return 0;
}