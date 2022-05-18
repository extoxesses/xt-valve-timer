echo '1. Test valves'
g++ ./valve_test.cpp ../valveTimer/src/xtv_valve.cpp -o unit_test
./unit_test

echo 'End unit tests'
rm unit_test