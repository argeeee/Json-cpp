#include "Json.h"

namespace JsonSer
{

    /************************** Json Reporter **************************/

    void Json::Reporter::ReportUnexpectedChar(
        const char& current, const int& position, const char& expected, const string& additional
    ){
        string diagnostic = "Unexpected char '";
        diagnostic.push_back(current);
        diagnostic += "' at position <";
        diagnostic += to_string(position);
        diagnostic += ">, expected '";
        diagnostic.push_back(expected);
        diagnostic += "'";
        if(additional != "") diagnostic += " >>> " + additional + " <<<";
        _diagnostics.push_back(diagnostic);
    }

    
    /************************** Json Parser **************************/

    /**
     * Default constructor 
     */
    Json::JsonParser::JsonParser(const string& text) 
        :_text(text) { }

    Json::JsonParser::~JsonParser() { }

    /**
     * Get the current char
     */
    char Json::JsonParser::current() {
        if (_position > _text.size())
            return '\0';
        return _text[_position];
    }

    /**
     * Next position
     */
    void Json::JsonParser::next() {
        _position++;
    }

    /**
     * Ignore whitespace
     */
    void Json::JsonParser::ignoreWhiteSpace() {
        while (isWhiteSpace(current())) next();
    }

    /**
     * returns true if char is a digit
     */
    bool Json::JsonParser::isDigit(const char& c) {
        return (c >= 48 && c <= 57);
    }
    /**
     * returns true if char is whitespace
     */
    bool Json::JsonParser::isWhiteSpace(const char& c) {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n';
    }
    
    /**
     * Parser methods for:
     * <number>
     * <bool>
     * <string>
     * <object>
     * <array>
     */

    Json Json::JsonParser::parseNumber() {
        int start = _position;

        while ( isDigit(current()) )
            next();

        if( current() == '.' )
            return parseFloat(start);
        
        int length = _position - start;
        
        long long value = stoll(_text.substr(start, length));

        return Json(value);
    }

    Json Json::JsonParser::parseFloat(int& start) {
        next();
        while ( isDigit(current()) )
            next();
        
        int length = _position - start;

        auto value = stold(_text.substr(start, length));

        return Json(value);
    }

    Json Json::JsonParser::parseBool() {
        bool value = _text.substr(_position, 4) == "true";

        if(value) _position += 4;
        else _position += 5;

        return Json(value);
    }
    
    Json Json::JsonParser::parseString() {
        return Json(getParsedString());
    }

    string Json::JsonParser::getParsedString() {
        next();
        int start = _position;

        while ( current() != '"' )
            next();
        
        int length = _position - start;
        
        next();
        return _text.substr(start, length);
    }
    
    pair<string, Json> Json::JsonParser::getKeyValue() {
        pair<string, Json> kv;

        if (current() != '"') {
            _reporter.ReportUnexpectedChar(current(), _position, '"', "[KEY, value] of an object");
            _position++;
            return kv;
        }

        kv.first = getParsedString();
        ignoreWhiteSpace();

        if (current() != ':') {
            _reporter.ReportUnexpectedChar(current(), _position, ':', "[key, value] of an object");
            _position++;
            return kv;
        }

        next();
        kv.second = parse();
        return kv;
    }

    Json Json::JsonParser::parseObject() {
        unordered_map<string, Json> value;
        pair<string, Json> kv;

        next();
        while ( true ) {

            ignoreWhiteSpace();
            kv = getKeyValue();
            value.insert(kv);
            ignoreWhiteSpace();

            char curr = current();

            if ( curr == ',' ) {
                next();
                continue;
            }
            
            else if( curr == '}' ) {
                next();
                break;
            }

            else {
                _reporter.ReportUnexpectedChar(current(), _position, '}', "End of an object");
                break;
            }

        }
        return Json(value);
    }
    
    Json Json::JsonParser::parseArray()  {
        vector<Json> value;

        next();

        while ( true ) {

            const auto& val = parse();

            value.emplace_back(val);

            ignoreWhiteSpace();

            char curr = current();

            if ( curr == ',' ) {
                next();
                continue;
            }
            
            else if( curr == ']' ) {
                next();
                break;
            }

            else {
                _reporter.ReportUnexpectedChar(current(), _position, ']', "End of an array");
                _position++;
                break;
            }

        }

        return Json(value);
    }
    
