#include "toml_loader.h"
#include "config_utils.h"
#include <cstdint>
#include <filesystem>
#include <limits>
#include <numeric>
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

  uint64_t TomlLoader::parse_unsigned (const std::vector<std::string> &tree_pos) {
    auto node = get_node(tree_pos);
    uint64_t num = node.value_or(std::numeric_limits<uint64_t>::max());
    if (num == std::numeric_limits<uint64_t>::max()) {
      throw std::runtime_error
        ("Unsigned integer read from the configuration is empty.");
    }
    return num;
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

  toml::node_view<toml::node> TomlLoader::get_node
  (const std::vector<std::string> &tree_pos) {
    if (tree_pos.empty())
      throw std::runtime_error("toml_loader::get_node: No nodes specified.");
    toml::node_view<toml::node> node {config};
    // Going down the node according to the strings in tree_pos.
    node = std::accumulate(tree_pos.begin(), tree_pos.end(), node,
                           [](const toml::node_view<toml::node> _node,
                              const std::string &_pos) {
                             return _node[_pos];
                           });
    return node;
  }

} // namespace config_lib
