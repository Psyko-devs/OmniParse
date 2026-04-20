#include "parser.hpp"
#include <chrono>

int main() {
    std::string baseJson = R"({ "project": "OmniParse", "version": 1.0, "modules": [ "AST", "Lexer", "Parser" ] })";
    
    // Simulate a massive 20MB JSON file
    std::cout << "Generating massive payload..." << std::endl;
    std::string rawJson = "[";
    for(int i = 0; i < 100000; i++) {
        rawJson += baseJson;
        if (i < 99999) rawJson += ",";
    }
    rawJson += "]";

    std::cout << "Payload Size: " << rawJson.length() / 1024 / 1024 << " MB" << std::endl;

    // --- START TIMING ---
    auto start = std::chrono::high_resolution_clock::now();

    Lexer lexer(rawJson);
    std::vector<Token> tokens = lexer.tokenize();

    // --- STOP TIMING ---
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finish - start;

    std::cout << "Lexer Status: SUCCESS. Generated " << tokens.size() << " tokens." << std::endl;
    std::cout << "Lexer Time: " << elapsed.count() << " milliseconds." << std::endl;

    return 0;
}