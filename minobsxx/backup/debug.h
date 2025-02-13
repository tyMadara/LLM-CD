/* Some debugging module
   To invoke the DBG macro, compile with flag DEBUG
   Then in code do DBG("some output");
   Since it is a macro, any valid substitution for output will be valid. Example:
   DBG("Hello World " << 42);
*/

#ifndef DEBUG_H
#define DEBUG_H 

#ifdef DEBUG
#include <iostream>
#define DBG(x) std::cerr << '[' << __FILE__ << ':' << __LINE__ << " in " << __PRETTY_FUNCTION__ << "] " << x << std::endl
#else
#define DBG(x) 
#endif

#endif /* DEBUG_H */
