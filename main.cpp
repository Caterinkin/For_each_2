#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include <iterator>
#include <clocale>

// Параллельная версия for_each
template<typename Iterator, typename Func>
void parallel_for_each(Iterator first, Iterator last, Func&& func, size_t min_block_size = 1000)
{
    const size_t size = std::distance(first, last);

    if (size < min_block_size)
    {
        std::for_each(first, last, std::forward<Func>(func));
        return;
    }

    Iterator middle = first;
    std::advance(middle, size / 2);

    auto future = std::async(std::launch::async,
        [first, middle, &func, min_block_size]()
        {
            parallel_for_each(first, middle, std::forward<Func>(func), min_block_size);
        });

    parallel_for_each(middle, last, std::forward<Func>(func), min_block_size);
    future.get();
}

int main()
{
    setlocale(LC_ALL, "Russian");

    std::vector<int> numbers;
    int n, num;

    // Ввод массива
    std::cout << "Введите количество элементов: ";
    std::cin >> n;

    std::cout << "Введите " << n << " чисел через пробел:\n";
    for (int i = 0; i < n; ++i)
    {
        std::cin >> num;
        numbers.push_back(num);
    }

    // Вывод исходного массива
    std::cout << "\nИсходный массив:\n";
    for (const auto& num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << "\n\n";

    // Функция для обработки (увеличиваем каждый элемент на 10)
    auto process_func = [](int& n) { n += 10; };

    // Применяем parallel_for_each
    parallel_for_each(numbers.begin(), numbers.end(), process_func);

    // Вывод результата
    std::cout << "Массив после обработки (каждый элемент увеличен на 10):\n";
    for (const auto& num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << "\n";

    return 0;
}
