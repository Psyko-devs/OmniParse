#include <iostream>
#include <string>
#include <vector>
#include <random>

#include "lexer.hpp"
#include "parser.hpp"
#include "arena.hpp"

// 1. EL GENERADOR DE CAOS
std::string generateGarbage(size_t length) {
    // Estos son los caracteres que más confunden a los parsers
    const std::string charset = "{}[],\":-123.abc\\\" \n\t"; 
    std::string result;
    result.reserve(length);

    // Herramientas modernas de C++ para números aleatorios
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, charset.size() - 1);

    // TU MISIÓN PARTE 1:
    // Escribe un bucle que se ejecute 'length' veces.
    // En cada iteración, elige un carácter aleatorio de 'charset' usando distrib(gen)
    // y añádelo a 'result'.
    for(size_t i = 0; i < length; i++){
        char car = charset[distrib(gen)];
        result += car;
    }

    return result;
}

int main() {
    std::cout << "--- INICIANDO FUZZER DE CAOS OMNIPARSE ---" << std::endl;
    
    int successCount = 0;
    const int TOTAL_ROUNDS = 10000;

    for (int i = 0; i < TOTAL_ROUNDS; i++) {
        // Generamos un string de longitud aleatoria (entre 1 y 500 caracteres)
        std::string garbage = generateGarbage(rand() % 500 + 1);

        try {
            ArenaAllocator arena;
            Lexer lexer(garbage);
            std::vector<Token> tokens = lexer.tokenize();
            
            Parser parser(tokens, arena);
            JsonNode* astRoot = parser.parseValue();
            
            // Si llega aquí, significa que, por un milagro estadístico, 
            // el Fuzzer generó un JSON válido, o... hay un falso positivo.
            
        } catch (const std::exception& e) {
            // ¡ESTO ES LO QUE QUEREMOS! 
            // El motor detectó la basura y abortó de forma segura.
            successCount++;
        } catch (...) {
            // Si atrapamos algo que no es un runtime_error, el motor colapsó gravemente.
            std::cerr << "\n[!] FALLO CATASTRÓFICO EN LA RONDA " << i << std::endl;
            std::cerr << "Payload que mató al motor: " << garbage << std::endl;
            return 1; 
        }

        // Imprimir progreso en la misma línea
        if (i % 1000 == 0) {
            std::cout << "Rondas completadas: " << i << " / " << TOTAL_ROUNDS << "\r" << std::flush;
        }
    }

    std::cout << "\n\n[+] PRUEBA DE ESTRÉS COMPLETADA." << std::endl;
    std::cout << "Supervivencia: " << successCount << " / " << TOTAL_ROUNDS << " cargas neutralizadas." << std::endl;

    return 0;
}