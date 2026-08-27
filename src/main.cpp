#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>
#include <iostream>

int main() {
    using namespace ftxui;

    // Create a simple document
    auto document = vbox({
        text("Welcome to StructurePad!") | bold | color(Color::Cyan),
        separator(),
        text("Built with FTXUI - Modern C++ terminal UI library"),
        separator(),
        hbox(Elements{
            text("► ") | color(Color::Green),
            text("Press Ctrl+C to exit")
        }),
        filler(),  // instead of gap(1) — fills remaining space, but if we want fixed 1 line maybe use size(HEIGHT, EQUAL, 1)
        // Alternatively, use text("") to add a blank line:
        // text(""),
        hbox(Elements{
            vbox({
                text("Features:"),
                text("✓ Cross-platform"),
                text("✓ Modern C++17"),
                text("✓ Responsive UI")
            }) | border
        })
    });

    // Render and display
    auto screen = Screen::Create(Dimension::Full(), Dimension::Full());
    Render(screen, document);
    std::cout << screen.ToString();

    return 0;
}
