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

// ---- Утилиты для работы с файловой системой ----
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
        // Сортировка: сначала папки, потом файлы
        sort(file_list.begin(), file_list.end(),
             [](const string& a, const string& b) {
                 bool a_dir = a.back() == '/';
                 bool b_dir = b.back() == '/';
                 if (a_dir != b_dir) return a_dir > b_dir;
                 return a < b;
             });
        // Добавляем ".." для перехода в родительскую папку (кроме корня)
        if (current_dir != "/" && current_dir != ".") {
            file_list.insert(file_list.begin(), "..");
        }
    } catch (...) {
        // Ошибка чтения – оставляем список пустым
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

    // Состояние главного меню
    vector<string> entries = {"File", "Edit", "View", "Tools", "Help"};
    int selected = 0;

    // Состояние файлового браузера
    bool show_file_browser = false;
    string current_dir = ".";
    vector<string> file_list;
    int file_browser_selected = 0;

    // Состояние редактора (пока только для отображения имени)
    string file_content;
    string saved_content;
    string filename;
    bool is_modified = false;

    // ---- Компоненты ----
    // Горизонтальное главное меню
    auto main_menu = Menu(&entries, &selected, MenuOption::Horizontal());

    // Файловый браузер (вертикальное меню)
    auto file_menu = Menu(&file_list, &file_browser_selected);
    file_menu->OnChange = [&] {
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
        if (selected_item.back() == '/') {  // это папка
            current_dir = full_path;
            refresh_file_list(current_dir, file_list, current_dir);
            file_browser_selected = 0;
        } else {  // файл
            load_file(full_path, file_content, saved_content, filename);
            show_file_browser = false;
            is_modified = false;
        }
    };

    // Общий контейнер (все активные компоненты)
    auto container = Container::Vertical({main_menu, file_menu});

    // ---- Рендерер с условным отображением ----
    auto renderer = Renderer(container, [&] {
        Elements children;

        // Заголовок с именем файла и звёздочкой
        string title = "Файл: " + (filename.empty() ? "(не открыт)" : filename);
        if (is_modified) title += " *";
        children.push_back(text(title) | bold | color(Color::Cyan));

        if (show_file_browser) {
            children.push_back(text("Текущая папка: " + current_dir) | dim);
            children.push_back(file_menu->Render() | border);
            children.push_back(text("Enter – выбрать, Esc – отмена") | dim);
        } else {
            children.push_back(main_menu->Render() | border);
            // Здесь позже можно добавить отображение содержимого файла или редактор
            if (!filename.empty()) {
                children.push_back(text("Содержимое файла (пока не редактируется)") | dim);
                children.push_back(text(file_content.substr(0, 200)) | dim);
            }
        }

        return vbox(children);
    });

    // ---- Обработка глобальных клавиш ----
    renderer = CatchEvent(renderer, [&](Event event) {
        // Ctrl+O – открыть браузер
        if (event == Event::Special("ctrl+o") ||
            (event.is_character() && event.ctrl() && event.character() == "o")) {
            show_file_browser = true;
            current_dir = ".";          // можно запоминать последнюю папку
            refresh_file_list(current_dir, file_list, current_dir);
            file_browser_selected = 0;
            return true;
        }
        // Esc – закрыть браузер без загрузки
        if (event == Event::Escape && show_file_browser) {
            show_file_browser = false;
            return true;
        }
        // Ctrl+S – сохранить (пока только обновляет сохранённую копию)
        if (event == Event::Special("ctrl+s") ||
            (event.is_character() && event.ctrl() && event.character() == "s")) {
            if (!filename.empty()) {
                ofstream ofs(filename);
                ofs << file_content;
                saved_content = file_content;
                is_modified = false;
            }
            return true;
        }
        return false;
    });

    // ---- Запуск ----
    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);

    return 0;
}
