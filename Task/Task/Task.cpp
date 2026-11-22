#include <iostream>
#include <cstring>
#include <utility>
#include <windows.h>

void copyText(char* destination, const char* source, size_t destinationSize)
{
    strcpy_s(destination, destinationSize, source);
}

class MyString
{
private:
    char* str;
    static int objectCount;

public:
    MyString() : str(new char[1]{ '\0' })
    {
        ++objectCount;
    }

    MyString(const char* text) : str(nullptr)
    {
        if (text == nullptr)
            text = "";

        str = new char[std::strlen(text) + 1];
        copyText(str, text, std::strlen(text) + 1);
        ++objectCount;
    }

    MyString(const MyString& other) : str(new char[other.length() + 1])
    {
        copyText(str, other.c_str(), other.length() + 1);
        ++objectCount;
    }

    MyString(MyString&& other) noexcept : str(other.str)
    {
        other.str = nullptr;
        ++objectCount;
    }

    MyString& operator=(const MyString& other)
    {
        if (this != &other)
        {
            char* newStr = new char[other.length() + 1];
            copyText(newStr, other.c_str(), other.length() + 1);
            delete[] str;
            str = newStr;
        }
        return *this;
    }

    MyString& operator=(MyString&& other) noexcept
    {
        if (this != &other)
        {
            delete[] str;
            str = other.str;
            other.str = nullptr;
        }
        return *this;
    }

    ~MyString()
    {
        delete[] str;
        --objectCount;
    }

    bool isEmpty() const
    {
        return str == nullptr || str[0] == '\0';
    }

    const char* c_str() const
    {
        return str == nullptr ? "" : str;
    }

    int len() const
    {
        return str == nullptr ? 0 : static_cast<int>(std::strlen(str));
    }

    int length() const
    {
        return len();
    }

    // Очищает строку.
    void erase()
    {
        if (str != nullptr)
            str[0] = '\0';
    }

    // Заменяет count символов, начиная с pos, строкой newText.
    void replace(int pos, int count, const char* newText)
    {
        int oldLength = len();

        if (pos < 0 || pos > oldLength || count < 0)
            return;

        if (count > oldLength - pos)
            count = oldLength - pos;

        if (newText == nullptr)
            newText = "";

        int textLength = static_cast<int>(std::strlen(newText));
        int newLength = oldLength - count + textLength;
        char* newStr = new char[newLength + 1];

        for (int i = 0; i < pos; ++i)
            newStr[i] = str[i];

        for (int i = 0; i < textLength; ++i)
            newStr[pos + i] = newText[i];

        for (int i = pos + count; i < oldLength; ++i)
            newStr[pos + textLength + i - pos - count] = str[i];

        newStr[newLength] = '\0';
        delete[] str;
        str = newStr;
    }

    // Возвращает символ по индексу. Если индекс неправильный, возвращает '\0'.
    char at(int index) const
    {
        if (index < 0 || index >= len())
            return '\0';

        return str[index];
    }

    void reverse()
    {
        if (str == nullptr)
            return;

        int left = 0;
        int right = len() - 1;

        while (left < right)
        {
            char temp = str[left];
            str[left] = str[right];
            str[right] = temp;
            ++left;
            --right;
        }
    }

    static int getObjectCount()
    {
        return objectCount;
    }
};

int MyString::objectCount = 0;

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    char input[256];
    std::cout << "Введите строку на английском: ";
    std::cin.getline(input, 256);

    MyString first(input);
    std::cout << "Введённая строка: " << first.c_str() << '\n';
    std::cout << "Длина строки: " << first.length() << '\n';

    int pos;
    int count;
    char replacement[256];
    std::cout << "Введите позицию для замены: ";
    std::cin >> pos;
    std::cout << "Сколько символов заменить: ";
    std::cin >> count;
    std::cin.ignore(10000, '\n');
    std::cout << "Введите текст для вставки: ";
    std::cin.getline(replacement, 256);

    first.replace(pos, count, replacement);
    std::cout << "Строка после replace: " << first.c_str() << '\n';

    std::cout << "Введите индекс символа: ";
    int index;
    std::cin >> index;
    char symbol = first.at(index);
    if (symbol == '\0')
        std::cout << "Такого символа нет.\n";
    else
        std::cout << "Символ по индексу: " << symbol << '\n';

    first.erase();
    std::cout << "Строка после erase: " << first.c_str() << '\n';

    return 0;
}
