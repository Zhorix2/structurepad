#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

using namespace std;
namespace fs = std::filesystem;

// ---- Вспомогательные функции ----
void refresh_file_list(const string& dir,
                       vector<string>& file_list,
                       string& current_dir) {
    file_list.clear();
    try {
        for (const auto& entry : fs::directory_iterator(dir)) {
            string name = entry.path().filename().string();
            if (entry.is_directory()) {
                name += "/";
            }
            file_list.push_back(name);
        }
        sort(file_list.begin(), file_list.end(),
             [](const string& a, const string& b) {
                 bool a_dir = a.back() == '/';
                 bool b_dir = b.back() == '/';
                 if (a_dir != b_dir) return a_dir > b_dir;
                 return a < b;
             });
        if (current_dir != "/" && current_dir != ".") {
            file_list.insert(file_list.begin(), "..");
        }
    } catch (...) {
        // ignore
    }
}

void load_file(const string& path,
               string& file_content,
               string& saved_content,
               string& filename) {
    ifstream ifs(path);
    if (!ifs) return;
    stringstream buffer;
    buffer << ifs.rdbuf();
    file_content = buffer.str();
    saved_content = file_content;
    filename = fs::path(path).filename().string();
}

// ---- Главная функция ----
int main() {
    using namespace ftxui;

    // ---- Состояние ----
    // Главное меню: теперь есть реальный пункт "Open"
    vector<string> main_entries = {"Open", "Save", "Exit"};
    int main_selected = 0;

    bool show_file_browser = false;
    string current_dir = ".";
    vector<string> file_list;
    int file_browser_selected = 0;

    string file_content;
    string saved_content;
    string filename;
    bool is_modified = false;

    // ---- Главное меню (вертикальное, чтобы было понятнее) ----
    auto main_menu = Menu(&main_entries, &main_selected);

    // ---- Файловое меню (браузер) ----
    MenuOption file_menu_option;
    file_menu_option.on_change = [&] {
        if (file_browser_selected < 0 || file_browser_selected >= file_list.size())
            return;
        string selected_item = file_list[file_browser_selected];
        if (selected_item == "..") {
            fs::path parent = fs::path(current_dir).parent_path();
            current_dir = parent.empty() ? "/" : parent.string();
            refresh_file_list(current_dir, file_list, current_dir);
            file_browser_selected = 0;
            return;
        }
        string full_path = current_dir + "/" + selected_item;
        if (selected_item.back() == '/') {
            current_dir = full_path;
            refresh_file_list(current_dir, file_list, current_dir);
            file_browser_selected = 0;
        } else {
            load_file(full_path, file_content, saved_content, filename);
            show_file_browser = false;
            is_modified = false;
        }
    };

    auto file_menu = Menu(&file_list, &file_browser_selected, file_menu_option);

    // ---- Контейнер всех активных компонентов ----
    // Важно: порядок определяет, кто получает фокус первым
    auto container = Container::Vertical({main_menu, file_menu});

    // ---- Рендерер ----
    auto renderer = Renderer(container, [&] {
        Elements children;

        // Заголовок
        string title = "Файл: " + (filename.empty() ? "(не открыт)" : filename);
        if (is_modified) title += " *";
        children.push_back(text(title) | bold | color(Color::Cyan));
        children.push_back(separator());

        if (show_file_browser) {
            children.push_back(text("Текущая папка: " + current_dir) | dim);
            children.push_back(file_menu->Render() | border);
            children.push_back(text("  Enter – выбрать, Esc – отмена  ") | dim | center);
        } else {
            children.push_back(main_menu->Render() | border);
            if (!filename.empty()) {
                children.push_back(text("Содержимое файла:") | bold);
                children.push_back(text(file_content.substr(0, 300)) | dim);
            }
            // Подсказка
            children.push_back(text("  ↑↓ выбор, Enter – действие, Ctrl+O – открыть, Ctrl+S – сохранить  ") | dim | center);
        }

        return vbox(children);
    });

    // ---- Обработка глобальных клавиш и действий главного меню ----
    renderer = CatchEvent(renderer, [&](Event event) {
        // Если главное меню активно и нажат Enter – обрабатываем выбранный пункт
        if (!show_file_browser && event == Event::Return) {
            if (main_selected == 0) { // "Open"
                show_file_browser = true;
                current_dir = ".";
                refresh_file_list(current_dir, file_list, current_dir);
                file_browser_selected = 0;
                return true;
            } else if (main_selected == 1) { // "Save"
                if (!filename.empty()) {
                    ofstream ofs(filename);
                    ofs << file_content;
                    saved_content = file_content;
                    is_modified = false;
                }
                return true;
            } else if (main_selected == 2) { // "Exit"
                // Выход из приложения
                ScreenInteractive::TerminalOutput().ExitLoop();
                return true;
            }
        }

        // Ctrl+O – открыть браузер (дублируем для удобства)
        if (event == Event::Special("ctrl+o")) {
            show_file_browser = true;
            current_dir = ".";
            refresh_file_list(current_dir, file_list, current_dir);
            file_browser_selected = 0;
            return true;
        }
        // Ctrl+S – сохранить
        if (event == Event::Special("ctrl+s")) {
            if (!filename.empty()) {
                ofstream ofs(filename);
                ofs << file_content;
                saved_content = file_content;
                is_modified = false;
            }
            return true;
        }
        // Esc – закрыть браузер без загрузки
        if (event == Event::Escape && show_file_browser) {
            show_file_browser = false;
            return true;
        }
        return false;
    });

    // ---- Запуск ----
    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);

    return 0;
}
