#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
typedef struct Node 
{
    int chord[6];
    struct Node *next;
} Node;

/*=============================================================================*/
void appendChord(Node **head, int chord[6]) 
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->next = NULL;
    for (int i = 0; i < 6; i++) 
    {
        newNode->chord[i] = chord[i];
    }
    if (*head == NULL) 
    {
        *head = newNode;
    } 
    else 
    {
        Node *temp = *head;
        while (temp->next != NULL) 
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}
void freeLinkedList(Node* head) 
{
    Node* temp;
    while (head != NULL) 
    {
        temp = head; 
        head = head->next;
        free(temp);
    }
}
/*=============================================================================*/
struct Note
{
    char note[3];    
    int fret;        
};
typedef struct Note Notes;
void PrintAllNotes(Notes Fretboard[6][18]);
void ConstructFretboard(Notes Fretboard[6][18]);
void PrintFretboard(Notes Fretboard[6][18], char *formula[], int length);
void PossibleChord(Notes Fretboard[6][18], char *formula[], int length, Node **chordList);
void findChords(Notes Fretboard[6][18], char *formula[], int length, int frets[6], int noteIndex, Node **chordList);
void PrintHorizontalChords(Notes Fretboard[6][18], Node *chordList);
int PrintChord(Notes Fretboard[6][18], int chord[6], char buffer[6][255]); 
void PrintBuffers(char buffer[6][255], int chordCount);
int RomanNumeral(char *str) 
{
    const char *RomanNumerals[] = {"i", "ii", "iii", "iv", "v", "vi", "vii", "I", "II", "III", "IV", "V", "VI", "VII", NULL};

    for (int i = 0; RomanNumerals[i] != NULL; i++) {
        if (strncmp(str, RomanNumerals[i], strlen(RomanNumerals[i])) == 0) {
            size_t length = strlen(RomanNumerals[i]);
            if (str[length] == '\0' || !isalpha((unsigned char)str[length])) {
                return i + 1;  // Return 1-7 for lowercase Roman numerals
            }
        }
    }
    return 0;  // No valid Roman numeral found
}
#define TOTAL_WIDTH 118

void ApplyChordFormula2(char *name, char *root, char **formula, int length, Notes Fretboard[6][18]) 
{
    char buffer[150];
    int bufferLength = snprintf(buffer, sizeof(buffer), " %s %s Chord: ", root, name);
    for (int i = 0; i < length; i++) 
    {
        bufferLength += snprintf(buffer + bufferLength, sizeof(buffer) - bufferLength, "%s ", formula[i]);
    }
    int padding = (TOTAL_WIDTH - bufferLength) / 2;
    padding = padding < 0 ? 0 : padding;
    for (int i = 0; i < padding; i++) 
    {
        putchar('=');
    }
    printf("%s", buffer);
    for (int i = 0; i < padding + ((TOTAL_WIDTH - bufferLength) % 2); i++) 
    {
        putchar('=');
    }
    printf("\n\n");
    
    Node *chordList = NULL;
    PossibleChord(Fretboard, formula, length, &chordList);
    PrintHorizontalChords(Fretboard, chordList);
    freeLinkedList(chordList);
}
void ApplyChordFormula(char *name, char *root, char **formula, int length, Notes Fretboard[6][18]) 
{
    printf("~ Notes in %s %s Chord: ", root, name);
    for (int i = 0; i < length; i++) 
    {
        printf("%s ", formula[i]);
    }
    printf("~\n\n\n");
    PrintFretboard(Fretboard, formula, length);
    printf("\n\n~ Possible %s %s Chords ~ \n\n\n", root, name);
    
    Node *chordList = NULL;
    PossibleChord(Fretboard, formula, length, &chordList);
    PrintHorizontalChords(Fretboard, chordList);
    freeLinkedList(chordList);
}

/*=========================================================================================================*/

