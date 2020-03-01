#include "../Json/Json.h"
#include "./Test.h"

#include <bits/stdc++.h>

using namespace std;

string readFile(const std::string& fileName) {
    
    ifstream fin(fileName);

    if( !fin ) return "";
    
    stringstream ss;
    string line;

    while ( getline( fin, line ) )
        ss << line << '\n';

    return ss.str();
}
// 12343

int main() {

    using namespace JsonSer;

    /**
     * ---------------------- Testing ---------------------- 
     * 
     */
    
    /**
     * Null to String
     */
    {
        TestAPI::TEST("NULL TO STRING");
        Json json(nullptr); // or Json json = nullptr;
        TestAPI::ASSERT((json.toString() == "null"));
    }
    /**
     * Null from String
     */
    {
        TestAPI::TEST("NULL FROM STRING");
        Json json = Json::fromString("null");
        TestAPI::ASSERT((json == nullptr) && (nullptr == json));
    }

    /**
     * Int to String
     */
    {
        TestAPI::TEST("INT TO STRING");
        Json json(10); // or Json json = 10;
        TestAPI::ASSERT((json.toString() == "10"));
    }
    /**
     * Int from String
     */
    {
        TestAPI::TEST("INT FROM STRING");
        Json json = Json::fromString("10");
        TestAPI::ASSERT((json == 10) && (10 == json));
    }

    /**
     * Long long to String
     */
    {
        TestAPI::TEST("LONG LONG TO STRING");
        Json json((long long)10); // or Json json = (long long)10;
        TestAPI::ASSERT((json.toString() == "10"));
    }
    /**
     * Long long from String
     */
    {
        TestAPI::TEST("LONG LONG FROM STRING");
        Json json = Json::fromString("10");
        TestAPI::ASSERT((json == (long long)10) && ((long long)10 == json));
    }

    /**
     * Double to String
     */
    {
        TestAPI::TEST("DOUBLE TO STRING");
        Json json(10.433423); // or Json json = 10.433423;
        TestAPI::ASSERT((json.toString() == "10.433423"));
    }
    /**
     * Double from String
     */
    {
        TestAPI::TEST("DOUBLE FROM STRING");
        Json json = Json::fromString("10.433423");
        TestAPI::ASSERT((json == 10.433423) && (10.433423 == json));
    }

    /**
     * Long double to String
     */
    {
        TestAPI::TEST("LONG DOUBLE TO STRING");
        Json json(10.433423L); // or Json json = 10.433423L;
        TestAPI::ASSERT((json.toString() == "10.433423"));
    }
    /**
     * Long double from String
     */
    {
        TestAPI::TEST("LONG DOUBLE FROM STRING");
        Json json = Json::fromString("43.43344");
        long double value = 43.43344L;
        TestAPI::ASSERT((json == value) && (value == json));
    }

    /**
     * Bool to String
     */
    {
        TestAPI::TEST("BOOL TO STRING");
        Json jsont(true); // or Json json = true;
        Json jsonf(false); // or Json json = false;
        TestAPI::ASSERT((jsont.toString() == "true") && (jsonf.toString() == "false"));
    }
    /**
     * Bool from String
     */
    {
        TestAPI::TEST("BOOL FROM STRING");
        Json jsont = Json::fromString("true");
        Json jsonf = Json::fromString("false");
        TestAPI::ASSERT((jsont == true) && (false == jsonf));
    }

    /**
     * String to String
     */
    {
        TestAPI::TEST("STRING TO STRING");
        Json json("Json String"); // or Json json = "Json String";
        TestAPI::ASSERT(json.toString() == "\"Json String\"");
    }
    /**
     * String from String
     */
    {
        TestAPI::TEST("STRING FROM STRING");
        Json json = Json::fromString("\"Json\"");
        TestAPI::ASSERT((json == "Json"));
    }

    /**
     * Object to String
     */
    {
        TestAPI::TEST("OBJECT TO STRING");

        Json json = JsonObject({
            {"height", 20},
            {"width", 10},
        });

        TestAPI::ASSERT(
            (json.toString() == "{\"width\":10,\"height\":20}")
        );
    }
    /**
     * Object from String
     */
    {
        TestAPI::TEST("OBJECT FROM STRING");
        Json json = Json::fromString("{\"height\":20,\"width\":10}");

        TestAPI::ASSERT(
            (json.toString() == "{\"width\":10,\"height\":20}")
        );
    }

    /**
     * Array to String
     */
    {
        TestAPI::TEST("ARRAY TO STRING");

        Json json = JsonArray({
            10, 230, "Mario", 34.76L, true, false, nullptr
        });

        TestAPI::ASSERT(
            (json.toString() == "[10,230,\"Mario\",34.760000,true,false,null]")
        );
    }
    /**
     * Array from String
     */
    {
        TestAPI::TEST("ARRAY FROM STRING");
        Json json = Json::fromString("[10,230,\"Mario\",34.760000,true,false,null]");

        TestAPI::ASSERT(
            (json.toString() == "[10,230,\"Mario\",34.760000,true,false,null]")
        );
    }

    /**
     * Random
     */
    {
        TestAPI::TEST("RANDOM");

        const string& figure = readFile("./static/figure.json");
        const string& testarr = readFile("./static/testarr.json");
        const string& testobj = readFile("./static/testobj.json");

        Json json;

        json = Json::fromString(figure);
        cout << "----------------------------------------------------\n" 
            << json << "\n\n";
        cout << "----------------------------------------------------\n" 
            << json[0]["ranges"] << "\n\n";

        json = Json::fromString(testarr);
        cout << "----------------------------------------------------\n" 
            << json << "\n\n";

        json = Json::fromString(testobj);
        cout << "----------------------------------------------------\n" 
            << json << "\n\n";
        
    }

    /**
     * Multiple (Random 2)
     */
    {
        TestAPI::TEST("MULTIPLE");

        Json json = JsonArray({
            10, 230, "Mario", 34.76L, true, false, nullptr, JsonObject({
                {"height", 20},
                {"width", 10},
            }),
            JsonArray({
                65.7676, "HEllo WORld"
            })
        });

        cout << json << '\n';
    }

    /**
     * ERROR HANDLING
     */
    {
        TestAPI::TEST("ERROR HANDLING");
        Json json = Json::fromString("[10, 230, \"Mario\"34.760000, true, false, null]");

        const auto& diagnostics = json.Diagnostics();

        for(const auto& diagnostic : diagnostics)
            cout << diagnostic << '\n';

        cout << json << endl;
    }

    return 0;
}