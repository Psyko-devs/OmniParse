#include "parser.hpp"

int main() {
    // 1. THE RAW DATA (The messy text file)
    std::string rawJson = R"(
    {
        "project": "OmniParse",
        "version": 1.0,
        "modules": [ "AST", "Lexer", "Parser" ],
        "metadata": {
            "architect": "Artemis",
            "level": 42.5
        }
    }
    )";

    std::cout << "--- OMNIPARSE INITIALIZATION ---" << std::endl;
    std::cout << "Raw Data Length: " << rawJson.length() << " characters." << std::endl;

    // 2. STATION 2: THE LEXER (Chop it into tokens)
    Lexer lexer(rawJson);
    std::vector<Token> tokens = lexer.tokenize();
    std::cout << "Lexer Status: SUCCESS. Generated " << tokens.size() << " tokens." << std::endl;

    // 3. STATION 3: THE PARSER (Build the AST)
    Parser parser(tokens);
    std::unique_ptr<JsonNode> astRoot = parser.parseValue();
    std::cout << "Parser Status: SUCCESS. AST Constructed." << std::endl;

    // 4. STATION 1: THE AST (Print the data)
    std::cout << "\n--- RECONSTRUCTED OUTPUT ---" << std::endl;
    std::cout << astRoot->print() << std::endl;

    return 0;
}