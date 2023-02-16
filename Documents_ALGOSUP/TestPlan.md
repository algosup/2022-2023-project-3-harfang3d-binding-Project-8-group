# Test Plan for the Rust language bindings

## **Introduction**

This test plan outlines the project "Rust bindings" for Team 8. Our ultimate goal is to bind Rust to C++. The plan will be updated as the project progresses, and it aims to test all aspects of the project including the code, Test-Driven Development (TDD), and its implementation in production.

## **Scope of the test plan**

The test plan is designed to test the FABGen library's Rust bindings. It covers all aspects of the project, including the code, TDD, and implementation in production. Additionally, the implementation of the code will be evaluated to ensure it conforms to specified standards and adheres to the coding guidelines set forth by Harfang. The code must be readable for the client and any open-source contributors, and comments will be included in the supervised subjects.

## **Resources**

### Team: The team will consist of the following members

```md
- Project manager
- Program manager
- Tech lead
- Software engineer
- QA
```

The team members will contribute to reporting bugs when found.
The lead tester/QA will be responsible for insuring that the found bugs are written down and that the team is aware of them.

### Tools: The tools used for the project will be

##### Hardware

```sh
- 4 MacBook Air M1 running on macOS Monterey
- 1 ThinkBook running on Windows 11
- 1 WhiteBoard
```

##### Software

```md
- Visual Studio Code
- Github
- Git
- Python
- Rust
```

## **Expected code architecture**

After a global analysis of the code, it was determined that the project would require new files and updates to existing files to function properly.
It will be using the following file structure contingent upon following the current architecture.

```md
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

### Long term

These tests will not be removed at the end of the project. They will allow contributors to ensure that their added code does not break the foundations. Examples include:

- the use of Harfang's given tests
- codes adapted from examples in other languages to compare the range of usability of the bindings
- badly written code to test the robustness of the bindings
- Random Cpp libraries to benchmark how usable it is outside of Harfang's framework
- checking all TDD tests and verifying their integrity (this is a verification and has to be added since it is a critical point in the team's flow of work)

### Starting from 0

Starting from 0 as a user. These tests will focus on intuitiveness, ease of installation, and understanding of how and where to use the bindings. They will also include environment testing.

## Long term tests

### Use of examples

The Harfang's Cpp library example given by the client have to be bound to the project by the end of the fourth week.
Examples from the official documentation that passes in other languages will be adapted and tested to see if they work in Rust.
They will be of different types to cover as many aspects of the Rust language as possible.

### Spaghetti code

Intentional error based code and creating poorly written versions of the examples are necessary to evaluate the robustness of the Rust bindings.

### TDD Tests

Tests will be run and verified to ensure they are neither true==true equalities nor outdated functions since they are a critical part of the team's workflow.

### External code

The client specified that they would like the binding to be as functional as possible with external code. The Rust code will be tested with external libraries to see if it is able to handle it. It is not expected to include advanced features, but it may lead to the discovery of new bugs that were not found or being handled by some other parts of FABGen in a non common environment.

## Primary User test

### Define primary users

To make sure that something is working, there is nothing better than multiple testers.
As FABGen is an open source repository, outside developers will be asked to test it and give their feedback. They will be asked to test the installation and the usage of the bindings. They will be asked to test the examples. We will call them Primary Users.

The Primary users are developers that create bindings in Rust. They are supposed to be familiar with Rust and/or Python as well as to be able to use the bindings without any help.
They are expected to pass all phases of the usage easily :

- Installation
  - Github repository download
  - Installation of the required languages and libraries
- Usage
  - Compilation and calling of FABGen
  - Executing the code that has been generated.

##### Example of a primary user

```md
  - Name: John Doe
  - Age: 25
  - Education: Master's degree in Computer Science
  - Experience: 3 years of experience in C++ and Python
  - Job: Software Engineer (Junior)
  - Why ? : His company is using Harfang wants to use Rust. His team needs to create the bindings for Rust.
  -Acceptance criteria : He is able to install the bindings and use them to create a small scale project.
```

```md
  - Name: Jane April
  - Age: 48
  - Education: Master's degree in Computer Science
  - Experience: 10 years of experience in C++ and Rust
  - Job: Software Engineer (Senior)
  - Why ? : She is a freelance developer and wants to use Harfang's framework in her projects. She only knows Rust.
  -Acceptance criteria : She is able to install the bindings and use them however she wants easily after a small period of time.
```

```md
  - Name: Laker James
  - Age: 32
  - Education: Bachelor's degree in Computer Science
  - Experience: 5 years of experience in Rust
  - Job: Software Engineer (Junior)
  - Why ? : He is a freelance developer and wants to start working in the field of 3D. He wants to work in Rust so he is not open to other languages.
  -Acceptance criteria : He is able to install the bindings and use them on existing projects.
```

```md
  - Name: Arthur Morgan
  - Age: 40
  - Education: Bachelor's degree in Computer Science
  - Experience: 5 years of experience in Java
  - Job: Software Engineer (mid)
  - Why ? : He owns a company that uses C++ since 10 years. He wants to upgrade their system to Rust because of some limitations of c++. They don't have anything to do with 3D.
  -Acceptance criteria : He is able to install the bindings and use them on his company's projects.
```

##### Usability testing

The team will ask for some external testers to try it out and answer the survey. To recruit them, the team will use the following methods :

- Social media
- Other teams
- Developper friends

##### Surveys

Testers will have to answer a survey containing categories such as :

- Ease of installation
- Tutorial and documentation
- Ease of use
- Time of execution
- Bugs found and how to reproduce them

Each result will be analyzed taking the user's profile into account. For example, a tester that is a beginner in Rust will be more prone to have a hard time installing the bindings than a tester that is a senior in Rust.

##### Simulated/Environment testing

The installation will be tested both on VMs and by the primary user. It will be tested with the Github repository download. The installation of the required languages and libraries will be tested. The OS that will be tested are :

- Windows 10
- Macos
- Ubuntu
- Debian
- a light linux distribution

### Real life

## Test Approach

- **Test Case Development:** Test cases will be developed based on the requirements and design documents made by the Program Manager.
- **Test Execution:** Test cases will be executed on the test environment. Dockers with a common base will be used to test multiplatform execution.
- **Test Data Preparation:** Test data will be prepared for functional and usability testing. All of them will be stored in Dated documents.
- **Defect Tracking:** Defects will be tracked using a bug tracking tool as Github issues.

## [**Test Cases can be found here**](https://github.com/algosup/2022-2023-project-3-harfang3d-binding-Project-8-group/blob/main/Documents_ALGOSUP/TestCase.md)

## **Schedule**

```sh
Test planning: 2 weeks
Test case development: 1 weeks
Test execution: 2 weeks
Defect tracking and resolution: 1 week
Final testing: 1 week
Total: 7 weeks
```

## **Test Deliverables**

```md
- Test plan
- Test cases
- Test data
- Test execution results
```

## Acceptance Criteria

For the project to be considered as a success, the following criteras must be met:

```sh
- All tests should pass
- The code should be Usable without crashes
- Examples from Harfang should work perfectly
- The code should be able to handle spaghetti code
```
