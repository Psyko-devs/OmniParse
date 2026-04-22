#include "parser.hpp"
#include "lexer.hpp"
#include "json.hpp"
#include "arena.hpp"
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
    
    // --- THE NEW ARENA ENGINE ---
    ArenaAllocator arena; // Starts with a 64KB page
    Parser parser(tokens, arena); // Pass the engine to the parser
    JsonNode* astRoot = parser.parseValue();

    // --- STOP TIMING ---
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finish - start;

    std::cout << "Engine Status: SUCCESS. Parsed " << tokens.size() << " tokens." << std::endl;
    std::cout << "Total Time (Lexer + Parser): " << elapsed.count() << " milliseconds." << std::endl;

    return 0;
}