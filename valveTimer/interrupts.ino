void onEnterRise() {
  if (refresh) {
     return;
  } else if (IDX != 2) {
    IDX++;
    NAV_PTR[2] = 0;
    setSubmenuStatus();
  } else {
    NAV_PTR[IDX]++;
  }
  refresh = true;
}

void onReturnRise() {
  if (refresh) {
     return;
  }
  IDX = max(IDX - 1, 0);
  setSubmenuStatus();
  refresh = true;
}

void onNextRise() {
  if (refresh) {
    return;
  } else if (IDX < 2) {
    NAV_PTR[IDX] = ++NAV_PTR[IDX] % getSize();
    if ((IDX != 0) && (0 == NAV_PTR[IDX])) {
      // If ciclying on menus, skip "landing" screen
      onNextRise();
    }
  } else {
    stateMachine[NAV_PTR[0]][NAV_PTR[1]](LCD, valves[NAV_PTR[0]], 1);
  }

  refresh = true;
}

void onPrevRise() {
  if (refresh) {
    return;
  } else if (IDX < 2) {
    NAV_PTR[IDX] = (getSize() + --NAV_PTR[IDX]) % getSize();
    if ((IDX != 0) && (0 == NAV_PTR[IDX])) {
      // If ciclying on menus, skip "landing" screen
      onPrevRise();
    }
  } else {
    stateMachine[NAV_PTR[0]][NAV_PTR[1]](LCD, valves[NAV_PTR[0]], -1);
  }

  refresh = true;
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