int main(int argc, char *argv[])
{
    Notes Fretboard[6][18];
    ConstructFretboard(Fretboard);

    char chord[7];
    if (argc > 1) 
    {
        strncpy(chord, argv[1], sizeof(chord) - 1);
        chord[sizeof(chord) - 1] = '\0';
        //printf("Chord: %s\n", chord);
    }

    char root[3];
    if (chord[1] == '#') {root[0] = chord[0]; root[1] = '#'; root[2] = '\0';} 
    else {root[0] = chord[0]; root[1] = '\0';} //printf("Root: %s\n", root);

    char *notes[] = {"E", "F", "F#", "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#"};
    int x; for (int i = 0; i < 12; i++) {if (strcmp(root, notes[i]) == 0){x = i;}}


    if (argc == 1)
    {
        printf("\n\n");
        PrintAllNotes(Fretboard);
        printf("\n\n");
        return 0;
    }
    if (argc == 2 && RomanNumeral(argv[1]) == 0)
    {
        printf("\n\n");
        char *minor[]       = {notes[x], notes[(x+3)%12], notes[(x+7)%12]};
        char *minor7[]      = {notes[x], notes[(x+3)%12], notes[(x+7)%12], notes[(x+10)%12]};
        char *minor9[]      = {notes[x], notes[(x+3)%12], notes[(x+10)%12], notes[(x+2)%12]};
        char *minoradd9[]   = {notes[x], notes[(x+3)%12], notes[(x+7)%12], notes[(x+2)%12]};
        char *dominant7[]   = {notes[x], notes[(x+4)%12], notes[(x+7)%12], notes[(x+10)%12]};
        char *diminished[]  = {notes[x], notes[(x+3)%12], notes[(x+6)%12]};
        char *diminished7[] = {notes[x], notes[(x+3)%12], notes[(x+6)%12], notes[(x+9)%12]};
        char *major[]       = {notes[x], notes[(x+4)%12], notes[(x+7)%12]};
        char *major7[]      = {notes[x], notes[(x+4)%12], notes[(x+7)%12], notes[(x+11)%12]};
        char *major9[]      = {notes[x], notes[(x+4)%12], notes[(x+11)%12], notes[(x+2)%12]};
        char *majoradd9[]   = {notes[x], notes[(x+4)%12], notes[(x+7)%12], notes[(x+2)%12]};

        if (strlen(root) == 2)
        {
            if (chord[2] == 'm') 
            {
                if (chord[3] == '7') 
                {   ApplyChordFormula2 ("Minor 7th", root, minor7, 4, Fretboard);        }
                else if (chord[3] == '9')
                {   ApplyChordFormula2 ("Minor 9th", root, minor9, 4, Fretboard);        }
                else if (chord[3] == 'a' && chord[4] == '9')
                {   ApplyChordFormula2 ("Minor add9", root, minoradd9, 4, Fretboard);    }
                else
                {   ApplyChordFormula2 ("Minor", root, minor, 3, Fretboard);             }
            }

            else if (chord[2] == '7') 
            {   ApplyChordFormula2 ("Dominant 7th", root, dominant7, 4, Fretboard);  } 
            else if (chord[2] == '0')
                {
                    if (chord[3] == '7')
                    {   ApplyChordFormula2 ("Diminished 7th", root, diminished7, 4, Fretboard); }
                    else
                    {   ApplyChordFormula2 ("Diminished", root, diminished, 3, Fretboard);  }
                }

            else if ((chord[2] == 'M')) 
            {
                if (chord[3] == '7') 
                {   ApplyChordFormula2("Major 7th", root, major7, 4, Fretboard);         }
                else if (chord[3] == '9')
                {   ApplyChordFormula2 ("Major 9th", root, major9, 4, Fretboard);        }
                else if (chord[3] == 'a' && chord[4] == '9')
                {   ApplyChordFormula2 ("Major add9", root, majoradd9, 4, Fretboard);    }
                else
                {   ApplyChordFormula2 ("Major", root, major, 3, Fretboard);             }
            }
        }

        else 
        {
            if (chord[1] == 'm') 
            {
                if (chord[2] == '7') 
                {   ApplyChordFormula2 ("Minor 7th", root, minor7, 4, Fretboard);        }
                else if (chord[2] == '9')
                {   ApplyChordFormula2 ("Minor 9th", root, minor9, 4, Fretboard);        }
                else if (chord[2] == 'a' && chord[3] == '9')
                {   ApplyChordFormula2 ("Minor add9", root, minoradd9, 4, Fretboard);    }
                else
                {   ApplyChordFormula2 ("Minor", root, minor, 3, Fretboard);             }
            }

            else if (chord[1] == '7') 
            {   ApplyChordFormula2 ("Dominant 7th", root, dominant7, 4, Fretboard);  } 
            else if (chord[1] == '0')
            {
                if (chord[2] == '7')
                {   ApplyChordFormula2 ("Diminished 7th", root, diminished7, 4, Fretboard); }
                else
                {   ApplyChordFormula2 ("Diminished", root, diminished, 3, Fretboard);  }
            }

            else if ((chord[1] == 'M')) 
            {
                if (chord[2] == '7') 
                {   ApplyChordFormula2("Major 7th", root, major7, 4, Fretboard);         }
                else if (chord[2] == '9')
                {   ApplyChordFormula2 ("Major 9th", root, major9, 4, Fretboard);        }
                else if (chord[2] == 'a' && chord[3] == '9')
                {   ApplyChordFormula2 ("Major add9", root, majoradd9, 4, Fretboard);    }
                else
                {   ApplyChordFormula2 ("Major", root, major, 3, Fretboard);             }
            }
        }
        return 0;
    }
    
    /*--------------------------------------------------------------------------------------*/

    char **Progression;
    int i;

    if (argc > 2 && RomanNumeral(argv[2]) != 0)
    {
        Progression = malloc((argc - 2) * sizeof(char*));
        if (!Progression) {perror("Memory allocation failed"); return 1;}
        for (i = 2; i < argc; i++) 
        {
            Progression[i - 2] = malloc(5 * sizeof(char));
            if (!Progression[i - 2]) 
            {
                perror("Memory allocation failed");
                while (--i >= 2) free(Progression[i - 2]);
                free(Progression);
                return 1;
            }
            strncpy(Progression[i - 2], argv[i], 4);
            Progression[i - 2][4] = '\0';
        }

        /* PADDING */
        printf("\n\n\n");
        int printedLength = 0;
        printedLength += snprintf(NULL, 0, "~ Chord Progression in Key of %s: ", root);
        for (int i = 0; i < argc - 2; i++) {printedLength += snprintf(NULL, 0, "%s ", Progression[i]);}
        printedLength += 2;
        int padding = (TOTAL_WIDTH - printedLength) / 2;
        padding = padding < 0 ? 0 : padding;
        for (int i = 0; i < padding; i++) {putchar(' ');}
        printf("~ Chord Progression in Key of %s: ", root);
        for (int i = 0; i < argc - 2; i++) {printf("%s ", Progression[i]);}
        printf("~\n\n\n");
        

        char *ichord[] = {notes[(x)%12], notes[(x+3)%12], notes[(x+7)%12], notes[(x)%12]};             // i
        char *iichord[] = {notes[(x+2)%12], notes[(x+5)%12], notes[(x+8)%12], notes[(x+2)%12]};        // ii0
        char *iiichord[] = {notes[(x+3)%12], notes[(x+7)%12], notes[(x+10)%12], notes[(x+3)%12]};      // III
        char *ivchord[] = {notes[(x+5)%12], notes[(x+8)%12], notes[(x+12)%12], notes[(x+5)%12]};       // iv
        char *vchord[] = {notes[(x+7)%12], notes[(x+10)%12], notes[(x+14)%12], notes[(x+7)%12]};       // v
        char *vichord[] = {notes[(x+8)%12], notes[(x+12)%12], notes[(x+15)%12], notes[(x+8)%12]};      // VI
        char *viichord[] = {notes[(x+10)%12], notes[(x+14)%12], notes[(x+17)%12], notes[(x+10)%12]};   // VII

        char *i7chord[] = {notes[(x)%12], notes[(x+3)%12], notes[(x+7)%12], notes[(x+10)%12]};         // i7
        char *ii7chord[] = {notes[(x+2)%12], notes[(x+5)%12], notes[(x+8)%12], notes[(x+12)%12]};      // ii07
        char *iii7chord[] = {notes[(x+3)%12], notes[(x+7)%12], notes[(x+10)%12], notes[(x+14)%12]};    // III7
        char *iv7chord[] = {notes[(x+5)%12], notes[(x+8)%12], notes[(x+12)%12], notes[(x+15)%12]};     // iv7
        char *v7chord[] = {notes[(x+7)%12], notes[(x+10)%12], notes[(x+14)%12], notes[(x+17)%12]};     // v7
        char *vi7chord[] = {notes[(x+8)%12], notes[(x+12)%12], notes[(x+15)%12], notes[(x+19)%12]};    // VI7
        char *vii7chord[] = {notes[(x+10)%12], notes[(x+14)%12], notes[(x+17)%12], notes[(x+20)%12]};  // VII7
        
        char *i9chord[] = {notes[(x)%12], notes[(x+3)%12], notes[(x+10)%12], notes[(x+2)%12]};         // i9
        char *ii9chord[] = {notes[(x+2)%12], notes[(x+5)%12], notes[(x+12)%12], notes[(x+3)%12]};      // ii09
        char *iii9chord[] = {notes[(x+3)%12], notes[(x+7)%12], notes[(x+14)%12], notes[(x+5)%12]};     // III9
        char *iv9chord[] = {notes[(x+5)%12], notes[(x+8)%12], notes[(x+3)%12], notes[(x+7)%12]};       // iv9
        char *v9chord[] = {notes[(x+7)%12], notes[(x+10)%12], notes[(x+5)%12], notes[(x+8)%12]};       // v9
        char *vi9chord[] = {notes[(x+8)%12], notes[(x+12)%12], notes[(x+7)%12], notes[(x+10)%12]};     // VI9
        char *vii9chord[] = {notes[(x+10)%12], notes[(x+14)%12], notes[(x+8)%12], notes[(x+12)%12]};   // VII9

        char *Ichord[] = {notes[x], notes[(x+4)%12], notes[(x+7)%12], notes[x]};                       // I
        char *IIchord[] = {notes[(x+2)%12], notes[(x+5)%12], notes[(x+9)%12], notes[(x+2)%12]};        // ii
        char *IIIchord[] = {notes[(x+4)%12], notes[(x+7)%12], notes[(x+11)%12], notes[(x+4)%12]};      // iii
        char *IVchord[] = {notes[(x+5)%12], notes[(x+9)%12], notes[(x+12)%12], notes[(x+5)%12]};       // IV
        char *Vchord[] = {notes[(x+7)%12], notes[(x+11)%12], notes[(x+14)%12], notes[(x+7)%12]};       // V
        char *VIchord[] = {notes[(x+9)%12], notes[(x+12)%12], notes[(x+16)%12], notes[(x+9)%12]};      // vi
        char *VIIchord[] = {notes[(x+11)%12], notes[(x+14)%12], notes[(x+17)%12], notes[(x+11)%12]};   // vii0

        char *I7chord[] = {notes[x], notes[(x+4)%12], notes[(x+7)%12], notes[(x+11)%12]};              // I7
        char *II7chord[] = {notes[(x+2)%12], notes[(x+5)%12], notes[(x+9)%12], notes[(x+12)%12]};      // ii7
        char *III7chord[] = {notes[(x+4)%12], notes[(x+7)%12], notes[(x+11)%12], notes[(x+14)%12]};    // iii7
        char *IV7chord[] = {notes[(x+5)%12], notes[(x+9)%12], notes[(x+12)%12], notes[(x+16)%12]};     // IV7
        char *V7chord[] = {notes[(x+7)%12], notes[(x+11)%12], notes[(x+14)%12], notes[(x+17)%12]};     // V7
        char *VI7chord[] = {notes[(x+9)%12], notes[(x+12)%12], notes[(x+16)%12], notes[(x+19)%12]};    // vi7
        char *VII7chord[] = {notes[(x+11)%12], notes[(x+14)%12], notes[(x+17)%12], notes[(x+21)%12]};  // vii07

        char *I9chord[] = {notes[x], notes[(x+4)%12], notes[(x+11)%12], notes[(x+2)%12]};              // I9
        char *II9chord[] = {notes[(x+2)%12], notes[(x+5)%12], notes[(x+12)%12], notes[(x+4)%12]};      // ii9
        char *III9chord[] = {notes[(x+4)%12], notes[(x+7)%12], notes[(x+14)%12], notes[(x+5)%12]};     // iii9
        char *IV9chord[] = {notes[(x+5)%12], notes[(x+9)%12], notes[(x+16)%12], notes[(x+7)%12]};      // IV9
        char *V9chord[] = {notes[(x+7)%12], notes[(x+11)%12], notes[(x+17)%12], notes[(x+9)%12]};      // V9
        char *VI9chord[] = {notes[(x+9)%12], notes[(x+12)%12], notes[(x+19)%12], notes[(x+11)%12]};    // vi9
        char *VII9chord[] = {notes[(x+11)%12], notes[(x+14)%12], notes[(x+21)%12], notes[(x+12)%12]};  // vii09

        //char *MajorChordScale[] = {"I", "ii", "iii", "IV", "V", "vi", "vii0"};
        ////                          C     d     e     F     G     a     b0

        //char *MinorChordScale[] = {"i", "ii0", "III", "iv", "v", "VI", "VII"};
        ////                          a     b0    C     d     e     F     G  

        /* --------------CHORD PROGRESSION-------------- */
        for (int i = 0; i < argc - 2; i++)
        {
            if ((RomanNumeral(Progression[i]) == 1))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Minor 9th", notes[(x)%12], i9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Minor 7th", notes[(x)%12], i7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Minor", notes[(x)%12], ichord, 4, Fretboard);}
            }
            
            else if ((RomanNumeral(Progression[i]) == 8))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Major 9th", notes[(x)%12], I9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Major 7th", notes[(x)%12], I7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Major", notes[(x)%12], Ichord, 4, Fretboard);}
            }

            else if ((RomanNumeral(Progression[i]) == 2))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Diminished 9th", notes[(x+2)%12], ii9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Diminished 7th", notes[(x+2)%12], ii7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Diminished", notes[(x+2)%12], iichord, 4, Fretboard);}
            }

            else if ((RomanNumeral(Progression[i]) == 9))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Minor 9th", notes[(x+2)%12], II9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Minor 7th", notes[(x+2)%12], II7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Minor", notes[(x+2)%12], IIchord, 4, Fretboard);}
            }

            else if ((RomanNumeral(Progression[i]) == 3))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Major 9th", notes[(x+3)%12], iii9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Major 7th", notes[(x+3)%12], iii7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Major", notes[(x+3)%12], iiichord, 4, Fretboard);}
            }

            else if ((RomanNumeral(Progression[i]) == 10))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Minor 9th", notes[(x+4)%12], III9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Minor 7th", notes[(x+4)%12], III7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Minor", notes[(x+4)%12], IIIchord, 4, Fretboard);}
            }

            else if ((RomanNumeral(Progression[i]) == 4))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Minor 9th", notes[(x+5)%12], iv9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Minor 7th", notes[(x+5)%12], iv7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Minor", notes[(x+5)%12], ivchord, 4, Fretboard);}
            }

            else if ((RomanNumeral(Progression[i]) == 11))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Major 9th", notes[(x+5)%12], IV9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Major 7th", notes[(x+5)%12], IV7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Major", notes[(x+5)%12], IVchord, 4, Fretboard);}
            }

            else if ((RomanNumeral(Progression[i]) == 5))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Minor 9th", notes[(x+7)%12], v9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Minor 7th", notes[(x+7)%12], v7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Minor", notes[(x+7)%12], vchord, 4, Fretboard);}
            }

            else if ((RomanNumeral(Progression[i]) == 12))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Dominant 9th", notes[(x+7)%12], V9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Dominant 7th", notes[(x+7)%12], V7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Dominant", notes[(x+7)%12], Vchord, 4, Fretboard);}
            }

            else if ((RomanNumeral(Progression[i]) == 6))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Major 9th", notes[(x+8)%12], vi9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Major 7th", notes[(x+8)%12], vi7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Major", notes[(x+8)%12], vichord, 4, Fretboard);}
            }

            else if ((RomanNumeral(Progression[i]) == 13))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Minor 9th", notes[(x+9)%12], VI9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Minor 7th", notes[(x+9)%12], VI7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Minor", notes[(x+9)%12], VIchord, 4, Fretboard);}
            }

            else if ((RomanNumeral(Progression[i]) == 7))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Dominant 9th", notes[(x+10)%12], vii9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Dominant 7th", notes[(x+10)%12], vii7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Dominant", notes[(x+10)%12], viichord, 4, Fretboard);}
            }

            else if ((RomanNumeral(Progression[i]) == 14))
            {
                if (strstr(Progression[i], "9") != NULL)
                {
                    ApplyChordFormula ("Diminished 9th", notes[(x+11)%12], VII9chord, 4, Fretboard);
                }
                else if (strstr(Progression[i], "7") != NULL)
                {
                    ApplyChordFormula ("Diminished 7th", notes[(x+11)%12], VII7chord, 4, Fretboard);
                }
                else {ApplyChordFormula ("Diminished", notes[(x+11)%12], VIIchord, 4, Fretboard);}
            }

            else printf("\n INVALID INPUT \n");
        }

        for (i = 0; i < argc - 2; i++) 
        {
            free(Progression[i]);
        }
        free(Progression);
    }

    /*--------------------------------------------------------------------------------------*/

    else if (argc > 2 && RomanNumeral(argv[2]) == 0)
    {
        Progression = malloc((argc - 1) * sizeof(char*));
        if (!Progression) {perror("Memory allocation failed"); return 1;}
        for (i = 1; i < argc; i++) 
        {
            Progression[i - 1] = malloc(6 * sizeof(char));
            if (!Progression[i - 1]) 
            {
                perror("Memory allocation failed");
                while (--i >= 1) free(Progression[i - 1]);
                free(Progression);
                return 1;
            }
            strncpy(Progression[i - 1], argv[i], 5);
            Progression[i - 1][5] = '\0';
        }

        /* PADDING */
        printf("\n\n\n");
        int printedLength = 0;
        printedLength += snprintf(NULL, 0, "~ Chord Progression: ");
        for (int i = 0; i < argc - 1; i++) {printedLength += snprintf(NULL, 0, "%s ", Progression[i]);}
        printedLength += 2;
        int padding = (TOTAL_WIDTH - printedLength) / 2;
        padding = padding < 0 ? 0 : padding;
        for (int i = 0; i < padding; i++) {putchar(' ');}
        printf("~ Chord Progression: ");
        for (int i = 0; i < argc - 1; i++) {printf("%s ", Progression[i]);}
        printf("~\n\n\n");
   
        /* --------------CHORD PROGRESSION-------------- */
        for (int i = 0; i < argc - 1; i++)
        {
            char rootX[3];
            if (Progression[i][1] == '#') {rootX[0] = Progression[i][0]; rootX[1] = '#'; rootX[2] = '\0';} 
            else {rootX[0] = Progression[i][0]; rootX[1] = '\0';}  

            char *notesX[] = {"E", "F", "F#", "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#"};
            int y; for (int i = 0; i < 12; i++) {if (strcmp(rootX, notesX[i]) == 0){y = i;}}

            char *minor[]       = {notesX[y], notesX[(y+3)%12], notesX[(y+7)%12]};
            char *minor7[]      = {notesX[y], notesX[(y+3)%12], notesX[(y+7)%12],  notesX[(y+10)%12]};
            char *minor9[]      = {notesX[y], notesX[(y+3)%12], notesX[(y+10)%12], notesX[(y+2)%12]};
            char *minoradd9[]   = {notesX[y], notesX[(y+3)%12], notesX[(y+7)%12],  notesX[(y+2)%12]};
            char *dominant7[]   = {notesX[y], notesX[(y+4)%12], notesX[(y+7)%12],  notesX[(y+10)%12]};
            char *diminished[]  = {notesX[y], notesX[(y+3)%12], notesX[(y+6)%12]};
            char *diminished7[] = {notesX[y], notesX[(y+3)%12], notesX[(y+6)%12],  notesX[(y+9)%12]};
            char *major[]       = {notesX[y], notesX[(y+4)%12], notesX[(y+7)%12]};
            char *major7[]      = {notesX[y], notesX[(y+4)%12], notesX[(y+7)%12],  notesX[(y+11)%12]};
            char *major9[]      = {notesX[y], notesX[(y+4)%12], notesX[(y+11)%12], notesX[(y+2)%12]};
            char *majoradd9[]   = {notesX[y], notesX[(y+4)%12], notesX[(y+7)%12],  notesX[(y+2)%12]};

            if (strlen(rootX) == 2)
            {
                if (Progression[i][2] == 'm') 
                {
                    if (Progression[i][3] == '7') 
                    {   ApplyChordFormula ("Minor 7th", rootX, minor7, 4, Fretboard);        }
                    else if (Progression[i][3] == '9')
                    {   ApplyChordFormula ("Minor 9th", rootX, minor9, 4, Fretboard);        }
                    else if (Progression[i][3] == 'a' && Progression[i][4] == '9')
                    {   ApplyChordFormula ("Minor add9", rootX, minoradd9, 4, Fretboard);    }
                    else
                    {   ApplyChordFormula ("Minor", rootX, minor, 3, Fretboard);             }
                }

                else if (Progression[i][2] == '7') 
                {   ApplyChordFormula ("Dominant 7th", rootX, dominant7, 4, Fretboard);  }
                else if (Progression[i][2] == '0')
                {
                    if (Progression[i][3] == '7')
                    {   ApplyChordFormula ("Diminished 7th", rootX, diminished7, 4, Fretboard); }
                    else
                    {   ApplyChordFormula ("Diminished", rootX, diminished, 3, Fretboard);  }
                }

                else if ((Progression[i][2] == 'M')) 
                {
                    if (Progression[i][3] == '7') 
                    {   ApplyChordFormula("Major 7th", rootX, major7, 4, Fretboard);         }
                    else if (Progression[i][3] == '9')
                    {   ApplyChordFormula ("Major 9th", rootX, major9, 4, Fretboard);        }
                    else if (Progression[i][3] == 'a' && Progression[i][4] == '9')
                    {   ApplyChordFormula ("Major add9", rootX, majoradd9, 4, Fretboard);    }
                    else
                    {   ApplyChordFormula ("Major", rootX, major, 3, Fretboard);             }
                }
            }

            else 
            {
                if (Progression[i][1] == 'm') 
                {
                    if (Progression[i][2] == '7') 
                    {   ApplyChordFormula ("Minor 7th", rootX, minor7, 4, Fretboard);        }
                    else if (Progression[i][2] == '9')
                    {   ApplyChordFormula ("Minor 9th", rootX, minor9, 4, Fretboard);        }
                    else if (Progression[i][2] == 'a' && Progression[i][3] == '9')
                    {   ApplyChordFormula ("Minor add9", rootX, minoradd9, 4, Fretboard);    }
                    else
                    {   ApplyChordFormula ("Minor", rootX, minor, 3, Fretboard);             }
                }

                else if (Progression[i][1] == '7') 
                {   ApplyChordFormula ("Dominant 7th", rootX, dominant7, 4, Fretboard);  }
                else if (Progression[i][1] == '0')
                {
                    if (Progression[i][2] == '7')
                    {   ApplyChordFormula ("Diminished 7th", rootX, diminished7, 4, Fretboard); }
                    else
                    {   ApplyChordFormula ("Diminished", rootX, diminished, 3, Fretboard);  }
                } 

                else if ((Progression[i][1] == 'M')) 
                {
                    if (Progression[i][2] == '7') 
                    {   ApplyChordFormula("Major 7th", rootX, major7, 4, Fretboard);         }
                    else if (Progression[i][2] == '9')
                    {   ApplyChordFormula ("Major 9th", rootX, major9, 4, Fretboard);        }
                    else if (Progression[i][2] == 'a' && Progression[i][3] == '9')
                    {   ApplyChordFormula ("Major add9", rootX, majoradd9, 4, Fretboard);    }
                    else
                    {   ApplyChordFormula ("Major", rootX, major, 3, Fretboard);             }
                }
            }
        }
        
        for (i = 0; i < argc - 1; i++) 
        {
            free(Progression[i]);
        }
        free(Progression);
    }

    return 0;
}    

