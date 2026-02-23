To run our client in the Linux terminal (Bash), navigate to the folder where your client is compiled and enter the following command:

./client.exe <filename_to_transfer> <server_ip> <server_port>

Example: ./client.exe input.bin 127.0.0.1 9000

Process Monitoring:
The client console will display the number of 4096-byte chunks being sent.

A total sum (size) will be displayed at the end of the process.

Once the command is executed, the client will connect to the server and begin the file transfer!

Note: Ensure that the server is already running before starting the client; otherwise, the connection will fail.