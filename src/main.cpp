#include <vector>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <iostream>

int main() {
    using namespace ftxui;

    std::vector<std::string> entries = {
    "File", "Edit", "View", "Tools", "Help"
    };
    int selected = 0;

    Element document = menu(&entries, &selected, MenuOption::HorizontalAnimated());

    auto render = Renderer(menu, [&] {
        return vbox({
            menu->Render() | border,
            });
    });
    
    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);
    
    return 0;
}
