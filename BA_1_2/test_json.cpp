
#include"BA_Base.hpp"
#include"BA_CMD.hpp"
#include"BA_Mem.hpp"
#include"BA_Thread.hpp"
#include"BA_Math.hpp"
#include"BA_File.hpp"
#include"BA_String.hpp"
#include"BA_Test.hpp"
#include"BA_BioInfo.hpp"
#include"BA_JSON.hpp"

    /* the library only supports UTF-8 */

using json = nlohmann::json;

void ba::test::_json::simpleJsonLife(void)
{
    /* create */

    auto j1 = R"(
          {
            "happy": true,
            "pi": 3.141
          }
        )"_json;
    auto j2 = json::parse(R"({"happy": true, "pi": 3.141})");
    //{
    //    "pi": 3.141,
    //        "happy" : true,
    //        "name" : "Niels",
    //        "nothing" : null,
    //        "answer" : {
    //        "everything": 42
    //    },
    //        "list" : [1, 0, 2] ,
    //            "object" : {
    //            "currency": "USD",
    //                "value" : 42.99
    //        }
    //}
    // or
    // j["list"] = { 1, 0, 2 };
    // j["answer"]["everything"] = 42;
    // j["object"] = { {"currency", "USD"}, {"value", 42.99} };
    // or
    json j3 = {
      {"pi", 3.141},
      {"happy", true},
      {"name", "Niels"},
      {"nothing", nullptr},
      {"answer", {
        {"everything", 42}
      }},
      {"list", {1, 0, 2}},
      {"object", {
        {"currency", "USD"},
        {"value", 42.99}
      }}
    };

    /* Serialization / Deserialization */

    // read a JSON file
    std::ifstream i(R"(E:\My_Progs\z_Progs_Data_HC\MyBA\test.json)", std::ifstream::in);
    json j4;
    i >> j4;
    //get a string representation of a JSON value(serialize) :
    std::string s4 = j4.dump(6);
    // the setw manipulator was overloaded to set the indentation for pretty printing
    std::cout << std::setw(8) << j4 << std::endl;
    // write prettified JSON to another file
    std::ofstream f(R"(E:\My_Progs\z_Progs_Data_HC\MyBA\test2.json)", std::ifstream::trunc);
    f << std::setw(7) << j4 << std::endl;

    //Read from iterator range
    std::vector<std::uint8_t> v = { 't', 'r', 'u', 'e' };
    json j = json::parse(v.begin(), v.end());
}

//https://github.com/nlohmann/json#custom-data-source
void ba::test::_json::customData(void)
{

}
