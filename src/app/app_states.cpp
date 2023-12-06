#include <app/app_states.h>

namespace minesweeper::app {
TestState2::TestState2(App *app)
    : AppState{app},
      test_button{Graph_lib::Point{100, 100}, 50, 50, "Change to state1",
                  [](Graph_lib::Address, Graph_lib::Address button_addr) {
                    App &app = get_app_ref(button_addr);
                    app.set_state(new TestState1{&app});
                  }} {};

TestState1::TestState1(App *app)
    : AppState{app},
      test_button{Graph_lib::Point{20, 20}, 50, 50, "Change to state2",
                  [](Graph_lib::Address, Graph_lib::Address button_addr) {
                    App &app = get_app_ref(button_addr);
                    app.set_state(new TestState2{&app});
                  }} {};
} // namespace minesweeper::app