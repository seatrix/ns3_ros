#include "catch.hpp"

#include <initializer_list>

#include "cxxopts.hpp"

class Argv {
  public:

  Argv(std::initializer_list<const char*> args)
  : m_argv(new char*[args.size()])
  , m_argc(args.size())
  {
    int i = 0;
    auto iter = args.begin();
    while (iter != args.end()) {
      auto len = strlen(*iter) + 1;
      auto ptr = std::unique_ptr<char[]>(new char[len]);

      strcpy(ptr.get(), *iter);
      m_args.push_back(std::move(ptr));
      m_argv.get()[i] = m_args.back().get();

      ++iter;
      ++i;
    }
  }

  char** argv() const {
    return m_argv.get();
  }

  int argc() const {
    return m_argc;
  }

  private:

  std::vector<std::unique_ptr<char[]>> m_args;
  std::unique_ptr<char*[]> m_argv;
  int m_argc;
};

TEST_CASE("Basic options", "[options]")
{

  cxxopts::Options options("tester", " - test basic options");

  options.add_options()
    ("long", "a long option")
    ("s,short", "a short option")
    ("value", "an option with a value", cxxopts::value<std::string>())
    ("a,av", "a short option with a value", cxxopts::value<std::string>())
    ("6,six", "a short number option")
    ("p, space", "an option with space between short and long")
    ;

  Argv argv({
    "tester",
    "--long",
    "-s",
    "--value",
    "value",
    "-a",
    "b",
    "-6",
    "-p",
    "--space",
  });

  char** actual_argv = argv.argv();
  auto argc = argv.argc();

  auto result = options.parse(argc, actual_argv);

  CHECK(result.count("long") == 1);
  CHECK(result.count("s") == 1);
  CHECK(result.count("value") == 1);
  CHECK(result.count("a") == 1);
  CHECK(result["value"].as<std::string>() == "value");
  CHECK(result["a"].as<std::string>() == "b");
  CHECK(result.count("6") == 1);
  CHECK(result.count("p") == 2);
  CHECK(result.count("space") == 2);

  auto& arguments = result.arguments();
  REQUIRE(arguments.size() == 7);
  CHECK(arguments[0].key() == "long");
  CHECK(arguments[0].value() == "true");
  CHECK(arguments[0].as<bool>() == true);

  CHECK(arguments[1].key() == "short");
  CHECK(arguments[2].key() == "value");
  CHECK(arguments[3].key() == "av");
}

TEST_CASE("Short options", "[options]")
{
  cxxopts::Options options("test_short", " - test short options");

  options.add_options()
    ("a", "a short option", cxxopts::value<std::string>());

  Argv argv({"test_short", "-a", "value"});

  auto actual_argv = argv.argv();
  auto argc = argv.argc();

  auto result = options.parse(argc, actual_argv);

  CHECK(result.count("a") == 1);
  CHECK(result["a"].as<std::string>() == "value");

  REQUIRE_THROWS_AS(options.add_options()("", "nothing option"), 
    cxxopts::invalid_option_format_error);
}

TEST_CASE("No positional", "[positional]")
{
  cxxopts::Options options("test_no_positional",
    " - test no positional options");

  Argv av({"tester", "a", "b", "def"});

  char** argv = av.argv();
  auto argc = av.argc();
  auto result = options.parse(argc, argv);

  REQUIRE(argc == 4);
  CHECK(strcmp(argv[1], "a") == 0);
}

TEST_CASE("All positional", "[positional]")
{
  std::vector<std::string> positional;

  cxxopts::Options options("test_all_positional", " - test all positional");
  options.add_options()
    ("positional", "Positional parameters",
      cxxopts::value<std::vector<std::string>>(positional))
  ;

  Argv av({"tester", "a", "b", "c"});

  auto argc = av.argc();
  auto argv = av.argv();

  options.parse_positional({"positional"});

  auto result = options.parse(argc, argv);

  REQUIRE(argc == 1);
  REQUIRE(positional.size() == 3);

  CHECK(positional[0] == "a");
  CHECK(positional[1] == "b");
  CHECK(positional[2] == "c");
}