/*=========================================================================================================*/

void PrintFretboard(Notes Fretboard[6][18], char *formula[], int length)
{
    for (int i = 0; i < 6; i++)
    {              
        for (int j = 0; j < 18; j++)
        {
            if (j == 0)
            {    printf("(%d)--|-", Fretboard[i][j].fret);  }
            else
            {   printf("(%d)----", Fretboard[i][j].fret);   } 
            for (int k = 0; k < length; k++)
            {  
                if (!(Fretboard[i][j].note[1] == '#') && (strcmp(Fretboard[i][j].note, formula[k]) == 0))
                { 
                    if (j == 0)
                    {   printf("\b\b\b\b%s-|-", Fretboard[i][j].note);  }
                    else
                    {   printf("\b\b\b\b%s---", Fretboard[i][j].note);  }
                }
                else if ((Fretboard[i][j].note[1] == '#') && (strcmp(Fretboard[i][j].note, formula[k]) == 0))
                {   printf("\b\b\b\b%s--", Fretboard[i][j].note);   }
            }
        }
        printf("|\n");
    }
}
void PrintAllNotes(Notes Fretboard[6][18])
{
    for (int i = 0; i < 6; i++)
    {
        printf("(%d)%s-|-", Fretboard[i][0].fret, Fretboard[i][0].note);
        for(int j = 1; j < 18; j++)
        {
            if (Fretboard[i][j].note[1] == '#')
            { printf("(%d)%s--", Fretboard[i][j].fret, Fretboard[i][j].note); }
            else
            {
                if (j == 17)
                { printf("(%d)%s---|", Fretboard[i][j].fret, Fretboard[i][j].note); }
                else if (j == 12)
                { printf("(%d)%s-|-", Fretboard[i][j].fret, Fretboard[i][j].note);}
                else
                { printf("(%d)%s---", Fretboard[i][j].fret, Fretboard[i][j].note); }
            }
        }
        printf("\n");
    }
}
void ConstructFretboard(Notes Fretboard[6][18])
{
    char *notes[] = {"E", "F", "F#", "G", "G#", "A", "A#", "B", "C", "C#", "D", "D#", "E"};
    for (int j = 0; j < 18; j++)
    {
        strncpy(Fretboard[0][j].note, notes[j % 12], sizeof(Fretboard[0][j].note) - 1);
        Fretboard[0][j].note[sizeof(Fretboard[0][j].note) - 1] = '\0';
        Fretboard[0][j].fret = j;
        strncpy(Fretboard[1][j].note, notes[(j + 7) % 12], sizeof(Fretboard[1][j].note) - 1);
        Fretboard[1][j].note[sizeof(Fretboard[1][j].note) - 1] = '\0';
        Fretboard[1][j].fret = j;
        strncpy(Fretboard[2][j].note, notes[(j + 3) % 12], sizeof(Fretboard[2][j].note) - 1);
        Fretboard[2][j].note[sizeof(Fretboard[2][j].note) - 1] = '\0';
        Fretboard[2][j].fret = j;
        strncpy(Fretboard[3][j].note, notes[(j + 10) % 12], sizeof(Fretboard[3][j].note) - 1);
        Fretboard[3][j].note[sizeof(Fretboard[3][j].note) - 1] = '\0';
        Fretboard[3][j].fret = j;
        strncpy(Fretboard[4][j].note, notes[(j + 5) % 12], sizeof(Fretboard[4][j].note) - 1);
        Fretboard[4][j].note[sizeof(Fretboard[4][j].note) - 1] = '\0';
        Fretboard[4][j].fret = j;
        strncpy(Fretboard[5][j].note, notes[j % 12], sizeof(Fretboard[5][j].note) - 1);
        Fretboard[5][j].note[sizeof(Fretboard[5][j].note) - 1] = '\0';
        Fretboard[5][j].fret = j;
    }
}

