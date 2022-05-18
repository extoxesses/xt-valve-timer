#include <cassert>
#include <iostream>
#include <string.h>

#include "../valveTimer/include/xtv_valve.h"

using namespace std;

void testValveManual() {
  // Test valve "isManual" default value
  Valve v;
  assert(v.isManual());
  // Test "setManual" with constant
  v.setManual(false);
  assert(!v.isManual());
  // Test "setManual" as timer
  v.setManual(!v.isManual());
  assert(v.isManual());
}

void testValveActive() {
  // Test valve "isActive" default value
  Valve v;
  assert(v.isActive());
  // Test "setActive" with constant
  v.setActive(false);
  assert(!v.isActive());
  // Test "setActive" as timer
  v.setManual(!v.isActive());
  assert(v.isActive());
}

int main() {
  testValveManual();
}