TEST_CASE("Some positional explicit", "[positional]")
{
  cxxopts::Options options("positional_explicit", " - test positional");

  options.add_options()
    ("input", "Input file", cxxopts::value<std::string>())
    ("output", "Output file", cxxopts::value<std::string>())
    ("positional", "Positional parameters",
      cxxopts::value<std::vector<std::string>>())
  ;

  options.parse_positional({"input", "output", "positional"});

  Argv av({"tester", "--output", "a", "b", "c", "d"});

  char** argv = av.argv();
  auto argc = av.argc();

  auto result = options.parse(argc, argv);

  CHECK(argc == 1);
  CHECK(result.count("output"));
  CHECK(result["input"].as<std::string>() == "b");
  CHECK(result["output"].as<std::string>() == "a");

  auto& positional = result["positional"].as<std::vector<std::string>>();

  REQUIRE(positional.size() == 2);
  CHECK(positional[0] == "c");
  CHECK(positional[1] == "d");
}

TEST_CASE("No positional with extras", "[positional]")
{
  cxxopts::Options options("posargmaster", "shows incorrect handling");
  options.add_options()
      ("dummy", "oh no", cxxopts::value<std::string>())
      ;

  Argv av({"extras", "--", "a", "b", "c", "d"});

  char** argv = av.argv();
  auto argc = av.argc();

  auto old_argv = argv;
  auto old_argc = argc;

  options.parse(argc, argv);

  REQUIRE(argc == old_argc - 1);
  CHECK(argv[0] == std::string("extras"));
  CHECK(argv[1] == std::string("a"));
}

TEST_CASE("Empty with implicit value", "[implicit]")
{
  cxxopts::Options options("empty_implicit", "doesn't handle empty");
  options.add_options()
    ("implicit", "Has implicit", cxxopts::value<std::string>()
      ->implicit_value("foo"));

  Argv av({"implicit", "--implicit="});

  char** argv = av.argv();
  auto argc = av.argc();

  auto result = options.parse(argc, argv);

  REQUIRE(result.count("implicit") == 1);
  REQUIRE(result["implicit"].as<std::string>() == "");
}

TEST_CASE("Default values", "[default]")
{
  cxxopts::Options options("defaults", "has defaults");
  options.add_options()
    ("default", "Has implicit", cxxopts::value<int>()
      ->default_value("42"));

  SECTION("Sets defaults") {
    Argv av({"implicit"});

    char** argv = av.argv();
    auto argc = av.argc();

    auto result = options.parse(argc, argv);
    CHECK(result.count("default") == 1);
    CHECK(result["default"].as<int>() == 42);
  }

  SECTION("When values provided") {
    Argv av({"implicit", "--default", "5"});

    char** argv = av.argv();
    auto argc = av.argc();

    auto result = options.parse(argc, argv);
    CHECK(result.count("default") == 1);
    CHECK(result["default"].as<int>() == 5);
  }
}

TEST_CASE("Parse into a reference", "[reference]")
{
  int value = 0;

  cxxopts::Options options("into_reference", "parses into a reference");
  options.add_options()
    ("ref", "A reference", cxxopts::value(value));

  Argv av({"into_reference", "--ref", "42"});

  auto argv = av.argv();
  auto argc = av.argc();

  auto result = options.parse(argc, argv);
  CHECK(result.count("ref") == 1);
  CHECK(value == 42);
}

TEST_CASE("Integers", "[options]")
{
  cxxopts::Options options("parses_integers", "parses integers correctly");
  options.add_options()
    ("positional", "Integers", cxxopts::value<std::vector<int>>());

  Argv av({"ints", "--", "5", "6", "-6", "0", "0xab", "0xAf", "0x0"});

  char** argv = av.argv();
  auto argc = av.argc();

  options.parse_positional("positional");
  auto result = options.parse(argc, argv);

  REQUIRE(result.count("positional") == 7);

  auto& positional = result["positional"].as<std::vector<int>>();
  REQUIRE(positional.size() == 7);
  CHECK(positional[0] == 5);
  CHECK(positional[1] == 6);
  CHECK(positional[2] == -6);
  CHECK(positional[3] == 0);
  CHECK(positional[4] == 0xab);
  CHECK(positional[5] == 0xaf);
  CHECK(positional[6] == 0x0);
}

TEST_CASE("Unsigned integers", "[options]")
{
  cxxopts::Options options("parses_unsigned", "detects unsigned errors");
  options.add_options()
    ("positional", "Integers", cxxopts::value<std::vector<unsigned int>>());

  Argv av({"ints", "--", "-2"});

  char** argv = av.argv();
  auto argc = av.argc();

  options.parse_positional("positional");
  CHECK_THROWS_AS(options.parse(argc, argv), cxxopts::argument_incorrect_type);
}

