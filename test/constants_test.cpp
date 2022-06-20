#include <cassert>
#include <iostream>
#include <string.h>

#include "../valve_timer/include/xtv_constants.h"

using namespace std;

void testFileName(const string &filename) {
  std::cout << filename << std::endl;
}


int main() {
  testFileName(SETTINGS_FILE_NAME);
}