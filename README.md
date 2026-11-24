# C++ Console Banking System

A small console banking program written in C++. It simulates an ATM-style system with six built-in users who log in with a PIN and can manage their money.

## Features

- Login using a PIN with limited attempts
- Deposit money
- Withdraw money
- Check balance
- Transfer money to another user
- Input validation with exceptions, so invalid input never crashes the program
- Every user starts each run with a random balance between 1€ and 10k€

## PIN codes

The PIN codes of all users are written as comments at the very start of the source file (`ATM.cpp`), so you can log in when testing the program.

## Notes

This is a learning project. Nothing is saved to disk, so all balances reset when the program closes, and the PINs are stored in plain text on purpose. Do not use it for anything real.

## License

Released under no license. Free to use!