TEST_CASE("Integer bounds", "[integer]")
{
  cxxopts::Options options("integer_boundaries", "check min/max integer");
  options.add_options()
    ("positional", "Integers", cxxopts::value<std::vector<int8_t>>());

  SECTION("No overflow")
  {
    Argv av({"ints", "--", "127", "-128", "0x7f", "-0x80", "0x7e"});

    auto argv = av.argv();
    auto argc = av.argc();

    options.parse_positional("positional");
    auto result = options.parse(argc, argv);

    REQUIRE(result.count("positional") == 5);

    auto& positional = result["positional"].as<std::vector<int8_t>>();
    CHECK(positional[0] == 127);
    CHECK(positional[1] == -128);
    CHECK(positional[2] == 0x7f);
    CHECK(positional[3] == -0x80);
    CHECK(positional[4] == 0x7e);
  }
}

TEST_CASE("Overflow on boundary", "[integer]")
{
  using namespace cxxopts::values;

  int8_t si;
  uint8_t ui;

  CHECK_THROWS_AS((integer_parser("128", si)), cxxopts::argument_incorrect_type);
  CHECK_THROWS_AS((integer_parser("-129", si)), cxxopts::argument_incorrect_type);
  CHECK_THROWS_AS((integer_parser("256", ui)), cxxopts::argument_incorrect_type);
  CHECK_THROWS_AS((integer_parser("-0x81", si)), cxxopts::argument_incorrect_type);
  CHECK_THROWS_AS((integer_parser("0x80", si)), cxxopts::argument_incorrect_type);
  CHECK_THROWS_AS((integer_parser("0x100", ui)), cxxopts::argument_incorrect_type);
}

TEST_CASE("Integer overflow", "[options]")
{
  cxxopts::Options options("reject_overflow", "rejects overflowing integers");
  options.add_options()
    ("positional", "Integers", cxxopts::value<std::vector<int8_t>>());

  Argv av({"ints", "--", "128"});

  auto argv = av.argv();
  auto argc = av.argc();

  options.parse_positional("positional");
  CHECK_THROWS_AS(options.parse(argc, argv), cxxopts::argument_incorrect_type);
}

TEST_CASE("Floats", "[options]")
{
  cxxopts::Options options("parses_floats", "parses floats correctly");
  options.add_options()
    ("double", "Double precision", cxxopts::value<double>())
    ("positional", "Floats", cxxopts::value<std::vector<float>>());

  Argv av({"floats", "--double", "0.5", "--", "4", "-4", "1.5e6", "-1.5e6"});

  char** argv = av.argv();
  auto argc = av.argc();

  options.parse_positional("positional");
  auto result = options.parse(argc, argv);

  REQUIRE(result.count("double") == 1);
  REQUIRE(result.count("positional") == 4);

  CHECK(result["double"].as<double>() == 0.5);

  auto& positional = result["positional"].as<std::vector<float>>();
  CHECK(positional[0] == 4);
  CHECK(positional[1] == -4);
  CHECK(positional[2] == 1.5e6);
  CHECK(positional[3] == -1.5e6);
}

TEST_CASE("Invalid integers", "[integer]") {
    cxxopts::Options options("invalid_integers", "rejects invalid integers");
    options.add_options()
    ("positional", "Integers", cxxopts::value<std::vector<int>>());

    Argv av({"ints", "--", "Ae"});

    char **argv = av.argv();
    auto argc = av.argc();

    options.parse_positional("positional");
    CHECK_THROWS_AS(options.parse(argc, argv), cxxopts::argument_incorrect_type);
}

TEST_CASE("Booleans", "[boolean]") {
  cxxopts::Options options("parses_floats", "parses floats correctly");
  options.add_options()
    ("bool", "A Boolean", cxxopts::value<bool>())
    ("debug", "Debugging", cxxopts::value<bool>())
    ("timing", "Timing", cxxopts::value<bool>())
    ("others", "Other arguments", cxxopts::value<std::vector<std::string>>())
    ;

  options.parse_positional("others");

  Argv av({"booleans", "--bool=false", "--debug=true", "--timing", "extra"});

  char** argv = av.argv();
  auto argc = av.argc();

  auto result = options.parse(argc, argv);

  REQUIRE(result.count("bool") == 1);
  REQUIRE(result.count("debug") == 1);
  REQUIRE(result.count("timing") == 1);

  CHECK(result["bool"].as<bool>() == false);
  CHECK(result["debug"].as<bool>() == true);
  CHECK(result["timing"].as<bool>() == true);

  REQUIRE(result.count("others") == 1);
}
