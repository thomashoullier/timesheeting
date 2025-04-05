/** @file
 *  @brief TOML configuration file loader library. A tree structure is assumed
           for the configuration sections. */
#ifndef TOML_LOADER_H
#define TOML_LOADER_H

#include <filesystem>
#include <toml++/toml.hpp>

namespace config_lib {
  /** @brief TOML configuration file loader. */
  class TomlLoader {
  public:
    /** @brief Initialize the configuration loading.

     The tilde in config_file_path is automatically expanded. */
    explicit TomlLoader(const std::filesystem::path &config_file_path);

    /** @brief Parse a string from the configuration at a particular location
        in the tree. */
    std::string parse_string(const std::vector<std::string> &tree_pos);
    /** @brief Parse a filepath from the configuration.

     With automatic tilde expansion. */
    std::filesystem::path
    parse_filepath(const std::vector<std::string> &tree_pos);
    /** @brief Parse an unsigned integer number from the configuration. */
    uint64_t parse_unsigned (const std::vector<std::string> &tree_pos);
    /** @brief Parse a float number from the configuration. */
    float parse_float(const std::vector<std::string> &tree_pos);
    /** @brief Parse a vector of successive strings from the configuration. */
    std::vector<std::string> parse_stringvec
    (const std::vector<std::string> &tree_pos);

  private:
    /** @brief Internal TOML document representation. */
    toml::parse_result config;
    /** @brief Get a config node given the vector the node path from the tree
               root. */
    toml::node_view<toml::node> get_node
    (const std::vector<std::string> &tree_pos);
  };
} // namespace config_lib

#endif // TOML_LOADER_H
