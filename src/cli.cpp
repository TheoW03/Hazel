#include <vector>
#include <map>
#include <iostream>
#include <cli.h>
#include <functional>
#include <optional>
struct Parse_F
{
    std::function<void(std::string &, Output &)> function;
    std::string help_text;
    int takes_args;
};
Output parse_cmd(std::vector<std::string> args)
{
    int state = 0;
    Output o;
    std::string optop;
    /* A function wrapper to a lambda function. */
    std::map<std::string,
             Parse_F>
        option_handlers;
    option_handlers["o"] = {[](std::string &value, Output &o)
                            {
                                o.output_files.push_back(value);
                            },
                            "output file", 1};
    option_handlers["print-llvm"] = {[](std::string &value, Output &o)
                                     {
                                         std::cout << "print-llvm" << std::endl;

                                         o.print_llvm = 1;
                                     },
                                     "prints the llvm output", 0};
    option_handlers["print-tokens"] = {[](std::string &value, Output &o)
                                       {
                                           std::cout << "print-rtokens" << std::endl;
                                           o.print_tokens = 1;
                                       },
                                       "prints tokens output", 0};

    // func_c(2017);
    std::optional<Parse_F> current;
    for (int i = 0; i < args.size(); i++)
    {

        if (args[i].at(0) == '-')
        {
            optop = args[i].substr(1, args[i].size());
            if (option_handlers.find(optop) != option_handlers.end())
            {
                current = option_handlers[optop];
                if (current.value().takes_args == 0)
                {
                    current.value().function(args[i], o);
                    current = {};
                }
            }
            else
            {
                std::cout << "unknown option \"" << optop << "\"" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if (current.has_value())
        {
            current.value().function(args[i], o);
        }
        else
        {
            o.files.push_back(args[i]);
        }
    }
    return o;
}
