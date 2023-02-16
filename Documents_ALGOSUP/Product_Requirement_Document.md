# Product Requirement

## Context

### Problem

Harfang3D is written in C++. To use Harfang3D you also need to write the
code in C++.  
A client may not have C++ qualified developers. In this case they would
have to either use an other technology or hire new developpers.  
It also takes longer to develop a program in C++.  
 To help alleviate this problem the client wants to bind other languages
to Harfang3D. Specifically, the client wants a Rust binding in
particular

### Target, User and Stakeholder

Our Target is Harfang3D. The users are Harfang3D, Harfang’s client and
the users. The Stakeholders are Harfang3D and ALGOSUP.

## Scope

### Success

-   Use the clients software and update it to add a Rust binding.

-   Base ourselves on the Go binding to do Rust.

-   Make sure the binding functions properly.

### Out of Scope

-   No polymorphism at runtime.

-   Implement another language binder.

-   Unit tests must be verified.

## Key decision / Constraints

### Sizing

We are not projecting any budget need as of today January 6, 2023. The
project needs to be delivered on the February 17, 2023. The Deliverable
project will also include a presentation in front of the stakeholders,
functional specifications and technical specifi- cations.

### Solution proposition

The solution selected is to use the client’s existing C++ binding
generator. This binding generator already supports GOlang, Python and
LUA. We plan to inspire ourselves from the Golang binding implementation
to do our Rust implementation.
