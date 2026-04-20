#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <unordered_map>

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

// --- THE UNIT TEST ---
int main() {
    std::cout << "--- OMNIPARSE AST BOOT SEQUENCE ---" << std::endl;

    // Create the Player Object
    JsonObject player;
    
    // Add simple data
    player.add("name", std::make_unique<JsonString>("Artemis"));
    player.add("level", std::make_unique<JsonNumber>(42));

    // Create an inventory array
    auto inventory = std::make_unique<JsonArray>();
    inventory->add(std::make_unique<JsonString>("Sword"));
    inventory->add(std::make_unique<JsonString>("Shield"));

    // Add the array INTO the object! (Tree branching!)
    player.add("inventory", std::move(inventory));

    std::cout << player.print() << std::endl;

    return 0; 
}