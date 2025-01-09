# Google Style Guide Instructions for Cursor

## C++ Coding Style Guidelines

### 1. Naming Conventions
- File names: lowercase with underscores (e.g., `my_useful_file.cc`)
- Type names (classes, structs, enums, typedefs): CamelCase (e.g., `MyClass`)
- Variable names: lowercase with underscores (e.g., `my_variable`)
- Constant names: k followed by mixed case (e.g., `kDaysInAWeek`)
- Function names: CamelCase (e.g., `DoSomething()`)
- Namespace names: lowercase (e.g., `my_namespace`)

### 2. Formatting
- Indentation: 2 spaces (no tabs)
- Line length: maximum 80 characters
- Function declarations: all on one line if possible
- Curly braces: attach to the enclosing statement
- Spaces vs. Parentheses: no spaces inside parentheses
- Pointer and reference expressions: `char* c`, `const string& str`

### 3. Comments
- File comments: start with license boilerplate, file contents description
- Class comments: describe class purpose and how to use the class
- Function comments: describe what the function does
- Variable comments: only for non-obvious cases
- End-line comments: use space after code (// comment)
- TODO comments: use `// TODO(username): comment`

### 4. Best Practices
- Use `nullptr` instead of `NULL` or 0
- Use `const` whenever possible
- Avoid using preprocessor macros
- Use C++-style casts (`static_cast<>()`, etc.)
- One class declaration per file
- Avoid multiple inheritance

## Python Coding Style Guidelines

### 1. Naming Conventions
- File names: lowercase with underscores (e.g., `my_module.py`)
- Function names: lowercase with underscores (e.g., `my_function`)
- Variable names: lowercase with underscores (e.g., `my_variable`)
- Class names: CamelCase (e.g., `MyClass`)
- Constant names: uppercase with underscores (e.g., `MAX_VALUE`)
- Module-level names: lowercase with underscores

### 2. Formatting
- Indentation: 4 spaces (no tabs)
- Line length: maximum 80 characters
- Import formatting:
  ```python
  import standard_library
  
  import third_party_library
  
  import local_library
  ```
- Function/class spacing: two blank lines
- Method spacing: one blank line

### 3. Comments and Docstrings
- Module docstring: start of file, describe module contents
- Function docstrings:
  ```python
  def function_name(param1, param2):
      """One-line summary of function.

      Detailed description of function.

      Args:
          param1: Description of param1.
          param2: Description of param2.

      Returns:
          Description of return value.

      Raises:
          ErrorType: Description of error condition.
      """
  ```
- Inline comments: use sparingly, start with "# "

### 4. Best Practices
- Use list comprehensions instead of map() or filter()
- Use `isinstance()` for type checking
- Use `@property` decorator instead of get/set methods
- Follow the "explicit is better than implicit" principle
- Use type hints for function arguments and return values:
  ```python
  def greeting(name: str) -> str:
      return f"Hello {name}"
  ```

### 5. Common Patterns
- Context managers using `with`
- Generator expressions for memory efficiency
- Exception handling with specific exceptions
- Use f-strings for string formatting (Python 3.6+)

## General Guidelines for Both Languages

### 1. File Organization
- One logical class/component per file
- Group related functionality together
- Keep files focused and manageable in size

### 2. Code Structure
- Write clear, self-documenting code
- Keep functions and methods focused and small
- Follow DRY (Don't Repeat Yourself) principle
- Maintain consistent error handling

### 3. Version Control
- Write clear, descriptive commit messages
- Keep commits focused and atomic
- Follow branch naming conventions
- Review code before committing

### 4. Documentation
- Keep documentation up to date
- Document why, not what (code should be self-documenting)
- Include examples for complex functionality
- Document assumptions and limitations

### 5. Testing
- Write unit tests for new code
- Maintain existing tests
- Test edge cases
- Keep tests readable and maintainable 