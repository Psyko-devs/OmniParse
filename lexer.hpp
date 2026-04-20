#include "json.hpp"

enum class TokenType {
    CURLY_OPEN,    // {
    CURLY_CLOSE,   // }
    BRACKET_OPEN,  // [
    BRACKET_CLOSE, // ]
    COLON,         // :
    COMMA,         // ,
    STRING,        // "Artemis"
    NUMBER         // 42
};

struct Token{
    TokenType type;
    std::string text;
};


class Lexer {
private:
    std::string src; // The raw JSON text
    size_t pos;      // Our current position in the text

    // Helper function: Are we at the end of the file?
    bool isAtEnd() const {
        return pos >= src.length();
    }

    // Helper function: Look at the current character without moving forward
    char peek() const {
        if (isAtEnd()) return '\0';
        return src[pos];
    }

    // Helper function: Get the current character AND move the cursor forward by 1
    char advance() {
        if (isAtEnd()) return '\0';
        return src[pos++];
    }

public:
    Lexer(const std::string& sourceText) {
        src = sourceText;
        pos = 0;
    }

    // THE MAIN ENGINE
    std::vector<Token> tokenize() {
        std::vector<Token> tokens;

        while (!isAtEnd()) {
            char c = peek();

            // --- YOUR MISSION STARTS HERE ---
            
            // 1. Skip Whitespace (' ', '\t', '\n', '\r')
            // If it's a space, just advance() and continue the loop.
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r'){
                advance();
                continue;
            }
            // 2. Handle Single-Character Tokens ({, }, [, ], :, ,)
            // If it's a '{', advance(), and push a Token into the vector.
            // Example: tokens.push_back({TokenType::CURLY_OPEN, "{"});
            if(c == '{'){
                advance();
                tokens.push_back({TokenType::CURLY_OPEN, "{"});
            }else if(c == '}'){
                advance();
                tokens.push_back({TokenType::CURLY_CLOSE, "}"});
            }else if(c == '['){
                advance();
                tokens.push_back({TokenType::BRACKET_OPEN, "["});
            }else if(c == ']'){
                advance();
                tokens.push_back({TokenType::BRACKET_CLOSE, "]"});
            }else if(c == ':'){
                advance();
                tokens.push_back({TokenType::COLON, ":"});
            }else if(c == ','){
                advance();
                tokens.push_back({TokenType::COMMA, ","});
            }

            // 3. Handle Strings (We will do this next!)
            else if(c == '"'){
                std::string temp = "";
                advance();
                while(peek() != '"' && !isAtEnd()){
                    temp += advance();
                }
                advance();
                tokens.push_back({TokenType::STRING, temp});
            }
            // 4. Handle Numbers (We will do this next!)
            else if(std::isdigit(c) || c == '-'){
                std::string temp = "";
                while(!isAtEnd() && (std::isdigit(peek()) || peek() == '.')){
                    temp += advance();
                }
                tokens.push_back({TokenType::NUMBER, temp});
            }
            // Temporary fail-safe so we don't infinite loop while building:
            else {
                std::cout << "Lexer error: Unknown character '" << c << "'" << std::endl;
                advance(); 
            }
        }

        return tokens;
    }
};