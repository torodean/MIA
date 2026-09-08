/**
 * @file PythonModule.hpp
 * @author Antonius Torode
 * @date 09/08/2026
 * @brief Declares a variadic-template variant of the Python module wrapper.
 */
#pragma once

#include <memory>
#include <string>
#include <type_traits>

#include <Python.h>

// Used for throwing on module load failure.
#include "MIAException.hpp"
// Used for the ErrorCode of the load failure exception.
#include "Error.hpp"

#include "PythonResult.hpp"


/**
 * Python objects are reference-counted instead of deleted directly: every API
 * which returns a PyObject* hands back a "new reference", and the caller is
 * responsible for releasing it later with Py_DECREF. This struct teaches
 * unique_ptr what "delete" means for such a pointer, since the default delete
 * keyword cannot be used on Python objects.
 *
 * It is a callable type (the operator() makes it behave like a function) rather
 * than a class which owns anything. The unique_ptr holds the pointer and calls
 * this deleter exactly where a raw-pointer implementation would call Py_DECREF.
 * Because the struct has no data members, the unique_ptr costs the same as a
 * raw pointer.
 *
 * Only wrap pointers you own (new references from PyImport_Import,
 * PyObject_GetAttrString, PyObject_CallObject, ...). Some Python APIs return
 * "borrowed references" which are NOT decrefed by the caller; wrapping one of
 * these would release an object the caller never owned.
 */
struct PyObjectDeleter
{
    /**
     * Releases one reference to the object.
     *
     * @param object The object to release. Never null: unique_ptr only calls
     *     the deleter on a non-null pointer, so Py_DECREF is safe here.
     */
    void operator()(PyObject* object) const
    {
        Py_DECREF(object);
    }
}; // struct PyObjectDeleter


/**
 * A unique_ptr for owned Python references.
 * Use this anywhere a PyObject* returned as a new reference needs automatic
 * Py_DECREF when it goes out of scope.
 */
using PyObjectPtr = std::unique_ptr<PyObject, PyObjectDeleter>;


/**
 * A loaded Python module which can be called from C++.
 * This owns both the interpreter lifetime and the imported module handle: the
 * first module constructed initializes the embedded interpreter, and destroying
 * the last module shuts it down. Code using this class never touches the
 * Python C API directly.
 *
 * call() is a variadic template: any mix of supported argument types is
 * converted to the matching Python type and packed into the argument tuple.
 * A new argument type is supported by extending the private toPython()
 * helpers, not by adding another call() overload. All calls return a
 * PythonResult, so expected call failures (missing method, Python exception,
 * conversion failure) surface as values without raw pointers or exceptions.
 * An app which wants to treat a call failure as fatal can check isValid()
 * and throw or exit on its own terms.
 *
 * The interpreter reference count is not thread-safe; constructing and
 * destroying modules is assumed to happen on one thread.
 */
class PythonModule
{
public:

    /**
     * Constructs and loads a Python module by name.
     * The module must be reachable on Python's module search path. The
     * embedded interpreter is initialized by this constructor if no other
     * module is currently alive.
     *
     * @param moduleName The name of the Python module to import.
     * @throws error::MIAException with ErrorCode::Python_Module_Load_Failure if
     *     the module could not be imported. An app which prefers to continue
     *     without the module can catch this exception and handle it gracefully.
     */
    explicit PythonModule(const std::string& moduleName);

    /**
     * Destructor. Releases the module handle and, if this is the last module
     * alive, shuts down the embedded interpreter.
     */
    ~PythonModule();

    /*
     * Copying is deleted because a PythonModule owns exactly one reference to
     * a Python module object. If two objects shared that pointer, whichever
     * was destroyed first would release the reference and leave the other
     * holding a handle Python has already freed (a use-after-free on the next
     * call). A copy would also corrupt the interpreter reference count, since
     * one construction would become two live objects. If a module ever needs
     * to be shared, share the PythonModule itself through a shared_ptr so
     * ownership stays with one object.
     */
    PythonModule(const PythonModule&) = delete;
    PythonModule& operator=(const PythonModule&) = delete;

