**Fretboard Explorer**

Overview:
Fretboard Explorer is designed for guitarists who want to explore various chord progressions and their corresponding finger-positions on the fretboard. The program takes user input for a chord, key, or chord progression and computes all possible fingerings across the fretboard. Users can specify detailed chord qualities including major, minor, diminished, augmented, and extended chords such as 7th, 9th, and more. It supports specification of chord criteria (such as 3 strings only, or 3 frets apart) or modifications to match user preferences. 

Compiling:
gcc -Wall -Werror -fsanitize=address Fretboard.c -o Fretboard

Running: 
./fretboard [chord/key/progression]
./fretboard Cmaj7
./fretboard C F G Am


