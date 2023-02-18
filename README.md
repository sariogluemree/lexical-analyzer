# lexical-analyzer
Psi++ Lexical Analyzer
This is a lexical analyzer for the programming language Psi++.
The program accepts a source file called code.psi and produces a text file named code.lex, which contains all the tokens of the code.psi listed one after the other.

Lexical Rules
  The lexical rules for the programming language Psi++ are as follows:

  Identifiers
    Maximum identifier size is 20 characters. If you use an identifier larger than that, the lexical analyzer issues an error message.
    Psi++ language is not case sensitive and all the identifier names are standardized as upper case.
    Identifiers start with an alphabetic character (a letter) and are composed of one or more letters, digits, or _ (underscore).
    Example Token: Identifier(my_var_1)
  
  Integer Constants
    Maximum integer size is 10 digits. If you use an integer value longer than that, the lexical analyzer issues an error message.
    Negative values are not supported.
    Example Token: IntConst(1290)

  Operators
    Valid operators of the language are +, -, *, /, ++, --, :=
    Example Token: Operator(++)
    Brackets
    LeftPar: ( RightPar: )
    LeftSquareBracket: [ RightSquareBracket: ]
    LeftCurlyBracket: { RightCurlyBracket: }
    Example Token: LeftCurlyBracket
    String Constants
    String constants of Psi++ are delimited by double quotes (ASCII code 34) as in "this is a string".
    String constants have unlimited size.
    String constants cannot contain the double quote character. When you reach one, the string terminates.
    If a string constant cannot terminate before the file end, there should be a lexical error issued.
  
  Keywords
    Keywords are: break, case, char, const, continue, do, else, enum, float, for, goto, if, int, long, record, return, static, while.
    Psi++ language is not case sensitive, and all the keywords are standardized as lower case.
  You can write the same word as "while" or "While" or "WHILE," and they all generate the same token.
    Example Token: Keyword(while)
  
  End of Line
    Example Token: EndOfLine
  
  Comments
    Anything between (* and *) is a comment.
    If a comment cannot terminate before the file end, there should be a lexical error issued.
    Comments are just like blank space, and they provide no tokens.
  
  How to Run
    To run the lexical analyzer, you need to have a source code file with a .psi extension. The file should be named code.psi.
    To execute the lexical analyzer, run the following command in the terminal:
      python lexer.py
    After running the command, a file named code.lex will be generated, which contains all the tokens of the code.psi listed one after the other.

  Conclusion
    This is a lexical analyzer for the programming language Psi++.
  The program accepts a source file called code.psi and produces a text file named code.lex, which contains all the tokens of the code.psi listed one after the other.
  This can be used as a starting point for building a Psi++ compiler.
