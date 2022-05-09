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
  // Serial.println("NAV_PTR:{" + NAV_PTR[0] + "," + NAV_PTR[1] + "," + NAV_PTR[2] + "}" );
}

void onReturnRise() {
  if (refresh) {
     return;
  }
  IDX = max(0, IDX - 1);
  setSubmenuStatus();
  refresh = true;
  // Serial.println("NAV_PTR:{" + NAV_PTR[0] + "," + NAV_PTR[1] + "," + NAV_PTR[2] + "}" );
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
    stateMachine[NAV_PTR[0]][NAV_PTR[1]](LCD, *getValve(), 1);
  }

  refresh = true;
  // Serial.println("NAV_PTR:{" + NAV_PTR[0] + "," + NAV_PTR[1] + "," + NAV_PTR[2] + "}" );
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
    stateMachine[NAV_PTR[0]][NAV_PTR[1]](LCD, *getValve(), -1);
  }

  refresh = true;
  // Serial.println("NAV_PTR:{" + NAV_PTR[0] + "," + NAV_PTR[1] + "," + NAV_PTR[2] + "}" );
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
