#include <cstring>
#include <iostream>
#include <variant>

#include "file_operations.h"

namespace rh {

struct generate_signature_command {
    std::string file_path;
    std::string signature_path;
};

struct generate_delta_command {
    std::string signature_path;
    std::string new_file_path;
    std::string delta_path;
};

struct show_help_command {};

using command_t = std::variant<generate_signature_command,
    generate_delta_command, show_help_command>;

std::string get_help_message() {
    std::string message;
    message += "Usage: \n";
    message += "    rh_diff signature OLD_FILE SIGNATURE_FILE\n";
    message += "    rh_diff delta SIGNATURE_FILE NEW_FILE DELTA_FILE\n";
    message += "    rh_diff --help\n";

    return message;
}

[[noreturn]] void throw_invalid_args() {
    throw std::runtime_error("Invalid arguments\n" + get_help_message());
}

command_t parse_args(int argc, char *argv[]) {
    if (argc == 1) {
        throw_invalid_args();
    }

    if (std::strcmp(argv[1], "--help") == 0) {
        return show_help_command{};
    } else if (std::strcmp(argv[1], "signature") == 0) {
        if (argc != 4) {
            throw_invalid_args();
        }

        return generate_signature_command{argv[2], argv[3]};
    } else if (std::strcmp(argv[1], "delta") == 0) {
        if (argc != 5) {
            throw_invalid_args();
        }

        return generate_delta_command{argv[2], argv[3], argv[4]};
    } else {
        throw_invalid_args();
    }
}

struct command_visitor {
    void operator()(const generate_signature_command& command) const {
        generate_signature(command.file_path, command.signature_path);
    }

    void operator()(const generate_delta_command& command) const {
        generate_diff(command.new_file_path, command.signature_path, command.delta_path);
    }

    void operator()(const show_help_command& command) const {
        std::cout << get_help_message() << std::endl;
    }
};

} // namespace rh

int main (int argc, char *argv[]) {
    try {
        const auto command = rh::parse_args(argc, argv);
        std::visit(rh::command_visitor{}, command);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