    /**
     * The parse method - the core of all
     */
    Json Json::JsonParser::parse() {
        
        ignoreWhiteSpace();

        if ( isDigit(current()) )
            return parseNumber();
        
        switch (current()) {
            case '"':
                return parseString();
            case '{':
                return parseObject();
            case '[':
                return parseArray();
        }

        if (_text.substr(_position, 4) == "true" || _text.substr(_position, 5) == "false")
            return parseBool();

        else if (_text.substr(_position, 4) == "null")
            return _position += 4, Json(nullptr);

        else if( _text.substr(_position, 9) == "undefined" )
            return _position += 9, Json();

        _reporter.ReportUnexpectedChar(current(), _position, '@', "Any valid json value");
        return Json();

    }

    /************************** Json ********************************/

    /**
     * Default constructor - undefindes
     */
    Json::Json() 
        :_impl(new Impl)
    { 
        _impl->_type = Json::JsonType::Undefined;
    }
    /**
     *  Constructor - null value initialized
     */
    Json::Json(const nullptr_t& nptr)
        :_impl(new Impl)
    {
        _impl->_type = Json::JsonType::Null;
    }
    /**
     *  Constructor - _int value initialized
     */
    Json::Json(const int& value) 
        :_impl(new Impl)
    {
        _impl->_type = Json::JsonType::Int;
        _impl->_int = value;
    }
    /**
     *  Constructor - _int value initialized
     */
    Json::Json(const long long& value) 
        :_impl(new Impl)
    {
        _impl->_type = Json::JsonType::Int;
        _impl->_int = value;
    }
    /**
     *  Constructor - _float value initialized
     */
    Json::Json(const double& value)
        :_impl(new Impl)
    {
        _impl->_type = Json::JsonType::Float;
        _impl->_float = value;
    }
    /**
     *  Constructor - _float value initialized
     */
    Json::Json(const long double& value)
        :_impl(new Impl)
    {
        _impl->_type = Json::JsonType::Float;
        _impl->_float = value;
    }
    /**
     *  Constructor - _bool value initialized
     */
    Json::Json(const bool& value)
        :_impl(new Impl)
    {
        _impl->_type = Json::JsonType::Bool;
        _impl->_bool = value;
    }
    /**
     *  Constructor - _string value initialized
     */
    Json::Json(const char* value)
        :_impl(new Impl)
    {
        _impl->_type = Json::JsonType::String;
        _impl->_string = new string(value);
    }
    /**
     *  Constructor - _string value initialized
     */
    Json::Json(const string& value)
        :_impl(new Impl)
    {
        _impl->_type = Json::JsonType::String;
        _impl->_string = new string(value);
    }
    /**
     *  Constructor - _object value initialized
     */
    Json::Json(const unordered_map<string, Json>& value)
        :_impl(new Impl)
    {
        _impl->_type = Json::JsonType::Object;
        _impl->_object = new unordered_map<string, Json>(value);
    }
    /**
     *  Constructor - _array value initialized
     */
    Json::Json(const vector<Json>& value)
        :_impl(new Impl)
    {
        _impl->_type = Json::JsonType::Array;
        _impl->_array = new vector<Json>(value);
    }
    
    Json::Json::~Json() { }

    Json::Json(const Json& other) { 
        _impl = other._impl;
    }

    Json& Json::operator=(const Json& other) {
        _impl = other._impl;
    }


    /**
     * Operator overloading
     * <array index>
     * <object key>
     */
    Json& Json::operator[](int i) {
        if(_impl->_type == JsonType::Array && i >= 0)
            return _impl->_array->at(i);
        return *this;
    }

    Json& Json::operator[](const char* key) {
        if(_impl->_type == JsonType::Object) 
            return _impl->_object->at(key);
        return *this;
    }

    Json& Json::operator[](string key) {
        if(_impl->_type == JsonType::Object) 
            return _impl->_object->at(key);
        return *this;
    }

