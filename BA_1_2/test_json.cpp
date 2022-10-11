// from https://github.com/nlohmann/json

#include"BA_Base.hpp"
#include"BA_File.hpp"
#include"BA_JSON.hpp"
#include"BA_Test.hpp"

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

void ba::test::_json::stlAccess(void)
{
    // create an array using push_back
    json j;
    j.push_back("foo");
    j.push_back(1);
    j.push_back(true);

    // also use emplace_back
    j.emplace_back(1.78);

    // iterate the array
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        std::cout << *it << '\n';
    }

    // range-based for
    for (auto& element : j) {
        std::cout << element << '\n';
    }

    // getter/setter
    const auto tmp = j[0].get<std::string>();
    j[1] = 42;
    bool foo = j.at(2);

    // comparison
    j == R"(["foo", 1, true, 1.78])"_json;  // true

    // other stuff
    j.size();     // 4 entries
    j.empty();    // false
    j.type();     // json::value_t::array
    j.clear();    // the array is empty again

    // convenience type checkers
    j.is_null();
    j.is_boolean();
    j.is_number();
    j.is_object();
    j.is_array();
    j.is_string();

    // create an object
    json o;
    o["foo"] = 23;
    o["bar"] = false;
    o["baz"] = 3.141;

    // also use emplace
    o.emplace("weather", "sunny");

    // special iterator member functions for objects
    for (json::iterator it = o.begin(); it != o.end(); ++it) {
        std::cout << it.key() << " : " << it.value() << "\n";
    }

    // the same code as range for
    for (auto& el : o.items()) {
        std::cout << el.key() << " : " << el.value() << "\n";
    }

    // even easier with structured bindings (C++17)
    for (auto& [key, value] : o.items()) {
        std::cout << key << " : " << value << "\n";
    }

    // find an entry
    if (o.contains("foo")) {
        // there is an entry with key "foo"
    }

    // or via find and an iterator
    if (o.find("foo") != o.end()) {
        // there is an entry with key "foo"
    }

    // or simpler using count()
    _ULL foo_present = o.count("foo"); // 1
    _ULL fob_present = o.count("fob"); // 0

    // delete an entry
    o.erase("foo");
}

