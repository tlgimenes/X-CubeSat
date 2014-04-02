#include "function.hpp"
#include "log.hpp"

/*  --------------------------------------------------------  */
/* Constructor
 */
Function::Function()
{
    this->interface = NULL;
    this->variables = NULL;
}
/*  --------------------------------------------------------  */

/*  --------------------------------------------------------  */
/* Constructor
 */
Function::Function(InOutInterface *interface, std::unordered_map<std::string, Function*> *variables)
{
    this->variables = variables;
    this->interface = interface;
}
/*  --------------------------------------------------------  */