    /**
     * To string
     */
    string Json::stringifyObject() const {
        string element = "";
        element += "{";

        for(const auto& kv : *_impl->_object) 
            element += "\"" + kv.first + "\":" + kv.second.toString() + ",";
        
        if(_impl->_object->begin() != _impl->_object->end())
            element.pop_back();

        element += "}";
        return element;
    }
    string Json::stringifyArray() const {
        string element = "";
        element += "[";

        for(const auto& e : *_impl->_array)
            element += e.toString() + ",";
        
        if(_impl->_array->size())
            element.pop_back();

        element += "]";
        return element;
    }
    /**
     * Getting a json from string
     */
    Json Json::fromString(const string& text) {
        auto parser = JsonParser(text);
        auto json = parser.parse();
        json._diagnostics = parser.Diagnostics();
        return json;
    }
    /**
     * Getting a string from json
     */
    string Json::toString() const {
        switch (_impl->_type) {
            case JsonType::Undefined: return "undefined";
            case JsonType::Null: return "null";
            case JsonType::Int: return to_string(_impl->_int);
            case JsonType::Float: return to_string(_impl->_float);
            case JsonType::String: return "\"" + *(_impl->_string) + "\"";
            case JsonType::Bool: return _impl->_bool == true ? "true":"false";
            case JsonType::Object: return stringifyObject();
            case JsonType::Array: return stringifyArray();
            default: return "";
        }
    }

    /**
     * Helper functions
     */
    Json JsonArray()
    {
        return Json(vector<Json>());
    }

    Json JsonArray(initializer_list<Json> arr)
    {
        return Json(vector<Json>(arr));
    }

    Json JsonObject()
    {
        return Json(unordered_map<string, Json>());
    }

    Json JsonObject(initializer_list<pair<string, Json>> obj)
    {
        unordered_map<string, Json> ret;

        for(auto& e : obj) 
            ret.insert(e); 

        return Json(ret);
    }

    /**
     * Operators overloading
     */
    bool operator==(const Json& instance, const nullptr_t& value) {
        return instance._impl->_type == Json::JsonType::Null;
    }
    bool operator==(const nullptr_t& value, const Json& instance) {
        return instance._impl->_type == Json::JsonType::Null;
    }

    bool operator==(const Json& instance, const int& value) {
        return instance._impl->_type == Json::JsonType::Int ?
            ((int)instance._impl->_int == value) : false;
    }
    bool operator==(const int& value, const Json& instance) {
        return instance._impl->_type == Json::JsonType::Int ?
            (instance._impl->_int == (long long)value) : false;
    }

    bool operator==(const Json& instance, const long long& value) {
        return instance._impl->_type == Json::JsonType::Int ?
            (instance._impl->_int == value) : false;
    }
    bool operator==(const long long& value, const Json& instance) {
        return instance._impl->_type == Json::JsonType::Int ?
            (instance._impl->_int == value) : false;
    }

    bool operator==(const Json& instance, const double& value) {
        return instance._impl->_type == Json::JsonType::Float ?
            ((double)instance._impl->_float == value) : false;
    }
    bool operator==(const double& value, const Json& instance) {
        return instance._impl->_type == Json::JsonType::Float ?
            ((double)instance._impl->_float == value) : false;
    }

    bool operator==(const Json& instance, const long double& value) {
        return instance._impl->_type == Json::JsonType::Float?
            (instance._impl->_float == value) : false;
    }
    bool operator==(const long double& value, const Json& instance) {
        return instance._impl->_type == Json::JsonType::Float ?
            (instance._impl->_float == value) : false;
    }

    bool operator==(const Json& instance, const bool& value) {
        return instance._impl->_type == Json::JsonType::Bool ?
            (instance._impl->_bool == value) : false;
    }
    bool operator==(const bool& value, const Json& instance) {
        return instance._impl->_type == Json::JsonType::Bool ?
            (instance._impl->_bool == value) : false;
    }

    bool operator==(const Json& instance, const char* value) {
        return instance._impl->_type == Json::JsonType::String ?
            ((*instance._impl->_string) == value) : false;
    }
    bool operator==(const char* value, const Json& instance) {
        return instance._impl->_type == Json::JsonType::String ?
            ((*instance._impl->_string) == value) : false;
    }
    bool operator==(const Json& instance, const string& value) {
        return instance._impl->_type == Json::JsonType::String ?
            ((*instance._impl->_string) == value) : false;
    }
    bool operator==(const string& value, const Json& instance) {
        return instance._impl->_type == Json::JsonType::String ?
            ((*instance._impl->_string) == value) : false;
    }

    ostream& operator<<(std::ostream& os, const Json& json) {
        return os << json.toString();
    }

} // namespace Json
