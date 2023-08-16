# Contributing to Siege Engine

The following is a set of guidelines for contributing to Siege Engine on GitHub. These are mostly guidelines, not rules. Use your best judgment, and feel free to propose changes to this document in a pull request.

## Code of Conduct

This project and everyone participating in it is governed by the [Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please make sure to report any unacceptable behavior you may observe.

## How Can I Contribute?

Bug reports and feature suggestions must use descriptive and concise titles and be submitted to GitHub Issues. Please use the search function to make sure that you are not submitting duplicates, and that a similar report or request has not already been resolved or rejected.

### Reporting Bugs

Before creating bug reports, please search for potential duplicates, as you might find out that you don't need to create one, but rather contribute to an ongoing thread. When you are creating a bug report, please include as many details as possible. Fill out the required template, the information it asks for helps us resolve issues faster and prevents us from returning with additional questions.

**Note:** If you find a closed issue that seems like it is the same thing that you're experiencing, open a new issue and include a link to the original issue in the body of your new one.

**A good bug report will:**
- Use a clear and descriptive title
- Describe the exact steps which reproduce the problem
- Provide specific examples to demonstrate the steps (such as test branches)
- Describe the behavior observed after following the steps
- Include screenshots and animated GIFs
- Include a crash report with a stack trace if possible as well as any relevant log files
- Include the frequency of reproduction, whether it occurs across multiple build configurations and operating systems, and if possible, the version in which it last was not occuring

### Requesting Features

Before creating feature requests, please search for potential duplicates as you might find out that you don't need to create one, but rather contribute to an ongoing thread. When you are creating a feature request, please include as many details as possible. Fill in the template, including its potential use case.

**A good feature request will:**
- Use a clear and descriptive title and description for the issue to identify the suggestion
- Provide a specific use cases to demonstrate the feature's value to the project
- Describe the current behavior and explain which behavior you expected to see instead and why

### Pull Requests

Please follow these steps to have your contribution considered by the maintainers:

**A good pull request will:**
- Use a clear and descriptive title and description
- Follow all instructions in the pull request template
- Make sure the changes are relevant and expected by the repository maintainers
- Ensure that the changes respect the repository's coding standards
- Verify that all status checks are passing
- Include pull request comments where decisions or clarification from the maintainers is required
- Introduces matching tests for any new functionality where possible

If a status check is failing, and you believe that the failure is unrelated to your change, please leave a comment on the pull request explaining why you believe the failure is unrelated. A maintainer will re-run the status check for you. If we conclude that the failure was a false positive, then we may decide to proceed with the merge.

Please note that the reviewer(s) may ask you to complete additional design work, tests, cleanup, or other changes before your pull request can be ultimately accepted.

## Coding Standards

### Git Commit Messages

- Use the past tense ("Added feature" not "Add feature")
- Limit the message length to 72 characters or less
- Avoid using extended commit message description
- Do not reference issues and pull requests in the commit
- Do not prefix commits with labels, such as brackets

### C++ Standard

All C++ code is formatted using the [ClangFormat file](.clang-format) that can be found in the repository root. Please ensure **all commits** are clang formatted before push so that the formatting tests will pass at any head.

Newly added source files should include the required legal header as so:

```cpp
//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//
```

### Make Standards

While there are currently no explicit standards in place for linting our Make files, we would appriciate it being kept to a similar style as has been established across the build system thus far.

Newly added Make files should include the required legal header as so:

```make
# Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib
```

### Documentation Standards

The project's C++ files largely use JavaDoc style documentation strings for documenting classes and functions as below:

```cpp
// Example.h

/**
 * The Example class provides exemplary functionality for your viewing pleasure.
 *
 * @note this is just an example
 */
class Example
{
public:

    // Public static members

    static constexpr unsigned int MAX_EXEMPLARY_VALUE = 1u;

    // 'Structors

    /**
     * Zero-param constructor for initialising empty Examples
     */
    Example();

    // Operator overloads

    /**
     * Assignment operator overload for assigning from others
     * @param rhs - the Example to assign
     * @return a reference to the original Example object
     */
    Example& operator=(const Example& rhs);

private:

    // Private methods

    /**
     * Helper method for determining the exemplary-ness of a given integer value
     * @note this method always returns true
     * @warning non-exemplary input will result in an application-wide crash
     * @param exmpl - the index of the desired exemplary behaviour
     * @return true if exemplary, false otherwise
     */
    bool IsExemplary(size_t exmpl);
};
```
