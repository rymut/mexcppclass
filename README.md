# mexcppclass

MEX function and a library for wrapping, managing and using C++ resources
objects, function and classes within MATLAB.

## Usage

1. Compile **cppmexclass** mex function.
2. Create resource wrapper and export create/destory functions.  
   Wrapper and functions can reside inside **mexcppclass** mex function or can  
   reside inside a shared library:
   * wrapper for <ClassName> C++ resource which implements
      `mexcppclass::ModuleInterface`,
   * write and export functions for creating wrapper instance  
      `void
      Destroy<ClassName>ModuleInterface(mexcppclass::ModuleInterface *obj)`  
      and destroying wrapper instance  
      `void
      Destroy<ClassName>ModuleInterface(mexcppclass::ModuleInterface *obj)`.
3. Put created <LibraryName>.(dll|so) library and **mexcppclass** mex  
   in the same directory. Object can be accessed by Matlab by use of commands:  
  ```
  % Create wrapper handle
  handle = mexcppclass('create', 'ClassName=LibraryName.so')
  % Create object constructor
  mexcppclass('call', handle, 'create');
  % Call simple methods
  mexcppclass('call', handle, 'setvalue', 10);
  v = mexcppclass('call', handle, 'getvalue');
  % destroy object and wrapper
  mexcppclass('destroy', handle);
  ```

## Example

See test/unit/testmodule.hpp and test/unit/testmodule.cpp for as a reference  
point.

## Alternative solutions/projects/examples 

* [https://github.com/chappjc/MATLAB.git](https://github.com/chappjc/MATLAB.git)
* [http://www.ee.columbia.edu/~mim/meap/paMat/mexhandle/](https://github.com/chappjc/MATLAB.git)
