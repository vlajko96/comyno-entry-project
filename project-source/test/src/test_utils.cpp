#include "test_utils.hpp"

int t_get_int(int *val)
{
    std::string str;
    std::cout << "Please enter an integer value: ";
    std::cin >> str;
    std::string::const_iterator it = str.begin();
    if (*it == '-' || *it == '+' || std::isdigit(*it)) ++it;
    while (it != str.end() && std::isdigit(*it)) ++it;
    if (str.empty() || it != str.end()) {
        std::cout << "Error. Please provide integer." << std::endl;
        return -1;
    } else {
        try {
            int i = stoi(str);
            *val = i;
        } catch (std::out_of_range& e) {
            std::cout << "Error. Number out of bounds." << std::endl;
            return -1;
        }
    }

    return 0;
}
