#include <stdio.h>

#include "TestMatrix.hpp"

int main (int ac, char** av)
{
    SUITE_START;

    TestMatrix::test ();

    SUITE_END;
}