/*=========================================================================================================*/

int PrintChord(Notes Fretboard[6][18], int chord[6], char buffer[6][255]) 
{
    for (int i = 0; i < 6; i++) 
    {
        char temp[255];
        
        /*
        for (int j = 0; j < 6; j++)
        {
            if (chord[j] > 13)
            {
                return 1;
            }
        }
        */
        
        if (chord[i] != -1) 
        {
            sprintf(temp, "|%s(%d)%s%s", (chord[i] < 10) ? "-" : "",
                    Fretboard[i][chord[i]].fret, Fretboard[i][chord[i]].note,
                    (strlen(Fretboard[i][chord[i]].note) == 2) ? "--|" : "---|");
        } 
        else
        {
            sprintf(temp, "|--------|");
        }
        strcat(buffer[i], temp);
        strcat(buffer[i], "  "); // Adding space between chords
    }
    return 0;
}
void PrintBuffers(char buffer[6][255], int chordCount) 
{
    for (int i = 0; i < 6; i++) 
    {
        printf("%s\n", buffer[i]);
        strcpy(buffer[i], ""); // Reset the buffer for the next line of chords
    }
    printf("\n"); // SPACE AFTER EACH CHORD IN PROGRESSION
}
void PrintHorizontalChords(Notes Fretboard[6][18], Node *chordList) 
{
    char buffer[6][255] = {0};
    int chordCount = 0;
    Node *currentChord = chordList;
    while (currentChord != NULL) 
    {
        if (PrintChord(Fretboard, currentChord->chord, buffer) != 1)
        {
            chordCount++;
        }
        currentChord = currentChord->next;

        if (chordCount == 10) 
        {
            PrintBuffers(buffer, chordCount);
            chordCount = 0;
        }
    }
    if (chordCount > 0)             // If there are chords left in the buffer after the loop
    { 
        PrintBuffers(buffer, 0);    // Print remaining chords without separator as it's the end
    }
}

