nt main() {
    int number1 = 88, number2 = 22;

    // Create a pointer pointing to number1
    int* pNumber1 = &number1;  // Explicit referencing

    cout << "Initial Assign: (pointer)" << *pNumber1 << endl;
    cout << "Initial Assign: " << pNumber1 << endl;
    cout << "Initial Assign:(ptr address) " << &pNumber1 << endl;
    cout << "Initial Assign: " << number1 << endl;
    cout << "Initial Assign: (no1 address)" << &number1 << endl;   // 0x22ff18

    *pNumber1 = 99;             // Explicit dereferencing
    cout << "Reassign as  *pNumber1 = 99: (pointer)" << *pNumber1 << endl;  // 99
    cout << "Reassign as  *pNumber1 = 99: " << pNumber1 << endl;   // 0x22ff18 (content of the pointer variable - same as above)
    cout << "Reassign as  *pNumber1 = 99 (ptr address): " << &pNumber1 << endl;  // 0x22ff10 (address of the pointer variable)

    cout << "Reassign as  *pNumber1 = 99: " << number1 << endl;
    cout << "Reassign as  *pNumber1 = 99 (no1 address): " << &number1 << endl;   // 0x22ff18

    pNumber1 = &number2;        // Pointer can be reassigned to store another address
    cout << "Reassign as  No2 (pointer)" << *pNumber1 << endl;  // 99
    cout << "Reassign as  No2 " << pNumber1 << endl;   // 0x22ff18 (content of the pointer variable - same as above)
    cout << "Reassign as  No2 (ptr address): " << &pNumber1 << endl;  // 0x22ff10 (address of the pointer variable)

    cout << "Reassign as  No2 (no1) " << number1 << endl;
    cout << "Reassign as  No2 (no1 address): " << &number1 << endl;   // 0x22ff18

    cout << "Reassign as  No2 (no2)" << number2 << endl;
    cout << "Reassign as  No2 (no2 address): " << &number2 << endl;   // 0x22ff18

    // Create a reference (alias) to number1
    int& refNumber1 = number1;  // Implicit referencing (NOT &number1)
    cout << "int& refNumber1 = number1" << endl;
    cout << number1 << endl;  // 11
    cout << &number1 << endl;    // 0x22ff18
    cout << refNumber1 << endl;  // 11
    cout << &refNumber1 << endl; // 0x22ff18


    refNumber1 = 11;
    cout << "  refNumber1 = 11;  " << endl;// Implicit dereferencing (NOT *refNumber1)
    cout << number1 << endl;  // 11
    cout << &number1 << endl;    // 0x22ff18
    cout << refNumber1 << endl;  // 11
    cout << &refNumber1 << endl; // 0x22ff18
    //refNumber1 = &number2;     // Error! Reference cannot be re-assigned
                                 // error: invalid conversion from 'int*' to 'int'
    refNumber1 = number2;        // refNumber1 is still an alias to number1.
    cout << "       refNumber1 = number2;  " << endl;// Implicit dereferencing (NOT *refNumber1)
    cout << number1 << endl;  // 11
    cout << &number1 << endl;    // 0x22ff18
    cout << refNumber1 << endl;  // 11
    cout << &refNumber1 << endl; // 0x22ff18 
    cout << number2 << endl;  // 11
    cout << &number2 << endl;    // 0x22ff18                          
                                 // Assign value of number2 (22) to refNumber1 (and number1).
    number2++;
    cout << "        number2++;" << endl;
    cout << number1 << endl;  // 11
    cout << &number1 << endl;    // 0x22ff18
    cout << refNumber1 << endl;  // 11
    cout << &refNumber1 << endl; // 0x22ff18 
    cout << number2 << endl;  // 11
    cout << &number2 << endl;    // 0x22ff18     

    number1 = number1 + 20;
    cout << "        number1+20;" << endl;
    cout << number1 << endl;  // 11
    cout << &number1 << endl;    // 0x22ff18
    cout << refNumber1 << endl;  // 11
    cout << &refNumber1 << endl; // 0x22ff18 
    cout << number2 << endl;  // 11
    cout << &number2 << endl;    // 0x22ff18  
    return 0;
}