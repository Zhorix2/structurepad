#include <vector>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

int main() {
    using namespace ftxui;

    // Данные для меню
    std::vector<std::string> entries = {
        "File", "Edit", "View", "Tools", "Help"
    };
    int selected = 0;   // текущий выбранный пункт

    // Создаём опции горизонтального меню и включаем анимацию
    auto option = MenuOption::Horizontal();
    option.animation = true;   // плавное перемещение выделения

    // Создаём компонент меню
    auto menu = Menu(&entries, &selected, option);

    // Отрисовщик, который оборачивает меню и добавляет заголовок
    auto renderer = Renderer(menu, [&] {
        return vbox({
            text("Горизонтальное меню:") | bold,
            separator(),
            menu->Render() | border,   // рендерим само меню с рамкой
        });
    });

    // Запускаем интерактивный экран
    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);

    return 0;
}
