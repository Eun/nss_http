#include <stdio.h>
#include <string.h>

#include "../../shared/user.h"

#include <assert.h>


#define testSan(USER, EXPECTED) \
do                                                                \
{                                                                 \
    char input[128] = "";                                         \
    memcpy(input, USER, strlen(USER));                            \
    assert(strcmp(sanitize_user_name(input), EXPECTED) == 0);     \
    assert(strcmp(input, EXPECTED) == 0);                         \
}                                                                 \
while (0)

void testSanitizeUserName()
{
    // valid
    testSan("user", "user");
    testSan("User", "User");
    testSan("USER", "USER");
    testSan("user_a", "user_a");
    testSan("user-a", "user-a");
    testSan("user-a_1", "user-a_1");
    testSan("user_a-1", "user_a-1");

    // invalid
    testSan("user$1", "user1");
    testSan("$user1", "user1");
    testSan("user1$", "user1");
    testSan("uäser1", "user1");

    // usernames must start with an letter
    testSan("-user", "user");
    testSan("_user", "user");
    testSan("0user", "user");
    testSan("0", "");

    testSan("0$user", "user");
    testSan("$äüöa!\"$", "a");
}


int main()
{
    testSanitizeUserName();
    return 0;
}