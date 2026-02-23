To run our server in the Linux terminal (Bash), navigate to the folder where your server is compiled and enter the following command:

./server.exe <output_filename> <port_number>

Example: ./server.exe result.bin 9000

What to expect:
The server console will display the number of 4096-byte chunks being received.

Once the transfer is complete, it will show the total sum (size) at the end.

After entering the command, the server will start and begin waiting for a client connection!