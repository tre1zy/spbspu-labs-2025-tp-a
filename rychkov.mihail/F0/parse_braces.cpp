#include "cparser.hpp"

#include <iostream>
#include "print_content.hpp"

void rychkov::CParser::parse_open_brace(CParseContext& context)
{
  if (global_scope())
  {
    log(context, "braced-enclosed body cannot be declared alone in global scope");
    return;
  }
  if (entities::is_decl(*stack_.top()))
  {
    entities::Declaration& decl = std::get< entities::Declaration >(stack_.top()->operands[0]);
    if (std::holds_alternative< entities::Struct >(decl.data))
    {
      entities::Struct& data = std::get< entities::Struct >(decl.data);
      if (data.name.empty())
      {
        log(context, "struct must have name");
        return;
      }
    }
    else if (std::holds_alternative< entities::Function >(decl.data))
    {
      entities::Function& data = std::get< entities::Function >(decl.data);
      for (size_t i = 0; i < data.parameters.size(); i++)
      {
        if (!data.parameters[i].empty())
        {
          variables_.insert({{data.type.function_parameters[i], data.parameters[i]}, stack_.size() + 1});
        }
      }
      if (find_overload(data.name, data.type.function_parameters).first != nullptr)
      {
        start_log(context) << "cannot define same function again - " << data;
        finish_log(context);
      }
      else
      {
        defined_functions_.insert({data.type, data.name});
      }
    }
    else
    {
      log(context, "braced enclosed body cannot follow this declaration");
      return;
    }
    decl.value = entities::Body{};
    stack_.push(&*decl.value);
    entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
    stack_.push(&body.data[0]);
    return;
  }
  if (stack_.top()->empty())
  {
    stack_.top()->operands.emplace_back(entities::Body{});
    entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
    stack_.push(&body.data[0]);
    return;
  }
  log(context, "unexpected braced-enclosed body");
}
void rychkov::CParser::parse_close_brace(CParseContext& context)
{
  if (stack_.size() == 1)
  {
    log(context, "unpaired closing brace");
    return;
  }
  if (stack_.top()->empty())
  {
    stack_.pop();
  }
  if (entities::is_body(*stack_.top()))
  {
    entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
    body.data.pop_back();
    stack_.pop();
    clear_scope();
    if (entities::is_decl(*stack_.top()))
    {
      entities::Declaration& decl = std::get< entities::Declaration >(stack_.top()->operands[0]);
      if (std::holds_alternative< entities::Struct >(decl.data))
      {
        entities::Struct& structure = std::get< entities::Struct >(decl.data);
        for (const entities::Expression& i: body.data)
        {
          if (entities::is_decl(i))
          {
            const entities::Declaration& var_decl = std::get< entities::Declaration >(i.operands[0]);
            if (!std::holds_alternative< entities::Variable >(var_decl.data))
            {
              log(context, "struct body must consist only of variable declarations");
              continue;
            }
            if (var_decl.value != nullptr)
            {
              log(context, "struct fields cannot have default values");
              continue;
            }
            const entities::Variable& var = std::get< entities::Variable >(var_decl.data);
            if (structure.fields.find(var.name) == structure.fields.end())
            {
              structure.fields.insert(var);
              continue;
            }
            log(context, "struct fields name duplicates");
            continue;
          }
          else if (!i.empty())
          {
            log(context, "struct body must consist only of variable declarations");
          }
        }
      }
      if (std::holds_alternative< entities::Function >(decl.data))
      {
        stack_.top() = &program_.emplace_back();
      }
      if (std::holds_alternative< entities::Statement >(decl.data))
      {
        entities::Statement& statement = std::get< entities::Statement >(decl.data);
        if ((statement.type == entities::Statement::IF) || (statement.type == entities::Statement::WHILE))
        {
          stack_.pop();
          append_empty(context);
          clear_scope();
          return;
        }
      }
    }
    else if (entities::is_body(*stack_.top()))
    {
      entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
      stack_.push(&body.data.emplace_back());
    }
    return;
  }
  log(context, "unpaired closing brace");
}
