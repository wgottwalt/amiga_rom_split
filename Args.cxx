// SPDX-License-Identifier: LGPL-3.0-or-later
#include <cctype>
#include <stdexcept>
#include <string_view>
#include "Args.hxx"

//--- public constructors ---

Args::Args(int32_t &argc, char **argv, char **envp, const HelpFunc func) noexcept
: _argc{argc}, _argv{argv}, _envp{envp}, _func{func}, _help_called{parseHelp()}
{
}

//--- public methods ---

int32_t Args::paramCount() const noexcept
{
    return _argc;
}

int32_t Args::envCount() const noexcept
{
    int32_t i = 0;

    while (_envp[i++]);

    return i;
}

bool Args::help(const bool no_args) const
{
    if ((_help_called || (no_args && (_argc == 1))) && _func)
    {
        _func(_argv[0]);
        return true;
    }

    return false;
}

Args::Result Args::longOpt(const std::string &param, const bool has_value) const
{
    if (!param.empty())
    {
        const int32_t psize = param.size();

        for (int32_t i = 1; i < _argc; ++i)
        {
            const std::string_view arg = _argv[i];
            const int32_t asize = arg.size();

            if ((asize <= 2) || (arg.substr(0, 2) != "--"))
                continue;

            if (arg.substr(2, psize) == param)
            {
                if (has_value)
                {
                    if ((asize > (psize + 3)) && (arg[psize + 2] == '='))
                        return {true, std::string(arg.substr(psize + 3, std::string::npos))};

                    if ((asize == (psize + 2)) && ((i + 1) < _argc) && (_argv[i + 1][0] != '-'))
                        return {true, _argv[i + 1]};
                }
                else
                {
                    if (asize == (psize + 2))
                        return {true, ""};
                }
            }
        }
    }

    return {false, ""};
}

Args::Result Args::shortOpt(const char param, const bool has_value) const
{
    if (std::isalnum(param))
    {
        for (int32_t i = 1; i < _argc; ++i)
        {
            const std::string_view arg = _argv[i];
            const int32_t asize = arg.size();

            if ((asize <= 1) || (arg[0] != '-'))
                continue;

            if ((asize > 1) && (arg[1] == param))
            {
                if (has_value)
                {
                    if ((asize > 3) && (arg[2] == '='))
                        return {true, std::string(arg.substr(3, std::string::npos))};

                    if ((asize > 2) && std::isalnum(arg[2]))
                        return {true, std::string(arg.substr(2, std::string::npos))};

                    if ((asize == 2) && ((i + 1) < _argc) && (_argv[i + 1][0] != '-'))
                        return {true, _argv[i + 1]};
                }
                else
                {
                    if (asize == 2)
                        return {true, ""};
                }
            }
        }
    }

    return {false, ""};
}

Args::Result Args::opt(const char sparam, const std::string &lparam, const bool has_value) const
{
    auto [sparam_found, sparam_val] = shortOpt(sparam, has_value);
    auto [lparam_found, lparam_val] = longOpt(lparam, has_value);

    if (sparam_found && lparam_found)
        throw std::logic_error("same long and and short opt set");

    if (sparam_found)
        return {true, sparam_val};
    if (lparam_found)
        return {true, lparam_val};

    return {false, ""};
}

Args::Result Args::env(const std::string &param, const bool has_value) const
{
    if (!param.empty())
    {
        const std::string tmp = (param + "=");
        size_t i = 0;

        while (_envp[i])
        {
            if (const std::string_view arg = _envp[i++]; arg.starts_with(tmp))
            {
                if (has_value)
                    return {true, std::string(arg.substr(tmp.size(), std::string::npos))};
                else
                    return {true, ""};
            }
        }
    }

    return {false, ""};
}

//--- protected methods ---

bool Args::parseHelp() const noexcept
{
    for (int32_t i = 1; i < _argc; ++i)
    {
        const std::string_view arg = _argv[i];

        if ((arg == "--help") || (arg == "-h"))
            return true;
    }

    return false;
}
