# How to Launch Client and Server

## By Jacky Chen -> jackyj.chen@mail.utoronto.ca, Student Number: 1008049666

### Steps
1. Run **gcc** on the required client and server files **server.c** and **client.c**, see below:

    ```
        gcc client.c -o client
        gcc server.c -o server
    ```
2. Start up the server first in a Linux terminal (WSL on windows), then start up the client. Make sure that you execute the files under different terminals, see below:
    ```
        ./server
        ./client localhost
    ```
    **Note**: you should always make sure to start the server before the client.
3. In the terminal that executed the client, type any string to stdin and the server terminal should receive it and send it back with the date in the following format below:
    ```
        {string_you_just_entered}
        {your_string}
        {year.month.date.hours:minutes}
    ```
    **Note:** The string that is sent back is of length MAX_LINE*2, but the string that you send should still be less than or equal to the MAX_LINE limit defined in the code.
4. Now lets try closing the server connection, type the following into your client's stdin **as a full string ">>> Ciao-Ciao"** including the arrows:
    ```
    >>> Ciao-Ciao
    ```
5. You should see the following message on the terminal that you launched the client:
    ```
    Server closing...
    ```
    This indicates that the server has closed.
6. After the server closes, note that the client should close as well right after and you should return to the default terminal process, which ends the procedure of running the code.

### Notes:
- The server may have issues connecting from bind or accept errors especially if you try to close either the server or client and relaunch it immediately after.
- You can always refer to the server.c code and client.c code to see the implementation for shutting down the server (if there was any confusion about >>> Ciao-Ciao).
- If needed, you can kill the port using ```npx kill-port [port number]``` if there are any issues with the server. You can also use a different port if needed.