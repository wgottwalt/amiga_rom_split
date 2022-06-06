// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include <functional>
#include <string>
#include <tuple>

class Args {
public:
    //--- public types and constants ---
    using HelpFunc = std::function<void(const char *)>;
    using Result = std::tuple<bool,std::string>;

    //--- public constructors ---
    Args(int32_t &argc, char **argv, char **envp, const HelpFunc func) noexcept;
    Args(const Args &rhs) = delete;
    Args(Args &&rhs) noexcept = delete;
    ~Args() noexcept = default;

    //--- public operators ---
    Args &operator=(const Args &rhs) = delete;
    Args &operator=(Args &&rhs) noexcept = delete;

    //--- public methods ---
    int32_t paramCount() const noexcept;
    int32_t envCount() const noexcept;
    bool help(const bool no_args = false) const;
    Result longOpt(const std::string &param, const bool has_value = false) const;
    Result shortOpt(const char param, const bool has_value = false) const;
    Result opt(const char sparam, const std::string &param, const bool has_value = false) const;
    Result env(const std::string &param, const bool has_value = false) const;

protected:
    //--- protected methods ---
    bool parseHelp() const noexcept;

private:
    //--- private properties ---
    int32_t &_argc;
    char **_argv;
    char **_envp;
    HelpFunc _func;
    bool _help_called;
};
