# Pokemon Arena
A simple Pokemon battle simulator demonstrating a knowledgeable use of maps, sets, and binary search trees

Pokemon are creatures that use specific moves to fight each other. Every Pokemon and every move is of a certain type (water, fire, steel, etc), and certain types are either effective, normal, or not effective against other types.

The simulator functions using four main maps:

1. Pokemon: pokemon names keyed to their type
2. Moves: move names keyed to their type
3. Effectivities: types keyed to the set of types they are effective against
4. Ineffectivities: types keyed to the set of types they are ineffective against


The following commands are used to operate the simulator:

1. **Pokemon: _name_ _type_** adds the Pokemon to the map of Pokemon
2. **Moves: _name_ _type_** adds the move to the map of moves
3. **Effective: _type_ _type type ... type_** adds a type to the map of effectivities, with the value being the types it is effective against
4. **Ineffective: _type_ _type type ... type_** adds a type to the map of ineffectivities, with the value being the types it is ineffective against
5. **Pokemon** 
6. **Moves**
7. **Effectivities**
8. **Ineffectivities**
9. **Battle: _PokemonA_ _moveA_ _PokemonB_ _MoveB_** outputs results of a battle between two specified pokemon using their specified moves, drawing from all four maps to determine the winner. The battle is essentially a game of rock, paper, scissors using the type effectiveness.


The PokemonData.txt file contains a massive database of pokemon, moves, effectivities, and ineffectivities. It is to be used as the main input file and will add every item to the corresponding map. At the end of the file, it contains the commands to output each of the four maps, and a sample battle command. More battle commands can be added as the user wishes.
