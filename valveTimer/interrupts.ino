void onEnterRise() {
  if (refresh) {
     return;
  } else if (IDX != 2) {
    IDX++;
    NAV_PTR[2] = 0;
    setSubmenuStatus();
  } else {
    NAV_PTR[2]++;
  }
  refresh = true;
}

void onReturnRise() {
  if (refresh) {
     return;
  }
  IDX = max(0, IDX - 1);
  setSubmenuStatus();
  refresh = true;
}

void onNextRise() {
  if (refresh) {
    return;
  } else if (IDX != 2) {
    NAV_PTR[IDX] = ++NAV_PTR[IDX] % getSize();
    if ((IDX != 0) && (0 == NAV_PTR[IDX])) {
      // If ciclying on menus, skip "landing" screen
      onNextRise();
    }
  } else {
    Valve* valve = XtvUtils::getValve(valves, NAV_PTR);
    stateMachine[NAV_PTR[0]][NAV_PTR[1]](*settings, valves, NAV_PTR, -1);
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
    Valve* valve = XtvUtils::getValve(valves, NAV_PTR);
    stateMachine[NAV_PTR[0]][NAV_PTR[1]](*settings, valves, NAV_PTR, -1);
  }

  refresh = true;
}

// -- Utilities functions --

short getSize() {
  return (IDX == 0) ? (1 + MAX_VALVES) : stateMachineSize[NAV_PTR[0]];
}

void setSubmenuStatus() {
  switch(IDX) {
    case 0:
      NAV_PTR[1] = 0;
      break;
    case 1:
      NAV_PTR[1] = 1;
      break;
    default:
      break;
  }
}
