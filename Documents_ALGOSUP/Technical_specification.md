<hr>
<p  align="center"  style="font-weight: bold; font-size: 21px"> ALGOSUP Binding Project </p>

<p  align="center"  style="font-weight: bold; font-size: 18px"> Technical Specification</p>

<br>

<p align="center"> Jason Grosso </p>

<br>

<p  align="center"> ALGOSUP, Group 8. All Rights Reserved. </p>

<hr>

<summary> Table of Content </summary>

- [Abstract](#abstract)
- [Introduction](#introduction)
  - [Objective](#objective)
  - [Project Team](#project-team)
- [Overview](#overview)
  - [Current State](#current-state)
  - [Scope](#scope)
  - [Approach](#approach)
  - [Risk and Assumptions](#risks-and-assumptions)
    - [Risk](#risk)
    - [Assumption](#assumption)
- [Proposition](#proposition)
  - [Process](#process)
  - [Success](#success-evaluation)
- [Glossary](#glossary)
- [Diagrams](#diagrams)

## Abstract

This is the Technical Specifications for the project group 8 during the
year 2022-23 at ALGOSUP. This document describes the practical aspects
of the project such as its functionalities, which technologies were used
and where, as well as the architectures used.

## Introduction

### Objective

The objective of the project is for us to implement Rust to FABGen, a
binding generator developed by Harfang.

### Project Team

This project will be made over the course of 7 weeks, our Project
Manager is [David](https://github.com/DavidCC812) and is in charge of
organizing the work and dividing the tasks to the team..
[Max](https://github.com/maxbernard3) is the Program Manager, his job is
to define the high level functionalities.
[Jason](https://github.com/JasonGROSSO) is the Tech Lead, his job is to
chose how the functionalities will be implemented and which technologies
will be used. [Louis](https://github.com/Louis-de-Lavenne-de-Choulot) is
in charge of the Quality Assurance, he will be testing every
functionality of the project to find and point out any bug found,
finally, [Leo](https://github.com/leo-chartier) is our software
engineer, and he will be the one responsible of implementing most of the
functionalities.

## Overview

### Current State

Currently SWIG is the main tool used to do bindings, but it has many
problems, such as missing features, old database, the use of a meta C
like language making it really hard to support and the extreme
difficulty of debugging due to the hiding of the type of variable by the
code-base. Harfang wanted to expand their user base, a way to do that is
to add new languages and a simple way to add languages is to implement
bindings, but as previously said SWIG is not usable, so Harfang decided
to create their own binding generator: FABGen. However there are only 3
languages available in FABGen :

- Python

- Lua

- GOlang

In order to expand their pool of languages Harfang via ALGOSUP
commissioned us to add Rust to FABGen

### Scope

                Item                In Scope   Out Of Scope
  -------------------------------- ---------- --------------
            Rust Binding               X      
             Unit Tests                X      
            Reliability                X   
    Woking Harfang3D-Rust Example                   X
             Ruby Binding                           X   
    No Polymorphism At Run Time                     X

### Approach

To complete the project, we decided to use bindgen for the bindings and
to translate the GOlang units tests to Rust as both of the languages are
statically typed.

### Risks and Assumptions

#### Assumption

We assume that our solution will be :

- Easy to support;

- Easy to extend/work on;

- With no missing features;

- Able to bind many C++ constructs;

#### Risk

Problems our project can encounter:

- Delays $\rightarrow$ use a planner and work overtime;

- OS related problems $\rightarrow$ use Docker or ask the school for a
    different OS;

- Difficult to use $\rightarrow$ follow what was done on the others
    languages;

- Difficult to work on $\rightarrow$ comment the code and keep the
    code as clear as possible;

- Low customers satisfaction $\rightarrow$ communicate regularly with
    the customers to update him on the improvements of the project and
    ask him to validate the direction of the project;

## Proposition

### Process

We use the Python description of the C++ library, pass them into FABGen,
which will return a .c, .h, and a .rc file; by using the clang command
we will transform the .c file into a .o file usable by every language.
This file will be then called by the users via the .rc file to access
the C++ library(see [Architecture Diagram](#diagrams)). Here we will demonstrate using the GO
version of the generator; we launch FABGen we use the command line in
[Starting Command](#diagrams). This
will start the process in [Architecture Diagram](#diagrams) to create the bindings. The first part consists
of setting the generator to the chosen language, to do this bind.py we
will go into the language file of the chosen language, and send the info
to gen.py in order to activate the GO version. Once it is done it will
send the info to bind.py. The second part is a verification of the
passed file in this case, harfang.py: it will execute lots of small
functions between go.py gen.py and himself and it will also check some
functionalities. The third part mainly takes place in the std.py file of
GOlang it consists of transforming the GO variable types to C variable
types, after that bind.py will execute the bind generator, apply the
clang command and print the files in the directory passed in the
starting command.

### Success Evaluation

To determine the success of the project we have two ways at our
disposition:

- Harfang gave us a file with 30 unit tests, that we will use to
    quantify the percentage of work done;

- We also have access to harfang.py witch is an example of a .py file
    passed into FABGen we can use it to make sure our bindings are
    working.

## Glossary

1. **Bind** Allow a programming language to use a library from another
    programming language.

2. **Bindgen** Tool used to automatically generate Rust bindings to C
    and C++ libraries.

3. **Binding generator** Automatically creates a binding for any given
    language.

4. **C++** Generalist programming language.

5. **C** Generalist programming language.

6. **Clang command** Transform a file into an object file.

7. **Docker** Docker packages software into standardized units called
    containers that have everything the software needs to run.

8. **Harfang** A French company based in Orleans that developed a 3D
    visualisation tool (Harfang3D) and a binding generator (FABGen).

9. **GOlang / GO** Programming language statically typed.

10. **Library** Reusable collection of prewritten code that can be used
    for optimisation.

11. **Polymorphism** In programming language theory polymorphism is
    defined as a function that can accept multiple types.

12. **OS** Stand for Operating System, is the software managing the
    hardware of the computer (exemple: Apple, Windows).

13. **Unit tests** Tests on a specific part of the code.

14. **Ruby** Programming language focusing on simplicity and
    productivity.

15. **Rust** Programming language statically typed.

16. **Statically Typed** Is said of a language where the variables types
    are known at run time.

17. **SWIG** Is a old binding generator encompassing multiple languages.

## Diagrams

![Request Analysis](/Documents_ALGOSUP/img/request%20analysis.png "Request Analysis")

![Architecture FABGen](/Documents_ALGOSUP/img/Architecture%20FABGen.png "Architecture FABGen")
