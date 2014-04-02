/*
 * CLASS INTERPRETER : This class defines methods for
 * interpreting a command file.
 *
 */

#include "interpreter.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
Interpreter::Interpreter(InOutInterface *inOutInterface)
{
    this->inOutInterface = inOutInterface;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
Interpreter::Interpreter(Glib::ustring *portName)
{
    this->inOutInterface = new InOutInterface(portName);
}
/*  --------------------------------------------------------  */

