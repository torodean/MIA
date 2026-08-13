# MIA
This repository is a recreation of the MIA project found here: https://github.com/torodean/Antonius-MIA

MIA (Multiple Integrated Applications) is a program where I can store useful coding functions and apps I've created in one place for easy accessibility.

For a list of commands and features see the MIA manual found in the docs folder: [https://github.com/torodean/MIA/tree/main/docs](https://github.com/torodean/MIA/tree/main/docs)

## Project Structure

The repository is organized into several top-level directories to separate source code, documentation, and support files.

### `bin/`
Contains the source code for all integrated applications and utilities. This includes core modules, libraries, and executable logic organized by purpose and functionality.

### `docs/`
Contains the documentation for the project, including the MIA manual, usage instructions, design notes, and module-specific references.

### `scripts/`
Used for storing various helper scripts to perform tasks. This includes various tasks related to installing files, building files, and more.

### `build/` (if applicable)
Used for build artifacts, scripts, or platform-specific build outputs. This folder may be generated during the build process and is not typically version-controlled.

### `release/` (if applicable)
This folder contains (when built) the executables and configuration files built for either combining into a release package or to be used standalone from the folder.

### `resources/` 
This folder contains resources and configuration files used by the MIA applications and tools.

---

Additional directories may be added as the project evolves to support new tools, modules, or documentation needs.

## Purpose and Scope 

MIA is a personal hobby project built primarily for experimentation, learning, and utility (primarily utilities which I need for whatever other projects I am working on at the time). It’s not intended for production use, and therefore, parts of the code-base may be inconsistent or incomplete. This is due to some elements being very old, outdated, or other various reasons. Despite this, I’ve made an effort to keep things reasonably organized and well-documented. This is ultimately a sandbox project. The goal isn’t perfection, but exploration. MIA also doubles as a portfolio piece, offering a brief view into how I think about code, tools, documentation, designs and systems in a more relaxed, informal setting.

**Note**: This entire project was developed exclusively using text editors, without the use of any Integrated Development Environments (IDEs). All code, configuration, and debugging were done manually to maintain full control over the development process and to ensure that I'm improving my own skills rather than relying on IDE features.

## Windows vs Linux

MIA is primarily developed on a Linux machine. For this reason, a stable Windows build may not always be the current state and some features on Windows may lag behind in compatibility.

## Use Of Artificial Intelligence

Since this project is primarily for learning and exploration, the use of Artificial Intelligence (AI, LLMs, etc) is not as conducive for the MIA project as it would be for many other projects. With that said, there are many tasks that are needed in this project which are primarily (by their nature) algorithmic, repetitive, or labor-intensive with any thought or design already done (e.g., fixing the format of Doxygen comments after a standard was updated). These types of tasks may not always provide any learning opportunities and are rather just time consuming. In cases like this, AI may or may not be leveraged. Task files are placed in the `.ai/` folder, which provide guidance for these kinds of mechanical tasks, such as standardizing comment formats.

### Approach to AI use

A common adage when referencing current AI systems is 'trust but verify.' My experience in using these modern systems is that they cannot be trusted at the level this statement implies - they produce too many hallucinations and mistakes. I have found these systems to be beneficial tools only in the context of the user having full knowledge of the systems they are affecting (essentially, as workhorses). Therefore, I use these systems more so with the perspective of 'do not trust, and therefore verify.' 
