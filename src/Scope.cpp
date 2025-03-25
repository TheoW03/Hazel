#include <backend/Scope.h>

/**
 * ProgramScope represents the whole scope of the program
 * and control flow. that "CompiledCOntext" was originally doing
 */
ProgramScope::ProgramScope()
{
}
ProgramScope::ProgramScope(std::map<std::string, CompiledModuleClass> modules)
{
    this->modules = modules;
}

void ProgramScope::set_current(Tokens name)
{
    this->current_module = modules[name.value];
}
Function ProgramScope::get_function(Tokens name)
{
    auto global = this->get_global_function(name);
    if (global.has_value())
    {
        return global.value();
    }
    else
    {
        return local_functions[name.value];
    }
}

std::optional<Function> ProgramScope::get_global_function(Tokens name)
{
    if (!this->current_module.get_function(name).has_value())
    {
        auto import_list = this->current_module.get_import_names();
        for (int i = 0; i < import_list.size(); i++)
        {
            if (modules[import_list[i].value].get_function(name).has_value())
            {
                return modules[import_list[i].value].get_function(name).value();
            }
        }
        return {};
    }
    return this->current_module.get_function(name);
}
Function ProgramScope::get_current_function()
{
    return current_module.get_current_function();
}

void ProgramScope::addLocal(Tokens name, Function function)
{
    if (this->local_functions.find(name.value) != this->local_functions.end())
    {
        local_functions[name.value] = function;
    }
    else
    {
        local_functions.insert(std::make_pair(name.value, function));
    }
}
CompiledModuleClass::CompiledModuleClass()
{
}

CompiledModuleClass::CompiledModuleClass(CompiledModule CompiledModule)
{
    this->compiled_module = compiled_module;
}

std::optional<Function> CompiledModuleClass::get_function(Tokens name)
{
    if (this->compiled_module.func_map.find(name.value) == this->compiled_module.func_map.find(name.value))
    {
        return {};
    }
    return this->compiled_module.func_map[name.value];
}

std::vector<Tokens> CompiledModuleClass::get_import_names()
{
    return compiled_module.imports;
}

Function CompiledModuleClass::get_current_function()
{
    auto f = this->compiled_module.functions.front();
    compiled_module.functions.pop();
    return f;
}
