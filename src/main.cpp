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
        hbox({
            text("► ") | color(Color::Green),
            text("Press Ctrl+C to exit")
        }),
        gap(1),
        hbox({
            box(vbox({
                text("Features:"),
                text("✓ Cross-platform"),
                text("✓ Modern C++17"),
                text("✓ Responsive UI")
            })) | border
        })
    });

    // Render and display
    auto screen = Screen::Create(Dimension::Full(), Dimension::Full());
    Render(screen, document);
    std::cout << screen.ToString();

    return 0;
}
