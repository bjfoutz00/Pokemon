#include <iostream>
#include <fstream>
#include "Set.h"
#include "HashMap.h"
#include "Pair.h"

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

// Control output direction: 0 for output file, 1 for stdout
#define CONSOLE 1

int main(int argc, char* argv[]) {
    //Check for memory leakage
    VS_MEM_CHECK

    //Open files and verify functionality
    if (argc < 3) {
        std::cerr << "Please provide name of input and output files.\n";
        return 1;
    }

    std::cout << "Input file: " << argv[1] << std::endl;
    std::ifstream in(argv[1]);
    if (!in) {
        std::cerr << "Unable to open " << argv[1] << " for input";
        return 2;
    }

    // output to stdout or an output file, see console definition above
    std::cout << "Output File: " << (CONSOLE ? "cout" : argv[2]) << std::endl;
    std::ostream& out = CONSOLE ? std::cout : *(new std::ofstream(argv[2]));

    if (!out) {
        in.close();
        std::cerr << "Unable to open " << argv[2] << " for output";
        return 3;
    }

    Set<std::string> types;
    HashMap<std::string, std::string> pokemon;
    HashMap<std::string, std::string> moves;
    HashMap<std::string, Set<std::string>> effectivities;
    HashMap<std::string, Set<std::string>> ineffectivities;
    std::string line, command, key, value;

    while (getline(in, line)) {
        try {
            if (line.size() == 0) {
                continue;
            }
            std::istringstream iss(line);
            iss >> command;
            if (command == "Set:") {
                out << command;
                if (types.size() != 0) {
                    types.clear();
                }
                while (iss >> key) {
                    out << " " << key;
                    types.insert(key);
                }
                out << std::endl << "  [" << types << "]" << std::endl;
            }
            // Insert a pokemon to the pokemon map. key = name, value = type
            else if (command == "Pokemon:") {
                iss >> key;
                iss >> value;
                out << command << " " << key << " " << value << std::endl;
                pokemon[key] = value;
            }
            // Insert a move to the move map. key = move name, value = move type
            else if (command == "Move:") {
                iss >> key;
                iss >> value;
                out << command << " " << key << " " << value << std::endl;
                moves[key] = value;
            }
            // Insert an effectivity to the map of effectivities.
            // key = type, value = set of types that it is effective against.
            else if (command == "Effective:") {
                // key will be used to store the hashmap key, value will be used to store the set keys
                iss >> key;
                out << command << " " << key;

                while (iss >> value) {
                    out << " " << value;
                    effectivities[key].insert(value);
                }
                out << std::endl;
            }
                // Insert an ineffectivity to the map of ineffectivities.
                // key = type, value = set of types that it is not effective against.
            else if (command == "Ineffective:") {
                // key will be used to store the hashmap key, value will be used to store the set keys
                iss >> key;
                out << command << " " << key;

                while (iss >> value) {
                    out << " " << value;
                    ineffectivities[key].insert(value);
                }
                out << std::endl;
            }
            // Output commands: output the corresponding maps
            else if (command == "Pokemon") {
                out << std::endl << command << ": " << pokemon;
            }
            else if (command == "Moves") {
                out << std::endl << command << ": " << moves;
            }
            else if (command == "Effectivities") {
                out << std::endl << command << ": " << effectivities;
            }
            else if (command == "Ineffectivities") {
                out << std::endl << command << ": " << ineffectivities;
            }
            // Determine winner between two pokemon based on their type and the move type
            else if (command == "Battle:") {
                std::string pokemonA, moveA, pokemonB, moveB;
                // Read in and output pokemon and attacks
                iss >> pokemonA >> moveA >> pokemonB >> moveB;
                out << std::endl << command << " " << pokemonA << " " << moveA
                    << " " << pokemonB << " " << moveB << std::endl;

                // Introduce pokemon and declare attacks
                out << "  " << pokemonA << " (" << moveA << ") vs " << pokemonB << " (" << moveB << ")\n";
                out << "  " << pokemonA << " is a " << pokemon[pokemonA] << " type Pokemon.\n";
                out << "  " << pokemonB << " is a " << pokemon[pokemonB] << " type Pokemon.\n";
                out << "  " << moveA << " is a " << moves[moveA] << " type move.\n";
                out << "  " << moveB << " is a " << moves[moveB] << " type move.\n";

                // Commence battle! Calculate damage stats
                // A pokemon will win if their move type is effective against the opposing pokemon's type,
                //  or if the opposing pokemon's move type is not effective against the pokemon's type.
                int damageAtoB = effectivities[moves[moveA]].count(pokemon[pokemonB])
                    - ineffectivities[moves[moveA]].count(pokemon[pokemonB]);
                int damageBtoA = effectivities[moves[moveB]].count(pokemon[pokemonA])
                    - ineffectivities[moves[moveB]].count(pokemon[pokemonA]);

                // Output pokemonA's effectiveness
                out << "  " << moveA << " is super effective against ["
                    << effectivities[moves[moveA]] << "] type Pokemon.\n";
                out << "  " << moveA << " is ineffective against ["
                    << ineffectivities[moves[moveA]] << "] type Pokemon.\n";
                out << "  " << pokemonA << "'s " << moveA << " is ";
                if (damageAtoB > 0) {
                    out << "super ";
                }
                else if (damageAtoB < 0) {
                    out << "in";
                }
                out << "effective against " << pokemonB << std::endl;

                // Output pokemonB's effectiveness
                out << "  " << moveB << " is super effective against ["
                    << effectivities[moves[moveB]] << "] type Pokemon.\n";
                out << "  " << moveB << " is ineffective against ["
                    << ineffectivities[moves[moveB]] << "] type Pokemon.\n";
                out << "  " << pokemonB << "'s " << moveB << " is ";
                if (damageBtoA > 0) {
                    out << "super ";
                }
                else if (damageBtoA < 0) {
                    out << "in";
                }
                out << "effective against " << pokemonA << std::endl;

                // Declare winner
                if ((damageAtoB - damageBtoA) > 0) {
                    out << "  In the battle between " << pokemonA << " and " 
                        << pokemonB << ", " << pokemonA << " wins!\n";
                }
                else if ((damageAtoB - damageBtoA) < 0) {
                    out << "  In the battle between " << pokemonA << " and "
                        << pokemonB << ", " << pokemonB << " wins!\n";
                }
                else {
                    out << "  The battle between " << pokemonA << " and " 
                        << pokemonB << " is a tie.\n";
                }
                out << std::endl;
            }
        }
        catch (std::logic_error ex) {
            out << ex.what();
        }
    }

    in.close();
    if (!CONSOLE) {
        delete& out;
    }
}