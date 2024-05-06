# guessing

You have to choose matrices $B_i^(-1)$ so that their sum is equal to scalar of unity. 

Then you have $B_iAB_i^(-1)A^(-1)$ you can multiply by $B_i^(-1)$ to get $AB_iA^-1$. 

Then their sum is equal to $A(sum(B_i))A^-1=A(kE)*A^-1=kE$ (k - scalar, E - unity) so the sum of these vectors would be just a scalar of the original one.

See healthcheck for the implementation.
