/*
This file contains everything related to
    Generating uniform samples
    Distributing samples according to pdfs

*/

/*
Computes the radical inverse of a number,
by flipping its digits around the decimal point.

Note: More efficitent implementations are possible.
See pbr rendering book.
However for now having it work in an understandable way is more important.
*/
float radicalInverse(const unsigned int base, unsigned int number)
{    
    // We compute the inverted base once
    const float invertedBase = 1.f / (float) base;
    // In this variable we collect the digits in reversed order
    int reversedDigits = 0;
    // This is used to then move the digits behind the decimal point
    // It has the value of our highest order of magnitude given the base
    float invertedBaseAccumulated = 1;
    while (number)
    {
        // We remove the last digit from the number
        unsigned int next = number / base;
        // All but the last digit subtracted from the number is the last digit
        // x / base * base makes the last digit 0
        unsigned int digit = number - next * base;
        // reversedDigits * base moves all digits one step
        // + digit adds the latest digit at the end
        reversedDigits = reversedDigits * base + digit;
        // We store how many orders of magnitude we used
        invertedBaseAccumulated *= invertedBase;
        // next contains all but the last digit,
        // as if we poped it off the stack of digits
        number = next;
    }
    // Moving the digits behind the decimal point
    return reversedDigits * invertedBaseAccumulated;
}

// Hammersley


// Halton
float sampleDimension(unsigned int index, unsigned int dimension)
{
    return radicalInverse(dimension+2, index);
}


// Lattice


// Sample a triangle


// Sample a circle


// Sample a hemisphere


// Sample a cosine weighted hemisphere


// Sample a phong lobe


// Sample a ward microfacets BRDF