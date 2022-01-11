// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь: 271 число с "3" в заданном диапазоне от 1 до 1000

// Закомитьте изменения и отправьте их в свой репозиторий.

#include<iostream>
#include<string>

using namespace std;

int main() {
	int count = 0;
	for (int i = 1; i <= 1000; i++) {
		for (char number : to_string(i)) {
			if (number == '3') {
				count = count + 1;
				break;

			}
		}
	}

	cout << "There are : " << count << " with 3" << endl;

	return 0;
}