void ba::test::_json::myAccess(void)
{
    json j = {
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
    int i = j["answer"]["everything"];
    PPX(i);
}

void ba::test::_json::convertionWithSTL(void)
{
    std::vector<int> c_vector{ 1, 2, 3, 4 };
    json j_vec(c_vector);
    // [1, 2, 3, 4]

    std::deque<double> c_deque{ 1.2, 2.3, 3.4, 5.6 };
    json j_deque(c_deque);
    // [1.2, 2.3, 3.4, 5.6]

    std::list<bool> c_list{ true, true, false, true };
    json j_list(c_list);
    // [true, true, false, true]

    std::forward_list<int64_t> c_flist{ 12345678909876, 23456789098765, 34567890987654, 45678909876543 };
    json j_flist(c_flist);
    // [12345678909876, 23456789098765, 34567890987654, 45678909876543]

    std::array<unsigned long, 4> c_array{ {1, 2, 3, 4} };
    json j_array(c_array);
    // [1, 2, 3, 4]

    std::set<std::string> c_set{ "one", "two", "three", "four", "one" };
    json j_set(c_set); // only one entry for "one" is used
    // ["four", "one", "three", "two"]

    std::unordered_set<std::string> c_uset{ "one", "two", "three", "four", "one" };
    json j_uset(c_uset); // only one entry for "one" is used
    // maybe ["two", "three", "four", "one"]

    std::multiset<std::string> c_mset{ "one", "two", "one", "four" };
    json j_mset(c_mset); // both entries for "one" are used
    // maybe ["one", "two", "one", "four"]

    std::unordered_multiset<std::string> c_umset{ "one", "two", "one", "four" };
    json j_umset(c_umset); // both entries for "one" are used
    // maybe ["one", "two", "one", "four"]

    std::map<std::string, int> c_map{ {"one", 1}, {"two", 2}, {"three", 3} };
    json j_map(c_map);
    // {"one": 1, "three": 3, "two": 2 }

    std::unordered_map<const char*, double> c_umap{ {"one", 1.2}, {"two", 2.3}, {"three", 3.4} };
    json j_umap(c_umap);
    // {"one": 1.2, "two": 2.3, "three": 3.4}

    std::multimap<std::string, bool> c_mmap{ {"one", true}, {"two", true}, {"three", false}, {"three", true} };
    json j_mmap(c_mmap); // only one entry for key "three" is used
    // maybe {"one": true, "two": true, "three": true}

    std::unordered_multimap<std::string, bool> c_ummap{ {"one", true}, {"two", true}, {"three", false}, {"three", true} };
    json j_ummap(c_ummap); // only one entry for key "three" is used
    // maybe {"one": true, "two": true, "three": true}
}

//https://github.com/nlohmann/json#custom-data-source
void ba::test::_json::customData(void)
{
}

void ba::test::_json::conversions(void)
{
    // strings
    std::string s1 = "Hello, world!";
    json js = s1;
    auto s2 = js.get<std::string>();
    // NOT RECOMMENDED
    std::string s3 = js;
    std::string s4;
    s4 = js;

    // Booleans
    bool b1 = true;
    json jb = b1;
    auto b2 = jb.get<bool>();
    // NOT RECOMMENDED
    bool b3 = jb;
    bool b4;
    b4 = jb;

    // numbers
    int i = 42;
    json jn = i;
    auto f = jn.get<double>();
    // NOT RECOMMENDED
    double f2 = jb;
    double f3;
    f3 = jb;

    // etc.

    char ch = 'A';                       // ASCII value 65
    json j_default = ch;                 // stores integer number 65
    json j_string = std::string(1, ch);  // stores string "A"
}

void ba::test::_json::customDataConv(void)
{
    /*Those methods MUST be in your type's namespace 
    (which can be the global namespace), 
    or the library will not be able to locate them 
    (in this example, they are in namespace ns, where person is defined).*/

    /*When using get<your_type>(), your_type MUST be DefaultConstructible.
    (There is a way to bypass this requirement described later.)*/

    //namespace ns {
    //    void to_json(json& j, const person& p) {
    //        j = json{ {"name", p.name}, {"address", p.address}, {"age", p.age} };
    //    }

    //    void from_json(const json& j, person& p) {
    //        j.at("name").get_to(p.name);
    //        j.at("address").get_to(p.address);
    //        j.at("age").get_to(p.age);
    //    }
    //} // namespace ns

    // then can do this

    //// create a person
    //ns::person p{ "Ned Flanders", "744 Evergreen Terrace", 60 };

    //// conversion: person -> json
    //json j = p;

    //std::cout << j << std::endl;
    //// {"address":"744 Evergreen Terrace","age":60,"name":"Ned Flanders"}

    //// conversion: json -> person
    //auto p2 = j.get<ns::person>();

    //// that's it
    //assert(p == p2);
}

void ba::test::_json::concat(void)
{
    json person = {
        {"name", "bob"},
        {"age", 21},
    };
    json university = {
        {"name", "lzu"},
        {"history", 117},
    };
    person["edu"] = university;
    std::cout << std::setw(4) << person << std::endl;
    std::cout << std::setw(4) << person["edu"] << std::endl;
}

/*the library supports BSON (Binary JSON), 
CBOR (Concise Binary Object Representation), 
MessagePack, 
UBJSON (Universal Binary JSON Specification) 
and BJData (Binary JData) 
to efficiently encode JSON values to byte vectors and to decode such vectors.*/
void ba::test::_json::binaryFormatStore(void)
{
    // create a JSON value
    json j = R"({"compact": true, "schema": 0})"_json;

    // serialize to BSON
    std::vector<std::uint8_t> v_bson = json::to_bson(j);
    // 0x1B, 0x00, 0x00, 0x00, 0x08, 0x63, 0x6F, 0x6D, 0x70, 0x61, 0x63, 0x74, 0x00, 0x01, 0x10, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    // roundtrip
    json j_from_bson = json::from_bson(v_bson);

    // serialize to CBOR
    std::vector<std::uint8_t> v_cbor = json::to_cbor(j);
    // 0xA2, 0x67, 0x63, 0x6F, 0x6D, 0x70, 0x61, 0x63, 0x74, 0xF5, 0x66, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x00
    // roundtrip
    json j_from_cbor = json::from_cbor(v_cbor);

    // serialize to MessagePack
    std::vector<std::uint8_t> v_msgpack = json::to_msgpack(j);
    // 0x82, 0xA7, 0x63, 0x6F, 0x6D, 0x70, 0x61, 0x63, 0x74, 0xC3, 0xA6, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x00
    // roundtrip
    json j_from_msgpack = json::from_msgpack(v_msgpack);

    // serialize to UBJSON
    std::vector<std::uint8_t> v_ubjson = json::to_ubjson(j);
    // 0x7B, 0x69, 0x07, 0x63, 0x6F, 0x6D, 0x70, 0x61, 0x63, 0x74, 0x54, 0x69, 0x06, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x69, 0x00, 0x7D
    // roundtrip
    json j_from_ubjson = json::from_ubjson(v_ubjson);
}
