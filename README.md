# CtoAssembly_Compiler

This repository is about implementing a C to ARM Assembly language compiler. 
For the specification of the project, check out the link: 'https://github.com/m8pple/langproc_2014_cw'

-----------------------------------------------------------

In order for a language to be compiled, a Lexer process will be needed to recognize every line of code and them into tokens.

After holding a cluster of tokens, a Parser process comes to analyse tokens and figure out the meaning of them (e.g token 'int' will be the start of a declaration, and a name of variable will come afterward). After analysing the tokens, parser will build up a structure called 'parsing tree' which indicates how all the tokens are related. 

After having the 'parsing tree', the next step will be simply translate each tree node into the language we wanted, however this stage costs massive amount of time. For a real compiler, an intermediate representation will be translated from the 'parsing tree'. 

The upcoming translation from the intermediate representation to the language we wanted will be easier since the intermediate representation is more straight forward.

