#include <iostream>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>

int
main(int argc, char *argv[])
{
 
  string_vector args (2);
  args(0) = "embedded";
  args(1) = "-q";

  octave_main (2, args.c_str_vec (), 1);

  octave_value_list in;
  in(0) = "sombrero.ofig";
  octave_value_list out = feval ("hgload", in, 1);

  if (! error_state && out.length () > 0)
    std::cout << "hgload ("
              << in(0).string_value ()
              << ") is " << out(0).int_value ()
              << std::endl;
  else
    std::cout << "invalid\n";

  in(0) = "sombrero.pdf";
  feval ("print", in, 1);

  if (error_state)
    std::cout << "print failed" << std::endl;

   clean_up_and_exit (0);
   return 0;
}
