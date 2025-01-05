#include "toml_loader.h"
#include "config_utils.h"
#include <filesystem>
#include <stdexcept>

namespace config_lib {
  TomlLoader::TomlLoader(const std::filesystem::path &config_file_path) {
    auto file_path = config_lib::expand_tilde(config_file_path);
    if (not(std::filesystem::exists(file_path)))
      throw std::runtime_error("toml_loader: The configuration file \"" +
                               file_path.string() + "\" does not exist.");
    config = toml::parse_file(file_path.u8string());
  }

  std::string
  TomlLoader::parse_string(const std::vector<std::string> &tree_pos) {
    auto node = get_node(tree_pos);
    std::string str = node.value_or(std::string{});
    if (str.empty()) {
      throw std::runtime_error("String read from the configuration is empty.");
    }
    return str;
  }

  std::filesystem::path
  TomlLoader::parse_filepath(const std::vector<std::string> &tree_pos) {
    std::filesystem::path path = parse_string(tree_pos);
    path = config_lib::expand_tilde(path);
    if (not std::filesystem::exists(path.parent_path()))
      throw std::runtime_error("Provided filepath does not exist: " +
                               std::string(path.parent_path()));
    return path;
  }

  float TomlLoader::parse_float(const std::vector<std::string> &tree_pos) {
    auto node = get_node(tree_pos);
    float num = node.value_or(std::numeric_limits<float>::quiet_NaN());
    if (std::isnan(num)) {
      throw std::runtime_error("Float read from the configuration is empty.");
    }
    return num;
  }

  std::vector<std::string>
  TomlLoader::parse_stringvec(const std::vector<std::string> &tree_pos) {
    std::vector<std::string> strings;
    auto node = get_node(tree_pos);
    node.as_array()->for_each([&strings](auto &&el) {
      if constexpr (toml::is_string<decltype(el)>)
        strings.push_back(std::string(el));
      else
        throw std::runtime_error("Invalid configuration array type: should be"
                                 " a string.");
    });
    return strings;
  }

  bool TomlLoader::node_exists (const std::vector<std::string> &tree_pos) {
    auto node = get_node(tree_pos);
    return node.is_value();
  }

  toml::node_view<toml::node> TomlLoader::get_node
  (const std::vector<std::string> &tree_pos) {
    if (tree_pos.empty())
      throw std::runtime_error("toml_loader::get_node: No nodes specified.");
    toml::node_view<toml::node> node {config};
    for (const auto &pos : tree_pos)
      node = node[pos];
    return node;
  }

} // namespace config_lib
