**Fretboard Explorer v1.0**

________________________________________________________________________________________________________________

**OVERVIEW:**

Fretboard Explorer is designed for guitarists who want to explore various chord progressions and their corresponding finger-positions on the fretboard. The program takes user input for a chord, key, or chord progression and computes all possible fingerings across the fretboard. Users can specify detailed chord qualities including major, minor, diminished, augmented, and extended chords such as 7th, 9th, and more. It supports specification of chord criteria (such as 3 strings only, or 3 frets apart) or modifications to match user preferences. 

________________________________________________________________________________________________________________

![image](https://github.com/user-attachments/assets/e79599b6-c0b5-4e35-ae1c-7b99ccd2deef)

________________________________________________________________________________________________________________

![image](https://github.com/user-attachments/assets/a2ae3ec6-5d85-46fc-8c49-8c5c42036b99)

________________________________________________________________________________________________________________

Compiling:
gcc -Wall -Werror -fsanitize=address Fretboard.c -o Fretboard

Running: 
./fretboard [chord/key/progression]

./fretboard Cmaj7

./fretboard C F G Am


