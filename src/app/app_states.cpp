#include <app/app_states.h>

namespace minesweeper::app {
TestState2::TestState2()
    : test_button{
          Graph_lib::Point{100, 100}, 50, 50, "Change to state1",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            get_app_ref(button_addr).set_state(TestState1::get_instance());
          }} {};

TestState1::TestState1()
    : test_button{
          Graph_lib::Point{20, 20}, 50, 50, "Change to state2",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            get_app_ref(button_addr).set_state(TestState2::get_instance());
          }} {};
} // namespace minesweeper::app