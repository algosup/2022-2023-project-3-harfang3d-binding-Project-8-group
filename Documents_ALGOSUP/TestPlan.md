# Test Plan for the Rust language bindings

## **Introduction**

This test plan follows the project "Rust bindings" of team 8. It will test the full course of the project and will be updated as the project evolves.

## **Scope of the test plan**

The test plan has for goal the testing of the FABGen library bindings in Rust. It includes everything from the code itself to the TDD implemented to it's use in production. Also, the way it is implemented will be checked to make sure it fits to the required form, it means following the rules of code from Harfang.

## **Resources**

### Team: The team will consist of the following members

```
- Project manager
- Program manager
- Tech lead
- Software engineer
- QA
```

They will help in the reporting of bugs when found (QA has a specialisation in the field).

### Tools: The tools used for the project will be

Hardware

```
- 4 MacBook Air M1 running on macOS Monterey
- 1 ThinkBook running on Windows 11
- 1 WhiteBoard
```

Software

```
- Visual Studio Code
- Github
- Git
- Python
- Rust
```

## **Expected code architecture**

After a global analysis of the code, it was concluded that the project should have new created files and some updated to have it working. Following what was made for Python, Lua and Go, it should at least ressemble the following:

```
- examples/harfang.py (updated)
- lang/rust.py (new)
- lib/std.py (updated)
- lib/stl.py (updated)
- lib/rust/std.py (new)
- lib/rust/stl.py (new)
- lib/rust/__init__.py (new)
- tests/ (updated)
- tests.py (updated)
- gen.py (updated)
- bind.py (updated)
- requirements.txt (updated)
- README.md (updated)
```

## **Test Strategy**

The test plan will be separated in two parts:

- The first part will be tests for long term usage like the use of the 3D engine with codes adapted from examples in other languages, new spaghetti code to see if it is able to handle it, checking all tests and verifying their integrity. To try some codes outside the game engine.

- The second part will be it's use from 0 as a user. It is less about the code and more about intuitiveness. How easy is it to install ? Do I know what to do and where ? Are there Additional steps ?
This will also contain environment testing.

## Long term tests

### Use of examples

The 3D engine Harfang will be installed and bound to the project by the end of the second week.
Examples from the official documentation that passes in other languages will be tested to see if they work in Rust after translation.
They must be of different types.

### Spaghetti code

To tests the toughness of the Rust bindings, we will mess with the code and make a horribly written version of the examples

### Tests

The tests are an important part of the project for the developpers. They must be fully functional and up to date. They must be run and verified. We need to make sure they are valid and do not test outdated functions or true==true.

### External code

The client precised he would like the binding to be as functional as possible with external code. The Rust code will be tested with external code to see if it is able to handle it. It is not supposed to include advanced features but it may lead to finding new bugs that were being handled by some other parts of FABGen in a non common environment.

## User test

### Define users

The users are the developpers that will use the bindings. They are supposed to be familiar with Rust and the 3D engine Harfang. They are supposed to be able to use the bindings without any help.
They are expected to pass all phases of the usage easily :

- Installation
  - Github repository download
  - Installation of the required languages and libraries
- Usage
  - Compilation and calling of FABGen
  - Running of the generated code

### Simulated

The installation will be tested on VMs. It will be tested with the Github repository download. The installation of the required languages and libraries will be tested. OS that will be tested are :

- Windows 10
- Macos
- Ubuntu
- Debian

For the purpose of testing, Harfang will also be installed.

### Real life

To make sure that something is working, there is nothing better than multiple testers. As FABGen is an open source repository, outside developpers will be asked to test it and give their feedback. They will be asked to test the installation and the usage of the bindings. They will be asked to test the examples.

## Test Approach

```
- Test Case Development: Test cases will be developed based on the requirements and design of the web application.
- Test Execution: Test cases will be executed on the test environment.
- Test Data Preparation: Test data will be prepared for functional and usability testing.
- Defect Tracking: Defects will be tracked using a bug tracking tool.
```

## **Schedule**

```
Test planning: 2 weeks
Test case development: 1 weeks
Test execution: 2 weeks
Defect tracking and resolution: 1 week
Final testing: 1 week
Total: 7 weeks
```
