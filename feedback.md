**NG 7/13/2026**
* To provide you with thorough feedback, your instructor reads your code line by line, sometimes multiple times. Jumping from file to file is too time-consuming. Unless noted otherwise, place your class definitions above the main() and all function definitions below main(), all in one file. While the industry standard involves multi-source file programs, we will use a single file format for these labs unless instructed otherwise. Submit a fully completed lab by th due date using correct GitHib invite link to get a second chance
* feedback.md is for instructor use only.  Please DO NOT change the feedback.md; make a copy if needed; do no add anything
* if any, items with (-X) - no deductions this time, serve as a warning; please ensure these errors are corrected, as repeating them in future assignments will result in X points being deducted
* in feedback, #N means line number, e.g., 
```text
* #25  global var(s); only constant may be global -10
meaning: see line #25 ; -10 deduction points for the mistake
```
* class definition style & standards:  first, member vars  one per line and do not forget to initialize them; altghou private is a default access specifier, still label private members  clearly for better readability;  second- functions: separate group of functions  by a blank line; either matching setter/getter pairs or list all setters back to back and list all getters back to back; start with a default c’tor, followed by  copy c’tor (if any), then other c’tors in the order of increased number of arguments, d’tor- right after c’tor(s), overloaded functions- list them back to back in the order of increased number of arguments;  all accessor/getter functions, except for static function,  should be const; all functions with more than one statement should be normal (defined externally, outside of the class and below main()), no inline functions with more than one statement; initialize all member variables to appropriate default values at the time of definition; all member variables must be private; classes must provide a strong exception guarantee; must have default c’tor; implement Rule of Three when necessary;  to promote strong exception guarantee and to reduce redundancy, all setter functions, including c’tors, must call a setter function that sets all member variables
* 79-82 if a case in the switch has more than two statements make it a function -1
* #135-137: cout should mimic output on the screen; in this context it should be on a single line; adds unnecessary amount of scrolling when reading the code  (-1)
* 143-148 and like: excessive use of printf/cout/cin/ file stream/stringstream and/or endl and/or << >>; applies to all output, not just menu -5
```text
cout << "\n\nMenu:\n"
        "1. Encrypt\n" //not ‘\n’ at the end of the previous line for better readability
        "2. Decrypt\n"
        "3. Quit\n"<<endl;
```
* 234 redundant; invalid menu choice should be handled by default case of the menu switch; create a highly reusable function that takes an error message string as a parameter, clears the failed input, and outputs the error message to be called in the default case of the menu switch  -2 (-5)
```text
do {
switch (sortChoice) {
            case SORT_NAME:
            case SORT_HEALTH:
                sortMenu(names, types, health, strength, numCreatures, sortChoice);
                break;
            case BACK:
		//message about going back to the previous menu
                break;
            default:
                clearCin("Invalid menu choice");
        }
}while (sortChoice!=QUIT) 
```


***
