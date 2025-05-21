# bin Directory Overview

This directory contains the primary code for the MIA project and associated libraries. Below is a brief description of each subfolder:

- **program/**  
  Contains code which is used by the MIA programs and core/common to all of the various app functionality. **TODO:** Rename this folder to core.

- **utilities/**  
  Contains reusable utilities and common functionality to support application or feature development.
  
- **libs/**  
  Contains libraries with core functionality for the various MIA apps and features.

- **apps/**
  Contains the applications/program code of the MIA project.

--

### Project Dependency Structure

The project is organized into layers to manage dependencies clearly:

- **Core:** Contains fundamental types, error codes, and logging interfaces. It has no dependencies on other modules.
- **Utilities:** Depend on Core and provide reusable helper functions like file and string operations.
- **Libraries:** Depend on Core and Utilities, implementing domain-specific logic such as math functions.
- **Applications:** Depend on Core, Utilities, and Libraries to build and run the final executable.

**Note:** Modules within the same layer may depend on each other as needed. The key rule is to avoid circular dependencies **between** layers to maintain clear and manageable dependency flow.

This layering ensures a unidirectional dependency flow, promoting modularity and easier maintenance.
