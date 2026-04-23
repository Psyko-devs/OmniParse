#include "json.hpp"
#include "lexer.hpp"
#include "arena.hpp"

const int MAX_DEPTH = 512;

class Parser {
private:
    std::vector<Token> tokens; // The list we got from the Lexer
    size_t pos;                // Our cursor pointing to the current Token
    ArenaAllocator& arena;

    bool isAtEnd() const {
        return pos >= tokens.size();
    }

    Token peek() const {
        if (isAtEnd()) return tokens.back(); // Safety check
        return tokens[pos];
    }

    Token advance() {
        if (!isAtEnd()) pos++;
        return tokens[pos - 1];
    }

public:
    Parser(const std::vector<Token>& lexerTokens, ArenaAllocator& allocator) 
    : tokens(lexerTokens), pos(0), arena(allocator) {}

    // This is the master function that builds the tree!
    JsonNode* parseValue(int depth = 0) {
        if(tokens.empty()){
            throw std::runtime_error("Parser Error: El archivo está vacío o solo contiene espacios.");
        }
        
        if(depth > MAX_DEPTH){
            throw std::runtime_error("Exceeded maximum depth!");
        }
        Token current = peek();

        if (current.type == TokenType::STRING) {
            advance(); // Consume the token
            return arena.create<JsonString>(std::string(current.text));
        }
        else if (current.type == TokenType::NUMBER) {
            advance(); // Consume the token
            // std::stod converts a std::string to a double!
            return arena.create<JsonNumber>(std::stod(std::string(current.text)));
        }
        else if (current.type == TokenType::BRACKET_OPEN) {
            return parseArray(depth + 1); // We will write this next
        }
        else if (current.type == TokenType::CURLY_OPEN) {
            return parseObject(depth + 1); // We will write this next
        }

        throw std::runtime_error("Parser Error: Unexpected token " + std::string(current.text));
    }

    // Helper method to parse Arrays
    JsonNode* parseArray(int depth = 0) {
        Token current = peek();
        if(current.type == TokenType::BRACKET_OPEN){
            advance();
            JsonArray* arrayNode = arena.create<JsonArray>();
            while(peek().type != TokenType::BRACKET_CLOSE && !isAtEnd()){
                arrayNode->add(parseValue(depth + 1));
                if(peek().type == TokenType::COMMA){
                    advance();
                }
            }
            if(peek().type == TokenType::BRACKET_CLOSE){
                advance();
            }
            return arrayNode;
        }
        throw std::runtime_error("Parser Error: Expected [ for array");
    }
        

    // Helper method to parse Objects
    JsonNode* parseObject(int depth = 0) {
        advance();
        JsonObject* objNode = arena.create<JsonObject>();
        while(peek().type != TokenType::CURLY_CLOSE && !isAtEnd()){
            std::string_view key = peek().text;
            advance();
            advance();
            objNode->add(std::string(key), parseValue(depth + 1));
            if(peek().type == TokenType::COMMA){
                advance();
            }
        }
        advance();
        return objNode;
    }
};