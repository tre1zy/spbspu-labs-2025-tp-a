#ifndef CONFIRMATION_SRC_HPP
#define CONFIRMATION_SRC_HPP
#include <iostream>
#include <limits>

namespace savintsev
{
  class ConfirmationPrompt
  {
  public:
    ConfirmationPrompt(std::istream & in, std::ostream & out):
      in_(in),
      out_(out)
    {}

    bool ask(const std::string & message)
    {
      std::string answer;
      while (in_)
      {
        out_ << message << " [Y / N]: ";
        in_ >> answer;

        if (is_yes(answer))
        {
          return true;
        }
        if (is_no(answer))
        {
          return false;
        }
      }
      return false;
    }

  private:
    std::istream & in_;
    std::ostream & out_;

    static bool is_yes(const std::string & answer)
    {
      return !answer.empty() && (answer[0] == 'Y' || answer[0] == 'y');
    }

    static bool is_no(const std::string & answer)
    {
      return !answer.empty() && (answer[0] == 'N' || answer[0] == 'n');
    }
  };
}

#endif
