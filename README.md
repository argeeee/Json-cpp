# JSON for Modern C++
A json library for c++ that allows you to work 
with json in c++ in the simples and intuitive way

#### This library contains:
* A Json parser in Json folder
* A tiny Console color library
* A tiny Test api for a pretty print

#### How types are converted?
* (undefined) -> (undefined)
* (null) -> c++ (nullptr)
* (number) -> c++ (long long) or (long double)
* (bool) -> c++ (bool)
* (string) -> c++ (std::string)
* (object) -> c++ (std::unordered_map)
* (array) -> c++ (std::vector)

#### Get started!!!
In ./Test/app.cpp there are all examples you need to start using this library