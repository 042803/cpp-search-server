// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь:
// 271 xD
#include <iostream>
#include <vector>

int main() {
    int start, end, numDigits;

    std::cout << "Enter the starting number: ";
    std::cin >> start;

    std::cout << "Enter the ending number: ";
    std::cin >> end;

    std::cout << "Enter the number of digits to search for: ";
    std::cin >> numDigits;

    std::vector<int> digits(numDigits);
    std::cout << "Enter the digits to search for, separated by spaces: ";

    for (int i = 0; i < numDigits; ++i) {
        std::cin >> digits[i];
    }

    int count = 0;

    for (int i = start; i <= end; ++i) {
        int number = i;
        bool found = false;

        while (number > 0 && !found) {
            int currentDigit = number % 10;

            for (int j = 0; j < numDigits; ++j) {
                if (currentDigit == digits[j]) {
                    ++count;
                    found = true;
                    break;
                }
            }

            number /= 10;
        }
    }

    std::cout << "Number of numbers containing at least one of the specified digits: " << count << std::endl;

    return 0;
}
// Закомитьте изменения и отправьте их в свой репозиторий.
