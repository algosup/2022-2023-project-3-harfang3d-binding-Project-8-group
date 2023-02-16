# Test Report

## Test Environments

### Hardware

* (1) Windows 11 Lenovo ThinkPad
 | *CPU: Intel(R) Core(TM) i7-1065G7 CPU @ 1.30GHz | | 1.50 GHz
|* 16 GB RAM DDR4
 | * No GPU

* (2) Windows 11 Lenovo ThinkPad *worn out*
 | *CPU: Intel(R) Core(TM) i7-1065G7 CPU @ 1.30GHz | | 1.50 GHz
|* 16 GB RAM DDR4
 | * No GPU

* (3) MacOS Ventura
 | *CPU: M1 apple chip
|* 8 GB RAM DDR4
 | * No GPU

* (3) MacOS Venturan *worn out*
 | *CPU: M1 apple chip
|* 8 GB RAM DDR4
 | * No GPU

* (5) Linux MintOS 19
 | *CPU: Intel(R) Core(TM) i7 11700KF
|* 32 GB RAM DDR4
 | * GPU: GTX 1660 Ti

## Software

* Python
* Rust
* LLVM
* CLang

## Test Results

Performances can be found in the files OS(num)_perf.txt .

The tests are run with the following command:

```bash
python3 tests_for_perfReport.py --rust (--linux if on linux or macos)
```

### (1) | Windows 11 Lenovo ThinkPad

#### Time to run a program

The global average time of a valid run is **20.52** seconds.

#### Passing tests

**23/29** tests passed.

### (2) | Windows 11 Lenovo ThinkPad | *worn out*

#### Time to run a program

The global average time of a valid run is **33.98** seconds.

#### Passing tests

**23/29** tests passed.

### Global tests on Windows

The global average time of a valid run is **27.25** seconds.

![Picture passing tests](./all_tests_picture.jpg)

| Test | Result |
| --- | --- |
| arg_out | [FAILED] |
| basic_type_exchange | [OK] |
| cpp_exceptions | [OK] |
| enumeration | [FAILED] |
| extern_type | [FAILED] |
| function_call | [OK] |
| function_template_call | [OK] |
| method_route_feature | [OK] |
| repr | [OK] |
| return_nullptr_as_none | [OK] |
| shared_ptr | [OK] |
| shared_ptr_default_comparison | [FAILED] |
| std_function | [OK] |
| std_future | [FAILED] |
| std_vector | [FAILED] |
| struct_bitfield_member_access | [OK] |
| struct_default_comparison | [OK] |
| struct_exchange | [OK] |
| struct_inheritance | [OK] |
| struct_inheritance_cast | [OK] |
| struct_instantiation | [OK] |
| struct_member_access | [OK] |
| struct_method_call | [OK] |
| struct_nesting | [OK] |
| struct_operator_call | [OK] |
| struct_static_const_member_access | [OK] |
| st_variable_access | [OK] |
| template_struct_nesting | [OK] |
| transform_rval | [OK] |

### (3) | MacOS Ventura

#### Time to run a program

The global average time of a run is **9.24** seconds.

#### Passing tests

**14/29** tests passed.

### (4) | MacOS Ventura *worn out*

#### Time to run a program

The global average time of a run is **16.62** seconds.

#### Passing tests

**14/29** tests passed.

### Global tests on MacOS

The global average time of a valid run is **12.93** seconds.

| Test | Result |
| --- | --- |
| struct_bitfield_member_access | [OK] |
| shared_ptr | [FAILED] |
| st_variable_access | [FAILED] |
| enumeration | [FAILED] |
| method_route_feature | [FAILED] |
| struct_method_call | [OK] |
| struct_default_comparison | [OK] |
| struct_inheritance | [FAILED] |
| std_future | [FAILED] |
| std_function | [OK] |
| transform_rval | [FAILED] |
| return_nullptr_as_none | [OK] |
| template_struct_nesting | [FAILED] |
| struct_member_access | [OK] |
| struct_exchange | [OK] |
| repr | [OK] |
| struct_nesting | [FAILED] |
| function_call | [OK] |
| basic_type_exchange | [OK] |
| cpp_exceptions | [OK] |
| shared_ptr_default_comparison | [FAILED] |
| struct_static_const_member_access | [FAILED] |
| struct_inheritance_cast | [FAILED] |
| std_vector | [FAILED] |
| extern_type | [FAILED] |
| struct_operator_call | [OK] |
| function_template_call | [OK] |
| arg_out | [FAILED] |
| struct_instantiation | [OK] |

### (5) | Linux MintOS 19

#### Time to run a program

The global average time of a run is **6.94** seconds.

#### Passing tests

**10/29** tests passed.

### Global tests on Linux

The global average time of a valid run is **6.94** seconds.

| Test | Result |
| --- | --- |
| struct_exchange | [FAILED] |
| cpp_exceptions | [OK] |
| std_function | [FAILED] |
| enumeration | [FAILED] |
| repr | [OK] |
| extern_type | [FAILED] |
| method_route_feature | [FAILED] |
| struct_inheritance_cast | [OK] |
| function_call | [OK] |
| return_nullptr_as_none | [OK] |
| std_vector | [FAILED] |
| shared_ptr_default_comparison | [FAILED] |
| struct_operator_call | [FAILED] |
| struct_inheritance | [FAILED] |
| struct_method_call | [FAILED] |
| struct_member_access | [OK] |
| function_template_call | [OK] |
| struct_static_const_member_access | [FAILED] |
| transform_rval | [FAILED] |
| st_variable_access | [OK] |
| struct_bitfield_member_access | [FAILED] |
| struct_default_comparison | [OK] |
| template_struct_nesting | [FAILED] |
| arg_out | [FAILED] |
| std_future | [FAILED] |
| struct_nesting | [FAILED] |
| shared_ptr | [FAILED] |
| basic_type_exchange | [OK] |
| struct_instantiation | [FAILED] |

## Ease of installation

### Windows

Really easy, the usual installs that our target audience is used to like python.
To get all clang dependencies you need to install the visual studio community edition.
The most technical part is llvm which is to be installed through their website but you need to navigate 3 links before finding it.

### MacOS

Less easy, the usual installs that our target audience is used to like python.
To get all clang dependencies you need to install the Xcode command line tools.
You can get errors if you do the mistake of using multiple installation sources or versions so you need to be extra careful.
LLVM is installed through brew.

### Linux

easy, the usual installs that our target audience is used to like python.
Clang and llvm are installed through the package manager apt.