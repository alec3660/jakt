#ifdef _WIN32
extern "C" __cdecl int SetConsoleOutputCP(unsigned int code_page);
#endif
#include "project.h"
#include "project.h"
namespace Jakt {
namespace project {
ByteString Jakt::project::Project::debug_description() const { auto builder = ByteStringBuilder::create();builder.append("Project("sv);{
JaktInternal::PrettyPrint::ScopedLevelIncrease increase_indent {};
JaktInternal::PrettyPrint::must_output_indentation(builder);
builder.appendff("name: \"{}\"", name);
}
builder.append(")"sv);return builder.to_string(); }
ErrorOr<void> Jakt::project::Project::populate() const {
{
ByteString const current_directory = TRY((Jakt::jakt__platform__unknown_fs::current_directory()));
ByteString const project_directory = (current_directory + ByteString::from_utf8_without_validation("/"sv)) + this->name;
outln(StringView::from_string_literal("Creating jakt project in {}.."sv),project_directory);
TRY((Jakt::jakt__platform__unknown_fs::make_directory(project_directory)));
TRY((Jakt::jakt__platform__unknown_fs::make_directory(project_directory + ByteString::from_utf8_without_validation("/src"sv))));
out(StringView::from_string_literal("\tGenerating CMakeLists.txt..."sv));
TRY((this->create_template_cmake_lists(project_directory)));
outln(StringView::from_string_literal(" done"sv));
out(StringView::from_string_literal("\tGenerating jakt source code..."sv));
TRY((this->create_sample_jakt_files(project_directory)));
outln(StringView::from_string_literal(" done"sv));
out(StringView::from_string_literal("\tGenerating README.md..."sv));
TRY((this->create_readme(project_directory)));
outln(StringView::from_string_literal(" done"sv));
outln(StringView::from_string_literal("Done!"sv));
}
return {};
}

ErrorOr<void> Jakt::project::Project::create_template_cmake_lists(ByteString const project_directory) const {
{
ByteString const cml_contents = (((ByteString::from_utf8_without_validation("cmake_minimum_required(VERSION 3.20)\nproject("sv) + this->name) + ByteString::from_utf8_without_validation("\n   VERSION 1.0.0\n   LANGUAGES CXX\n)\n\nfind_package(Jakt REQUIRED)\n\nadd_jakt_executable("sv)) + this->name) + ByteString::from_utf8_without_validation("\n   MAIN_SOURCE src/main.jakt\n   MODULE_SOURCES\n     src/second_module.jakt\n)\n"sv);
TRY((Jakt::utility::write_to_file(cml_contents,project_directory + ByteString::from_utf8_without_validation("/CMakeLists.txt"sv))));
}
return {};
}

ErrorOr<void> Jakt::project::Project::create_sample_jakt_files(ByteString const project_directory) const {
{
ByteString const main_jakt = ByteString::from_utf8_without_validation("import second_module { get_string }\n\nfn main() throws -> c_int {\n    println(\"{}!\", get_string())\n    return 0\n}\n"sv);
ByteString const second_module_jakt = ByteString::from_utf8_without_validation("fn get_string() throws -> String {\n    return \"Hello, World\"\n}\n"sv);
TRY((Jakt::utility::write_to_file(main_jakt,project_directory + ByteString::from_utf8_without_validation("/src/main.jakt"sv))));
TRY((Jakt::utility::write_to_file(second_module_jakt,project_directory + ByteString::from_utf8_without_validation("/src/second_module.jakt"sv))));
}
return {};
}

ErrorOr<void> Jakt::project::Project::create_readme(ByteString const project_directory) const {
{
ByteString const readme_text = (((ByteString::from_utf8_without_validation("# Example Jakt Project\n\nThis example jakt project has two modules, hurray!\n\n## Building with jakt\n\n```console\njakt src/main.jakt -o "sv) + this->name) + ByteString::from_utf8_without_validation("\n```\n\n## Building with CMake\n\nMake sure to install the ``jakt`` compiler somewhere. For example, ``/path/to/jakt-install``.\n\nThis can be done by cloning ``jakt``, and running the following commands from its directory:\n\n```console\njakt> cmake -GNinja -B build -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_INSTALL_PREFIX=/path/to/jakt-install\njakt> cmake --build build\njakt> cmake --install build\n```\n\nNext you can build this project by configuring CMake to know where to find the ``jakt`` cmake helper scripts.\n\n```console\n> cmake -GNinja -B build -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_PREFIX_PATH=/path/to/jakt-install\n> cmake --build build\n```\n\n## Running the application\n\nAfter building, the program will be in the ``build`` directory\n\n```console\n./build/"sv)) + this->name) + ByteString::from_utf8_without_validation("\n```\n\nWhich should print:\n\n```console\nHello, World!\n```\n"sv);
TRY((Jakt::utility::write_to_file(readme_text,project_directory + ByteString::from_utf8_without_validation("/README.md"sv))));
}
return {};
}

Jakt::project::Project::Project(ByteString a_name): name(move(a_name)){}

}
} // namespace Jakt
