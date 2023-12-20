#include <windows.h>
#include <iostream>
#include <stack>
#include <string>
#include <cctype>

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    } else if (op == '*' || op == '/') {
        return 2;
    }
    return 0;
}

double applyOperation(double a, double b, char op) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if (b == 0) {
                throw std::runtime_error("Деление на ноль");
            }
            return a / b;
        default:
            return 0;
    }
}

double calculate(std::string expression) {
    std::stack<double> values;
    std::stack<char> ops;

    size_t i = 0;
    while (i < expression.length()) {
        if (expression[i] == ' ') {
            ++i;
            continue;
        }

        if (isdigit(expression[i])) {
            std::string numStr;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                numStr += expression[i++];
            }
            --i;
            values.push(std::stod(numStr));
        } else if (expression[i] == '(') {
            ops.push(expression[i]);
        } else if (expression[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOperation(val1, val2, op));
            }
            ops.pop(); // Remove '('
        } else if (isOperator(expression[i])) {
            if (i == 0 || expression[i - 1] == '(' || (expression[i] == '-' && (i == 0 || expression[i - 1] == '('))) {
                values.push(0); // Унарный минус, добавляем 0 в стек
            }
            else if (isOperator(expression[i - 1])) {
                std::cout << "Ошибка: два оператора идут подряд" << std::endl;
                return 0;
            }

            while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOperation(val1, val2, op));
            }
            ops.push(expression[i]);
        }

        ++i;
    }

    while (!ops.empty()) {
        double val2 = values.top();
        values.pop();

        double val1 = values.top();
        values.pop();

        char op = ops.top();
        ops.pop();

        values.push(applyOperation(val1, val2, op));
    }

    return values.top();
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    std::string expression;
    std::cout << "Введите математическое выражение (окончание на '='): ";
    std::getline(std::cin, expression);

    if (!expression.empty() && expression.back() == '=') {
        expression.pop_back(); // Убираем '='
        expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end()); // Убираем пробелы

        int openBrackets = std::count(expression.begin(), expression.end(), '(');
        int closeBrackets = std::count(expression.begin(), expression.end(), ')');
        if (openBrackets != closeBrackets) {
            std::cout << "Ошибка: количество открывающих и закрывающих скобок не совпадает" << std::endl;
            return 1;
        }

        try {
            double result = calculate(expression);
            std::cout << "Результат: " << result << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    } else {
        std::cout << "Ошибка: выражение должно заканчиваться на '='." << std::endl;
    }

    return 0;
}


