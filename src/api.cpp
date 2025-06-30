#include <crow/app.h>
#include <crow/common.h>
#include <crow/multipart.h>

using TypeParam = double;

constexpr uint16_t PORT = 443;

auto main(int argc, char *argv[]) -> int {

  crow::SimpleApp app;

  CROW_ROUTE(app, "/health")([]() { return "Hello world"; });

  app.port(PORT).multithreaded().run();
}
