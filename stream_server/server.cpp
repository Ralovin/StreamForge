#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <cstdint>




struct PacketHeader{
    uint32_t chank_size;
    uint32_t chank_id;
    uint8_t chank_flag;
}__attribute__((packed));

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

    int client_socket = accept(server_socket,(struct sockaddr *)&client_address,&client_address_sizeof);

    char buffer[4096];
    int counter = 1;
    int finish_counter = 0;
    ssize_t  result_recv;

    PacketHeader header;

    while(true){
        ssize_t result_header= recv(client_socket, &header, sizeof(header),MSG_WAITALL);
        if (result_header <=0){
            break;
        }

        if (header.chank_size > sizeof(buffer)) {
        std::cout << "Error: chunk size too large!" << std::endl;
        break;
        }

        ssize_t result = recv(client_socket, buffer, header.chank_size,MSG_WAITALL);

        writefile.write(buffer,result);

        std::cout << "Chunk ID: " << header.chank_id << " Size: " << result << std::endl;

        if (header.chank_flag == 1) {
        std::cout << "Last chunk received. Finidhing..." << std::endl;
        break; 
    }
    finish_counter += result;
    }

    std::cout << "Total amount: " << finish_counter << std::endl;

    close(client_socket);
    close(server_socket);
    writefile.close();


    return 0;
}