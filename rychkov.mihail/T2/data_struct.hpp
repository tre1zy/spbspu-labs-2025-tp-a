#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <iosfwd>

namespace rychkov
{
  struct DataStruct
  {
    double key1;
    long long key2;
    std::string key3;
  };
  std::istream& operator>>(std::istream& in, DataStruct& link);

  namespace input
  {
    struct nth_ds_field
    {
      size_t field_number;
      DataStruct& link;
    };
    std::istream& operator>>(std::istream& in, nth_ds_field link);

    template< class T, size_t Id = 0 >
    class io_literal;
    template< class T, size_t Id >
    extern std::istream& operator>>(std::istream& in, io_literal< T, Id >&&);
    template< class T, size_t Id >
    extern std::ostream& operator<<(std::ostream& in, const io_literal< T, Id >&&);

    template< class T, size_t Id >
    class io_literal
    {
    public:
      using value_type = T;
      static constexpr size_t id = Id;

      io_literal(const value_type& initVal):
        value(initVal),
        link(value)
      {}
      io_literal(value_type& initLink):
        value(),
        link(initLink)
      {}
      friend std::istream& rychkov::input::operator>>< T, Id >(std::istream& in, io_literal&&);
      friend std::ostream& rychkov::input::operator<<< T, Id >(std::ostream& in, const io_literal&&);
    private:
      T value;
      T& link;
    };
    using science_literal = io_literal< double >;
    using ull_literal = io_literal< long long >;
    using string_literal = io_literal< std::string >;
  }
}

#endif
