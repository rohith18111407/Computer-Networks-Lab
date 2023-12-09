/*
Run Server first and then client
*/

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int readnew(char direc[], char file[], char content[]) {
    struct dirent *dir;
    DIR *dr;
    int n;

    if ((dr = opendir(direc)) == NULL) {
        printf("Error opening the directory: %s\n", strerror(errno));
        return 0;
    }

    while ((dir = readdir(dr)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue; // Skip current and parent directory entries
        }

        char filePath[PATH_MAX];
        snprintf(filePath, sizeof(filePath), "%s/%s", direc, dir->d_name);

        if (dir->d_type == DT_DIR) {
            // If it's a directory, recursively search it
            if (readnew(filePath, file, content) == 1) {
                closedir(dr);
                return 1; // File found, terminate the search
            }
        } else if (strcmp(file, dir->d_name) == 0) {
            int fd = open(filePath, O_RDONLY);
            if (fd == -1) {
                printf("Failed to open source file: %s\n", strerror(errno));
            } else {
                if ((n = read(fd, content, 255)) > 0) {
                    printf("File has been read from: %s\n", filePath);
                }
                close(fd);
                closedir(dr);
                return 1; // File found, terminate the search
            }
        }
    }
    closedir(dr);
    return 0;
}

int main(int argc, char const *argv[]) {

    int sockD = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[1]));
    servAddr.sin_addr.s_addr = INADDR_ANY;
    
    bind(sockD, (struct sockaddr *) &servAddr, sizeof(servAddr));
    listen(sockD, 1);
    int serverSocket = accept(sockD, NULL, NULL);

    printf("Connected..\n");
    char fileName[255];
    char fileCont[255];
    recv(serverSocket, fileName, sizeof(fileName), 0);
    printf("Filename from client for FTP transfer: %s\n", fileName);

    // Specify the absolute path to the user's desktop directory
    char cdir[255] = "/home/cseb1"; // Replace "username" with the actual username

    char notAvail[15] = "Not Available\n";

    int found = readnew(cdir, fileName, fileCont);
    if (found == 0) {
        send(serverSocket, notAvail, sizeof(notAvail), 0);
    } else {
        send(serverSocket, fileCont, sizeof(fileCont), 0);
    }

    return 0;
}