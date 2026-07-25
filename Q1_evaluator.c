#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

// Stack for values
int values[MAX];
int valTop = -1;

// Stack for operators
char ops[MAX];
int opTop = -1;

// Push and Pop for values
void pushValue(int value) {
    values[++valTop] = value;
}

int popValue() {
    return values[valTop--];
}

// Push and Pop for operators
void pushOp(char op) {
    ops[++opTop] = op;
}

char popOp() {
    return ops[opTop--];
}

char peekOp() {
    return ops[opTop];
}

// Operator precedence
int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

// Apply operation
int applyOperation(int a, int b, char op, int *error) {

    switch(op) {

        case '+':
            return a + b;

        case '-':
            return a - b;

        case '*':
            return a * b;

        case '/':
            if(b == 0) {
                *error = 1;   // Divide by zero
                return 0;
            }
            return a / b;

        default:
            *error = 2;       // Invalid operator
            return 0;
    }
}

// Evaluate expression
int evaluate(char expr[], int *error) {

    int i;

    for(i = 0; i < strlen(expr); i++) {

        char ch = expr[i];

        if(ch == ' ')
            continue;

        // Number
        if(isdigit(ch)) {

            int num = 0;

            while(i < strlen(expr) && isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }

            pushValue(num);
            i--;
        }

        // Opening bracket
        else if(ch == '(') {
            pushOp(ch);
        }

        // Closing bracket
        else if(ch == ')') {

            while(opTop != -1 && peekOp() != '(') {

                if(valTop < 1) {
                    *error = 3;
                    return 0;
                }

                int b = popValue();
                int a = popValue();

                pushValue(applyOperation(a, b, popOp(), error));

                if(*error)
                    return 0;
            }

            if(opTop == -1) {
                *error = 4;
                return 0;
            }

            popOp(); // Remove '('
        }

        // Operator
        else if(ch=='+' || ch=='-' || ch=='*' || ch=='/') {

            while(opTop!=-1 &&
                  precedence(peekOp()) >= precedence(ch)) {

                if(valTop < 1) {
                    *error = 3;
                    return 0;
                }

                int b = popValue();
                int a = popValue();

                pushValue(applyOperation(a,b,popOp(),error));

                if(*error)
                    return 0;
            }

            pushOp(ch);
        }

        else {
            *error = 5;
            return 0;
        }
    }

    while(opTop != -1) {

        if(peekOp() == '(') {
            *error = 4;
            return 0;
        }

        int b = popValue();
        int a = popValue();

        pushValue(applyOperation(a,b,popOp(),error));

        if(*error)
            return 0;
    }

    return popValue();
}

int main() {

    char expression[100];

    printf("Enter billing rule: ");
    fgets(expression, sizeof(expression), stdin);

    int error = 0;

    int result = evaluate(expression, &error);

    if(error == 0)
        printf("Accepted. Result = %d\n", result);

    else if(error == 1)
        printf("ERROR_DIVIDE_BY_ZERO\n");

    else if(error == 2)
        printf("ERROR_INVALID_OPERATOR\n");

    else if(error == 3)
        printf("ERROR_INVALID_EXPRESSION\n");

    else if(error == 4)
        printf("ERROR_UNBALANCED_PARENTHESES\n");

    else if(error == 5)
        printf("ERROR_INVALID_CHARACTER\n");

    return 0;
}