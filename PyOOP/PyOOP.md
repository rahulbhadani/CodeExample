
# This tutorial and its explanation are for people who already familiar with OOP in C++

# Creating simplest class


```python
class Person:
    pass #empty block
p = Person
print(p)
```

    <class '__main__.Person'>


# Functions in class definition with self


```python
class Person:
    def say_hi(self):
        print('How are you?')
p = Person()
p.say_hi()
# Previous 2 lines can also be written as Person().say_hi()
```

    How are you?


# Using \_\_init\_\_

The `__init__` method is run as soon as an object of a class is instantiated. This is like constructor in C++ and does the job of any initialization required for class members, etc. 


```python
class Person:
    def __init__(self, name):
        self.name = name
    def say_hi(self):
        print('Hello, my name is ', self.name)
p = Person('Wolverine')
p.say_hi()

```

    Hello, my name is  Wolverine


Here, we define `__init__` method with parameters `name` and `self`. Here, also by writing `self.name`, we created a member variable of class Person with the name `name`, although it is different from argument `name` being passed. Hence, a way of defining object variable is to write `self.<variableName>` in the `__init__` function.

# Class variables and Object variables
Just like static in C++, we have class variables in python. We already saw a means of creating object variables in the above example.


```python
class Robot:
    """ Representes a robot with a name.""" 
    #In the above way the description about class is provided
    population = 0
    def __init__(self, name):
        """Initialize the data."""
        self.name = name
        print("(Initializing {})".format(self.name))
        # When this person is created, the robot adds to the population
        Robot.population += 1
    
    def die(self):
        """ I am dying"""
        print("{} is being")
        if Robot.population == 0:
            print("No more robot to die")
        else:
            Robot.population -= 1
    def say_hi(self):
        print("Greeting from {}".format(self.name))
    
    @classmethod
    def how_many(cls):
        """Print the current number of robots."""
        print("We have {:d} robots".format(cls.population))
    # Class method is like a static function in C++
        
```


```python
robo1 = Robot("R2-D2")
robo1.say_hi()
Robot.how_many()
```

    (Initializing R2-D2)
    Greeting from R2-D2
    We have 1 robots



```python
robo2 = Robot("C3PO")
robo2.say_hi()
Robot.how_many()
```

    (Initializing C3PO)
    Greeting from C3PO
    We have 2 robots


# Inheritance

In this example, the superclass is `SchoolMember` and subclass is `Student`.


```python
class SchoolMember:
    '''Represents any school member'''
    def __init__(self, name, age):
        self.name = name
        self.age = age
        print('(Initialized SchoolMember:{})'.format(self.name))
    def tell(self):
        '''Tell my details'''
        print('Name:"{}", Age:"{}"'.format(self.name, self.age), end=" ")
```


```python
# Class student inherits from SchoolMember
class Student(SchoolMember):
    '''Represents a student'''
    def __init__(self, name, age, marks):
        SchoolMember.__init__(self, name, age)
        self.marks = marks
        print('(Initialized Student:{})'.format(self.name))
    def tell(self):
        '''Tell my details'''
        print('Marks:"{}"'.format(self.marks))
```


```python
s = Student('Cyclops', 24, 99)
```

    (Initialized SchoolMember:Cyclops)
    (Initialized Student:Cyclops)


# More examples


```python
class Vehicle:
    def __init__(self, wheels_num, tanktype, seat_num, max_vel):
        self.wheels_num = wheels_num
        self.tanktype = tanktype
        self.seat_num = seat_num
        self.max_vel = max_vel
```


```python
toyota_prius = Vehicle(4, 'electric', 5, 80)
```

## Adding functions to the class
A normal way of writing a getter and a setter is as follows:


```python
class Vehicle:
    def __init__(self, wheels_num, tanktype, seat_num, max_vel):
        self.wheels_num = wheels_num
        self.tanktype = tanktype
        self.seat_num = seat_num
        self.max_vel = max_vel
    def getWheelNums(self):
        return self.wheels_num
    def setWheelNums(self, number):
        self.wheels_num = number
```

In python, there is a special way of writing getter and setter of a member variable


```python
class Vehicle:
    def __init__(self, wheels_num, tanktype, seat_num, max_vel):
        self.wheels_num = wheels_num
        self.tanktype = tanktype
        self.seat_num = seat_num
        self.max_vel = max_vel
        
    @property
    def wheels_num(self):
        return self.__wheels_num
    
    @wheels_num.setter
    def wheels_num(self, number):
        self.__wheels_num = number
```

We can use getter and setter in the following ways


```python
toyota_prius = Vehicle(4, 'hybrid', 5, 100)
print(toyota_prius.wheels_num)
toyota_prius.wheels_num = 2
print(toyota_prius.wheels_num)

```

    4
    2


## Adding more methods


```python
class Vehicle:
    def __init__(self, wheels_num, tanktype, seat_num, max_vel):
        self.wheels_num = wheels_num
        self.tanktype = tanktype
        self.seat_num = seat_num
        self.max_vel = max_vel
        
    @property
    def wheels_num(self):
        return self.__wheels_num
    
    @wheels_num.setter
    def wheels_num(self, number):
        self.__wheels_num = number
        
    def make_noise(self):
        print('VRUUM VRUUM')
```


```python
toyota_prius = Vehicle(4, 'hybrid', 5, 100)
print(toyota_prius.wheels_num)
toyota_prius.wheels_num = 2
print(toyota_prius.wheels_num)
toyota_prius.make_noise()
```

    4
    2
    VRUUM VRUUM

