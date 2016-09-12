#include <string>
#include <iostream>

int main()
{
    constexpr char prefix[] = "@expected(";
    constexpr char suffix[] = ")";
    constexpr char comment[] = "//";
    constexpr char separators[] = " \t,;";

    std::string line;
    size_t lineNumber = 0;
    while (std::getline(std::cin, line))
    {
        ++lineNumber;

        size_t start = std::string::npos;
        if ((start = line.find(comment)) == std::string::npos)
            continue;

        size_t end = std::string::npos;
        while ((start = line.find(prefix, start)) != std::string::npos && (end = line.find(suffix, start)) != std::string::npos)
        {
            start += std::char_traits<char>::length(prefix);

            while (start < end)
            {
                size_t separator = line.find_first_of(separators, start);
                if (separator == std::string::npos || separator > end)
                    separator = end;

                if (separator > start + 1)
                    std::cout << "line " << lineNumber << ": " << line.substr(start, separator - start) << std::endl;

                start = separator + 1;
            }
        }
    }

    return 0;
}

