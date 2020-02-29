#pragma once

/**
 * Libraries
 */
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <iostream>
#include <memory>

namespace JsonSer
{
    using namespace std;

    class Json {

        /**
         * An enum that give a type to a JSON Json
         */
        enum class JsonType
        {
            Null,
            Undefined,
            Int,
            Float,
            Bool,
            String,
            Object,
            Array
        };

        /**
         * A parser for the json format
         */
        class JsonParser {

            int _position = 0;
            string _text;

            FILE* _fptr;

            /**
             * Returns the char of the <_text>
             * at position <_position>
             */
            char current();

            /**
             * Increments position by 1
             */
            void next();

            /**
             * Ignore whitespace
             */
            void ignoreWhiteSpace();

            /**
             * Report errors
             */
            void report(const char*, char, int);
            void report(const char*, char, int, char);

            /**
             * Helper functions
             */
            bool isDigit(const char&);
            bool isWhiteSpace(const char&);

            /**
             * Parsers
             */
            Json parseNumber();
            Json parseFloat(int&);
            Json parseBool();
            Json parseString();
            Json parseObject();
            Json parseArray();
            string getParsedString();
            pair<string, Json> getKeyValue();
            
            public: 
            /**
             *  Default constructor 
             */
            JsonParser(const string&, FILE*);

            ~JsonParser();

            Json parse();

        };

        /**
         * Contains the private members of Json class
         */
        struct Impl
        {
            /**
             * Type of the Json
             */
            JsonType _type;

            /**
             * The value of the json 
             */
            union 
            {
                long long _int;
                long double _float;
                bool _bool;
                string* _string;
                unordered_map<string, Json>* _object;
                vector<Json>* _array;
            };

            Impl() {};

            ~Impl() {
                switch (_type) {
                case JsonType::String:
                    delete _string;
                    break;
                case JsonType::Object:
                    delete _object;
                    break;
                case JsonType::Array:
                    delete _array;
                    break;
                }
            }

        };


        /**
         * Actually the implemented
         */
        shared_ptr<struct Impl> _impl;

        /**
         * To string 
         */
        string stringifyObject() const;
        string stringifyArray() const;

        /*********************** Public members ***********************/        
        public: 

        /**
         *  Default constructor - undefined value initialized
         */
        Json();
        /**
         *  Default constructor - undefined value initialized
         */
        Json(const nullptr_t&);
        /**
         *  Constructor - _int value initialized
         */
        Json(const int&);
        /**
         *  Constructor - _int value initialized
         */
        Json(const long long&);
        /**
         *  Constructor - _float value initialized
         */
        Json(const double&);
        /**
         *  Constructor - _float value initialized
         */
        Json(const long double&);
        /**
         *  Constructor - _bool value initialized
         */
        Json(const bool&);
        /**
         *  Constructor - _string value initialized
         */
        Json(const char*);
        /**
         *  Constructor - _string value initialized
         */
        Json(const string&);
        /**
         *  Constructor - _object value initialized
         */
        Json(const unordered_map<string, Json>&);
        /**
         *  Constructor - _array value initialized
         */
        Json(const vector<Json>&);

        ~Json();
        Json(const Json&);
        Json& operator=(const Json&);

        /**
         * Accessing operators
         */
        Json& operator[](int i);
        Json& operator[](const char* key);
        Json& operator[](string key);

        operator int () { return (_impl->_type == JsonType::Int) ? _impl->_int : 0; }
        operator long long () { return (_impl->_type == JsonType::Int) ? _impl->_int : 0; }
        operator double () { return (_impl->_type == JsonType::Float) ? _impl->_float : 0; }
        operator long double () { return (_impl->_type == JsonType::Float) ? _impl->_float : 0; }
        operator bool () { return (_impl->_type == JsonType::Bool) ? _impl->_bool : false; }
        operator string () { return (_impl->_type == JsonType::String) ? *_impl->_string : ""; }

        /**
         * Getting a json from string
         */
        static Json fromString(const string&, FILE* fptr = nullptr);
        /**
         * Getting a string from json
         */
        string toString() const;

        /**
         * Comparation
         */
        friend bool operator==(const Json&, const nullptr_t&);
        friend bool operator==(const nullptr_t&, const Json&);

        friend bool operator==(const Json&, const int&);
        friend bool operator==(const int&, const Json&);

        friend bool operator==(const Json&, const long long&);
        friend bool operator==(const long long&, const Json&);

        friend bool operator==(const Json&, const double&);
        friend bool operator==(const double&, const Json&);

        friend bool operator==(const Json&, const long double&);
        friend bool operator==(const long double&, const Json&);

        friend bool operator==(const Json&, const bool&);
        friend bool operator==(const bool&, const Json&);

        friend ostream& operator<<(std::ostream& out, const Json& json);
    
    };

    /**
     * Helper functions for creating a json
    */
    Json JsonArray();
    Json JsonArray(initializer_list<Json>);
    Json JsonObject();
    Json JsonObject(initializer_list<pair<string, Json>>);

} // namespace Json
