/* Created by Vecnik88

    Простая консольная программа, которая запрашивает имя пользователя и пароль.
    Она проверяет содержит ли введенный пароль символы верхнего, нижнего регистра, а также
    содержит ли пароль хотя бы одну цифру.

    A simple console program, which requires a username and password.
    It checks whether the entered password is uppercase, lowercase, and
    whether the password contains at least one digit.

 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    int i;
    int hasUpper, hasDigit, hasLower, hasSymbol;

    char user[25], password[25];

   // инициализируем все три переменные 0
    hasUpper = hasDigit = hasLower = hasSymbol = 0;

    printf("What is your name ");
    scanf(" %s", user);
    printf("Please create a password ");
    scanf(" %s", password);
    for(i = 0; i < strlen(password); ++i)
    {
        if(isdigit(password[i]))
        {
            hasDigit = 1;
            continue;
        }
        if(isupper(password[i]))
        {
            hasUpper = 1;
            continue;
        }
            if(islower(password[i]))
        {
            hasLower = 1;
            continue;
        }
            else                        // <---. говорит о наличии символов !"№;%:?*()
            {
            hasSymbol = 1;
            continue;
            }
    }

if(hasDigit && hasUpper && hasLower && hasSymbol)
{
    printf("Nise work %s", user);
    printf("\n\nYour password contains upper and lowercase letters, symbols and numbers\n");
}
else
{
    printf("Come up with a new password %s", user);
    printf(" containing upper and lowercase letters, symbols and numbers");
}
return 0;
}
