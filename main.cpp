#include <iostream>

bool conjunction(bool A, bool B) {
    return A && B;
}

bool disjunction(bool A, bool B) {
    return A || B;
}

int initStr(char *arr) {
    int j = 0;

    while (arr[j] != '\0') {
        j++;
    }

    return j;
}

void initValues(int *arr) {
    for (int i = 0; i < 26; i++) {
        arr[i] = -2;
    }
}

bool check(int size, char *arr, int *values) {
    for (int i = 0; i < size; i++) {
        if ((arr[i] < 'A' || arr[i] > 'Z') && arr[i] != '(' && arr[i] != ')' &&
            arr[i] != '+' && arr[i] != '*' && arr[i] != '!') {
            std::cout << "Entered unknown symbol" << std::endl;
            return 0;
        } else if (arr[i] >= 'A' && arr[i] <= 'Z') {
            values[arr[i] - 'A'] = -1;
        }
    }

    return 1;
}

int getValues(int *values) {
    int amountVal = 0;
    for (int i = 0; i < 26; i++) {
        if (values[i] == -1) {
            amountVal++;
        }
    }
    return amountVal;
}

bool calculate(char *arr, bool *stack, int index, int *values, int *start) {
    while (arr[*start] != ')') {
        switch (arr[*start]) {

            case '(':
                *start += 1;
                calculate(arr, stack, index, values, start);
                index += 1;
                *start += 1;
                break;

            case '!':
                stack[index] = !values[arr[*start + 1] - 'A'];
                *start += 2;
                index += 1;
                break;

            case '+':
                if (arr[*start + 1] != '(') {
                    if (arr[*start + 1] != '!') {
                        stack[index - 1] = disjunction(stack[index - 1], values[arr[*start + 1] -
                                                                                'A']);
                        *start += 2;
                    } else {
                        stack[index - 1] = disjunction(stack[index - 1], !values[arr[*start + 2] -
                                                                                 'A']);
                        *start += 3;
                    }
                } else {
                    *start += 2;
                    stack[index - 1] = disjunction(stack[index - 1],
                                                   calculate(arr, stack, index, values, start));
                    *start += 1;
                }
                break;

            case '*':
                if (arr[*start + 1] != '(') {
                    if (arr[*start + 1] != '!') {
                        stack[index - 1] = conjunction(stack[index - 1], values[arr[*start + 1] -
                                                                                'A']);
                        *start += 2;
                    } else {
                        stack[index - 1] = conjunction(stack[index - 1], !values[arr[*start + 2]
                                                                                 - 'A']);
                        *start += 3;
                    }
                } else {
                    *start += 2;
                    stack[index - 1] = conjunction(stack[index - 1],
                                                   calculate(arr, stack, index, values, start));
                    *start += 1;
                }
                break;

            default:
                stack[index] = values[arr[*start] - 'A'];
                index += 1;
                *start += 1;
                break;
        }
    }
    return stack[index - 1];
}

void changeVal(int *values, int i) {
    for (int j = 0; j < 26; j++) {
        if (values[j] > -2) {
            values[j] = i % 2;
            i >>= 1;
        }
    }
}

void makePCNF(char *arr, int *values, bool *stack, int amountVal, char *str, int *z, int *count) {
    *count = 0;
    int max = 1;
    max <<= amountVal;

    for (int i = 0; i < max; i++) {
        changeVal(values, i);

        int start = 0;
        bool res = calculate(arr, stack, 0, values, &start);

        if (!res) {
            str[*z] = '(';
            *z += 1;
            for (int j = 0; j < 26; j++) {
                if (values[j] > -2) {
                    if (values[j]) {
                        str[*z] = '!';
                        str[*z + 1] = (char) ('A' + j);
                        str[*z + 2] = '+';
                        *z += 3;
                    } else {
                        str[*z] = (char) ('A' + j);
                        str[*z + 1] = '+';
                        *z += 2;
                    }
                }
            }
            str[*z - 1] = ')';
            str[*z] = '\0';
            *z += 1;
            *count += 1;
        }
    }
}

int calculateFunction(char *str, char *arr, int *count) {
    bool stack[3];

    int size = initStr(str);


    int values[26];
    initValues(values);

    if (!check(size, str, values)) {
        return -1;
    }
    str[size] = ')';

    int amountVal = getValues(values);

    int j = 0;
    makePCNF(str, values, stack, amountVal, arr, &j, count);

    return 1;
}

void enterPackages(char *arr) {
    int sends;
    std::cout << "Enter amount of packages" << std::endl;
    std::cin >> sends;

    getchar();
    int j = 0;
    std::cout << "Enter packages" << std::endl;
    for (int i = 0; i < sends; i++) {
        char smb;
        arr[j] = '(';
        j++;
        while ((smb = getchar()) != '\n' && j < 40) {
            arr[j] = smb;
            j++;
        }
        if (j >= 38) {
            std::cout << "Too big function was entered" << std::endl;
            return;
        }
        if (i < sends - 1) {
            arr[j] = ')';
            arr[j + 1] = '*';
            j += 2;
        } else {
            arr[j] = ')';
            arr[j + 1] = '\0';
            j += 2;
        }
    }
}

void outStrPart(char *str, int begin, int end) {
    for (int i = begin; i < end; i++) {
        std::cout << str[i];
    }
}

void returnAllPackages(char *str, int count) {
    int temp[10][2];
    temp[0][0] = 0;
    int indexTmp = 0;
    for (int i = 0; i < 41 && indexTmp < count; i++) {
        if (str[i] == '\0') {
            temp[indexTmp][1] = i;
            temp[indexTmp + 1][0] = i + 1;
            indexTmp += 1;
        }
    }

    int amount = 1;
    amount <<= count;

    for (int i = 1; i < amount; i++) {
        std::cout << i << ") ";
        int iTpm = i;
        int strIndex = 0;
        while (iTpm > 0) {
            bool flag = false;
            if (iTpm % 2) {
                outStrPart(str, temp[strIndex][0], temp[strIndex][1]);
                flag = true;
            }
            strIndex += 1;
            iTpm >>= 1;
            if (iTpm > 0 && flag) {
                std::cout << "&";
            }
        }
        std::cout << std::endl;
    }

}

void outPCNF (char *arr, int count){
    std::cout << "PCNF:" << std::endl;

    for (int i = 0; i < 41 && count > 0; i++){
        if (arr[i] != '\0'){
            std::cout << arr[i];
        } else {
            count -= 1;
            if (count){
                std::cout << "&";
            }
        }
    }

    std::cout << std::endl;
}

int main() {
    char str[41];
    char arr[41];
    int count;
    enterPackages(str);

    calculateFunction(str, arr, &count);
    std::cout << std::endl;

    outPCNF(arr, count);
    std::cout << std::endl;

    returnAllPackages(arr, count);

    return 0;
}