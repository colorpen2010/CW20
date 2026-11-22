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
    MyString empty;
    MyString copy(first);
    MyString moved(std::move(copy));

    std::cout << "Введённая строка: " << first.c_str() << '\n';
    std::cout << "Длина строки: " << first.length() << '\n';
    std::cout << "Пустая ли строка empty: " << (empty.isEmpty() ? "да" : "нет") << '\n';

    moved.reverse();
    std::cout << "Копия строки в обратном порядке: " << moved.c_str() << '\n';
    std::cout << "Живых объектов MyString: " << MyString::getObjectCount() << '\n';

    return 0;
}
