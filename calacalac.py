import re

# regex pattern
pattern = r'^\s*([\w\s*&]+(?<!\boperator\s))\s+(\w+)\((.*)\)(?<!;)'

# example .h file content
content = """
    auto plot(NotAxesHandle<T1> x, Args&&... args) {
        return gca()->plot(x, std::forward<Args>(args)...);
    }
        inline void show() { return gcf()->show(); }
"""

# iterate over matches and print the desired format
for match in re.finditer(pattern, content, re.MULTILINE):
    return_type, function_name, arguments = match.groups()
    argument_list = [f'"{arg.strip()}"' for arg in arguments.split(',')]
    argument_str = ', '.join(argument_list)
    print(f'gen.bind_function("{function_name}", "{return_type.strip()}", [{argument_str}])')
