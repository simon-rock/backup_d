class Arg_parser
  {
public:
  enum Has_arg { no, yes, maybe };

  struct Option
    {
    int code;
    const char * name;
    Has_arg has_arg;
    };

private:
  struct Record
    {
    int code;
    std::string argument;
    explicit Record( const int c = 0 ) : code( c ) {}
    };

  std::string error_;
  std::vector< Record > data;

  bool parse_long_option( const char * const opt, const char * const arg,
                          const Option options[], int & argind );
  bool parse_short_option( const char * const opt, const char * const arg,
                           const Option options[], int & argind );

public:
  Arg_parser( const int argc, const char * const argv[],
              const Option options[], const bool in_order = false );

  Arg_parser( const char * const opt, const char * const arg,
              const Option options[] );

  const std::string & error() const { return error_; }

  int arguments() const { return data.size(); }


  int code( const int i ) const
    {
    if( i >= 0 && i < arguments() ) return data[i].code;
    else return 0;
    }

  const std::string & argument( const int i ) const
    {
    if( i >= 0 && i < arguments() ) return data[i].argument;
    else return error_;
    }
  };
