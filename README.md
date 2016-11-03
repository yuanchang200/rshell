# rshell

## Catalog
* [Overview](#Overview)
* [Instructions](#Instructions)
* [Project Introduction](#Project Introduction)
  * [Data Structures](#Data Structures)
  * [Framework](#Framework)
* [Known bugs](#Known bugs)

<a name="Overview"></a>
##Overview
This is a command shell written by C++， which can read in one or several commands at a time. Composite pattern is used in this design to implement the function of connectors between different single commands. Before the composition of several single commands, we should separate what user inputs into single commands and connectors. So we design a simple parser to parse the string from the user. There are also a Executor class to execute every single command. This is the basic idea of our design.

<a name="Instrcutions"></a>
##Instructions

User can input single command like "ls -a" and "mkdir test". User can also input combined commands with "&&", "||" or ";" like: echo chang && echo yuan || mkdir test; ls -a && git status. ATTENTION: the cd command may not work well.

<a name="Project Introduction"></a>
##Project Introduction

<a name="Data Structures"></a>
###Data Structures

We use stacks and queues to implement our parser, in which stacks are used to store symbols and our commands, and queues are used to store arguments. Arrays are also used to store users' input and commands that are parsed from input.

<a name="Framework"></a>
###Framework

Composite pattern is the main structure in this project. We designed a abstract base class "Base". Then our leaf is single command class, which store single commands. Compositions are And, Or and Semicolon class, which implement the function of "&&", "||" and ";" between commands. Besides, parser class and executor class are also necessary.

<a name="Known bugs"></a>
##Known bugs

1. In the parser class, when the root node is created, we want it to be returned to main function, then we can execute it in the main function. But during our debug process, we found that this root node can not be returned to main function. The commands in the root node would become NULL. So we have to execute it in the parser class. This makes our program a little bit messy.

2. When we test our program with the case "echo a; efh p && ; || exit", command "echo a" will be executed successfully and command "efh p" will print "command not found". Then there is a fault message "segmentation fault", then command "exit" could also be executed successfully.

3. There is a little problem with the input "&". If we want to input "&&", but we input "&" wrongly, then the program will skip the character "&".
