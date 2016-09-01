#include "../shared/log.h"
#include "../shared/global.h"
#include "../shared/user_object.h"
#include "../shared/user.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <nss.h>
#include <pwd.h>



int main(int argc, char const *argv[])
{
    if (argc < 1) {
        fprintf(stderr, "usage: %s [username]\n", argv[0]);
        return 1;
    }

    const char* username;
    if (argc >= 2) {
        if (geteuid() == 0) {
            username = argv[1];    
        }
        else {
            fprintf(stderr, "Querying other user information is only available for root!\n");
            return 1;
        }
        
    }
    else {
        struct passwd *pw;
        pw = getpwuid(geteuid());
        username = pw->pw_name;
    }

    
    char name[MAX_NAME_SIZE + 1];
    int len = strlen(username);
    if (len > MAX_NAME_SIZE) {
        fprintf(stderr, "username exceeds %d\n", MAX_NAME_SIZE);
        return 1;
    }
    memcpy(name, username, len);
    name[len] = 0;

    if (strlen(sanitize_user_name(name)) == 0) {
        fprintf(stderr, "username is invalid\n");
        return 1;
    }

    char buffer[1024];
    struct user_object user;
    enum nss_status status = get_user(name, 0, &user, buffer, sizeof(buffer));

    if (status == NSS_STATUS_SUCCESS) {
        // print and clear all AuthKeys
        if (user.CustomData != NULL) {
            puts(user.CustomData);
        }
        clear_user(&user);
    }
    return 0;
}