#include <iostream>
#include "boostSnippets.h"
#include "Visitor.h"

int main()
{
    ////////////////////////////////////////////
    // cool and useful snipptes
    ////////////////////////////////////////////
    
#ifdef HAS_BOOST
    boost_loop_combine();
#endif

    visitor_example();
    visitor_example_container();
}
