void onEnterRise() {
  if (!refresh) {
    IDX = min(IDX+1, 3);
    setSubmenuStatus();
    refresh = true;
  }
}

void onReturnRise() {
  if (!refresh) {
    IDX = max(IDX-1, 0);
    setSubmenuStatus();
    refresh = true;
  }
}

void onNextRise() {
  if (!refresh) {
    NAV_PTR[IDX] = ++NAV_PTR[IDX] % getSize();
    if ((IDX != 0) && (0 == NAV_PTR[IDX])) {
      // If ciclying on menus, skip "landing" screen
      onNextRise();
    }
    refresh = true;
  }
}

void onPrevRise() {
  if (!refresh) {
    NAV_PTR[IDX] = (getSize() + --NAV_PTR[IDX]) % getSize();
    if ((IDX != 0) && (0 == NAV_PTR[IDX])) {
      // If ciclying on menus, skip "landing" screen
      onPrevRise();
    }
    refresh = true;
  }
}

// -- Utilities functions --

short getSize() {
  return (IDX == 0) ? (1 + MAX_VALVES) : stateMachineSize[NAV_PTR[0]];
}

void setSubmenuStatus() {
  if (IDX == 0) {
    NAV_PTR[1] = 0;
  } else if (IDX == 1) {
    NAV_PTR[1] = 1;
  }
}
