Wordle Design:

# means that the letter is not in the word
* means that the letter is in the word but in the wrong position
c means that the letter 'c' is in the correct position

e.g. if the word is "apple" and the guess is "allow" then the program
will output a * # # # (allow), meaning that 'a' is in the right position,
'l' is in the wrong position, the second 'l' is not in the word, 'o' is not 
in the word and 'w' is not in the word.

if the word is "apple" and the guess is poppe, then the program will output
* # p # e. We see that the first instance of 'p' is in the word but
in the wrong spot, the second instance is in the right position, and the third 'p' is one 'p'
too many, so its not in the word.

NOTE: I gave guess a 64 character buffer, so if the user types more
than 63 characters, then it will overflow to the next memory addresses. I
require the user to end a 5 letter string.
