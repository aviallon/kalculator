# Kalculator

A Work In Progress calculator app for [KnightOS](https://knightos.org/).
It is coded in a subset of C (since KCC, based on SDCC, only supports a small subset of C itself).

I could have done it in assembly, but this way it will be easier for a lot of people to contribute since it does not require a ton of knowledge, just "some" time and a colossal amount of patience.

# How to build ?

```text
git clone https://github.com/aviallon/kalculator.git
cd kaclulator
knightos init
make
```

# How to run ?

Just type this in the project directory. Make sure that you have knightos sdk installed. More informations on knightos.org
```text
make run
```

# Current status

- [X] Debug functions and code
- [X] `float`s display and manipulation
- [X] Stack helper
- [X] NPI evaluator
- [] Error handling
- [] Shunting yard algorithm
- [] User input and interface
- [] Common functions (sin, cos, log, etc.)
- [] Optimisation for speed
- [] Upstreaming some very functions to KnightOS libs.
