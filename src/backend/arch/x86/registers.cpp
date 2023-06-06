#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <cstdlib>

#include <registers.h> /* TODO add -Ibackend/arch/$(TARGET_ARCH)/ */
#include <common.h>

x86_64_registers amd64;


void parse_ir(std::string s) {
    std::string parsemov;
    std::vector<std::string> tokenized_vec;
    std::stringstream check(s);
    std::string intermediate;
    std::string list = "=+-*/;";

    if (s.find("=") != std::string::npos) {
        parsemov = s.substr(s.find("=")+1);
    }
    std::cout << s << '\n';

    try {
        std::cout << parsemov << '\n';
    } catch (const std::out_of_range& e) {
        std::cout << "parsemov is out of range\n";
        exit(EXIT_FAILURE);
    }

    for (std::string::size_type i = 0; i < parsemov.length(); i++) {
        if (is_any_of(parsemov[i], list.c_str())) {
            std::cout << "Found tokenizer!\n";
            getline(check, intermediate, parsemov[i]);
            tokenized_vec.push_back(intermediate);
        }
    }

    for (std::string str : tokenized_vec) {
        std::cout << str << '\n';
    }


}