    /**
     * Checks whether the module defines a callable method with the given name.
     *
     * @param name The name of the method to look up.
     * @return True if the method exists and is callable, false otherwise.
     */
    bool hasMethod(const std::string& name) const;

    /**
     * Calls a Python method with any mix of supported argument types.
     * C++ integer types become Python ints, floating point types become
     * Python floats, and std::string or string literals become Python strs.
     * Calling a method with no arguments is done with an empty argument list.
     * An argument whose C++ type has no converter is a compile error, and a
     * type mismatch with the Python method's expectations surfaces through
     * the returned PythonResult as a normal call failure.
     *
     * @param name The name of the method to call.
     * @param args Zero or more arguments, converted per their C++ type.
     * @return A PythonResult holding the return value of the call.
     */
    template<typename... Args>
    PythonResult call(const std::string& name, Args... args)
    {
        return invoke(name, buildArgs(toPython(args)...));
    }

private:

    /**
     * Initializes the embedded interpreter if it is not running yet.
     * This is a no-op while at least one module is alive. Besides starting
     * the interpreter, it puts the current working directory on Python's
     * module search path so modules next to the executable are importable.
     */
    static void ensureInterpreter();

    /**
     * Creates a new Python int object from any C++ integer type.
     * The constraint keeps this template from also matching floating point
     * types, which have their own overload below.
     *
     * @param value The value to convert.
     * @return The Python object, or null if creation failed.
     */
    template<typename T> requires std::is_integral_v<T>
    static PyObjectPtr toPython(T value)
    {
        return PyObjectPtr(PyLong_FromLongLong(value));
    }

    /**
     * Creates a new Python float object from any C++ floating point type.
     *
     * @param value The value to convert.
     * @return The Python object, or null if creation failed.
     */
    template<typename T> requires std::is_floating_point_v<T>
    static PyObjectPtr toPython(T value)
    {
        return PyObjectPtr(PyFloat_FromDouble(value));
    }

    /**
     * Creates a new Python str object from a C++ string.
     *
     * @param value The value to convert.
     * @return The Python object, or null if creation failed.
     */
    static PyObjectPtr toPython(const std::string& value)
    {
        return PyObjectPtr(PyUnicode_FromString(value.c_str()));
    }

    /**
     * Creates a new Python str object from a string literal.
     *
     * @param value The value to convert.
     * @return The Python object, or null if creation failed.
     */
    static PyObjectPtr toPython(const char* value)
    {
        return toPython(std::string(value));
    }

    /**
     * Packs converted arguments into a Python tuple for a method call.
     * Each argument is a PyObjectPtr. The tuple takes its own reference to
     * every argument, so the arguments passed in keep their references and
     * release them as usual when they go out of scope.
     *
     * If any argument is null (its creation failed) or the tuple cannot be
     * created, the returned pointer is null, which invoke() reports as an
     * error. A call with no arguments produces an empty tuple.
     *
     * @param args Zero or more PyObjectPtr arguments to pack.
     * @return The argument tuple, or null on failure.
     */
    template<typename... Args>
    static PyObjectPtr buildArgs(const Args&... args)
    {
        // The fold expression is true when at least one argument is null.
        if ((!args || ...))
            return nullptr;

        return PyObjectPtr(PyTuple_Pack(sizeof...(args), args.get()...));
    }

    /**
     * Calls a method with a pre-built argument tuple.
     * This is the shared implementation behind the public call() template.
     *
     * @param name The name of the method to call.
     * @param args The argument tuple. An empty tuple calls the method with no
     *     arguments; a null tuple reports a failed argument build.
     * @return A PythonResult holding the return value of the call.
     */
    PythonResult invoke(const std::string& name, PyObjectPtr args);

    /// The name of the loaded module.
    std::string name;

    /**
     * The imported module handle. The PyObjectDeleter releases the reference
     * when this object is destroyed.
     */
    PyObjectPtr module;

    /**
     * The number of PythonModule objects currently alive. The embedded
     * interpreter runs while this count is above zero.
     */
    static int interpreterCount;
}; // class PythonModule
