/*
 * =====================================================================================
 *
 *       Filename:  parser.c
 *
 *    Description:  A recursive descent parser
 *
 *        Version:  1.0
 *        Created:  11/30/2017 03:07:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  nvengal (nv), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef enum {PLUS, MINUS, DIVIDE, MULT, REMAINDER, POWER, LPAREN, RPAREN, NUMBER, ERROR, EOL} TokenType;

struct Token { 
  TokenType type;
  int value;
};

void getToken(char c);
char *getAllDigitsOfNum();
int match(TokenType type);
int command();
int expr();
int term();
int power();
int factor();
int factor1();

struct Token token;

int main() {
  printf("Input: ");
  //Wait for user input before printing next line
  char c = getchar();
  printf("Output: \n\n");
  getToken(c);
  printf("    Result: %d\n", command(0));
  return 0;
}

void getToken(char c) {
  if (c == ' ') {
    //Skip spaces
    getToken(getchar());
  } else if (c == '+') {
    token.type = PLUS;
    token.value = 0;
    printf("    +    PLUS\n");
  } else if (c == '-') {
    token.type = MINUS;
    token.value = 0;
    printf("    -    MINUS\n");
  } else if (c == '/') {
    token.type = DIVIDE;
    token.value = 0;
    printf("    /    DIVIDE\n");
  } else if (c == '*') {
    token.type = MULT;
    token.value = 0;
    printf("    *    MULT\n");
  } else if (c == '%') {
    token.type = REMAINDER;
    token.value = 0;
    printf("    %c    REMAINDER\n", '%');
  } else if (c == '^') {
    token.type = POWER;
    token.value = 0;
    printf("    ^    POWER\n");
  } else if (c == '(') {
    token.type = LPAREN;
    token.value = 0;
    printf("    (    LPAREN\n");
  } else if (c == ')') {
    token.type = RPAREN;
    token.value = 0;
    printf("    )    RPAREN\n");
  } else if (isdigit(c)) {
    token.type = NUMBER;
    //Create dest string for concatenation
    char dest[50];
    dest[0] = c;
    dest[1] = '\0';
    //Get all the digits in the number
    token.value = atoi(strcat(dest, getAllDigitsOfNum()));
    printf("    %d    NUMBER\n", token.value);
  } else if (c == '\n') {
    token.type = EOL;
    token.value = 0;
    printf("\n");
  } else {
    token.type = ERROR;
    token.value = 0;
    printf("         ERROR\n");
    exit(1);
  }
}

char *getAllDigitsOfNum() {
  char c = getchar();
  //If next char is not a num unget it so that a token is not missed
  if (!isdigit(c)) {
    ungetc(c, stdin);
    return "";
  }
  char dest[50];
  dest[0] = c;
  dest[1] = '\0';
  //Recursively check for more digits
  return strcat(dest, getAllDigitsOfNum());
}

int match(TokenType type) {
  return (token.type == type);
}

//command -> expr '\n'
int command(int runningTotal) {
  //base case
  if (match(EOL)) {
    return runningTotal;
  }
  int total = expr(runningTotal);
  return command(total);
}

//expr -> term { + term | - term }
int expr(int runningTotal) {
  int total = term(runningTotal);
  if (match(PLUS)) {
    getToken(getchar());
    return total + term(runningTotal);
  } else if (match(MINUS)) {
    getToken(getchar());
    return total - term(runningTotal);
  }
  return total;
}

//term -> power { ∗ power | / power | % power }
int term(int runningTotal) {
  int total = power() + runningTotal;
  if (match(MULT)) {
    getToken(getchar());
    return total * power();
  } else if (match(DIVIDE)) {
    getToken(getchar());
    return total / power();
  } else if (match(REMAINDER)) {
    getToken(getchar());
    return total % power();
  }
  return total;
}

//power -> factor [ ^ power ]
int power() {
  int total = factor();
  if (match(POWER)) {
    getToken(getchar());
    return pow(total, power());
  }
  return total;
}

//factor -> [-] factor1
int factor() {
  if (match(MINUS)) {
    getToken(getchar());
    return -1 * factor1();
  }
  return factor1();
}

//factor1 -> ( expr ) | NUMBER
int factor1() {
  if (match(NUMBER)) {
    int total = token.value;
    getToken(getchar());
    return total;
  }
  if (match(LPAREN)) {
    getToken(getchar());
    int total = expr(0);
    if (match(RPAREN)) {
      getToken(getchar());
      return total;
    } else {
      printf("        ERROR\n");
      exit(1);
    }
  }
}