/*=========================================================================================================*/

void PossibleChord(Notes Fretboard[6][18], char *formula[], int length, Node **chordList) 
{
    int frets[6] = {-1, -1, -1, -1, -1, -1};
    findChords(Fretboard, formula, length, frets, 0, chordList);
}
void findChords(Notes Fretboard[6][18], char *formula[], int length, int frets[6], int noteIndex, Node **chordList)
{
    if (noteIndex == length) 
    {
        appendChord(chordList, frets);
        return;
    }

    for (int i = 0; i < 6; i++) 
    {
        for (int j = 0; j < 18; j++) 
        {
            if (strcmp(Fretboard[i][j].note, formula[noteIndex]) == 0 && frets[i] == -1) 
            {
                int valid = 1;
                int maxString = -1;
                int minString = 6;
                for (int k = 0; k < 6; k++)
                {
                    if (frets[k] != -1)
                    {
                        if (k > maxString) 
                        {   maxString = k;  }
                        if (k < minString) 
                        {   minString = k;  }
                    }
                }
                if (maxString - minString > 2 || i - minString > 2 || maxString - i > 2)
                {   valid = 0;  }               // CHECKS STRING DISTANCE
            
                for (int k = 0; k < 6; k++) 
                {
                    if (frets[k] != -1 && abs(j - frets[k]) > 3)   
                    {   valid = 0;  break;  }   // CHECKS FRET DISTANCE
                }
                if (valid) 
                {
                    frets[i] = j;
                    findChords(Fretboard, formula, length, frets, noteIndex + 1, chordList);
                    frets[i] = -1;              // backtrack
                }
            }
        }
    }
}


