#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <unordered_map>
#include <cctype>

// --- THE BASE INTERFACE ---
class JsonNode {

public:
    virtual ~JsonNode() = default;
    
    // Every node MUST know how to print itself as valid JSON text.
    virtual std::string print() const = 0; 
};

// --- THE LEAF NODES ---
class JsonString : public JsonNode {
private:
    std::string value;
public:
    JsonString(const std::string& val) : value(val) {}
    
    std::string print() const override {
        // JSON strings must be wrapped in quotes!
        return "\"" + value + "\""; 
    }
};

class JsonNumber : public JsonNode {
private:
    double value;
public:
    JsonNumber(const double& val) : value(val) {}
    
    std::string print() const override {
        // std::to_string converts numbers to text
        return std::to_string(value); 
    }
};

// --- THE COMPOSITE NODE (YOUR MISSION) ---
class JsonArray : public JsonNode {
private:
    // This vector exclusively owns whatever nodes are inside it!
    std::vector<std::unique_ptr<JsonNode>> elements;

public:
    // 1. Write an 'add' method that takes a unique_ptr<JsonNode> and std::move's it into the vector.
    void add(std::unique_ptr<JsonNode> element){
        elements.push_back(std::move(element));
    }
    // 2. Override the print() method! 
    // It needs to return a string that looks like this: "[ item1, item2, item3 ]"
    std::string print() const override{
        std::string temp = "[ ";
        if(elements.empty()){
            return "[ ]";
        }else{
            for(size_t i = 0; i < elements.size(); i++){
                if(i != (elements.size() - 1)){
                    temp += elements[i]->print() + ", ";
                }else{
                    temp += elements[i]->print() + " ]";
                }
            }
        return temp;
        }
    }
};

class JsonObject : public JsonNode {
private:
    // The dictionary mapping strings to polymorphic nodes
    std::unordered_map<std::string, std::unique_ptr<JsonNode>> map;

public:
    // 1. Write the add() method. Use std::move() again!
    void add(const std::string& key, std::unique_ptr<JsonNode> value) {
        map[key] = std::move(value);
    }

    // 2. Override print(). Format must be: { "key1": value1, "key2": value2 }
    std::string print() const override {
        std::string temp = "{ "; 
        bool isFirst = true;
        for(const auto& pair : map){
            if(!isFirst){
                temp += ", ";
            }
            temp += "\"" + pair.first + "\": " + pair.second->print();
            isFirst = false;
        }
        temp += " }";
        return temp;
    }
};

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
            if(c == '"'){
                std::string temp = "";
                advance();
                while(peek() != '"' && !isAtEnd()){
                    temp += advance();
                }
                advance();
                tokens.push_back({TokenType::STRING, temp});
            }
            // 4. Handle Numbers (We will do this next!)
            if(std::isdigit(c) || c == '-'){
                std::string temp = "";
                while(!isAtEnd() && (std::isdigit(peek()) || peek() == '.')){
                    temp += advance();
                }
                tokens.push_back({TokenType::NUMBER, temp});
            }


            // --------------------------------
            
            // Temporary fail-safe so we don't infinite loop while building:
            else {
                std::cout << "Lexer error: Unknown character '" << c << "'" << std::endl;
                advance(); 
            }
        }

        return tokens;
    }
};

// --- THE UNIT TEST ---
int main() {
    std::string rawJson = "{ [ : ] } ,   \n  {";
    Lexer lexer(rawJson);
    
    std::vector<Token> tokens = lexer.tokenize();
    
    for (const auto& t : tokens) {
        std::cout << "Found Token Type: " << static_cast<int>(t.type) << " | Text: " << t.text << std::endl;
    }
    return 0;
}