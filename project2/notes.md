A good approach is to complete the project in three steps:

1.  write a parser that only checks syntax. (Does not build any data structures.)

2.  write classes for data structures. (Rule, Predicate, Parameter, etc)

3.  add code to the parser to create data structures. For example when a parameter is being parsed a Parameter object is created and added to the current Predicate.
